#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
// #include <stdio.h>      /* printf, scanf, puts, NULL */
// #include <stdlib.h>     /* srand, rand */
// #include <time.h>       /* time */
#include "App.h"

using namespace std;

App* singleton;

void timer(int id){
    // This will get called every 16 milliseconds after
    // you call it once
    
    // If you want to manipulate the app in here
    // do it through the singleton pointer

    for(int i = 0; i < singleton->guards.size(); i++) {
        bool cat_in_graphic = singleton->withinBounds(singleton->guards[i]->getX(), singleton->guards[i]->getY() );
        bool cat_not_hitting_walls = !singleton->catTouchWalls(i, singleton->guards[i]->getX(), singleton->guards[i]->getY());
        bool catOK = cat_in_graphic && cat_not_hitting_walls;
        if(!catOK) {
            // cout << "NOT WITHIN BOUND!!!" << endl;
            if(singleton->guard_dir[i] == 0) {
                // cout << "Changing direction to Right" << endl;
                singleton->guard_dir.at(i) = 2;
            } else {
                // cout << "Changing direction to Left" << endl;
                singleton->guard_dir.at(i) = 0;
            }
        }

        if(singleton->guard_dir[i] == 0) {  // Going Left
            singleton->guards[i]->setX(singleton->guards[i]->getX() - singleton->speed);
        } else {    // Going Right
            singleton->guards[i]->setX(singleton->guards[i]->getX() + singleton->speed);
        }
    }

    singleton->ypos = singleton->mouse->getY();
    singleton->xpos = singleton->mouse->getX();

    if(singleton->alive) {
        if (singleton->up){
            singleton->ypos +=singleton->speed;
        }
        if (singleton->left){
            singleton->xpos -=singleton->speed;
        }
        if (singleton->down){
            singleton->ypos -=singleton->speed;
        }
        if (singleton->right){
            singleton->xpos +=singleton->speed;
        }
        
        if(singleton->withinBounds(singleton->xpos, singleton->ypos) && !singleton->touchWalls(singleton->xpos, singleton->ypos)) {
            singleton->mouse->setX(singleton->xpos);
            singleton->mouse->setY(singleton->ypos);
            singleton->redraw();
        }

        for(int i = 0; i < singleton->obstacle.size(); i++) {
            if(singleton->obstacle[i]->contains(singleton->xpos, singleton->ypos)) {
                // singleton->mouse->setStatus(false);
                singleton->alive = false;
                singleton->death->setX(singleton->xpos);
                singleton->death->setY(singleton->ypos);
                singleton->death->playOnce();
                singleton->redraw();
            }
        }

        for(int i = 0; i < singleton->guards.size(); i++) {
            if(singleton->guards.at(i)->contains(singleton->xpos, singleton->ypos)) {
                singleton->alive = false;
                singleton->death->setX(singleton->xpos);
                singleton->death->setY(singleton->ypos);
                singleton->death->playOnce();
                singleton->redraw();
            }
        }

         if(singleton->mushroom->contains(singleton->xpos, singleton->ypos)) {  
                singleton->explode = true;
                // singleton->explosion->playOnce();
                singleton->redraw();
        }
    }
    
    
    glutTimerFunc(16, timer, id);
}


App::App(int argc, char** argv, int width, int height, const char* title): GlutApp(argc, argv, width, height, title){
    mapWidth = width/250.0;
    mapHeight = height/250.0;
    // cout << "width/height:\t" << mapWidth << "/" << mapHeight << endl;
    mapHalfWidth = mapWidth/2.0;
    mapHalfHeight = mapHeight/2.0;
    // cout << mapHalfWidth << ", " << mapHalfHeight<<endl;

    speed = 0.01;
    
    // map/0.txt containsthe number of maps available besides itself
    // map/1.txt and so on will contain the map/layout of the level
    // Using: 0 for empty space, 1 starting loc, 2 for ending loc, 3 for CAT, 4 for obstacles
    // First line of textfile is the number of row followed by number of columns
    // int m, lvl;
    // ifstream infile("maps/0.txt");
    // infile >> m;
    // infile.close();
    // // if(m == 1) { cout << ""}
    // cout << "Choose from level 1 to " << m  << ": (Enter ints)"<< endl;
    // while(!(cin >> lvl) || lvl < 1 || lvl > m) {
    //     cin.clear();
    //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    //     cout << "Integer only. Try Again: ";
    // }
    // cout << "You chose level " << lvl << "!" << endl;
    // layout(lvl);
    createMap(1);
    // dir = Right;
    dir = 2;

    // mushroom = new TexRect("mushroom.png", -0.25, 0.9, 0.5, 0.5);
    // projectile = new Rect(-0.05, -0.8, 0.1, 0.1);
    // projectile->setR(1.0);
    // projectile->setG(0.0);
    // projectile->setB(0.0);
    up = false;
    down = false;
    right = false;
    left = false;
    explode = false;
    alive = true;
    //5x5 of blocks for now
    // float wid = 4/5.0;    //0.8
    // float hei = 2/5.0;   //0.4
    // srand( time(NULL) );
    // grid.push_back(new Block(-2, 1, wid, hei, 1, 0, 0));
    /*for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            float red = rand() % 2;
            float green = rand() % 2;
            float blue = rand() % 2;
            // std::cout << "x: " << i*wid-2 << ", y: " << j*-hei+1 << std::endl;
            grid.push_back(new Block(i*wid - 2, j*-hei + 1, wid, hei, red, green, blue));
        }
    }*/

    singleton = this;

    timer(1);
}

void App::createMap(int i) {
    string filename = "maps/" + to_string(i) + ".txt";
    cout << "filename:\t" << filename << endl;
    ifstream infile(filename);
    int r, c;
    if(infile >> r >> c) {
        cout << "row, col:\t" << r << ", " << c << endl;
    }
    float blockHeight = mapHeight / r;
    float blockWidth = mapWidth / c;
    cout << "block w/h:\t" << blockWidth << ", " << blockHeight << endl;

    // map/0.txt containsthe number of maps available besides itself
    // map/1.txt and so on will contain the map/layout of the level
    // Using: 0 for empty space, 1 starting loc, 2 for ending loc, 3 for CAT, 4 for wall, 5 for poison
    // First line of textfile is the number of row followed by number of columns
    for(int i = 0; i < r; i++) {    // rows
        for(int j = 0; j < c; j++) {    // columns
            float x = -mapHalfWidth+j*blockWidth;
            float y = mapHalfHeight-i*blockHeight;
            
            int temp;
            infile >> temp;
            cout << temp << " ";
            switch (temp) {
                case 1: {   // Starting the mouse: 
                            // projectile = new Rect(x, y, 0.1, 0.1);
                            // projectile->setR(0.0);
                            // projectile->setG(0.0);
                            // projectile->setB(0.0);
                            mouse = new Animal("images/mouse/", x, y, 1.0f, 0.1f, 0.1f);
                            cout << "testing" << endl;
                            cout << "Color RGB:\t" << mouse->getR() << ", " << mouse->getG() << ", " << mouse->getB() << endl;
                            death = new AnimatedRect("images/dyinganimation/dyinganimation.png", 4, 4, 75, true, true, x, y, 0.1f, 0.1f);
                            break;
                        }
                case 2: {   // Goal: 
                            mushroom = new TexRect("images/cheese/0.png", x, y, blockWidth, blockHeight);
                            // (const char* map_filename, int rows, int cols, int rate, bool visible=false, bool animated=false, float x=0, float y=0, float w=0.5, float h=0.5)
                            explosion = new AnimatedRect("fireball.bmp", 6, 6, 50, true, true, x, y, blockWidth, blockHeight);
                            // explode = false;
                            break;
                        }
                case 3: {   // Cat or some patrolling bot here:
                            // Cats will patrol left to right
                            guards.push_back(new Animal("images/cat/", x, y, 1.0f, 0.2f, 0.2f));
                            guard_dir.push_back(0);
                            break;
                        }
                case 4: {   // Walls that form the layout of the level: 
                            // cout << "block" << endl;
                            map.push_back(new TexRect("images/wall/wall.png", x, y, blockWidth, blockHeight));     //White walls
                            break;
                        }
                case 5: {   // Poison
                            obstacle.push_back(new TexRect("images/poison/0.png", x, y, blockWidth, blockHeight));
                            break;
                        }
                default: 
                    // cout <<"Error " << temp << endl;
                    break;
            }
        }
        cout << endl;
    }
    cout << "Done printing" << endl;
}

bool App::touchWalls(float mx, float my) {
    // cout << "Checking if touching walls" << endl;
    for(int i = 0; i < map.size(); i++) {
        // Checking all 4 corners:
        if( map[i]->contains(mx+speed, my) ||
            map[i]->contains(mx+mouse->getW(), my) ||
            map[i]->contains(mx+mouse->getW(), my-mouse->getH()+speed ) ||
            map[i]->contains(mx+speed, my-mouse->getH()) ) {
            return true;
        }
    }
    return false;
}

bool App::catTouchWalls(int j, float mx, float my) {
    // cout << "Checking if touching walls" << endl;
    for(int i = 0; i < map.size(); i++) {
        // cout << "Checking" << endl;
        // Checking all 4 corners:
        if( map[i]->contains(mx+speed, my) ||
            map[i]->contains(mx+guards.at(j)->getW(), my) ||
            map[i]->contains(mx+guards.at(j)->getW(), my-guards.at(j)->getH()+speed ) ||
            map[i]->contains(mx+speed, my-guards.at(j)->getH()) ) {
            return true;
        }
    }
    return false;
}

bool App::withinBounds(float mx, float my) {
    // cout << mx << ", " << my << endl;
    // if(mx > mapHalfWidth) cout << "testing"<< endl;
    // cout << "my\t" << my << ", H:\t" << projectile->getH() << endl;
    // cout << "Lower-Height:\t" << my-projectile->getH() << endl;
    return (mx >= -mapHalfWidth && mx+mouse->getW() <= mapHalfWidth && my-mouse->getH()+speed >= -mapHalfHeight && my <= mapHalfHeight);
}

void App::draw() {
    // End of Level or level-ending Stuff here:
    if(explode) {
        explosion->draw(0.2);
    } else {
        mushroom->draw(0.1);
    }

    // Drawing the moving parts: 
    // projectile->draw();
    if(alive) {
        mouse->draw(dir);
    } else {
        death->draw(1.0);
    }

    // Drawing the map itself and obstacles
    for(int i = 0; i < map.size(); i++) {
        map[i]->draw(0.1f);
    }
    for(int i = 0; i < guards.size(); i++) {
        guards[i]->draw(guard_dir[i]);
    }
    for(int i = 0; i < obstacle.size(); i++) {
        obstacle[i]->draw(0.5f);
    }
    // death->draw(0.2);
}

void App::keyUp(unsigned char key, float x, float y) {
    // std::cout <<"testing regular up" << std::endl;
    if(key == 'w') {
        // std::cout << "Done going up!" << std::endl;
        // dir = 1;
        up = false;
    }
    if(key == 'a') {
        // std::cout << "Done going left!" << std::endl;
        // dir = 0;
        left = false;
    }
    if(key == 's') {
        // std::cout << "Done going down!" << std::endl;
        // dir = 3;
        down = false;
    }
    if(key == 'd') {
        // std::cout << "Done going right!" << std::endl;
        // dir = 2;
        right = false;
    }
}

void App::keyDown(unsigned char key, float x, float y){
    // std::cout <<"testing regular down" << std::endl;
    if (key == 27){ //ESC key
        exit(0);
    }
    if(key == 'w') {
        // std::cout << "Going up!" << std::endl;
        dir = 1;
        up = true;
    }
    if(key == 'a') {
        // std::cout << "Going left!" << std::endl;
        dir = 0;
        left = true;
    }
    if(key == 's') {
        // std::cout << "Going down!" << std::endl;
        dir = 3;
        down = true;
    }
    if(key == 'd') {
        // std::cout << "Going right!" << std::endl;
        dir = 2;
        right = true;
    }
}

void App::specialKeyUp(int key, float x, float y) {
    // std::cout <<"Special Up called" << std::endl;
    if(key == 101) {
        // std::cout << "Going up!" << std::endl;
        up = false;
    }
    if(key == 100) {
        // std::cout << "Going left!" << std::endl;
        left = false;
    }
    if(key == 103) {
        // std::cout << "Going down!" << std::endl;
        down = false;
    }
    if(key == 102) {
        // std::cout << "Going right!" << std::endl;
        right = false;
    }
}

void App::specialKeyDown(int key, float x, float y){
    // std::cout <<"Special Down called" << std::endl;
    if(key == 101) {
        // std::cout << "Going up!" << std::endl;
        dir = 1;
        up = true;
    }
    if(key == 100) {
        // std::cout << "Going left!" << std::endl;
        dir = 0;
        left = true;
    }
    if(key == 103) {
        // std::cout << "Going down!" << std::endl;
        dir = 3;
        down = true;
    }
    if(key == 102) {
        // std::cout << "Going right!" << std::endl;
        dir = 2;
        right = true;
    }
}

App::~App(){
    std::cout << "Exiting..." << std::endl;
}
