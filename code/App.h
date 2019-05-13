#ifndef App_h
#define App_h

#include "GlutApp.h"
#include "AnimatedRect.h"
#include "TexRect.h"
// #include "Block.h"
#include "Layout.h"
#include "Animal.h"
#include "vector"


class App: public GlutApp {
// enum Direction { Left, Up, Right, Down};  //0, 1, 2, 3

int dir;
float mapWidth, mapHeight, mapHalfWidth, mapHalfHeight;

// std::vector<Block*> grid;
std::vector<TexRect*> map;
std::vector<TexRect*> obstacle;
std::vector<Animal*> guards;
std::vector<int> guard_dir;


TexRect* mushroom;
// Rect* projectile;
AnimatedRect* explosion;
Animal* mouse;
AnimatedRect* death;
float xpos, ypos, speed;

bool up, down, left, right;
bool explode;
bool alive;
    
public:
    
    App(int argc, char** argv, int width, int height, const char* title);

    void createMap(int i);

    bool touchWalls(float mx, float my);

    bool catTouchWalls(int i, float mx, float my);

    bool withinBounds(float mx, float my);
    
    void draw();

    void keyUp(unsigned char key, float x, float y);
    
    void keyDown(unsigned char key, float x, float y);

    void specialKeyUp(int key, float x, float y);
    
    void specialKeyDown(int key, float x, float y);
    
    ~App();
    
    friend void timer(int);
    
};

#endif
