#ifndef vehicle_h
#define vehicle_h

class vehicle {
public:
    float length;
    float width;
    float speed;
    int* color;
    float xPos;
    float yPos;
    float inTime;
    vehicle(float len,float wid,float spee,int col1,int col2,int col3,float tim);
    void updatePos();
};

#endif /* vehicle_h */
