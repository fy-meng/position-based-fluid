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

}

void Liquid::reset() {
  particles.clear();
  for (int t = 0; t < anchors.size(); ++t)
    for (int i = 0; i < sizes[t].x / params.granularity; ++i)
      for (int j = 0; j < sizes[t].y / params.granularity; ++j)
        for (int k = 0; k < sizes[t].z / params.granularity; ++k)
          this->particles.emplace_back(LiquidParticle(anchors[t] + (Vector3D(i, j, k) + 0.5) / params.granularity));
}
