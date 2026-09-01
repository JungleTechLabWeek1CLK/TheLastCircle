#pragma once

#include "Math.h"

///////////////////////////////////////////////
// Shapes
//#include "Sphere.h"

extern FVertexSimple sphere_vertices[2400];
extern FVertexSimple triangle_vertices[3];
extern FVertexSimple cube_vertices[36];

// Enumerator for showing current shape
enum ETypePrimitive
{
    EPT_Triangle,
    EPT_Cube,
    EPT_Sphere,
    EPT_BackgroundQuad,
    EPT_UIQuad,
    EPT_Max,
};

///////////////////////////////////////////////