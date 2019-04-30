//
// Created by Fanyu Meng on 2019-04-21.
//

#include "liquid.h"

using namespace nanogui;

void Liquid::addLiquid(Vector3D anchor, Vector3D size, LiquidParameters params) {
  this->anchors.push_back(anchor);
  this->sizes.push_back(size);
  this->params = params;

  for (int i = 0; i < size.x * params.granularity; ++i)
    for (int j = 0; j < size.y * params.granularity; ++j)
      for (int k = 0; k < size.z * params.granularity; ++k)
        this->particles.emplace_back(LiquidParticle(anchor + (Vector3D(i, j, k) + 0.5) / params.granularity));
}

void Liquid::render(GLShader &shader) {
  for (LiquidParticle &p : particles)
    p.m_sphere_mesh.draw_sphere(shader, p.pos, 1 / params.granularity / 2 * 0.92);
}

void Liquid::simulate(double frames_per_sec, double simulation_steps,
                      vector<Vector3D> external_accelerations,
                      vector<CollisionObject *> *collision_objects) {
  double delta_t = 1.0f / frames_per_sec / simulation_steps;

  Vector3D externAcc = {};
  for (Vector3D &a : external_accelerations)
    externAcc += a;

  // apply external forces
  for (LiquidParticle &p : particles) {
    p.vel += externAcc * delta_t;
    p.pos += p.vel * delta_t;
  }

  build_spatial_map();

  cout << "spatial map\n";

  float h = params.kernel_radius;

  // finding neighbors
  for (LiquidParticle &p : particles) {
    p.neighbors.clear();
    for (int dx = -1; dx <= 1; ++dx)
      for (int dy = -1; dy <= 1; ++dy)
        for (int dz = -1; dz <= 1; ++dz) {
          int hash = hash_position(p.pos + Vector3D(dx, dy, dz) * h);
          if (map[hash])
            for (LiquidParticle *q : *map[hash_position(p.pos + Vector3D(dx, dy, dz) * h)])
              if (&p != q && (p.pos - q->pos).norm2() <= h * h)
                p.neighbors.push_back(q);
        }
  }

  cout << "neighbors for p0: " << particles[0].neighbors.size() << endl;

  // calculating density
  for (LiquidParticle &p : particles) {
    p.density = 0;
    for (LiquidParticle *q : p.neighbors)
      p.density += W((float) (p.pos - q->pos).norm2(), h);
  }

  cout << "density for p[0]: " << particles[0].density << endl;

  // solve constraint using Newton's method
  for (int iter = 0; iter < params.density_iter; ++iter) {

    cout << "iter = " << iter << endl;

    // calculate lambda
    for (LiquidParticle &p : particles) {
      float denom = 0;
      for (LiquidParticle *q : p.neighbors)
        denom += Spiky_grad(p.pos - q->pos, h).norm2();
      p.lambda = -(p.density / params.rest_density - 1) / (denom + params.eps);
    }

    cout << "    lambda for p0: " << particles[0].lambda << endl;

    // calculate delta_pos
    for (LiquidParticle &p : particles) {
      p.delta_pos = {0, 0, 0};
      for (LiquidParticle *q : p.neighbors)
        p.delta_pos += (p.lambda + q->lambda) * Spiky_grad(p.pos - q->pos, h);
      p.delta_pos /= params.rest_density / 2000;
    }

    cout << "    delta_pos for p0: " << particles[0].delta_pos << endl;

#define SURFACE_OFFSET 0.0001

    // collision
    // TODO: remove hard coded boundaries
    for (LiquidParticle &p : particles) {
      if (p.pos.y < 0)
        p.pos = p.prev_pos + 0.7 * (Vector3D(p.pos.x, SURFACE_OFFSET, p.pos.z) - p.prev_pos);
      if (p.pos.x < -0.2)
        p.pos = p.prev_pos + 0.7 * (Vector3D(-0.2 + SURFACE_OFFSET, p.pos.y, p.pos.z) - p.prev_pos);
      if (p.pos.x > 1.2)
        p.pos = p.prev_pos + 0.7 * (Vector3D(1.2 - SURFACE_OFFSET, p.pos.y, p.pos.z) - p.prev_pos);
      if (p.pos.z < -0.2)
        p.pos = p.prev_pos + 0.7 * (Vector3D(p.pos.x, p.pos.y, -0.2 + SURFACE_OFFSET) - p.prev_pos);
      if (p.pos.z > 1.2)
        p.pos = p.prev_pos + 0.7 * (Vector3D(p.pos.x, p.pos.y, 1.2 - SURFACE_OFFSET) - p.prev_pos);
    }

    cout << "    collision\n";

    // update position
    for (LiquidParticle &p : particles)
      p.pos += p.delta_pos;
  }

  cout << "delta_pos for particle 0: " << particles[0].prev_pos << ' ' << particles[0].pos << ' ' << particles[0].delta_pos << endl;

  // update velocity
  for (LiquidParticle &p : particles) {
    p.vel = (p.pos - p.prev_pos) / delta_t;
    p.prev_pos = p.pos;
  }
}

void Liquid::reset() {
  particles.clear();
  for (int t = 0; t < anchors.size(); ++t)
    for (int i = 0; i < sizes[t].x * params.granularity; ++i)
      for (int j = 0; j < sizes[t].y * params.granularity; ++j)
        for (int k = 0; k < sizes[t].z * params.granularity; ++k)
          this->particles.emplace_back(LiquidParticle(anchors[t] + (Vector3D(i, j, k) + 0.5) / params.granularity));
}

void Liquid::build_spatial_map() {
  for (const auto &entry : map)
    if (entry.second)
      entry.second->clear();

  // (Part 4): Build a spatial map out of all of the point masses.

  for (LiquidParticle &p : particles) {
    int hash = hash_position(p.pos);
    if (!map[hash])
      map[hash] = new vector<LiquidParticle *>();

    map[hash]->push_back(&p);
  }
}

#define HASH_P1 73856093
#define HASH_P2 19349663
#define HASH_P3 83492791
#define HASH_SIZE 99999989

int Liquid::hash_position(Vector3D pos) {
  int x = (int) std::floor(pos.x / params.kernel_radius);
  int y = (int) std::floor(pos.y / params.kernel_radius);
  int z = (int) std::floor(pos.z / params.kernel_radius);
  return (x * HASH_P1 ^ y * HASH_P2 ^ z * HASH_P3) % HASH_SIZE;
}

float inline Liquid::W(float r_2, float h) {
  // assumes r_2 < h * h
  return (float) (315 / (64 * PI * pow(h, 9)) * pow(pow(h, 2) - r_2, 3));
}

Vector3D inline Liquid::Spiky_grad(const Vector3D &r, float h) {
  // assumes r_2 < h * h
  return (float) (-45 / PI / pow(h, 6) * pow(h - r.norm(), 2)) * r.unit();
}

