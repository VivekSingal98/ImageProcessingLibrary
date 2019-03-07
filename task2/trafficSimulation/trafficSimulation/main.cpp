#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <GL/glut.h>
#include<iostream>//for strlen
#include<string>
#include <fstream>
#include <sstream>
#include "readConfig.hpp"
#include "background.hpp"
#include "constants.hpp"
#include "utility.hpp"
#include "vehicle.hpp"

using namespace std;

vector<vehicle> vehicleVector;

void drawTraffic()//car for racing
{
    glMatrixMode(GL_MODELVIEW);     // To operate on Model-View matrix
    for(vehicle& v: vehicleVector){
        if(v.inTime<=frameSim) {
            glLoadIdentity();// Reset the model-view matrix
            //            cout<<v.xPos;
            glTranslatef(v.xPos, v.yPos, 0.0f);
            
            glBegin(GL_QUADS);
            glColor3f(v.color[0],v.color[1],v.color[2]);
            glVertex2f(-v.length/2, v.width/2);
            glVertex2f(v.length/2, v.width/2);
            glVertex2f(v.length/2, -v.width/2);
            glVertex2f(-v.length/2, -v.width/2);
            glEnd();
            if(!(v.xPos==0 && isRed==1)) {
                v.updatePos();
            }
        }
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
    setConstants(mapVehicleParam,mapRoadParam,mapDefaultParam,signalFrame,mapPass,vehicleVector);
    setConstants_Background();
    float yPos=mapRoadParam["Road_Width"]/2;
    for(vehicle& v: vehicleVector){
        yPos=yPos-v.width/2;
        if(yPos<-mapRoadParam["Road_Width"]/2+v.width/2) {
            yPos=mapRoadParam["Road_Width"]/2-v.width/2;
        }
        v.yPos=yPos;
        yPos=yPos-v.width/2;
        v.xPos=-mapRoadParam["Road_Length"]/2;
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
