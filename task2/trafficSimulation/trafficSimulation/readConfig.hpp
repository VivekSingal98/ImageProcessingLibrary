#ifndef readConfig_hpp
#define readConfig_hpp

#include <stdio.h>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "vehicle.hpp"

using namespace std;

void readConfigFile_RoadParam(ifstream& fin,map<string, float>& mapRoadParam);
void readConfigFile_DefaultParam(ifstream& fin,map<string,float>& mapDefaultParam);
void readConfigFile_VehicleParam(ifstream& fin, string carType,map<string, float>& mapVehicleParam);
void readConfigFile_Simulation(ifstream& fin,map<int,string>& signalFrame,map<int,int>& mapPass,vector<vehicle>& vehicleVector);
void setConstants(map<string, float>& mapVehicleParam,map<string, float>& mapRoadParam,map<string,float>& mapDefaultParam,map<int,string>& signalFrame,map<int,int>& mapPass,vector<vehicle>& vehicleVector);
void setConstants_Background(map<string, float>& mapRoadParam);

#endif /* readConfig_hpp */
