#include <boost/algorithm/string.hpp>
#include <string> 
#include <fstream>
#include <cmath>
#include "Raytracer.h"

using namespace Eigen;
using namespace std;

Pixel::Pixel(const Vector3d& eye, const Vector3d& look, const Vector3d& up, const vector<double>& info, const vector<Model>& model,const vector<Light>& light, const Vector3d& amb){
    EV= eye;
    LV = look;
    UPV = up;
    near = info[0];
    left = info[1];
    bottom = info[2];
    right = info[3];
    top = info[4];
    width = info[5];
    height  = info[6];
    models = model;
    lights = light;
    ambient = amb;
}

Pixel::~Pixel(){}

void Pixel::generateObjPoints(){
        pixelRay(); 
}
void Pixel::pixelRay(){
     ofstream out("RenderedImage.ppm");
     out<<"P3\n";
     out<<height<<" "<<width<<" 255\n";
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            Vector3d point = pixelPoint(i,j);
            Vector3d direction = point-EV;
            Vector3d normDirection = direction.normalized();
            double t = calculateT(point, models[0], normDirection);
             if(t!=1000){
                 Vector3d surfacePoint = point+t*direction;
                 Vector3d color = generateObject(models[0],surfacePoint);
                 out<<(int)(255*color[0])<<" "<<(int)(255*color[1])<<" "<<(int)(255*color[2])<<" ";
             }else{
                 out<<"0 0 0 ";
             }           
        }
        out << "\n";
    }
}

Vector3d Pixel::generateObject(Model& model, Vector3d& surfacePoint){
    Vector3d modelAmbience = model.getAmbiance();
    Vector3d color;
    color<<modelAmbience[0]*ambient[0],modelAmbience[1]*ambient[1],modelAmbience[2]*ambient[2];
    for(size_t i = 0; i != lights.size(); i++){
        Vector3d L = lights[i].position - surfacePoint;
        Vector3d normL = L.normalized();
        Vector3d N = model.calculatedNormals[intersectFace];
        double NdotL = N.dot(normL);
        if(!(NdotL>0)){
            NdotL=(-1)*NdotL;
        }
        color += model.getDiffuse().cwiseProduct(lights[i].emission) * NdotL;
    }
    return color;
    
}
Vector3d Pixel::pixelPoint(int i, int j){
    Vector3d WV, UV, VV, normWV;
    WV = EV - LV; 
    normWV = WV.normalized();
    UV = UPV.cross(normWV).normalized();    
    VV = normWV.cross(UV).normalized(); 

    double px = ((double)i/(width-1))*(right-left)+left;
    double py = ((double)j/(height-1))*(top-bottom)+bottom;
    Vector3d pixelPt = EV + (near * normWV) + (px * UV) + (py * VV);
    return pixelPt;
}

double Pixel::calculateT(Vector3d& point, Model& mod, Vector3d& direction){
    double minT=1000;  
        for(size_t i = 0; i !=  mod.Faces.size(); i++){
            Vector3d PointToA = mod.Faces[i].A - point;
            Matrix3d MM;
            for(int b=0; b<3;b++){
                MM(b,0) = mod.Faces[i].AB[b];
                MM(b,1) = mod.Faces[i].AC[b];
                MM(b,2) = direction[b];
            }
            Vector3d solution = MM.colPivHouseholderQr().solve(PointToA);
            double beta = solution(0);
            double gamma = solution(1);
            double t = solution(2);
           if(beta>=0 && gamma>=0&&(beta+gamma)<=1){
                if(t < minT){
                    minT = t;
                    intersectFace = i;
            }
        }
    } 
    return minT;

}
