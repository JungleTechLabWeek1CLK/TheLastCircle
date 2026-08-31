#include "Physics.h"
#include "Renderer.h"

enum class ETypeCharacter
{
    ETC_Player,
    ETC_Enemy,
    ETC_PlayerProjectile,
    ETC_EnemyProjectile
};

class UCharacter
{
public:
    ////////
    // Required for test
    FVector Location;
    FVector Velocity;
    float Radius;
    float Mass;
    float Hp;
    float Speed;
    float Damage;
    ETypePrimitive HitBox;
    FVector Color;
    ETypeCharacter CharacterType;
    ////////

    UCharacter() : Location(0.f), Velocity(0.f)
    {
    }
    ~UCharacter()
    {
    }
private:
    void Move();
    void Attack();
    void GetDamage();
    void Die();
};

void DrawBalls(UPrimitive** PrimitiveList, URenderer* Renderer);
void AddBall(UPrimitive**& PrimitiveList, INT32& CurrentListSize);
void RemoveBall(UPrimitive** PrimitiveList);