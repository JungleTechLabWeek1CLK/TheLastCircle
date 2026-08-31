#include "Ball.h"

void DrawBalls(UPrimitive** PrimitiveList, URenderer* Renderer)
{
    UBall* CurrentBall = nullptr;

    // sphere's radius
    const float MIN_RADIUS = 0.07f;
    const float MAX_RADIUS = 0.17f;
    FVector BallColor;

    for (INT32 CurrentIndex = 0; CurrentIndex < UBall::TotalNumBalls; ++CurrentIndex)
    {
        CurrentBall = static_cast<UBall*>(PrimitiveList[CurrentIndex]);

        float Intensity = (CurrentBall->Radius - MIN_RADIUS) / (MAX_RADIUS - MIN_RADIUS);
        switch (CurrentBall->BallType)
        {
        case EBT_Bouncy: BallColor = FVector(1.f, Intensity * 0.3f, Intensity * 0.3f); break;
        case EBT_Magnetic: BallColor = FVector(Intensity * 0.3f, 1.f, Intensity * 0.3f); break;
        case EBT_Normal: BallColor = FVector(Intensity, Intensity, Intensity); break;
        }
        Renderer->UpdateConstantBuffer(CurrentBall->Location, CurrentBall->Radius, BallColor);
        Renderer->RenderPrimitive();
    }
}