#ifndef constants_h
#define constants_h
#include <map>
#include<vector>

using namespace std;


static bool isRed=true;
static int refreshMills=200; //in sec
static int frameSim=-1;


static map<string, float> mapVehicleParam;
static map<string, float> mapRoadParam;
static map<string,float> mapDefaultParam;
static map<int,string> signalFrame;
static map<int,int> mapPass;

#endif /* constants_h */
