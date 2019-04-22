#ifndef COLLISIONOBJECT
#define COLLISIONOBJECT

#include "../pointMass.h"
#include <nanogui/nanogui.h>

using namespace CGL;
using namespace std;
using namespace nanogui;

class CollisionObject {
public:
  virtual void render(GLShader &shader) = 0;
  virtual void collide(PointMass &pm) = 0;

private:
  double friction;
};

#endif /* COLLISIONOBJECT */
