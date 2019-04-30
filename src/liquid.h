//
// Created by Fanyu Meng on 2019-04-21.
//

#ifndef CLOTHSIM_LIQUID_H
#define CLOTHSIM_LIQUID_H

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <CGL/vector3D.h>
#include <nanogui/nanogui.h>

#include "misc/sphere_drawing.h"
#include "CGL/CGL.h"
#include "CGL/misc.h"
#include "collision/collisionObject.h"

using namespace CGL;
using namespace nanogui;
using namespace std;

struct LiquidParameters {
  LiquidParameters() = default;

  LiquidParameters(float granularity,
                   float particle_mass,
                   float kernel_radius,
                   float rest_density,
                   int density_iter,
                   float eps,
                   float s_corr,
                   float delta_q,
                   int pressure_pow,
                   float viscosity)
          : granularity(granularity),
            particle_mass(particle_mass),
            kernel_radius(kernel_radius),
            rest_density(rest_density),
            density_iter(density_iter),
            eps(eps),
            s_corr(s_corr),
            delta_q(delta_q),
            pressure_pow(pressure_pow),
            viscosity(viscosity) {}

  ~LiquidParameters() = default;

  float granularity = 10.0;
  float particle_mass = 1;
  float kernel_radius = 0.2;
  float rest_density = 350;
  int density_iter = 4;
  float eps = 600;
  float s_corr = 0.0001;
  float delta_q = 0.03;
  int pressure_pow = 4;
  float viscosity = 0.01;
};

struct LiquidParticle {
public:
  LiquidParticle() = default;

  explicit LiquidParticle(const Vector3D &pos) {
    this->pos = this->prev_pos = pos;
    this->vel = {0, 0, 0};
    this->m_sphere_mesh = Misc::SphereMesh(8, 8);
  }

  ~LiquidParticle() = default;

  Vector3D pos, prev_pos, delta_pos;
  Vector3D vel;
  float density, lambda;
  vector<LiquidParticle *> neighbors;

  Misc::SphereMesh m_sphere_mesh;
};

struct Liquid {
  Liquid() = default;

  ~Liquid() = default;

  vector<Vector3D> anchors, sizes;
  LiquidParameters params;

  vector<LiquidParticle> particles;

  void addLiquid(Vector3D anchor, Vector3D size, LiquidParameters params = {});
  void reset();
  void render(GLShader &shader);

  void simulate(double frames_per_sec, double simulation_steps,
                vector<Vector3D> external_accelerations,
                vector<CollisionObject *> *collision_objects);

  // spatial hashing
  unordered_map<int, vector<LiquidParticle *> *> map;

  void build_spatial_map();
  int hash_position(Vector3D pos);

  // smoothing kernel function
  static float W(float r_2, float h);
  static Vector3D Spiky_grad(const Vector3D &r, float h);
};

#endif //CLOTHSIM_LIQUID_H