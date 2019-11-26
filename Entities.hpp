#ifndef ENTITIES_H
#define ENTITIES_H

class Vec{
public:
    Vec(double x, double y);
    double x, y;
};

class MovingObject{
public:
    MovingObject(Vec position, Vec velocity);
    void update(double dt);
    Vec pos, vel;
};

class Shot: public MovingObject{
public:
    Shot(Vec position, Vec velocity);
};

class Ship: public MovingObject{
public:
    Ship(Vec initialPos, int shipFrontSize, int shipHalfSide);
    void update(double dt);
    void setEngineOn();
    void setEngineOff();
    void rotate(double factor, double dt);
    Shot shoot();    
    Vec dir;
    double thrust, angle;
    bool engineOn, isFiring, isRotatingLeft, isRotatingRight;
    int shipFrontSize, shipHalfSide;
};

class Asteroid: public MovingObject{
public:
    Asteroid(Vec position, Vec velocity, int asteroidSize);
    int size;
};

class Debris: public MovingObject{
public:
    Debris(Vec position, Vec velocity);
    void update(double dt);
    double existenceTime;

};

#endif