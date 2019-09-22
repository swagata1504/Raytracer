#include <iostream>
#include <fstream>
#include <string> 
#include <sstream>

#include "Raytracer.h"
#include <vector> 
//#include <Eigen/Dense>

/*#include <boost/algorithm/string.hpp>
#include <iomanip> // setprecision

#include <cmath>
#include <dirent.h>
#include <Dense>
#include <Geometry> */

using namespace Eigen;
using namespace std;

int main(int argc, char* argv[]){
    if(argc<2){cerr<<"enter file";}
    string line;
    double ex, ey, ez;
    double lx, ly, lz;
    double upx, upy, upz;
    double left, bottom, right, top;
    double width, height;
    double ambientR, ambientG, ambientB;
    double near;
    Vector3d EV;
    Vector3d LV;
    Vector3d UPV;
    Vector3d ambient;
    vector<double> info;
    vector<Light> lights;
    vector<Model> models;
    ifstream in(argv[1]);
    if(!in.is_open()){
        cerr<<"Error in opening the "<<argv[1]<<" file.\n";
        return -1;
    }
    while (getline(in, line)){
        if(line[0]!='#'){
            stringstream ss;
            ss<<line;
            string label;
            ss>>label;
            if(label=="eye"){
                ss>>ex>>ey>>ez; 
                EV<<ex,ey,ez;
            }else if(label == "look"){
                ss>>lx>>ly>>lz;
                LV<<lx,ly,lz;
            }else if(label=="up"){
                ss>>upx>>upy>>upz;
                UPV<<upx,upy,upz;
            }else if(label == "d"){
                ss>>near;
                info.push_back(-1*near);
            }
            else if(label=="bounds"){
                ss>>left>>bottom>>right>>top;
                info.push_back(left);
                info.push_back(bottom);
                info.push_back(right);
                info.push_back(top);
            }else if(label=="res"){
                ss>>width>>height;
                info.push_back(width);
                info.push_back(height);
            }else if(label=="ambient"){
                ss>>ambientR>>ambientG>>ambientB;
                ambient<<ambientR,ambientG,ambientB;
            }else if(label=="light"){
                double px,py,pz,pw;
                double er,eg,eb;
                ss>>px>>py>>pz>>pw>>er>>eg>>eb;
                Vector3d position, emission;
                position<<px,py,py;
                emission<<er,eg,eb;
                
                Light light(position, emission);
                
                lights.push_back(light);
            }else if(label=="model"){
                Model model(line);  
                model.transform();
                models.push_back(model);
            }
        }
        
    }
    
    Pixel pixels(EV, LV, UPV, info, models,lights, ambient);
    pixels.generateObjPoints();
}
