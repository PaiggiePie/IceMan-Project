#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <iomanip>


class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    ~StudentWorld();
    
    
    
    // Add an actor to the world.
        void addActor(Actor* a);
        
          // Clear a 4x4 region of Ice.
        void clearIce(int x, int y);

          // Can actor move to x,y?
        bool canActorMoveTo(Actor* a, int x, int y) const;

          // Annoy all other actors within radius of annoyer, returning the
          // number of actors annoyed.
        int annoyAllNearbyActors(Actor* annoyer, int points, int radius);

          // Reveal all objects within radius of x,y.
        void revealAllNearbyObjects(int x, int y, int radius);

    
          // If the IceMan is within radius of a, return a pointer to the
          // IceMan, otherwise null.
        Actor* findNearbyIceMan(Actor* a, int radius) const;

          // If at least one actor that can pick things up is within radius of a,
          // return a pointer to one of them, otherwise null.
        Actor* findNearbyPickerUpper(Actor* a, int radius) const;

          // Annoy the IceMan.
        void annoyIceMan();

          // Give IceMan some sonar charges.
        void giveIceManSonar();

          // Give IceMan some water.
        void giveIceManWater();

          // Is the Actor a facing toward the IceMan?
        bool facingTowardIceMan(Actor* a) const;

          // If the Actor a has a clear line of sight to the IceMan, return
          // the direction to the IceMan, otherwise GraphObject::none.
        GraphObject::Direction lineOfSightToIceMan(Actor* a) const;

          // Return whether the Actor a is within radius of IceMan.
        bool isNearIceMan(Actor* a, int radius) const;

          // Determine the direction of the first move a quitting protester
          // makes to leave the oil field.
        GraphObject::Direction determineFirstMoveToExit(int x, int y);

          // Determine the direction of the first move a hardcore protester
          // makes to approach the IceMan.
        GraphObject::Direction determineFirstMoveToIceMan(int x, int y);
    


    
    //getter functions
    virtual const int getCurrentGameLevel() { return gameLevel; }
    virtual const int getNumLivesLeft() { return lives; }
    virtual const int getCurrentHealth() { return health; }
    virtual const int getSquirtsLeftInSquirtGun() { return squirtGun;}
    virtual const int getPlayerGoldCount() { return gold; }
    virtual const int getNumberOfBarrelsRemainingToBePickedUp() { return barrels; }
    virtual const int getPlayerSonarChargeCount() { return sonarCharge; }
    virtual const int getCurrentScore() { return score; }

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
        s = "Lvl: " + (string) (level > 10 ? "" : "_") + std::to_string(level) + "  Lives : "
            + std::to_string(lives) + "  Hlth : "
            + (health > 100 ? "" : "_") + to_string(health) + "%  Wtr : "
            + (squirts > 10 ? "" : "_") + std::to_string(squirts) + "  Gld : "
            + (gold > 10 ? "" : "_") + std::to_string(gold) + "  Oil Left : "
            + (barrelsLeft > 10 ? "" : "_") + std::to_string(barrelsLeft) + "  Sonar : "
            + (sonar > 10 ? "" : "_") + std::to_string(sonar) + "  Scr : "
            + std::string(6 - std::to_string(score).length(), '0') + std::to_string(score);
        GameWorld::setGameStatText(s); // in GameWorld.cpp
    }

private:

    
    std::vector <Actor*> actors {}; //ice boulders, squirt
    std::vector<Agent*> agents {}; // Icemean, Protesters
    std::vector<ActivatingObject*> aobj; // barrels, gold, sonar, waterpool
    std::vector <Ice*> iceField;
    
    int ticks = 0;
    int currentLevelNumber;
    int gameLevel;
    int health;
    int lives;
    int squirtGun;
    int gold;
    int barrels;
    int sonarCharge;
    int score;
    
};


#endif // STUDENTWORLD_H_
