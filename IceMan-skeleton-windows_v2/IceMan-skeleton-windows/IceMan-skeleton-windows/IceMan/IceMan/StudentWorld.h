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
    
    //our defined functions
    virtual Iceman* getIceman() const;
    virtual void setWorld(StudentWorld*& sp);
    void setIceman(Iceman* a);

    // Add an agent to the world.
    void addObj(ActivatingObject* a);

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
    bool nearBoulder(int x, int y, int radius) const;
      // If at least one actor that can pick things up is within radius of a,
      // return a pointer to one of them, otherwise null.
    //Agent* findNearbyPickerUpper(Agent* a, int radius) const;
    Agent* findNearbyPickerUpper(Actor* a, int radius) const;

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
    virtual const int getCurrentHealth() { return iceman->getHitPoints(); };
    virtual const int getSquirtsLeftInSquirtGun() { return iceman->getWater(); };
    virtual const int getPlayerGoldCount() { return iceman->getGold(); };
    virtual const int getBarrelsRemaining() {
        int barrels = 0;
        for (int i = 0; i < aobj.size(); i++) {
            if (aobj[i]->getID() == IID_BARREL) {
                barrels++;
            }
        }
        //cout << barrels  << endl;
        return barrels; // if no barrels, return 0
    };
    
    virtual const int getPlayerSonarChargeCount() { return iceman->getSonar(); };
    
    virtual const int getTicks() const;

    //setter functions
    virtual void setDisplayText() {
        int level = getLevel();
        int lives = getLives();
        int health = getCurrentHealth();
        int squirts = getSquirtsLeftInSquirtGun();
        int gold = getPlayerGoldCount();
        int barrelsLeft = getBarrelsRemaining();
        int sonar = getPlayerSonarChargeCount();
        int score = getScore();
        
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

    Iceman* iceman = nullptr;
    std::vector<Agent*> agents; // Protesters & iceman
    std::vector<ActivatingObject*> aobj; // for barrels, gold, sonar, waterpools
    std::vector<Actor*> iceField; // ice, boulders, squirts
    int ticks = 1; // global variable to keep track of ticks
    int currentLevelNumber = 1;
    
};


#endif // STUDENTWORLD_H_
