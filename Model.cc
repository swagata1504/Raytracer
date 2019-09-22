#include <iostream>
#include <boost/algorithm/string.hpp>
//#include <iomanip> // setprecision
#include <string> 
#include <vector> 
#include <fstream>
#include <cmath>
#include <sstream>
//#include <Eigen>
//#include <dirent.h>
//#include <Dense>
//#include <Geometry> 
#include "Raytracer.h"

using namespace Eigen;
using namespace std;

Model::Model(const string& word){
    line = word;
}

Model::~Model(){}

void Model::getFace(){
    double face;
    double normal;
    char slash1, slash2;
    string sentance;
    ifstream in(filename);
    while (getline(in, sentance)){
        vector<string> word;
        boost::split(word, sentance, [](char c){return c == ' ';});
        if(word[0]=="f"){
            vector<Vector3d> faceVector;
            for(int i =0; i<3; i++){
                stringstream ss;
                ss<<word[i+1];
                ss>>face;
                faceVector.push_back(transformedPoints[face-1]);
                ss>>slash1;ss>>slash2;
                ss>>normal;
                Normals.push_back(VN[normal-1]);
            }
            Face faceObj(faceVector[0],faceVector[1],faceVector[2]);
            Faces.push_back(faceObj);
        }

    }
    
}

void Model::calNormal(){
    for(size_t i = 0; i !=  Faces.size(); i++){
        Vector3d AB = (Faces[i].B- Faces[i].A);
        Vector3d BC = (Faces[i].C - Faces[i].B);
        Vector3d normal = AB.cross(BC).normalized();
        calculatedNormals.push_back(normal);    
    }
    
}


void Model::getTransformed(Matrix4f transform){
    string sentance;
    ifstream in(filename);
    ofstream out("objDrawn.obj");
    while (getline(in, sentance)){
            vector<string> word;
            MatrixXf objLine(4,1);
            Vector3d transformedVector;
            boost::split(word, sentance, [](char c){return c == ' ';});
            if(word[0]=="v"){
                for(int i =0; i<3; i++){
                    objLine(i)=stod(word[i+1]);
                }
                objLine(3)=1;
                objLine = transform*objLine;  
                for(int i =0; i<3; i++){
                    transformedVector[i] = objLine(i);
                }
                transformedPoints.push_back(transformedVector);
            }else if(word[0]=="vn"){
                Vector3d VertexNormal;
                for(int i =0; i<3; i++){
                    VertexNormal[i]=stod(word[i+1]);
                }
                VN.push_back(VertexNormal);
            }else if(word[0]=="mtllib"){
                materialFile = word[1];
            }
    }
   
    
}

Vector3d Model::getAmbiance(){
    ifstream in(materialFile);
    string sentance;
    Vector3d ambiance;
    while (getline(in, sentance)){
        vector<string> word;
        boost::split(word, sentance, [](char c){return c == ' ';});
        if(word[0]=="Ka"){
            for(int i =0; i<3; i++){
                ambiance[i] = stod(word[i+1]);
            }
        }
    }
    return ambiance;
}

Vector3d Model::getDiffuse(){
    ifstream in(materialFile);
    string sentance;
    Vector3d diffuse;
    while (getline(in, sentance)){
        vector<string> word;
        boost::split(word, sentance, [](char c){return c == ' ';});
        if(word[0]=="Kd"){
            for(int i =0; i<3; i++){
                diffuse[i] = stod(word[i+1]);
            }
        }
    }
    return diffuse;
}

MatrixXf Model::rotateByAxis(double& radians){
    MatrixXf ZRotation;
    ZRotation.setIdentity(4, 4);

    double ca = cos(radians);
    double sa = sin(radians);
    ZRotation(0,0) = ca; ZRotation(0,1) = -sa;
    ZRotation(1,0) = sa; ZRotation(1,1) = ca;
    
    return ZRotation;
}


MatrixXf Model::createRotation(Vector3d& wAxis, double& degrees){//revise!
    MatrixXf rotation;
    rotation.setIdentity(4, 4);
    
    Vector3d normWAxis = wAxis.normalized(); 
    
    Vector3d mAxis;
    mAxis<<1,0,0;
    if(mAxis.dot(normWAxis)!=0.0){
        mAxis[0]=0;
        mAxis[1]=1;
    }
    
    Vector3d uAxis= normWAxis.cross(mAxis).normalized();
    Vector3d vAxis = normWAxis.cross(uAxis).normalized();
    
    for(int i=0;i<3;i++){
        for(int j =0; j<3;j++){
            if(i==0){rotation(i,j)= uAxis[j];}
            else if(i==1){rotation(i,j)= vAxis[j];}
            else{rotation(i,j)= normWAxis[j];}            
        }
    }
    MatrixXf TRotation = rotation.transpose(); 
    double radians = (degrees*M_PI)/180.0;
    MatrixXf ZRotation = rotateByAxis(radians);
    rotation = TRotation*ZRotation*rotation;
    return rotation;
}

MatrixXf Model::transform(){
    //initializing the transformation matrices
    Vector3d wAxis;
    MatrixXf rotation;
    rotation.setIdentity(4, 4);
    
    double degrees;
    
    MatrixXf scaling;
    scaling.setIdentity(4, 4);
    
    MatrixXf translate;
    translate.setIdentity(4, 4);
    MatrixXf transform;
    
    vector<string> results;
    boost::split(results, line, [](char c){return c == ' ';});//splitting the lines to a vector of string words
    for(int i =0; i < 3;i++){
        wAxis[i] = stod(results[i+1]);
    }
    degrees = stod(results[4]);
    rotation = createRotation(wAxis,degrees);
    for(int j =0; j < 3;j++){
        scaling(j,j)=stod(results[5]);
    }
    for(int k =0; k <3;k++){
        translate(k,3)=stod(results[k+6]);
    }
    //slap them together!
    transform = translate*rotation*scaling;
    filename = results[9];
    getTransformed(transform);
    getFace();
    calNormal();
    return transform;
}
