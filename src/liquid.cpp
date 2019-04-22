//
// Created by Fanyu Meng on 2019-04-21.
//

#include "liquid.h"

using namespace nanogui;

Liquid::Liquid(Vector3D size, float granularity, LiquidParameters params) {
  this->size = size;
  this->granularity = granularity;
  this->params = params;

  for (int i = 0; i < size.x * granularity; ++i)
    for (int j = 0; j < size.y * granularity; ++j)
      for (int k = 0; k < size.z * granularity; ++k)
        this->particles.emplace_back(LiquidParticle((Vector3D(i, j, k) + 0.5) / granularity));
}

void Liquid::render(GLShader &shader) {
  for (LiquidParticle &p : particles)
    p.m_sphere_mesh.draw_sphere(shader, p.pos, 1 / granularity / 2 * 0.92);
}

void Liquid::simulate(double frames_per_sec, double simulation_steps,
                      vector<Vector3D> external_accelerations,
                      vector<CollisionObject *> *collision_objects) {

}

void Liquid::reset() {
  particles.clear();
  for (int i = 0; i < size.x / granularity; ++i)
    for (int j = 0; j < size.y / granularity; ++j)
      for (int k = 0; k < size.z / granularity; ++k)
        this->particles.emplace_back(LiquidParticle((Vector3D(i, j, k) + 0.5) / granularity));
}
