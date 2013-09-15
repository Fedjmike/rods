#include <iostream>
#include <list>
#include <string>
#include <sstream>

#include "SDL/SDL.h"

#include "vector.hpp"
#include "drawing.hpp"
#include "geometry.hpp"

using namespace std;

bool isOneLengthUnknown(Triangle* triangle) {
    return   (triangle->edges[0]->fixed != edgeFlexi ? 0 : 1)
           + (triangle->edges[1]->fixed != edgeFlexi ? 0 : 1)
           + (triangle->edges[2]->fixed != edgeFlexi ? 0 : 1)
                == 1;
}

bool isOneVertexUnknown(Triangle* triangle) {
    return   (triangle->vertices[0]->fixed ? 0 : 1)
           + (triangle->vertices[1]->fixed ? 0 : 1)
           + (triangle->vertices[2]->fixed ? 0 : 1)
                == 1;
}

bool isTriangleSoluble(Triangle* triangle) {
    return !(isOneLengthUnknown(triangle) && isOneVertexUnknown(triangle));
}

void solveTriangle(Triangle* triangle) {
    if (isOneLengthUnknown(triangle)) {
        int unknownIndex = !triangle->edges[0]->fixed != edgeFlexi ? 0 :
                           !triangle->edges[1]->fixed != edgeFlexi ? 1 : 2;

        double b = triangle->edges[(unknownIndex+1) % 3]->length;
        double c = triangle->edges[(unknownIndex+2) % 3]->length;

        cout << "Length {" << unknownIndex << "} unknown\n"
             << "Length {" << (unknownIndex + 1) % 3 << "} is " << b << "; length {" << (unknownIndex + 2) % 3 << "} is " << c << "\n";

        Vector<2> B = triangle->vertices[(unknownIndex+1) % 3]->pos - triangle->vertices[unknownIndex]->pos;
        Vector<2> C = triangle->vertices[(unknownIndex+2) % 3]->pos - triangle->vertices[unknownIndex]->pos;
        cout << b*b + c*c - 2*b*c*cos(angleBetween(B, C));
        triangle->edges[unknownIndex]->length = sqrt(abs((b*b + c*c - 2*b*c*cos(angleBetween(B, C)))));
        triangle->edges[unknownIndex]->fixed = edgeFlexiFixed;

        cout << "Length {" << unknownIndex << "} unknown; set to " << triangle->edges[unknownIndex]->length << "\n";

        cout << "\n";

    } else if (isOneVertexUnknown(triangle)) {
        int unknownIndex = !triangle->vertices[0]->fixed ? 0 :
                           !triangle->vertices[1]->fixed ? 1 : 2;

        cout << "Vertex {" << unknownIndex << "} unknown\n"
             << "Vertex {" << (unknownIndex+1) % 3 << "} at " << triangle->vertices[(unknownIndex+1) % 3]->pos << "\n"
             << "Vertex {" << (unknownIndex+2) % 3 << "} at " << triangle->vertices[(unknownIndex+2) % 3]->pos << "\n";

        double a = triangle->edges[unknownIndex]->length;
        double b = triangle->edges[(unknownIndex+1) % 3]->length;
        double c = triangle->edges[(unknownIndex+2) % 3]->length;
        Vector<2> BtoC = triangle->vertices[(unknownIndex+2) % 3]->pos - triangle->vertices[(unknownIndex+1) % 3]->pos;
        
        cout << "a = " << a << "; b = " << b << "; c = " << c << "\n";

        double beta = -acos((a*a - b*b + c*c)/(2*a*c));
        cout << "Beta = " << beta << "\n";

        double angleOfBtoC = angleBetween(Vector<2>({1, 0}), BtoC);
        cout << "Angle of {" << (unknownIndex+1) % 3 << (unknownIndex+2) % 3 << "} is " << angleOfBtoC << "\n";

        Vector<2> BA = fromPolar(c, beta + angleOfBtoC);
        cout << "Vector {" << (unknownIndex+1) % 3 << unknownIndex
             /*cout in polars*/
             << "} = {" << c << ", " << beta + angleOfBtoC
             /*cout as vector*/
             << "} = " << BA << "\n";

        cout << "Vector {" << unknownIndex << "} = {"            << (unknownIndex+1) % 3 
                                                      << "} + {" << (unknownIndex+1) % 3 << unknownIndex << "}\n"
             << "           = "          << triangle->vertices[(unknownIndex + 1) % 3]->pos
                                << " + " << BA << "\n"
             << "           = " <<   triangle->vertices[(unknownIndex + 1) % 3]->pos
                                   + BA << "\n\n";

        triangle->vertices[unknownIndex]->pos =   triangle->vertices[(unknownIndex + 1) % 3]->pos
                                                + fromPolar(c, beta + angleOfBtoC);
        triangle->vertices[unknownIndex]->fixed = true;
    }
}

void solvePolygon(Triangle* ultimateNecessitor) {
    list<Triangle*> fringe = {ultimateNecessitor}, nextfringe;

    while (fringe.size() != 0) {
        cout << "Nth while loop over fringe...\n";

        for (Triangle* triangle: fringe) {
            cout << "Mth triangle in fringe: test for solubility ";
            if (isTriangleSoluble(triangle)) {
                cout << "passed \n";
                solveTriangle(triangle);
                nextfringe.insert(nextfringe.end(), triangle->necessitors.begin(), triangle->necessitors.end());

            } else {
                cout << "failed!\n";
            }
        }

        fringe = nextfringe;
        nextfringe.clear();
    }
}

// void drawText (SDL_Surface* window, TTF_Font* font, string text, Vector<2, int> pos) {
//     const char* cstr = text.c_str();
//     SDL_Surface* image = TTF_RenderText_Solid(font, cstr, SDL_Color{255, 255, 255, 0});

//     SDL_Rect rect;
//     rect.x = pos[0]; rect.y = pos[1];
//     rect.w = 
//     SDL_BlitSurface(window, NULL, image, &rect);
//     SDL_FreeSurface(image);
// }

int main(int argc, char** argv) {
    /*cout << "\n";

    cout << "RESULTS \n";
    for (int i = 0; i < vertexNumber; i++) {
        cout << vertices[i].pos << "\n";
    }*/

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Unable to initiota SDL: " << SDL_GetError() << "\n";
        return 1;
    }

    atexit(SDL_Quit);

    freopen("/dev/tty", "w", stdout);

    SDL_Surface* window = SDL_SetVideoMode(512, 512, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    // TTF_Init();
    // TTF_Font* Droid = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 14);

    /*for (int i = -3; i <= 3; i++) {
        drawBigPoint(window, Vector<2>({double(i), 0}), 200, 200, 200);
    }

    for (int j = -3; j <= 3; j++) {
        drawBigPoint(window, Vector<2>({0, double(j)}), 200, 200, 200);
    }

    for (int i = 0; i < vertexNumber; i++) {
        drawBigPoint(window, vertices[i].pos, i % 3 == 0 ? 255 : 0, i % 3 == 1 ? 127 : 0, i % 3 == 2 ? 255 : 0);
    //     stringstream str;
    //     str << "Vertex " << i;
    //     puts(str.str().c_str());
    //     drawText(window, Droid, str.str().c_str(), vertices[i].pos);
    }*/

    SDL_Event event;

    double angle = 0;

    do {
        angle += 0.05;


        // START RECIPE

        const int vertexNumber = 7;

        Vertex vertices[vertexNumber] = {{Vector<2>({0.0, 0.0}), false},     //0, red
                                         {Vector<2>({0.3, 0.0}) + fromPolar(0.15, angle), true},     //1, green
                                         {Vector<2>({0.0, 0.0}), false},     //2, blue
                                         {Vector<2>({0.0, 0.0}), false},     //3, red
                                         {Vector<2>({-0.1, -0.08}), true},     //4, green
                                         {Vector<2>({0.0, 0.0}), false},     //6, green
                                         {Vector<2>({0.0, 0.0}), false}};    //7, blue

        Edge edges[] = {{0.415, edgeFixed},      //0
                        {0.5, edgeFixed},        //1
                        {(vertices[4].pos - vertices[1].pos).length(), edgeFixed},        //2
                        {0.619, edgeFixed},      //3
                        {0.393, edgeFixed},      //4
                        {0.0, edgeFlexi},        //5
                        {0.401, edgeFixed},      //6
                        {0.558, edgeFixed},      //7
                        {0.394, edgeFixed},      //8
                        {0.657, edgeFixed},      //9
                        {0.49, edgeFixed},       //10
                        {0.367, edgeFixed}};     //11

        Triangle sixth  = {{&vertices[5], &vertices[0], &vertices[6]},
                           {   &edges[9],    &edges[11],   &edges[10]}, {}};

        Triangle fifth  = {{&vertices[5], &vertices[6], &vertices[3]},
                           {   &edges[8],    &edges[5],    &edges[11]}, {&sixth}};

        Triangle fourth = {{&vertices[5], &vertices[3], &vertices[4]},
                           {   &edges[6],    &edges[4],    &edges[5]}, {&fifth}};

        Triangle third  = {{&vertices[4], &vertices[3], &vertices[2]},
                           {   &edges[7],    &edges[0],    &edges[6]}, {&fourth}};

        Triangle second = {{&vertices[1], &vertices[5], &vertices[4]},
                           {   &edges[4],    &edges[2],    &edges[3]}, {&fourth}};

        Triangle first  = {{&vertices[1], &vertices[4], &vertices[2]},
                           {   &edges[0],    &edges[1],    &edges[2]}, {&second, &third}}; 

        // END RECIPE


        solvePolygon(&first);

        SDL_Rect rect = {0, 0, 512, 512};
        SDL_FillRect(window, &rect, ~0);

        edges[2].fixed = edgeFlexiFixed;

        drawPolygon(window, &first, 0, 0, 0);
        SDL_Flip(window);
        SDL_Delay(10);
        SDL_PollEvent(&event);
        
        /*do SDL_WaitEvent(&event);
        while (event.type != SDL_KEYDOWN && event.type != SDL_QUIT);*/
    } while (event.type != SDL_QUIT);
}