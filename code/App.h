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
bool creatingMap = false;

int dir = 0;
float mapWidth, mapHeight, mapHalfWidth, mapHalfHeight;

// std::vector<Block*> grid;
std::vector<Rect*> map;
std::vector<TexRect*> obstacle;


// Game editor stuff:
std::vector<Rect*> mapMaking;
int mapCounter = 0;
std::vector<int> mapBits;
int row, col;
float rectHeight, rectWidth;
bool black = true;
bool m1 = false;
float m1x, m1y;

TexRect* mushroom;
// Rect* projectile;
AnimatedRect* explosion;
Animal* mouse;
Animal* cat;
float xpos, ypos, speed;

bool up, down, left, right;
bool explode;
    
public:
    
    App(int argc, char** argv, int width, int height, const char* title);

    void createMap(int i);

    bool touchWalls(float mx, float my);

    bool withinBounds(float mx, float my);
    
    void draw();

    void keyUp(unsigned char key, float x, float y);
    
    void keyDown(unsigned char key, float x, float y);

    void specialKeyUp(int key, float x, float y);
    
    void specialKeyDown(int key, float x, float y);

    void leftMouseUp(float mx, float my);

    void leftMouseDown(float mx, float my);
    
    ~App();
    
    friend void timer(int);
    
};

#endif
