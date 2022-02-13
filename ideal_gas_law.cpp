// g++ ideal_gas_law.cpp `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c++14 -Wall -lSDL2 -lm && ./a.out

#include "sdl_fw.h"

#define NUM_PARTICLES (1500)
#define NUM_RECTANGLES (16)

#define WIDTH (1200)
#define HEIGHT (800)

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

class Wall {
    Framework* a;
public:
    float x, y;
    float dx, dy;

    int width, height; 

    Wall(Framework& fw) {
        a = &fw;
    }

    void show() {
        a->draw_rectangle(x, y, width, height);
    }

    void update(SDL_Joystick* joy1) {
        // move wall will controller input
        dx = r3y(joy1) / 10000;
        if (x + dx < 0) {
            dx = 0; x = 0;
        } else if ( x + dx >= WIDTH) {
            dx = 0; x = WIDTH;
        }
        else x += dx;
    }
};

class Rectangle {
    Framework* a;
public:
    int x, y;

    int width, height; 
    int num;
    Rectangle(Framework& fw) {
        a = &fw;
        x = 0; y = 0;
    }

    void update() {
        //height = num;
    }

    void show() {
        a->draw_rectangle(x+1, y+1, width+10, height+100);
    }
};

class Particle {
    Framework* a;
public:
    float x, y;
    float dx, dy;

    float mass;
    int radius;

    Particle(Framework& fw) {
        a = &fw;
        radius = 5;
        mass = 1;
    }




    void update(Particle P[NUM_PARTICLES], Wall wall) {
        // wall for wall postion


        if (y + dy > HEIGHT) {
            float overshot = (y + dy ) - HEIGHT;
            x += dx;
            y = HEIGHT - overshot;
            dy = -dy;
        } else if (y + dy <= 0) {
            float overshot = - (y + dy);
            y = overshot;
            x += dx;
            dy = -dy;
        } else if (x + dx > WIDTH) {
            float overshoot = (x + dx) - WIDTH;
            y += dy;
            x = WIDTH - overshoot;
            dx = -dx;
        } else if (x + dx <= 0) {
            float overshoot = - (x + dx);
            x = overshoot;
            y += dy;
            dx = -dx;
        } else if (x + dx + radius > wall.x + wall.dx + (wall.width/2) ) {
            // for the wall
            float overshoot = (x + dx) + radius + (wall.width/2) - (wall.x + wall.dx);
            y += dy;
            x = (wall.x + wall.dx) - overshoot;
            dx = -dx + 2*wall.dx;
        }

        else { x += dx; y += dy; }
    }

    float speed() {
        return mass * sqrt( (dx*dx) + (dy*dy) );
    }

    void show() {
        a->draw_circle(Vector(x, y), radius);
    }
};

int main() {
    // initilise random seed
    srand(time(NULL));

    //stastics window
    Framework stats("stats bar-graph speed distrubutuion", WIDTH + 80, HEIGHT);

    // add window
    Framework fw("ideal_gas_law", WIDTH, HEIGHT);
    SDL_Event event;
    fw.set_offset(0, 0);

    // add controller
    if( SDL_NumJoysticks() == 0) {
        printf("you have 0 controllers connected faggot\n");
        return 0;
    } SDL_Joystick *joystick = SDL_JoystickOpen(0);

    // add particles
    Particle* gas = (Particle*) malloc(sizeof(Particle) * NUM_PARTICLES);
    for (int i = 0; i < NUM_PARTICLES; i++) {
        gas[i] = Particle(fw);
        gas[i].x = rand() % WIDTH;
        gas[i].y = rand() % HEIGHT;
        gas[i].dx = RandomFloat(-4, 4);
        gas[i].dy = RandomFloat(-4, 4);
    }

    // add wall
    Wall wall(fw);
    wall.dx = 0;
    wall.dy = 0;

    wall.x = WIDTH;
    wall.y = HEIGHT/2;

    wall.width = 10;
    wall.height = HEIGHT;

    // add rectangles for stats
    Rectangle* rects = (Rectangle*) malloc(sizeof(Rectangle) * 16);
    for (int i = 0; i < NUM_RECTANGLES; i++) {
        rects[i] = Rectangle(stats);
    } Rectangle square(stats);


    // game loop
    while(!(event.type == SDL_QUIT)) {
        //SDL_Delay(20);
        SDL_PollEvent(&event);
        fw.blank();
        stats.blank();

        wall.update(joystick);

        // update gas
        for (int i = 0; i < NUM_PARTICLES; i++) {
            gas[i].update(gas, wall);
            gas[i].show();
        } wall.show();


        // calculate number of particles in each speed range
        for (int i = 0; i < NUM_RECTANGLES; i++) {
            for (int j = 0; j < NUM_PARTICLES; j++) {
                if ( gas[j].speed() < i && gas[j].speed() >= i - 1) {
                    rects[i].num += 1;
                }
            }
        }

        // render rectangles
        for (int i = 0; i < NUM_RECTANGLES; i++) {
            rects[i].update(); 
            rects[i].show();
        } square.show();
        stats.draw_rectangle(0,0,12,12);
        fw.render(); stats.render();
    }
}