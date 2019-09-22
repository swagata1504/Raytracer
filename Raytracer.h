#ifndef RAYCAPTURE_H_INCLUDED
#define RAYCAPTURE_H_INCLUDED
#include "Eigen/Dense"
#include <string> 
#include <vector> 

class Light{
public:
    //Light(); //default constructor 
    Light(const Eigen::Vector3d&, const Eigen::Vector3d&);
    Eigen::Vector3d position;
    Eigen::Vector3d emission;
    ~Light();

};
class Face{
public:
    Face(const Eigen::Vector3d,const Eigen::Vector3d,const Eigen::Vector3d);
    
    Eigen::Vector3d A;
    Eigen::Vector3d B;
    Eigen::Vector3d C;
    Eigen::Vector3d AB;
    Eigen::Vector3d AC;
};
class Model{
public:
    Model(const std::string&);
    ~Model();
    
    Eigen::MatrixXf rotateByAxis(double&);
    Eigen::MatrixXf createRotation(Eigen::Vector3d&, double&);
    Eigen::MatrixXf transform();
    Eigen::Vector3d getAmbiance();
    Eigen::Vector3d getDiffuse();

    void getTransformed(Eigen::Matrix4f);
    void getFace();
    void calNormal();
    std::vector<Eigen::Vector3d> transformedPoints;
    std::vector<Face> Faces;
    std::vector<Eigen::Vector3d> Normals;
    std::vector<Eigen::Vector3d> calculatedNormals;
    std::string filename;
    std::string materialFile;
private:
    std::string line;
    std::vector<Eigen::Vector3d> VN;

};
class Pixel{
public:
    Pixel(const Eigen::Vector3d&, const Eigen::Vector3d&, const Eigen::Vector3d&, const std::vector<double>&,const std::vector<Model>&,const std::vector<Light>&,const Eigen::Vector3d&);
    ~Pixel();
    
    void generateObjPoints();
    double calculateT(Eigen::Vector3d&, Model&, Eigen::Vector3d&);
    void pixelRay();
    Eigen::Vector3d generateObject(Model&, Eigen::Vector3d&);
    Eigen::Vector3d pixelPoint(int, int);
    std::vector<Eigen::Vector3d> points;
    
private:
    Eigen::Vector3d EV;
    Eigen::Vector3d LV;
    Eigen::Vector3d UPV;
    Eigen::Vector3d ambient;
    int intersectFace;
    double near;
    double left;
    double bottom;
    double right;
    double top;
    double width;
    double height;
    std::vector<Model> models;
    std::vector<Light> lights;
};
#endif 
