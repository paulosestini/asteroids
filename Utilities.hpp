#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include "Entities.hpp"

Asteroid createAsteroid(int asteroidMaxSize, int asteroidMinimumSize, int windowWidth, int windowHeight, double asteroidMinimumVelocity);
std::vector<Asteroid> explodeAsteroid(Asteroid & ast, int maxAsteroidFragments, int asteroidMinimumSize, int numberAsteroids, int currentNumberAsteroids);
std::vector<Debris> generateDebris(Asteroid & ast, int numberOfDebris);

#endif