#include <iostream>
#include <boost/algorithm/string.hpp>
#include <iomanip> // setprecision
#include <string> 
#include <vector> 
#include <fstream>
#include <cmath>
#include <sstream>
#include <Eigen>
#include <dirent.h>
#include <Dense>
#include <Geometry> 

using namespace Eigen;
using namespace std;

string modelToWorld::makeFileName(string filename, string folder){
    int count=0;
    ostringstream strS;
    filename= filename.substr(0,(filename.length()-4));
    strS<<filename;
    strS<<"_mw"<<fixed<<setw(2)<<setfill('0')<<count;
    strS<<".obj";
    filename = strS.str();
    DIR* dirp = opendir(folder.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
       if(filename==dp->d_name){
           count++;
           ostringstream objS;
            filename= filename.substr(0,(filename.length()-6));
            objS<<filename;
            objS<<fixed <<setw(2)<<setfill('0')<<count;
            objS<<".obj";
            filename = objS.str();
        }
    }
    closedir(dirp);
    return filename;
}

string modelToWorld::charToString(string name){
    return name;
}


string modelToWorld::transformObjectFile(Matrix4f transform, string filename, string folder){
    string line;
    string newFilename = makeFileName(filename, folder);
    ofstream out(newFilename.c_str());
    
    ifstream in(filename);
    while (getline(in, line)){
            vector<string> word;
            MatrixXf objLine(4,1);
            boost::split(word, line, [](char c){return c == ' ';});
            if(word[0]=="v"){
                for(int i =0; i<3; i++){
                    objLine(i)=stod(word[i+1]);
                }
                objLine(3)=1;
            objLine = transform*objLine;
            ostringstream strs;
            for(int i =0; i<3; i++){
                    strs <<fixed << setprecision(6)<< objLine(i);
                    strs<<" ";
                }
            string str = strs.str();
            out<<"v "<<str<<endl;
            }else if(word[0]!="vn"&&word[0]!="s"){
                out<<line<<endl;
            }
        }//
    
    return newFilename;
}

MatrixXf modelToWorld::rotateByAxis(double radians){
    MatrixXf ZRotation;
    ZRotation.setIdentity(4, 4);

    double ca = cos(radians);
    double sa = sin(radians);
    ZRotation(0,0) = ca; ZRotation(0,1) = -sa;
    ZRotation(1,0) = sa; ZRotation(1,1) = ca;
    
    return ZRotation;
}

MatrixXf modelToWorld::createRotation(Vector3d wAxis, double degrees){//revise!
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
    cout<<"Z:\n"<<ZRotation<<"\n";
    cout<<"T:\n"<<TRotation<<"\n";
    cout<<"normal\n"<<rotation<<"\n";
    rotation = TRotation*ZRotation*rotation;
    return rotation;
}

void modelToWorld::transform(string line){
    if(line[0]!='#'){
        //initializing the transformation matrices
        vector<string> results;
        boost::split(results, line, [](char c){return c == ' ';});//splitting the lines to a vectsystem(strcpy(char_a,command.c_str()));or of string words
        description =results[0];
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
        string objFile =transformObject(transform,filename,folder);
        command = "mv " + objFile + " ./" + folder;
        system(strcpy(char_a,command.c_str()));
        
    }
        
    
}
