#include <list>
#include "vector.hpp"

typedef struct {
    Vector<2> pos;
    bool fixed;
} Vertex;

typedef enum {edgeFixed, edgeFlexi, edgeFlexiFixed} edgeFixedness;

typedef struct {
    double length;
    edgeFixedness fixed;
} Edge;

typedef struct Triangle {
    Vertex* vertices[3];
    Edge* edges[3];

    std::list<struct Triangle*> necessitors;
} Triangle;

/*class Geometry {
    std::list<Vertex> vertices;
    std::list<Edge> edges;
    std::list<Triangle> triangles;

public:
    Geometry (std::list<Vertex> v, std::list<Edge> e, std::list<Triangle> t);
    Geometry (const Geometry& g);
};*/