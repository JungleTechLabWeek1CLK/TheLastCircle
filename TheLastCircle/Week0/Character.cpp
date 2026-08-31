#include "Character.h"

void Move() {

}
void Attack() {

}
void GetDamage() {

}
void Die() {

}











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

void AddBall(UPrimitive**& PrimitiveList, INT32& CurrentListSize)
{
    UBall* NewBall = new UBall();
    if (UBall::TotalNumBalls > CurrentListSize)
    {
        INT32 NewListSize = CurrentListSize * 2;
        UPrimitive** NewList = new UPrimitive * [NewListSize]();
        for (INT32 CurrentIndex = 0; CurrentIndex < CurrentListSize; ++CurrentIndex)
        {
            NewList[CurrentIndex] = PrimitiveList[CurrentIndex];
        }
        delete[] PrimitiveList;
        CurrentListSize = NewListSize;
        PrimitiveList = NewList;
    }
    PrimitiveList[UBall::TotalNumBalls - 1] = NewBall;
}

void RemoveBall(UPrimitive** PrimitiveList)
{
    if (UBall::TotalNumBalls > 0)
    {
        INT32 TargetIndexToDelete = (static_cast<float>(rand()) / RAND_MAX) * UBall::TotalNumBalls;
        if (TargetIndexToDelete == UBall::TotalNumBalls)
            --TargetIndexToDelete;
        delete PrimitiveList[TargetIndexToDelete];
        PrimitiveList[TargetIndexToDelete] = PrimitiveList[UBall::TotalNumBalls];
        PrimitiveList[UBall::TotalNumBalls] = nullptr;
    }
}