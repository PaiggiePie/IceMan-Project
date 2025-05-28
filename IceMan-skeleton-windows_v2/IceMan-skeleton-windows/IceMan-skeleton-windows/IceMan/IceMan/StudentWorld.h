#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <iomanip>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp




class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
        : GameWorld(assetDir)
    {
    }

    virtual int init();

    virtual int move();
    /*{
        // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
        // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }*/

    virtual void cleanUp()
    {
    }

private:
    std::vector<Actor*> actors;
};


/*class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
        : GameWorld(assetDir)
    {
        
    }

    virtual int init();
    virtual int move();
    {
        // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
        // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    virtual void cleanUp();
    
    ~StudentWorld(){
        
    }

    //getter functions
    virtual const int getCurrentGameLevel() { return gameLevel; };
    virtual const int getNumLivesLeft() { return lives; };
    virtual const int getCurrentHealth() { return health; };
    virtual const int getSquirtsLeftInSquirtGun() { return squirtGun; };
    virtual const int getPlayerGoldCount() { return gold; };
    virtual const int getNumberOfBarrelsRemainingToBePickedUp() { return barrels; };
    virtual const int getPlayerSonarChargeCount() { return sonarCharge; };
    virtual const int getCurrentScore() { return score; };

    //setter functions
    virtual void setDisplayText() {
        int level = getCurrentGameLevel();
        int lives = getNumLivesLeft();
        int health = getCurrentHealth();
        int squirts = getSquirtsLeftInSquirtGun();
        int gold = getPlayerGoldCount();
        int barrelsLeft = getNumberOfBarrelsRemainingToBePickedUp();
        int sonar = getPlayerSonarChargeCount();
        int score = getCurrentScore();

        // Next, create a string from your statistics, of the form:
        // Lvl: 52 Lives : 3 Hlth : 80 % Wtr : 20 Gld : 3 Oil Left : 2 Sonar : 1 Scr : 321000
        std::string s;
        s = "Lvl: " + (level > 10 ? "" : "_") + std::to_string(level) + "  Lives : "
            + std::to_string(lives) + "  Hlth : "
            + (health > 100 ? "" : "_") + health + "%  Wtr : "
            + (squirts > 10 ? "" : "_") + std::to_string(squirts) + "  Gld : "
            + (gold > 10 ? "" : "_") + std::to_string(gold) + "  Oil Left : "
            + (barrelsLeft > 10 ? "" : "_") + std::to_string(barrelsLeft) + "  Sonar : "
            + (sonar > 10 ? "" : "_") + std::to_string(sonar) + "  Scr : "
            + std::string(6 - std::to_string(score).length(), '0') + std::to_string(score);
        GameWorld::setGameStatText(s); // in GameWorld.cpp
    }

private:
    int gameLevel;
    int lives;
    int health;
    int squirtGun;
    int gold;
    int barrels;
    int sonarCharge;
    int score;
};
*/

#endif // STUDENTWORLD_H_
