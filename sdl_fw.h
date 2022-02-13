#include <SDL.h>
#include "vector2d.h"

using namespace std;
class Framework {
    SDL_Renderer *renderer;      // Pointer for the renderer
    SDL_Window *window;      // Pointer for the window

    int r, g, b;    // used for background colours


public:
    int height;     // Height of the window
    int width;      // Width of the window

    int xoff;       // number of pixels to offset drawing stuff
    int yoff;

    Framework(int width_, int height_): height(height_), width(width_){
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);       // Initializing SDL as Video
        //SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
        window = SDL_CreateWindow("un named window", SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED, width_, height, 0);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);      // setting draw color
        SDL_RenderClear(renderer);      // Clear the newly created window
        r = 0; g = 0; b = 0;    // set background colour to black
        xoff = 0; yoff = 0;     // set default offset
    }

    Framework(const char* title, int width_, int height_): height(height_), width(width_) {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);       // Initializing SDL as Video
        //SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED, width_, height, 0);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);      // setting draw color
        SDL_RenderClear(renderer);      // Clear the newly created window
        r = 0; g = 0; b = 0;    // set background colour to black
        xoff = 0; yoff = 0;     // set default offset
    }

    // Destructor
    ~Framework(){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }


    void set_offset(int x, int y) {
        xoff = x; 
        yoff = y;
    }

    // drawing stuff 
    void draw_point(int x, int y) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawPoint(renderer, x+xoff, y+yoff);
    }

    void draw_point(Vector A) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawPoint(renderer, A.x+xoff, A.y+yoff);
    }

    void draw_rectangle(int centre_x, int centre_y,
    int width, int height) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                SDL_RenderDrawPoint(renderer,
                                     x + centre_x - (width/2),
                                     y + centre_y - (height/2));
            }
        }
    }

    void draw_circle(int centre_x, int centre_y, int radius) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        // Drawing circle
        for(int x=centre_x-radius; x<=centre_x+radius; x++) {
            for(int y=centre_y-radius; y<=centre_y+radius; y++) {
                if((pow(centre_y-y,2) + pow(centre_x-x,2)) <= pow(radius,2)) {
                    SDL_RenderDrawPoint(renderer, x+xoff, y+yoff);
                }
            }
        }
    }

    void draw_circle(Vector centre, int radius) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        // Drawing circle
        for(int x=centre.x-radius; x<=centre.x+radius; x++) {
            for(int y=centre.y-radius; y<=centre.y+radius; y++) {
                if((pow(centre.y-y,2) + pow(centre.x-x,2)) <= pow(radius,2)) {
                    SDL_RenderDrawPoint(renderer, x+xoff, y+yoff);
                }
            }
        }

    }
    
    void draw_circle(Vector centre, int radius, int R, int G, int B) {
        SDL_SetRenderDrawColor(renderer, R, G, B, 255);
        // Drawing circle
        for(int x=centre.x-radius; x<=centre.x+radius; x++) {
            for(int y=centre.y-radius; y<=centre.y+radius; y++) {
                if((pow(centre.y-y,2) + pow(centre.x-x,2)) <= pow(radius,2)) {
                    SDL_RenderDrawPoint(renderer, x+xoff, y+yoff);
                }
            }
        }

    }



    void draw_line(Vector start, Vector end) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, start.x+xoff , start.y+yoff,
                                     end.x+xoff, end.y+yoff);
    }

    void draw_triangle(Vector p1, Vector p2, Vector p3) {
        draw_line(p1, p2); draw_line(p2, p3); draw_line(p3, p1);
    }

    void draw_line(int x1, int y1, int x2, int y2) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, x1+xoff, y1+yoff,
                                     x2+xoff, y2+yoff);
    }

    void draw_bezier_point(
        /* draw a point on the line defined by the
         * the cubic bezier */
        Vector P1, Vector P2, Vector P3, Vector P4, float t
    ) {
        draw_point( (pow(1-t, 3.)*P1) +
                    (3*t * pow(1-t, 2.) * P2) +
                    (3 * t*t * ( 1-t ) * P3) +
                    (pow(t, 3) * P4)
                     );
    }

    Vector bezier_point(
        Vector P1, Vector P2, Vector P3, Vector P4, float t
    ) {
        return (
                    (pow(1-t, 3.)*P1) +
                    (3*t * pow(1-t, 2.) * P2) +
                    (3 * t*t * ( 1-t ) * P3) +
                    (pow(t, 3) * P4)
        );
    }

    void draw_bezier_line(Vector P1, Vector P2, Vector P3, Vector P4) {
        int Range = 1000;
        for (int i = 0; i < Range; i++) {
            draw_bezier_point(P1, P2, P3, P4, (float) i / (float) Range);
        }
    }

    void draw_postion_vector(int x, int y, Vector V) {
        // draw vector with origin at postion (x,y)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, x + xoff, y + yoff, x + V.x + xoff, y + V.x + yoff);
        Vector a(-V.y, V.x);    // a vector perpendicular to V
        //a.normalize(); 
        
    }

    void set_background_colour(int R, int G, int B) {
        r = R; g = G; b = B;
    }

    void blank() {
        SDL_SetRenderDrawColor(renderer, r, g, b, 0);
        SDL_RenderClear(renderer);
    }
    void render() {
        SDL_RenderPresent(renderer);
    }


};


float l3x(SDL_Joystick* joy1) { /* returns the x displacement of controller one */
    return ((float) SDL_JoystickGetAxis(joy1, 0));
}

float l3y(SDL_Joystick* joy1) { /* returns the y displacement of controller one */
    return ((float) SDL_JoystickGetAxis(joy1, 1));
}

float r3x(SDL_Joystick* joy1) {
    return SDL_JoystickGetAxis(joy1, 2);
}

float r3y(SDL_Joystick* joy1) {
    return SDL_JoystickGetAxis(joy1, 3);
}
