#include "vehicle.hpp"

vehicle::vehicle(float len,float wid,float spee,int col1,int col2,int col3,float tim) {
    length=len;
    width=wid;
    speed=spee;
    color=new int[3];
    color[0]=col1;
    color[1]=col2;
    color[2]=col3;
    inTime=tim;
    xPos=0;
    yPos=0;
}

void vehicle::updatePos() {
    xPos+=speed;
}

