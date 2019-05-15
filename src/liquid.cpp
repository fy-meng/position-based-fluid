//
// Created by Fanyu Meng on 2019-04-21.
//

#include "liquid.h"
#include <omp.h>

//#define DEBUG_PRINT

using namespace nanogui;

void Liquid::addLiquid(Vector3D anchor, Vector3D size, LiquidParameters params) {
  this->anchors.push_back(anchor);
  this->sizes.push_back(size);
  this->params = params;

  size_t t = num_particles;
  num_particles += (size_t) (size.x * size.y * size.z * pow(params.granularity, 3));

  if (!particles)
    particles = (LiquidParticle **) malloc(num_particles * sizeof(LiquidParticle *));
  else
    particles = (LiquidParticle **) realloc(particles, num_particles  * sizeof(LiquidParticle *));

  int x_num = (int) (size.x * params.granularity);
  int y_num = (int) (size.y * params.granularity);
  int z_num = (int) (size.z * params.granularity);

  #pragma omp parallel for num_threads(16)
  for (int i = 0; i < size.x * params.granularity; ++i)
    for (int j = 0; j < size.y * params.granularity; ++j)
      for (int k = 0; k < size.z * params.granularity; ++k)
        this->particles[t + (i * y_num + j) * z_num + k] =
                new LiquidParticle(anchor + (Vector3D(i, j, k) + 0.5) / params.granularity);
}

void Liquid::render(GLShader &shader) {
  for (int i = 0; i < num_particles; ++i)
    particles[i]->m_sphere_mesh.draw_sphere(shader, particles[i]->pos, 1 / params.granularity / 2 * 0.8);
}

void Liquid::simulate(double frames_per_sec, double simulation_steps,
                      vector<Vector3D> external_accelerations,
                      vector<CollisionObject *> *collision_objects) {
  double delta_t = 1.0f / frames_per_sec / simulation_steps;

  Vector3D externAcc = {};
  for (Vector3D &a : external_accelerations)
    externAcc += a;

  // apply external forces
  #pragma omp parallel for num_threads(16)
  for (int i = 0; i < num_particles; ++i) {
    LiquidParticle *p = particles[i];
    p->vel += externAcc * delta_t;
    p->pos += p->vel * delta_t;
  }

  build_spatial_map();

  #ifdef DEBUG_PRINT
    cout << "spatial map\n";
  #endif

  float h = params.kernel_radius;

  // finding neighbors
  for (int i = 0; i < num_particles; ++i) {
    LiquidParticle *p = particles[i];
    p->neighbors.clear();
    for (int dx = -1; dx <= 1; ++dx)
      for (int dy = -1; dy <= 1; ++dy)
        for (int dz = -1; dz <= 1; ++dz) {
          int hash = hash_position(p->pos + Vector3D(dx, dy, dz) * h);
          if (map[hash])
            for (LiquidParticle *q : *map[hash_position(p->pos + Vector3D(dx, dy, dz) * h)])
              if (p != q && (p->pos - q->pos).norm2() <= h * h)
                p->neighbors.push_back(q);
        }
  }

  #ifdef DEBUG_PRINT
    cout << "neighbors for middle: " << particles[num_particles / 2]->neighbors.size() << endl;
  #endif

  // calculating density
  #pragma omp parallel for
  for (int i = 0; i < num_particles; ++i) {
    LiquidParticle *p = particles[i];
    p->density = 0;
    for (LiquidParticle *q : p->neighbors)
      p->density += W((float) (p->pos - q->pos).norm2(), h);
  }

  #ifdef DEBUG_PRINT
    cout << "density for middle: " << particles[num_particles / 2]->density << endl;
  #endif

  // solve constraint using Newton's method
  for (int iter = 0; iter < params.density_iter; ++iter) {

  #ifdef DEBUG_PRINT
      cout << "iter = " << iter << endl;
  #endif

    // collision
    // TODO: remove hard coded boundaries
    #pragma omp parallel for
    for (int i = 0; i < num_particles; ++i) {
      LiquidParticle *p = particles[i];
      if (p->pos.y <= 0)
        p->pos.y = 0 - 0.5 * p->vel.y * delta_t;
      if (p->pos.x <= -0.1)
        p->pos.x = -0.1 - 0.5 * p->vel.x * delta_t;
      if (p->pos.x >= 1.1)
        p->pos.x = 1.1 - 0.5 * p->vel.x * delta_t;
      if (p->pos.z <= -0.1)
        p->pos.z = -0.1 - 0.5 * p->vel.z * delta_t;
      if (p->pos.z >= 1.1)
        p->pos.z = 1.1 - 0.5 * p->vel.z * delta_t;
    }

#ifdef DEBUG_PRINT
    cout << "    collision\n";
#endif

    // calculate lambda
    #pragma omp parallel for
    for (int i = 0; i < num_particles; ++i) {
      LiquidParticle *p = particles[i];
      float denom = 0;
      for (LiquidParticle *q : p->neighbors)
        denom += Spiky_grad(p->pos - q->pos, h).norm2();
      p->lambda = -(p->density / params.rest_density - 1) / (denom + params.eps);
    }

#ifdef DEBUG_PRINT
    cout << "    lambda for middle: " << particles[num_particles / 2]->lambda << endl;
#endif

    // calculate delta_pos
    #pragma omp parallel for
    for (int i = 0; i < num_particles; ++i) {
      LiquidParticle *p = particles[i];
      p->delta_pos = {0, 0, 0};
      for (LiquidParticle *q : p->neighbors) {
        // tensile instability
        float s_corr = -params.k * pow(W((float) (p->pos - q->pos).norm2(), h) / W(params.delta_q, h),
                                       params.pressure_pow);
        p->delta_pos += (p->lambda + q->lambda) * Spiky_grad(p->pos - q->pos, h);
      }
      p->delta_pos /= params.rest_density;
      p->delta_pos *= 5000;
    }

#ifdef DEBUG_PRINT
    cout << "    delta_pos for middle: " << particles[num_particles / 2]->delta_pos << endl;
#endif

    float sum = 0, density;
    #pragma omp parallel for
    for (int i = 0; i < num_particles; ++i) {
      LiquidParticle *p = particles[i];
      density = 0;
      for (LiquidParticle *q : p->neighbors)
        density += W((float) (p->pos - q->pos).norm2(), h);
      #pragma omp critical
      sum += -(density / params.rest_density - 1);
    }

#ifdef DEBUG_PRINT
    cout << "    deviation = " << sum << endl;
#endif

    // update position
    #pragma omp parallel for
    for (int i = 0; i < num_particles; ++i)
      particles[i]->pos += particles[i]->delta_pos;
  }

#ifdef DEBUG_PRINT
  cout << "delta_pos for middle: " << particles[num_particles / 2]->prev_pos << ' ' << particles[num_particles / 2]->pos << ' ' << particles[num_particles / 2]->delta_pos << endl;
#endif

  // update velocity
  #pragma omp parallel for
  for (int i = 0; i < num_particles; ++i) {
    LiquidParticle *p = particles[i];
    p->vel = 0.99 * (p->pos - p->prev_pos) / delta_t;
    // viscosity
//    for (LiquidParticle *q : p->neighbors)
//      p->vel += params.viscosity * (q->vel - p->vel) * W((float) (p->pos - q->pos).norm2(), h);
    p->prev_pos = p->pos;
  }

#define VEL_EPS 0.1
#define COLLISION_EPS 0.01

  // additional collision to address vanishing momentum
//#pragma omp parallel for
  for (int i = 0; i < num_particles; ++i) {
    LiquidParticle *p = particles[i];

    if (p->pos.y <= 0.0 + COLLISION_EPS && abs(p->vel.y) <= VEL_EPS) {
      p->vel.y = VEL_EPS;
      p->pos.y += COLLISION_EPS;
    }

    if (p->pos.x <= -0.1 + COLLISION_EPS && abs(p->vel.x) <= VEL_EPS) {
      p->vel.x = VEL_EPS;
      p->pos.x += COLLISION_EPS;
    }

    if (p->pos.x >= 1.1 - COLLISION_EPS && abs(p->vel.x) <= VEL_EPS) {
      p->vel.x = -VEL_EPS;
      p->pos.x -= COLLISION_EPS;
    }

    if (p->pos.z <= -0.1 + COLLISION_EPS && abs(p->vel.z) <= VEL_EPS) {
      p->vel.z = VEL_EPS;
      p->pos.z += COLLISION_EPS;
    }

    if (p->pos.z >= 1.1 - COLLISION_EPS && abs(p->vel.z) <= VEL_EPS) {
      p->vel.z = -VEL_EPS;
      p->pos.z -= COLLISION_EPS;
    }
  }
}

void Liquid::reset() {
  int t = 0;
  for (int n = 0; n < anchors.size(); ++n)
    for (int i = 0; i < sizes[n].x * params.granularity; ++i)
      for (int j = 0; j < sizes[n].y * params.granularity; ++j)
        for (int k = 0; k < sizes[n].z * params.granularity; ++k)
          this->particles[t++] = new LiquidParticle(anchors[n] + (Vector3D(i, j, k) + 0.5) / params.granularity);
}

void Liquid::build_spatial_map() {
  for (const auto &entry : map)
    if (entry.second)
      entry.second->clear();

  // (Part 4): Build a spatial map out of all of the point masses.

  for (int i = 0; i < num_particles; ++i) {
    LiquidParticle *p = particles[i];
    int hash = hash_position(p->pos);
    if (!map[hash])
      map[hash] = new vector<LiquidParticle *>();

    map[hash]->push_back(p);
  }
}

#define HASH_PRIME 31253
#define HASH_SIZE 99999989

int Liquid::hash_position(Vector3D pos) {
  int x = (int) std::floor(pos.x / params.kernel_radius);
  int y = (int) std::floor(pos.y / params.kernel_radius);
  int z = (int) std::floor(pos.z / params.kernel_radius);
  return ((x * HASH_PRIME + y) * HASH_PRIME + z) % HASH_SIZE;
}

float inline Liquid::W(float r_2, float h) {
  // assumes r_2 < h * h
  return 0 <= r_2 && r_2 <= h * h
         ? (float) (315 / (64 * PI * pow(h, 9)) * pow(pow(h, 2) - r_2, 3))
         : 0;
}

Vector3D inline Liquid::Spiky_grad(const Vector3D &r, float h) {
  // assumes r * r < h * h
  return 0 <= r.norm2() && r.norm2() <= h * h
         ? (float) (-45 / PI / pow(h, 6) * pow(h - r.norm(), 2)) * r.unit()
         : 0;
}
