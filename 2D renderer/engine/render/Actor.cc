#include "config.h"
#include "render/Actor.h"

Actor::Actor()
{
    position = V4();
    mass = 1.f;
    velocity = 0.f;
}

Actor::Actor(V4 position, float mass, float velocity, Quat torque) : position(position), mass(mass), velocity(velocity), torque(torque)
{

}

M4 Actor::ToRotationMatrix()
{
    return M4();
}
