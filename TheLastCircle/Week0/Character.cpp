#include "Character.h"

void UCharacter::Move(FVector location, float delta) {
    float d = sqrt((Location.x - location.x) * (Location.x - location.x) + (Location.y - location.y) * (Location.y - location.y));
    if (d <= Range) {

    }
    else {
        Velocity = { (location.x - Location.x) / d, (location.y - Location.y) / d, 0 };
        FVector deltaLocation = { Velocity.x * delta * Speed , Velocity.y * delta * Speed , 0 };
        if (Location.x + deltaLocation.x - Radius <= MinLocation || Location.x + deltaLocation.x + Radius >= MaxLocation ||
            Location.y + deltaLocation.y - Radius <= MinLocation || Location.y + deltaLocation.y + Radius >= MaxLocation) return;
        Location.x += deltaLocation.x;
        Location.y += deltaLocation.y;
    }
    return;
}
void UCharacter::Attack(FVector location) {
    
}
void UCharacter::GetDamage(float damage) {
    Hp -= damage;
    if (Hp <= 0) {
        Die();
    }
}
void UCharacter::Die() {
    bIsActive = false;
}
bool UCharacter::IsActive() {
    return bIsActive;
}