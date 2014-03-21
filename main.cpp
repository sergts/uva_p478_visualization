#include <iostream>
#include <cmath>
#include <vector>
#include <GL/glut.h>
using namespace std;

class Shape
{
public:
    virtual bool contains(float x, float y) = 0;
    virtual void draw() = 0;
};

class Circle : public Shape
{
public:
    float x1,y1,r;
    Circle(float xx1, float yy1, float rr) : x1(xx1) , y1(yy1), r(rr) {}
    bool contains(float x, float y)
    {
        if(sqrt( abs(x-x1)*abs(x-x1) + abs(y-y1)*abs(y-y1) ) < r) return true;
        return false;
    }
    void draw()
    {
        glBegin(GL_LINE_LOOP);
        for(float a = 0; a < 2 * M_PI; a+= 0.01){
            glVertex2f(r*cos(a)+x1,r*sin(a)+y1);
        }
        glEnd();
    }
};

class Rectangle : public Shape
{
public:
    float x1,x2,y1,y2;
    Rectangle(float xx1, float yy1, float xx2, float yy2) : x1(xx1), x2(xx2), y1(yy1), y2(yy2) {}
    bool contains(float x, float y)
    {
        if(x > x1 && y < y1 && x < x2 && y > y2) return true;
        return false;
    }
    void draw()
    {
        glBegin(GL_LINE_LOOP);
        glVertex2f(x1,y1);
        glVertex2f(x2,y1);
        glVertex2f(x2,y2);
        glVertex2f(x1,y2);
        glEnd();

    }
};

class Triangle : public Shape
{
public:
    float x1,y1,x2,y2,x3,y3;
    Triangle(float xx1, float yy1, float xx2, float yy2, float xx3, float yy3) : x1(xx1) , y1(yy1), x2(xx2), y2(yy2), x3(xx3), y3(yy3) {}
    bool contains(float x, float y)
    {
        bool b1,b2,b3;
        b1 = sign(x,y,x1,y1,x2,y2) < 0.0f;
        b2 = sign(x,y,x2,y2,x3,y3) < 0.0f;
        b3 = sign(x,y,x3,y3,x1,y1) < 0.0f;
        return ((b1 == b2) && (b2 == b3));

    }
    float sign(float xx1, float yy1, float xx2, float yy2, float xx3, float yy3)
    {
        return (xx1-xx3) * (yy2-yy3) - (xx2-xx3) * (yy1-yy3);
    }
    void draw()
    {
        glBegin(GL_LINE_LOOP);
        glVertex2f(x1,y1);
        glVertex2f(x2,y2);
        glVertex2f(x3,y3);
        glEnd();

    }
};


vector<Shape*> vec;
vector<int> px;
vector<int> py;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0,0.0,0.0);
    for(int i = 0; i < vec.size(); ++i)
        vec[i]->draw();
    glBegin(GL_LINES);
    for(int i = 0; i < px.size(); ++i){
       glVertex2f(px[i]-1.0, py[i]-1.0);
       glVertex2f(px[i]+1.0, py[i]+1.0);
       glVertex2f(px[i]-1.0, py[i]+1.0);
       glVertex2f(px[i]+1.0, py[i]-1.0);
    }
    glEnd();
    glFlush();
}


int main(int argc, char **argv)
{

    char c;
    float f[6];
    while(1){
        cin >> c;
        if(c == '*') break;
        else if(c == 'r'){
            for(int i = 0; i < 4; ++i)
                cin >> f[i];
            vec.push_back(new Rectangle(f[0], f[1], f[2], f[3]));
        }
        else if(c == 'c'){
            for(int i = 0; i < 3; ++i)
                cin >> f[i];
                vec.push_back(new Circle(f[0], f[1], f[2]));
        }
        else if(c == 't'){
            for(int i = 0; i < 6; ++i)
                cin >> f[i];
            vec.push_back(new Triangle(f[0], f[1], f[2], f[3], f[4], f[5]));
        }

    }
    int counter(1);
    float x,y;
    float M = 9999.9;
    float delta = 0.01;
    int n = vec.size();
    bool contained;
    while(1){
        cin >> x >> y;
        contained = false;
        if(abs(x-M) < delta && abs(y-M) < delta) break;

        px.push_back(x);
        py.push_back(y);

        for(int i = 0; i < n; ++i){
            if(vec[i]->contains(x,y)){
                cout << "Point " << counter << " is contained in figure " << (i+1)<<endl;
                contained = true;
            }
        }
        if(!contained) cout << "Point " << counter << " is not contained in any figure" << endl;
        ++counter;
    }


    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(450,350);
    glutInitWindowPosition(400,940);
    glutCreateWindow("Shapes and points");
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0,30.0,-10.0,25.0,-30.0,30.0);  //change these values accordingly with dataset to properly display it
    glutDisplayFunc(display);
    glutMainLoop();



    for(int i = 0; i < n; ++i)
        delete vec[i];
    return 0;
}
