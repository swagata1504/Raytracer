#include "Raytracer.h"

using namespace Eigen;

Face::Face(const Vector3d a,const Vector3d b,const Vector3d c){
    A = a;
    B = b;
    C = c;
    AB = A - B;
    AC = A - C;
}
