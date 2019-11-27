#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <chrono>
#include <list>
#include <ctime>
#include <string>
#include "Utilities.hpp"

int windowWidth = 800;
int windowHeight = 600;
int maxAsteroidsToSpawn = 10;
int numberAsteroids = 20;
int maxAsteroidFragments = 3;
double asteroidMinimumVelocity = 1;
int asteroidMaxSize = 100;
int asteroidMinimumSize = 20;
int numberOfDebris = 8;
double debrisMaxTime = 2;
int debrisSize = 4;
double shipFrontSize = 23;
double shipHalfSide = 13;
double shipFireLargeSize = 28;
double shipFireShortSize = 20;
double shipFireHalfSide = 8;
double shotRadius = 3;

double dt = 0;

int main(){
    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Asteroids");

    sf::SoundBuffer shotBuffer;
    sf::SoundBuffer explosionBuffer;
    sf::SoundBuffer engineBuffer;
    sf::Sound shotSound;
    sf::Sound explosionShound;
    sf::Sound engineSound;
    shotBuffer.loadFromFile("Sounds/shot.wav");
    shotSound.setBuffer(shotBuffer);
    explosionBuffer.loadFromFile("Sounds/explosion.wav");
    explosionShound.setBuffer(explosionBuffer);
    engineBuffer.loadFromFile("Sounds/engine.wav");
    engineSound.setBuffer(engineBuffer);

    int score = 0;
    sf::Font font;
    font.loadFromFile("Fonts/arial.ttf");
    sf::Text scoreText("Score: 0", font);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setCharacterSize(18);
    scoreText.setPosition(sf::Vector2f(windowWidth - 200, 10));

    Ship ship(Vec(windowWidth/2, windowHeight/2), shipFrontSize, shipHalfSide);
    sf::VertexArray shipShape(sf::Triangles, 3);
    shipShape[0].color = sf::Color::Blue;
    shipShape[1].color = sf::Color::Blue;
    shipShape[2].color = sf::Color::Blue;
    
    sf::VertexArray shipFire(sf::Triangles, 3);
    shipFire[0].color = sf::Color::Yellow;
    shipFire[1].color = sf::Color::Yellow;
    shipFire[2].color = sf::Color::Yellow;
    double shipFireSize = shipFireLargeSize;
    bool swapFireSize = false;
    double swapTime = 0;

    std::list<Shot> shots;
    sf::CircleShape shotShape(shotRadius);
    shotShape.setFillColor(sf::Color::Red);
    double shootTime = 0;

    std::list<Asteroid> asteroids;
    sf::CircleShape asteroidShape(10);
    asteroidShape.setFillColor(sf::Color::Transparent);
    asteroidShape.setOutlineColor(sf::Color::Cyan);
    asteroidShape.setOutlineThickness(2);

    std::list<Debris> debris;
    sf::CircleShape debrisShape(debrisSize);
    debrisShape.setFillColor(sf::Color::Transparent);
    debrisShape.setOutlineColor(sf::Color::Cyan);
    debrisShape.setOutlineThickness(1);

    while(window.isOpen()){
        
        auto start = std::chrono::steady_clock::now();

        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
        
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ship.setEngineOn();
            else ship.setEngineOff();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) ship.isRotatingRight = true;
            else ship.isRotatingRight = false;

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ship.isRotatingLeft = true;
            else ship.isRotatingLeft = false;
        
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) ship.isFiring = true;
            else ship.isFiring = false;
        }

        window.clear(sf::Color::Black);
        
        ship.update(dt);

        if(ship.isFiring){
            if(shootTime >= 0.2){
                shots.push_back(ship.shoot());
                shotSound.play();
                shootTime = 0;
            }
            else shootTime += dt;
        }

        double sinAngleShip = std::sin(ship.angle);
        double cosAngleShip = std::cos(ship.angle);

        if(ship.pos.x < 0) ship.pos.x = windowWidth;
        else if (ship.pos.x > windowWidth) ship.pos.x = 0;
        if(ship.pos.y < 0) ship.pos.y = windowHeight;
        else if(ship.pos.y > windowHeight) ship.pos.y = 0;
        
        shipShape[0].position = sf::Vector2f(0*cosAngleShip - shipFrontSize*sinAngleShip + ship.pos.x, 0*sinAngleShip + shipFrontSize*cosAngleShip + ship.pos.y);
        shipShape[1].position = sf::Vector2f(shipHalfSide*(-cosAngleShip + sinAngleShip) + ship.pos.x, shipHalfSide*(-sinAngleShip - cosAngleShip) + ship.pos.y);
        shipShape[2].position = sf::Vector2f(shipHalfSide*(cosAngleShip + sinAngleShip) + ship.pos.x, shipHalfSide*(sinAngleShip - cosAngleShip) + ship.pos.y);

        
        if(ship.engineOn){
            if(!(engineSound.getStatus() == sf::SoundSource::Status::Playing)) engineSound.play();

            if(swapFireSize){
                shipFireSize = shipFireLargeSize;
            }
            else shipFireSize = shipFireShortSize;
            if(swapTime > 0.1) swapFireSize = !swapFireSize;
            swapTime += dt;

            shipFire[0].position = sf::Vector2f(0*cosAngleShip + shipFireSize*sinAngleShip + ship.pos.x, 0*sinAngleShip - shipFireSize*cosAngleShip + ship.pos.y);
            shipFire[1].position = sf::Vector2f(shipFireHalfSide*(-cosAngleShip + sinAngleShip) + ship.pos.x, shipFireHalfSide*(-sinAngleShip - cosAngleShip) + ship.pos.y);
            shipFire[2].position = sf::Vector2f(shipFireHalfSide*(cosAngleShip + sinAngleShip) + ship.pos.x, shipFireHalfSide*(sinAngleShip - cosAngleShip) + ship.pos.y);
            window.draw(shipFire);
        }
        else{
            if(engineSound.getStatus() == sf::SoundSource::Status::Playing) engineSound.stop();
        }

        window.draw(shipShape);
        
        if(asteroids.size() < maxAsteroidsToSpawn){
            asteroids.push_back(createAsteroid(asteroidMaxSize, asteroidMinimumSize, windowWidth, windowHeight, asteroidMinimumVelocity));
        }

        for(auto shot = shots.begin(); shot != shots.end();){
            shot->update(dt);
            shotShape.setPosition(sf::Vector2f(shot->pos.x - shotRadius, shot->pos.y - shotRadius));
            window.draw(shotShape);
            
            if(shot->pos.x > windowWidth || shot->pos.x < 0 || shot->pos.y > windowHeight || shot->pos.y < 0){
                auto it = shot;
                shot++;
                shots.erase(it);
            }
            else shot++;
        }

        bool shipCollided = false;
        for(auto asteroid = asteroids.begin(); asteroid != asteroids.end();){
            int size = asteroid->size;
            if(asteroid->pos.x  + size< 0) asteroid->pos.x = windowWidth + size;
            else if (asteroid->pos.x - size> windowWidth) asteroid->pos.x = 0 - size;
            if(asteroid->pos.y  + size< 0) asteroid->pos.y = windowHeight + size;
            else if(asteroid->pos.y - size> windowHeight) asteroid->pos.y = 0 - size;

            asteroid->update(dt);
            asteroidShape.setPosition(asteroid->pos.x - asteroid->size, asteroid->pos.y - asteroid->size);
            asteroidShape.setRadius(asteroid->size);
            window.draw(asteroidShape);
            
            double radius = asteroid->size;

            double shipDistanceX0 = asteroid->pos.x - shipShape[0].position.x;
            double shipDistanceY0 = asteroid->pos.y - shipShape[0].position.y;

            double shipDistanceX1 = asteroid->pos.x - shipShape[1].position.x;
            double shipDistanceY1 = asteroid->pos.y - shipShape[1].position.y;

            double shipDistanceX2 = asteroid->pos.x - shipShape[2].position.x;
            double shipDistanceY2 = asteroid->pos.y - shipShape[2].position.y;

            if(shipDistanceX1*shipDistanceX0 + shipDistanceY1*shipDistanceY0 < radius*radius) shipCollided = true;
            else if(shipDistanceX1*shipDistanceX1 + shipDistanceY1*shipDistanceY1 < radius*radius) shipCollided = true;
            else if(shipDistanceX1*shipDistanceX2 + shipDistanceY1*shipDistanceY2 < radius*radius) shipCollided = true;
            
            bool deletedAsteroid = false;
            for(auto shot = shots.begin(); shot != shots.end(); shot++){
                double distanceX = shot->pos.x - asteroid->pos.x;
                double distanceY = shot->pos.y - asteroid->pos.y;

                if(distanceX*distanceX + distanceY*distanceY < radius*radius){
                    score += asteroid->size * 10;

                    std::vector<Asteroid> newAsts = explodeAsteroid(*asteroid, maxAsteroidFragments, asteroidMinimumSize, numberAsteroids, asteroids.size()-1);
                    if(!newAsts.empty()){
                        for(auto ast: newAsts){
                            asteroids.push_back(ast);
                        }
                    }
                    std::vector<Debris> generatedDebris = generateDebris(*asteroid, numberOfDebris);
                    for(auto & newDebris: generatedDebris) debris.push_back(newDebris);
                    auto it = asteroid;
                    asteroid++;
                    asteroids.erase(it);
                    shots.erase(shot);
                    deletedAsteroid = true;
                    explosionShound.play();
                    break;
                }
            }
            if(!deletedAsteroid) asteroid++;
        }

        for(auto it = debris.begin(); it != debris.end(); it++){
            if(it->existenceTime < debrisMaxTime){
                it->update(dt);
                debrisShape.setPosition(it->pos.x, it->pos.y);
                debrisShape.setOutlineColor(sf::Color(0, 255, 255, 255*(1 - it->existenceTime/debrisMaxTime)));
                window.draw(debrisShape);
            }
            else{
                auto newIt = it;
                it++;
                debris.erase(newIt);
                if(it == debris.end()) break;
            }
        }

        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);

        window.display();
        if(shipCollided) window.close();

        auto end = std::chrono::steady_clock::now();
        dt = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count()*1e-9;
    }
    return 0;
}
