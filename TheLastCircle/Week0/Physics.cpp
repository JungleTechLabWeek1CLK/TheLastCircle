#include "Physics.h"
#include <Windows.h>





///////////////////////////////////////////////
// Physics

void ApplyGravity(UPrimitive** PrimitiveList, const float DELTA_TIME)
{
    UBall* CurrentBall = nullptr;
    for (INT32 CurrentIndex = 0; CurrentIndex < UBall::TotalNumBalls; ++CurrentIndex)
    {
        CurrentBall = static_cast<UBall*>(PrimitiveList[CurrentIndex]);
        CurrentBall->Velocity.y -= 9.8f * DELTA_TIME;
    }
}

void HandleCollision(UPrimitive** PrimitiveList, const float DELTA_TIME)
{
    // detecting collisions
    UBall* CurrentBall, * BallToCheck = nullptr;
    for (INT32 CurrentIndex = 0; CurrentIndex < UBall::TotalNumBalls; ++CurrentIndex)
    {
        CurrentBall = static_cast<UBall*>(PrimitiveList[CurrentIndex]);
        for (INT32 IndexToCheck = CurrentIndex + 1; IndexToCheck < UBall::TotalNumBalls; ++IndexToCheck)
        {
            BallToCheck = static_cast<UBall*>(PrimitiveList[IndexToCheck]);

            FVector CollisionNormal = BallToCheck->Location - CurrentBall->Location; // not normalized yet
            const float DISTANCE = CollisionNormal.GetMagnitude();

            // magnetic ball
            if (DISTANCE > 0.01f)
            {
                if (CurrentBall->BallType == EBT_Magnetic || BallToCheck->BallType == EBT_Magnetic)
                {
                    FVector Direction = CollisionNormal * (1.f / DISTANCE);
                    float MagneticStrength = 0.2f;     // can be varied

                    float Force = MagneticStrength * (CurrentBall->Mass * BallToCheck->Mass) / (DISTANCE * DISTANCE);

                    // apply magnetic force
                    CurrentBall->Velocity = CurrentBall->Velocity + Direction * (Force / CurrentBall->Mass) * DELTA_TIME;
                    BallToCheck->Velocity = BallToCheck->Velocity - Direction * (Force / BallToCheck->Mass) * DELTA_TIME;
                }
            }

            // collision - ball / ball
            if (DISTANCE < (CurrentBall->Radius + BallToCheck->Radius))
            {
                // apply depenetration
                const float Depth = (CurrentBall->Radius + BallToCheck->Radius) - DISTANCE;
                CollisionNormal.Normalize();

                CurrentBall->Location = CurrentBall->Location - (CollisionNormal * ((BallToCheck->Mass / (CurrentBall->Mass + BallToCheck->Mass)) * Depth));
                BallToCheck->Location = BallToCheck->Location + (CollisionNormal * ((CurrentBall->Mass / (BallToCheck->Mass + CurrentBall->Mass)) * Depth));

                // resolve collision only if the balls face each other
                FVector RelativeVelocity = BallToCheck->Velocity - CurrentBall->Velocity;
                if (RelativeVelocity.Dot(CollisionNormal) < 0.f)
                {
                    // calcualte collision impulse
                    const float IMPULSE = -(1 + CurrentBall->Resitution) * (RelativeVelocity.Dot(CollisionNormal)) / (1.0f / CurrentBall->Mass + 1.0f / BallToCheck->Mass);

                    // resolve collision
                    CurrentBall->Velocity = CurrentBall->Velocity - CollisionNormal * (IMPULSE * (1.f / CurrentBall->Mass));
                    BallToCheck->Velocity = BallToCheck->Velocity + CollisionNormal * (IMPULSE * (1.f / BallToCheck->Mass));
                }
            }
        }

        // collision - ball / wall
        if (CurrentBall->Location.x + CurrentBall->Radius > 1)
        {
            CurrentBall->Velocity = CurrentBall->Velocity - DIRECTION_LEFT * (1 + CurrentBall->Resitution) * (CurrentBall->Velocity.Dot(DIRECTION_LEFT));
            CurrentBall->Location.x = 1 - CurrentBall->Radius;
        }
        if (CurrentBall->Location.x - CurrentBall->Radius < -1)
        {
            CurrentBall->Velocity = CurrentBall->Velocity - DIRECTION_RIGHT * (1 + CurrentBall->Resitution) * (CurrentBall->Velocity.Dot(DIRECTION_RIGHT));
            CurrentBall->Location.x = -1 + CurrentBall->Radius;
        }
        if (CurrentBall->Location.y + CurrentBall->Radius > 1)
        {
            CurrentBall->Velocity = CurrentBall->Velocity - DIRECTION_DOWN * (1 + CurrentBall->Resitution) * (CurrentBall->Velocity.Dot(DIRECTION_DOWN));
            CurrentBall->Location.y = 1 - CurrentBall->Radius;
        }
        if (CurrentBall->Location.y - CurrentBall->Radius < -1)
        {
            CurrentBall->Velocity = CurrentBall->Velocity - DIRECTION_UP * (1 + CurrentBall->Resitution) * (CurrentBall->Velocity.Dot(DIRECTION_UP));
            CurrentBall->Location.y = -1 + CurrentBall->Radius;
        }
    }
}

void MoveBalls(UPrimitive** PrimitiveList, const float DELTA_TIME)
{
    UBall* CurrentBall = nullptr;
    for (INT32 CurrentIndex = 0; CurrentIndex < UBall::TotalNumBalls; ++CurrentIndex)
    {
        CurrentBall = static_cast<UBall*>(PrimitiveList[CurrentIndex]);
        CurrentBall->Location = CurrentBall->Location + CurrentBall->Velocity * DELTA_TIME;
    }
}
///////////////////////////////////////////////