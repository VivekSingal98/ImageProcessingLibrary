#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <GL/glut.h>
#include<string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "background.hpp"
#include "readConfig.hpp"

using namespace std;

static map<string, float> mapRoadParam;

void setConstants_Background() {
    
    mapRoadParam["Road_Length"]=30;
    mapRoadParam["Road_Signal"]=0;
    mapRoadParam["Road_Width"]=5;
    
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
            break;
        }
        
    }
    
    fin.close();
}

void redLight(bool light,int frameNum)//draw the text for score and game over
{
    std::string ch="RED";
    if(light==false) {
        ch="GREEN";
    }
    ch=ch+" "+to_string(frameNum);
    int numofchar = (int)ch.length();
    glLoadIdentity ();
    float xpos=mapRoadParam["Road_Signal"];
    float ypos=mapRoadParam["Road_Width"]*1.2/2.0;
    glRasterPos2f( xpos , ypos);
    for (int i = 0; i <= numofchar - 1; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ch[i]);//font used here, may use other font also
    }
}

void drawRoad()
{
    glMatrixMode(GL_MODELVIEW);     // To operate on Model-View matrix
    glLoadIdentity();               // Reset the model-view matrix
    glBegin(GL_QUADS);
    glColor3f(0.5,0.5,0.5);
    float xPos=mapRoadParam["Road_Length"]/2.0;
    float yPos=mapRoadParam["Road_Width"]/2.0;
    glVertex2f(-xPos, yPos);
    glVertex2f(xPos, yPos);
    glVertex2f(xPos, -yPos);
    glVertex2f(-xPos, -yPos);
    glEnd();
    glPushAttrib(GL_ENABLE_BIT); // glPushAttrib is done to return everything to normal after drawing
    glColor3f(0,0,0);
    glLineStipple(5, 0x9999);
    glEnable(GL_LINE_STIPPLE);
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(-xPos,0);
    glVertex2f(xPos,0);
    glEnd();
    glPopAttrib();
}
