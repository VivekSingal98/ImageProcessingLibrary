#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <GL/glut.h>
#include<iostream>//for strlen
#include<string>
#include <fstream>
#include <sstream>
#include <cmath>
#include "readConfig.hpp"
#include "background.hpp"
#include "constants.hpp"
#include "utility.hpp"
#include "vehicle.hpp"

using namespace std;

vector<vehicle> vehicleBuffer;
vector<vector<vehicle>> vehiclesOnLane;
int** mesh;


void drawTraffic();
void checkFilled(float xpos,float ypos);
void setZeroMeshVehicle(vehicle& v);
void fillMeshVehicle(vehicle& v);
void updateTraffic();
void init();
void display();
void Timer(int value);


void drawTraffic()//car for racing
{
    glMatrixMode(GL_MODELVIEW);     // To operate on Model-View matrix
    for(int i=0;i<(int)mapRoadParam["Num_Lanes"];i++) {
        for(int j=0;j<vehiclesOnLane[i].size();j++) {
            vehicle& v=vehiclesOnLane[i][j];
            glLoadIdentity();// Reset the model-view matrix
            glTranslatef(v.xPos, v.yPos, 0.0f);
            glBegin(GL_QUADS);
            glColor3f(v.color[0],v.color[1],v.color[2]);
            glVertex2f(-v.length/2, v.width/2);
            glVertex2f(v.length/2, v.width/2);
            glVertex2f(v.length/2, -v.width/2);
            glVertex2f(-v.length/2, -v.width/2);
            glEnd();
        }
    }
}
bool checkFilled(float xpos,float ypos,vehicle& v) {
    setZeroMeshVehicle(v); //deleting v's position on mesh
 
    float trans_X=xpos+mapRoadParam["Road_Length"]/2;
    float trans_Y=mapRoadParam["Road_Width"]/2-ypos;
    int left_X=(int)floor(trans_X-v.length/2);
    int right_X=(int)ceil(trans_X+v.length/2);
    int up_Y=(int)floor(trans_Y-v.width/2);
    int down_Y=(int)ceil(trans_Y+v.width/2);
    
    for(int i=up_Y;i<down_Y;i++) {
        for(int j=left_X;j<right_X;j++) {
            if(j<mapRoadParam["Road_Length"] && j>=0 && i<mapRoadParam["Road_Width"] && i>=0 && mesh[i][j]==1) {
                fillMeshVehicle(v);
                return true; //position already occupied by another vehicle
            }
        }
    }
    fillMeshVehicle(v); //filling v's position again
    return false;
}
bool checkFilled1(float xpos,float ypos,vehicle& v) {

    float trans_X=xpos+mapRoadParam["Road_Length"]/2;
    float trans_Y=mapRoadParam["Road_Width"]/2-ypos;
    int left_X=(int)floor(trans_X-v.length/2);
    int right_X=(int)ceil(trans_X+v.length/2);
    int up_Y=(int)floor(trans_Y-v.width/2);
    int down_Y=(int)ceil(trans_Y+v.width/2);
    for(int i=up_Y;i<down_Y;i++) {
        for(int j=left_X;j<right_X;j++) {
            if(j<mapRoadParam["Road_Length"] && j>=0 && i<mapRoadParam["Road_Width"] && i>=0 && mesh[i][j]==1) {
                return true; //position already occupied by another vehicle
            }
        }
    }
    return false;
}

void setZeroMeshVehicle(vehicle& v) {
    float trans_X=v.xPos+mapRoadParam["Road_Length"]/2;
    float trans_Y=mapRoadParam["Road_Width"]/2-v.yPos;
    int left_X=(int)floor(trans_X-v.length/2);
    int right_X=(int)ceil(trans_X+v.length/2);
    int up_Y=(int)floor(trans_Y-v.width/2);
    int down_Y=(int)ceil(trans_Y+v.width/2);
    
    for(int i=up_Y;i<down_Y;i++) {
        for(int j=left_X;j<right_X;j++) {
            if(j<mapRoadParam["Road_Length"] && j>=0 && i<mapRoadParam["Road_Width"] && i>=0) {
                mesh[i][j]=0;
            }
        }
    }
    
}

void fillMeshVehicle(vehicle& v) {
    float trans_X=v.xPos+mapRoadParam["Road_Length"]/2;
    float trans_Y=mapRoadParam["Road_Width"]/2-v.yPos;
    int left_X=(int)floor(trans_X-v.length/2);
    int right_X=(int)ceil(trans_X+v.length/2);
    int up_Y=(int)floor(trans_Y-v.width/2);
    int down_Y=(int)ceil(trans_Y+v.width/2);
    
    for(int i=up_Y;i<down_Y;i++) {
        for(int j=left_X;j<right_X;j++) {
            if(j<mapRoadParam["Road_Length"] && j>=0 && i<mapRoadParam["Road_Width"] && i>=0) {
                mesh[i][j]=1;
            }
        }
    }
        
}

bool checkVehicleInMesh(vehicle& v) {
    float trans_X=v.xPos+mapRoadParam["Road_Length"]/2;
    float trans_Y=mapRoadParam["Road_Width"]/2-v.yPos;
    int left_X=(int)floor(trans_X-v.length/2);
    int right_X=(int)ceil(trans_X+v.length/2);
    int up_Y=(int)floor(trans_Y-v.width/2);
    int down_Y=(int)ceil(trans_Y+v.width/2);
    
    for(int i=up_Y;i<down_Y;i++) {
        for(int j=left_X;j<right_X;j++) {
            if(j<mapRoadParam["Road_Length"] && j>=0 && i<mapRoadParam["Road_Width"] && i>=0) {
                return true;
            }
        }
    }
    return false;
}

void updateTraffic(bool printMatrixTerminal) {
    
    if(printMatrixTerminal) {
        cout<<frameSim<<endl;
        printMatrix(mesh,mapRoadParam["Road_Width"],mapRoadParam["Road_Length"]);
    }
    //moving vehicles forward and updating mesh
    for(int i=0;i<mapRoadParam["Num_Lanes"];i++) {
        for(int j=0;j<vehiclesOnLane[i].size();j++) {
            vehicle& v=vehiclesOnLane[i][j];
            //check if red light then dont move
            if(abs(v.xPos-mapRoadParam["Road_Signal"])<1 && isRed==true) {
                continue; //red light
            }
            
            float newX=v.xPos+v.speed;
            
            //check if car new's position is not going over the car in front
            if(j!=0 && newX>vehiclesOnLane[i][j-1].xPos-vehiclesOnLane[i][j-1].length/2) {
                continue;
            }
            //check if newX position is filled or not because of cars in front
            if(checkFilled(newX, v.yPos,v)) {
                continue;
            }
            
            setZeroMeshVehicle(v);
            v.xPos=newX;
            //check if vehicle has driven out of window
            if(checkVehicleInMesh(v)==false) {
                vehiclesOnLane[i].erase(vehiclesOnLane[i].begin()+j);
                j--;
            }
            else {
                fillMeshVehicle(v);
            }
            
        }
    }
    
    
    //add vehicles in buffer to vehicles on lane
    for(int i=0;i<vehicleBuffer.size();i++) {
        
        vehicle& v=vehicleBuffer[i];
        //check if v has entered yet
        if(v.inTime>frameSim) {
            continue;
        }
        v.xPos=-mapRoadParam["Road_Length"]/2;
        bool noentry=true;
        int laneNo=0;
        
        //choosing the lane with last vehicle farthest away
        int maxDist=-1;
        for(int i=0;i<mapRoadParam["Num_Lanes"];i++) {
            int lastVehicleLane_xpos=mapRoadParam["Road_Length"]/2;
            if(vehiclesOnLane[i].size()>0) {
                lastVehicleLane_xpos=vehiclesOnLane[i][vehiclesOnLane[i].size()-1].xPos;
            }
            if(checkFilled1(-mapRoadParam["Road_Length"]/2,mapRoadParam["Road_Width"]/2-mapRoadParam["Lane_Width"]*i-v.width/2,v)==false && maxDist<lastVehicleLane_xpos-v.xPos) {
                noentry=false;
                laneNo=i;
                maxDist=lastVehicleLane_xpos-v.xPos;
            }
        }
        if(noentry==true) {
            break; //road entrance is full
        }

        v.laneNo=laneNo;
        v.yPos=mapRoadParam["Road_Width"]/2-mapRoadParam["Lane_Width"]*laneNo-v.width/2;
        v.laneId=(int)vehiclesOnLane[laneNo].size();
        vehiclesOnLane[laneNo].push_back(v);
        fillMeshVehicle(v);
        vehicleBuffer.erase(vehicleBuffer.begin()+i);
        i--;

    }
    
}

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Black and opaque
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float sizeX=mapRoadParam["Road_Length"]/2.0;
    float sizeY=mapRoadParam["Road_Width"]*1.5/2.0;
    gluOrtho2D(-sizeX,sizeX,-sizeY,sizeY);
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawRoad();
    redLight(isRed,frameSim);
    updateTraffic(false);
    drawTraffic();
    glFlush();  // Render now
}

void Timer(int value) {
    frameSim++;
    //changing signal if it is to be changed
    if(signalFrame.find(frameSim)!=signalFrame.end()) {
        if(signalFrame[frameSim]=="RED") {
            isRed=true;
        }
        else {
            isRed=false;
        }
    }
    glutPostRedisplay();      // Post re-paint request to activate display()
    glutTimerFunc(refreshMills, Timer, 0); // next Timer call milliseconds later
}


int main(int argc, char **argv)
{
    setConstants(mapVehicleParam,mapRoadParam,mapDefaultParam,signalFrame,mapPass,vehicleBuffer);
    setConstants_Background();
    mesh = createMatrix(mapRoadParam["Road_Width"],mapRoadParam["Road_Length"]);
    for(int i=0;i<(int)mapRoadParam["Num_Lanes"];i++) {
        vehiclesOnLane.push_back(*(new vector<vehicle>()));
    }
    glutInit(&argc,argv);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(mapRoadParam["Road_Length"]*30,mapRoadParam["Road_Width"]*30);
    glutCreateWindow("Traffic simulation");
    init();
    glutDisplayFunc(display);
    frameSim=-1;
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    
    
    return 0;
}
