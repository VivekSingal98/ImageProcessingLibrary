#include <stdio.h>
#include <GL/glut.h>
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;
//----------------------
int** createMatrix(int m,int n) {
    int** matrix=new int*[m];
    for(int i=0;i<m;i++) {
        matrix[i]=new int[n];
        for(int j=0;j<n;j++){
          matrix[i][j] = 0;
        }

    }
    return matrix;
}
//----------------------

int** maze = createMatrix(30,24);
int vehicleNumber = 0;

//vehicle id,vehicle lane, vehicle number
vector<vector<int>> vehiclesOnLane;

//vehuvle id, vehicle lane
vector<vector<int>> vehicleBuffer; 


GLfloat rotation = 90.0;
float posX = 0, posY = 0, posZ = 0,posX1 = 0, posY1 = 0, posZ1 = 0;
int signal =1;
int frames = 1;
void reshape(int width, int heigth){
    /* window ro reshape when made it bigger or smaller*/

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //clip the windows so its shortest side is 2.0
    if (width < heigth) {
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat)heigth / (GLfloat)width, 2.0 * (GLfloat)heigth / (GLfloat)width, 2.0, 2.0);
    }
    else{
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat)width / (GLfloat)heigth, 2.0 * (GLfloat)width / (GLfloat)heigth,2.0 , 2.0);
    }
    // set viewport to use the entire new window
    glViewport(0, 0, width, heigth);
}

//4*4
void car(int y){
    float x = 1.0/15.0f-(3.0f-(float)y)*0.4f-0.2f;
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(x, -1.2666f);
    glVertex2f(x,-1 );
    glVertex2f(x+0.2666f, -1);
    glVertex2f(x+0.2666f, -1.2666f);
    glEnd();

}

//2*4
void bike(int y){
    float x = 2.0/15.0f-(3.0f-(float)y)*0.4f-0.2f;
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(x, -1.2666f);
    glVertex2f(x,-1 );
    glVertex2f(x+0.1333f, -1);
    glVertex2f(x+0.1333f, -1.2666f);
    glEnd();

}

void lane(){
    glBegin(GL_LINES);
    glColor3f(1.0,1.0,1.0);
    glVertex2f(-0.6, -1);
    glVertex2f(-0.6, 1);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0,1.0,1.0);
    glVertex2f(-0.2, -1);
    glVertex2f(-0.2, 1);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0,1.0,1.0);
    glVertex2f(0.2, -1);
    glVertex2f(0.2, 1);
    glEnd();


}

void light(){
    glBegin(GL_POLYGON);
    if(signal==1){
      glColor3f(1.0, 0.0, 0.0);  
    }
    else{
      glColor3f(0.0, 1.0, 0.0);
    }
    glVertex2f(0.55, 0);
    glVertex2f(0.55,0.1 );
    glVertex2f(0.65, 0.1);
    glVertex2f(0.65, 0);
    glEnd();

}
void trafficLine1(){
    glBegin(GL_LINES);
    glColor3f(1.0,1.0,1.0);
    glVertex2f(-1, 0);
    glVertex2f(1, 0);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0,1.0,1.0);
    glVertex2f(-1, 0.01);
    glVertex2f(1, 0.01);
    glEnd();


}

void timer(){
  glRasterPos3f(0.6,0.05,0);

  const char *t  = to_string(20 - (frames/40)%20).c_str();
  int len = strlen(t);
  for (int i = 0; i < len; i++) {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, t[i]);
  }
}


void display(){
    //Clear Window
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    car(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(posX1,posY1,posZ1);
    bike(2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,0);
    lane();
    light();
    trafficLine1();
    // trafficLine2();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0,0.0,0.0);
    timer();
    glPopMatrix();  


    glFlush();
}


void init(){
    // set clear color to black
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // set fill color to white
    glColor3f(1.0, 1.0, 1.0);

    //set up standard orthogonal view with clipping
    //box as cube of side 2 centered at origin
    //This is the default view and these statements could be removed
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

}

float speedcar = 1.0/15.0f;
float speedbike = 1.0/15.0f;


void update(int value)
{
  if(posY-1<0 || (posY-1>=0  && posY-1<= 0.01 && signal==0) || posY-1>0.01){
    // cout << posY-1 << "\n";
    posY+=speedcar*0.025f;
  }

  if(posY1-1<0 || (posY1-1>=0 && posY1-1<=0.01 && signal==0) || posY1-1>0.01){
  
    if(frames>80){
      // cout << posY1-1 << "\n";
      posY1+=speedbike*0.025f;
    }
  }
  //20 sec red, 20 sec green
  if(frames%1600>800){
    signal = 0;
  }
  else{
    signal = 1;
  }

  // if()

  glutPostRedisplay();
  // cout << frames << "\n";
  frames++;
  glutTimerFunc(25,update,0);
}


int main(int argc, char** argv){

    //initialize mode and open a windows in upper left corner of screen
    //Windows tittle is name of program

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Practice 1");
    glutDisplayFunc(display);
    init();
    glutTimerFunc(25,update,0);
    glutMainLoop();

}