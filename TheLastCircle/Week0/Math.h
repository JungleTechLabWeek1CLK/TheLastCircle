#pragma once

#include <cmath>

///////////////////////////////////////////////
// Structure for a vertex
struct FVertexSimple
{
    float x, y, z;    // Position
    float r, g, b, a; // Color
    float u, v;       // Texture
};

// Structure for a 3D vector
struct FVector
{
    float x, y, z;

    FVector(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
    FVector operator+(const FVector& input) const
    {
        return FVector(x + input.x, y + input.y, z + input.z);
    }
    FVector operator-(const FVector& input) const
    {
        return *this + (input * -1.0f);
    }
    FVector operator*(const float input) const
    {
        return FVector(x * input, y * input, z * input);
    }
    float Dot(const FVector& input) const
    {
        return x * input.x + y * input.y + z * input.z;
    }

    void Normalize()
    {
        float Magnitude = GetMagnitude();
        x /= Magnitude;
        y /= Magnitude;
        z /= Magnitude;
    }
    float GetMagnitude()
    {
        return sqrtf(x * x + y * y + z * z);
    }
};
///////////////////////////////////////////////
