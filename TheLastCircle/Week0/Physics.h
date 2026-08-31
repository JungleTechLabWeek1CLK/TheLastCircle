#pragma once

#include "Math.h"
#include <cmath>

///////////////////////////////////////////////
// Structure for a Primitive
class UPrimitive
{
public:
    virtual ~UPrimitive() {}
};

enum EBallType
{
    EBT_Normal,
    EBT_Magnetic,
    EBT_Bouncy
};

// Structure for a Ball
class UBall : public UPrimitive
{
public:
    ////////
    // Required for test
    FVector Location;
    FVector Velocity;
    float Radius;
    float Mass;
    static int TotalNumBalls;
    ////////

    EBallType BallType;
    float Resitution;

    UBall() : Location(0.f), Velocity(0.f)
    {
        ++TotalNumBalls;

        Radius = GetRandomValue() * 0.1f + 0.07f;
        Mass = Radius * Radius * Radius;

        Location.x = GetRandomValue() - 0.5f;
        Location.y = GetRandomValue() - 0.5f;


        Velocity.x = (GetRandomValue() - 0.5f) * 3.9f;
        Velocity.y = (GetRandomValue() - 0.5f) * 3.9f;


        float RandomToPickType = GetRandomValue();
        if (RandomToPickType < 0.2f)
        {
            BallType = EBT_Magnetic;
            Resitution = 0.3f;
        }
        else if (RandomToPickType < 0.4f)
        {
            BallType = EBT_Bouncy;
            Resitution = 0.95f;
        }
        else
        {
            BallType = EBT_Normal;
            Resitution = 0.3f;
        }

    }
    ~UBall()
    {
        --TotalNumBalls;
    }
private:
    // random value from 0 to 1
    float GetRandomValue()
    {
        return (static_cast<float>(rand()) / RAND_MAX);
    }

};

///////////////////////////////////////////////


// Basic Directions
const FVector DIRECTION_LEFT(-1.f, 0.f, 0.f);
const FVector DIRECTION_RIGHT(1.f, 0.f, 0.f);
const FVector DIRECTION_UP(0.f, 1.f, 0.f);
const FVector DIRECTION_DOWN(0.f, -1.f, 0.f);


void ApplyGravity(UPrimitive** PrimitiveList, const float DELTA_TIME);
void HandleCollision(UPrimitive** PrimitiveList, const float DELTA_TIME);
void MoveBalls(UPrimitive** PrimitiveList, const float DELTA_TIME);