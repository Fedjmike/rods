#include <iostream>

#include "SDL/SDL.h"

#include "geometry.hpp"
#include "vector.hpp"

using namespace std;

Vector<2, int> pointToPixel(Vector<2> point) {
    return Vector<2>({256, 256}) + 256.0*Vector<2>({point[0], -point[1]});
}

void drawPixel(SDL_Surface* window, Vector<2, int> pos, int r, int g, int b) {
    ((Uint32*) window->pixels)[pos[0] + 512 * pos[1]] = r << 16 | g << 8 | b;
}

void drawPoint (SDL_Surface* window, Vector<2> pos, int r, int g, int b) {
    drawPixel(window, pointToPixel(pos), r, g, b);
}

void drawBigPoint (SDL_Surface* window, Vector<2> pos, int r, int g, int b) {
    drawPixel(window, Vector<2, int>({256, 256}) + 64.0*Vector<2>({pos[0], -pos[1]}), r, g, b);
    drawPixel(window, Vector<2, int>({255, 256}) + 64.0*Vector<2>({pos[0], -pos[1]}), r, g, b);
    drawPixel(window, Vector<2, int>({257, 256}) + 64.0*Vector<2>({pos[0], -pos[1]}), r, g, b);
    drawPixel(window, Vector<2, int>({256, 255}) + 64.0*Vector<2>({pos[0], -pos[1]}), r, g, b);
    drawPixel(window, Vector<2, int>({256, 257}) + 64.0*Vector<2>({pos[0], -pos[1]}), r, g, b);
    drawPixel(window, Vector<2, int>({255, 255}) + 64.0*Vector<2>({pos[0], -pos[1]}), r, g, b);
    drawPixel(window, Vector<2, int>({255, 257}) + 64.0*Vector<2>({pos[0], -pos[1]}), r, g, b);
    drawPixel(window, Vector<2, int>({257, 255}) + 64.0*Vector<2>({pos[0], -pos[1]}), r, g, b);
    drawPixel(window, Vector<2, int>({257, 257}) + 64.0*Vector<2>({pos[0], -pos[1]}), r, g, b);
}

void drawLine(SDL_Surface* window, Vector<2, int> start, Vector<2, int> end, int r, int g, int b) {
    Vector<2, double> lineVector = end - start;
    // double slope = (double) lineVector[1] / lineVector[0];
    double length = lineVector.length();

    cout << start << " - " << end << " = " << lineVector << "\n";
    cout << "Length of lineVector is " << length << "\n";

    int maxPixels = abs(lineVector[0]) > abs(lineVector[1]) ? abs(lineVector[0]) : abs(lineVector[1]);
    int counter = 0;
    for (Vector<2, double> current = start; counter <= maxPixels; current += lineVector * (1.0 / maxPixels)) {
        counter++;
        drawPixel(window, current, r, g, b);
    }
}

void drawEdge(SDL_Surface* window, Vector<2> start, Vector<2> end, int r, int g, int b) {
    drawLine(window, pointToPixel(start), pointToPixel(end), r, g, b);
}

void drawTriangle(SDL_Surface* window, Triangle triangle, int r, int g, int b) {
    for (int i = 0; i < 3; i++) {
        if (triangle.edges[(i + 2) % 3]->fixed == edgeFixed) {
            drawEdge(window, triangle.vertices[i]->pos, triangle.vertices[(i + 1) % 3]->pos, r, g, b);
        } else {
            drawEdge(window, triangle.vertices[i]->pos, triangle.vertices[(i + 1) % 3]->pos, 200, 200, 200);
        }
    }
}

void drawPolygon(SDL_Surface* window, Triangle* triangle, int r, int g, int b) {
    drawTriangle(window, *triangle, r, g, b);
    for (Triangle* necessitor: triangle->necessitors) {
    drawPolygon(window, necessitor, r, g, b);
    }
}