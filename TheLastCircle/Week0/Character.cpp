#include "Character.h"

void UCharacter::Move(FVector location, float delta) {
    float d = sqrt((Location.x - location.x) * (Location.x - location.x) + (Location.y - location.y) * (Location.y - location.y));
    if (d <= range)  Attack(location);
    else {
        Velocity = { (location.x - Location.x) / d, (location.y - Location.y) / d, 0 };
        Location.x += Velocity.x * delta;
        Location.y += Velocity.y * delta;
    }
    return;
}
void UCharacter::Attack(FVector location) {

}
void UCharacter::GetDamage() {

}
void UCharacter::Die() {

}