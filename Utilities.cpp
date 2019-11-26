#include "Utilities.hpp"
#include <cmath>

Asteroid createAsteroid(int asteroidMaxSize, int asteroidMinimumSize, int windowWidth, int windowHeight, double asteroidMinimumVelocity){
    double decision = rand() % 10 - 5;
    double x, y;
    double size = rand() % (asteroidMaxSize - asteroidMinimumSize) + asteroidMinimumSize;  
    if(decision > 0){
        x = rand() % windowWidth;
        y = 0 - size;
    }
    else{
        x = 0 - size;
        y = rand() % windowHeight;
    }
    double velX = 0;
    double velY = 0;
    while(velX*velX + velY*velY < asteroidMinimumVelocity*asteroidMinimumVelocity){
        velX = ((rand() % 200)/ 100.f - 1) * 50;
        velY = ((rand() % 200)/ 100.f - 1) * 50;
    }
    return Asteroid(Vec(x, y), Vec(velX, velY), size);
}

std::vector<Asteroid> explodeAsteroid(Asteroid & ast, int maxAsteroidFragments, int asteroidMinimumSize, int numberAsteroids, int currentNumberAsteroids){
    std::vector<Asteroid> newAsts;
    int maxFragments = numberAsteroids - currentNumberAsteroids;
    double numberOfFragments = rand() % (maxAsteroidFragments + 1);
    if(numberOfFragments > maxFragments) numberOfFragments = maxFragments;
    int asteroidsNewSize = ast.size/2;
    if(asteroidsNewSize > asteroidMinimumSize){
        double velX = ast.vel.x;
        double velY = ast.vel.y;
        double vel = std::sqrt(velX*velX + velY*velY) * 0.8;
        Vec dir = ast.vel;
        double astVel = std::sqrt(ast.vel.x*ast.vel.x + ast.vel.y*ast.vel.y);
        dir.x /= astVel;
        dir.y /= astVel;

        double newDirX = -dir.y;
        double newDirY = dir.x;
        dir.x = newDirX;
        dir.y = newDirY;

        double angleStep = 2*3.1415/numberOfFragments;
        double angleSin = std::sin(angleStep);
        double angleCos = std::cos(angleStep);
        for(int i = 0; i < numberOfFragments; i++){
            newAsts.push_back(Asteroid(Vec(ast.pos.x + dir.x*asteroidsNewSize, ast.pos.y + dir.y*asteroidsNewSize),
                                     Vec(vel*dir.x + ast.vel.x, vel*dir.y + ast.vel.y), asteroidsNewSize));
            double newDirX = dir.x*angleCos - dir.y*angleSin;
            double newDirY = dir.x*angleSin + dir.y*angleCos;
            dir.x = newDirX;
            dir.y = newDirY;
        }
    }
    return newAsts;
}

std::vector<Debris> generateDebris(Asteroid & ast, int numberOfDebris){
    double angleStep = 2*3.1415/numberOfDebris;
    double debrisVel = std::sqrt(ast.vel.x*ast.vel.x + ast.vel.y*ast.vel.y);
    double velX, velY, x, y;
    double angleSin = std::sin(angleStep);
    double angleCos = std::cos(angleStep);
    Vec dir(0, 1);
    std::vector<Debris> generatedDebris;
    for(int i = 0; i < numberOfDebris; i++){
        velX = dir.x * debrisVel + ast.vel.x;
        velY = dir.y * debrisVel + ast.vel.y;
        x = ast.pos.x + dir.x;
        y = ast.pos.y + dir.y;
        generatedDebris.push_back(Debris(Vec(x, y), Vec(velX, velY)));
        double newDirX = dir.x*angleCos - dir.y*angleSin;
        double newDirY = dir.x*angleSin + dir.y*angleCos;
        dir.x = newDirX;
        dir.y = newDirY;
    }
    return generatedDebris;
};