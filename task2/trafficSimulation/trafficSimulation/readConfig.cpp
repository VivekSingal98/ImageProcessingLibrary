#include "readConfig.hpp"

void readConfigFile_RoadParam(ifstream& fin,map<string, float>& mapRoadParam){
    string line;
    while(getline(fin,line)) {
        istringstream iss(line);
        if(line.size()==0) {
            continue;
        }
        string word;
        iss >>word;
        if(word[0]=='#') {
            continue;
        }
        if(word=="END"){
            break;
        }
        string tmp;
        iss>>tmp; //= sign
        iss >> tmp;
        if(tmp!="") {
            mapRoadParam[word]=stof(tmp);
        }
    }
    mapRoadParam["Road_Width"]=mapRoadParam["Num_Lanes"]*mapRoadParam["Lane_Width"];
}

void readConfigFile_DefaultParam(ifstream& fin,map<string,float>& mapDefaultParam){
    string line;
    while(getline(fin,line)) {
        istringstream iss(line);
        if(line.size()==0) {
            continue;
        }
        string word;
        iss >>word;
        if(word[0]=='#') {
            continue;
        }
        if(word=="END"){
            break;
        }
        string tmp;
        iss>>tmp; //= sign
        iss >> tmp;
        if(tmp!="") {
            mapDefaultParam[word]=stof(tmp);
        }
    }
}

void readConfigFile_VehicleParam(ifstream& fin, string carType,map<string, float>& mapVehicleParam) {
    string line;
    while(getline(fin,line)) {
        istringstream iss(line);
        if(line.size()==0) {
            continue;
        }
        string word;
        iss >>word;
        if(word[0]=='#') {
            continue;
        }
        if(word=="END"){
            break;
        }
        string tmp;
        iss>>tmp; //= sign
        iss >> tmp;
        if(tmp!="") {
            string param=carType;
            param.append(word);
            mapVehicleParam[param]=stof(tmp);
        }
    }
}

void readConfigFile_Simulation(ifstream& fin,map<string, float>& mapVehicleParam,map<int,string>& signalFrame,map<int,int>& mapPass,vector<vehicle>& vehicleVector){
    string line;
    int frameConfig=0;
    int vehicleId=0;
    while(getline(fin,line)) {
        istringstream iss(line);
        if(line.size()==0) {
            continue;
        }
        string word;
        string tmp;
        iss >>word;
        if(word[0]=='#') {
            continue;
        }
        if(word=="END"){
            break;
        }
        if(word=="Signal") {
            iss >> tmp;
            signalFrame[frameConfig]=tmp;
        }
        else if(word=="Pass") {
            iss>>tmp;
            mapPass[frameConfig]=stoi(tmp);
            frameConfig+=stoi(tmp);
        }
        else {
            int col1=0,col2=0,col3=0;
            iss>>tmp;
            if(tmp!="") {
                col1=stoi(tmp);
            }
            iss>>tmp;
            if(tmp!="") {
                col2=stoi(tmp);
            }
            iss>>tmp;
            if(tmp!="") {
                col3=stoi(tmp);
            }
            vehicle v(mapVehicleParam[word+"Length"], mapVehicleParam[word+"Width"],
                      mapVehicleParam[word+"Speed"],col1,col2,col3,frameConfig);
            v.id=vehicleId;
            vehicleId++;
            vehicleVector.push_back(v);
            frameConfig++;
        }
    }
}


void setConstants(map<string, float>& mapVehicleParam,map<string, float>& mapRoadParam,map<string,float>& mapDefaultParam,map<int,string>& signalFrame,map<int,int>& mapPass,vector<vehicle>& vehicleVector) {
    
    mapRoadParam["Road_Length"]=30;
    mapRoadParam["Road_Signal"]=0;
    mapRoadParam["Road_Width"]=5;
    mapDefaultParam["Default_MaxSpeed"]=1;
    mapDefaultParam["Default_Acceleration"]=1;
    
    ifstream fin;
    fin.open("config.ini");
    if(!fin.is_open()) {
        cout<<"file not opened";
    }
    string line;
    while (getline(fin,line) ) {
        istringstream iss(line);
        string word;
        iss >>word;
        if(word[0]=='#') {
            continue;
        }
        if(word=="Road_Param") {
            readConfigFile_RoadParam(fin,mapRoadParam);
        }
        else if(word=="Default_Param") {
            readConfigFile_DefaultParam(fin, mapDefaultParam);
        }
        else if(word=="Vehicle_Type") {
            string tmp;
            iss>>tmp;
            iss>>tmp;
            readConfigFile_VehicleParam(fin,tmp,mapVehicleParam);
        }
        else if(word=="START") {
            readConfigFile_Simulation(fin,mapVehicleParam,signalFrame,mapPass,vehicleVector);
        }
    }
    
    fin.close();
}
