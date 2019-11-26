#include "Entities.hpp"
#include <cmath>

Vec::Vec(double x = 0, double y = 0){
    this->x = x; this->y = y;
}

MovingObject::MovingObject(Vec position, Vec velocity)
: pos{position}, vel{velocity}
{

}

void MovingObject::update(double dt){
    this->pos.x += this->vel.x * dt;
    this->pos.y += this->vel.y * dt;
}

Shot::Shot(Vec position, Vec velocity)
    : MovingObject(position, velocity)
{

}

Ship::Ship(Vec initialPos, int shipFrontSize, int shipHalfSide)
: MovingObject(initialPos, Vec(0, 0)), thrust{200}, dir{Vec(0, 1)}, engineOn{false},
     angle{0}, isRotatingLeft{false}, isRotatingRight{false}, isFiring{false}
{
    this->shipFrontSize = shipFrontSize;
    this->shipHalfSide = shipHalfSide;
};

void Ship::update(double dt){
    if(isRotatingLeft) this->rotate(-4, dt);
    if(isRotatingRight) this->rotate(4, dt);
    if(this->engineOn){
        this->vel.x += this->dir.x * thrust * dt;
        this->vel.y += this->dir.y * thrust * dt;
    }
    this->pos.x += this->vel.x * dt;
    this->pos.y += this->vel.y * dt;
}

void Ship::setEngineOn(){
    this->engineOn = true;
}
void Ship::setEngineOff(){
    this->engineOn = false;
}
void Ship::rotate(double factor, double dt){
    this->angle += factor * dt;
    double x = this->dir.x;
    double y = this->dir.y;
    double angleSin = std::sin(factor*dt);
    double angleCos = std::cos(factor*dt);
    this->dir.x = x*angleCos - y*angleSin;
    this->dir.y = x*angleSin + y*angleCos;
}

Shot Ship::shoot(){
    Vec shotVel = this->dir;
    shotVel.x *= 600;
    shotVel.y *= 600;
    shotVel.x += this->vel.x;
    shotVel.y += this->vel.y;
    double posX = this->pos.x + this->dir.x*shipFrontSize;
    double posY = this->pos.y + this->dir.y*shipFrontSize;
    return Shot(Vec(posX, posY), shotVel);
}

Asteroid::Asteroid(Vec position, Vec velocity, int asteroidSize)
: MovingObject(position, velocity), size{asteroidSize}
{

}

Debris::Debris(Vec position, Vec velocity)
: MovingObject(position, velocity), existenceTime{0}
{

}
void Debris::update(double dt){
    this->pos.x += this->vel.x * dt;
    this->pos.y += this->vel.y * dt;
    this->existenceTime += dt;
}