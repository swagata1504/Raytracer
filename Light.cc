#include "Raytracer.h"

using namespace Eigen;

Light::Light(const Vector3d& p, const Vector3d& e){
    position = p;
    emission = e;
} 

Light::~Light(){}
