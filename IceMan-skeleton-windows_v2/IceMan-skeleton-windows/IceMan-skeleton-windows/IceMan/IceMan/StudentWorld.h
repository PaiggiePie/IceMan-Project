#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <iomanip>
#include <unordered_map>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir); //contructor1

    ~StudentWorld() {
        // destructor to clean up memory
        for (auto& agent : agents) {
            delete agent; // delete all actors
        }
        agents.clear();
        for (auto& object : aobj) {
            delete object; // delete all actors
        }
        aobj.clear();
        for (auto& iceObj : iceField) {
            delete iceObj; // delete all ice objects
        }
        iceField.clear();
        //cout << "StudentWorld dtor" << endl;

    };

    virtual int init();

    virtual int move();

    virtual void cleanUp();

    void addObj(ActivatingObject* a);
    void addActor(Actor* a);
    Iceman* getIceman() const { return iceman; } // getter for iceman
    void clearIce(int x, int y);
    bool canActorMoveTo(Actor* a, int x, int y) const;
    int annoyAllNearbyActors(Actor* annoyer, int points, int radius);
    void revealAllNearbyObjects(int x, int y, int radius);
    Iceman* findNearbyIceMan(Actor* a, int radius) const;
    Agent* findNearbyPickerUpper(Actor* a, int radius) const;
    //void annoyIceMan();
    void giveIceManSonar();
    void giveIceManWater();
    bool facingTowardIceMan(Actor* a) const;
    GraphObject::Direction lineOfSightToIceMan(Actor* a, bool facing) const;
    bool isNearIceMan(Actor* a, int radius) const;
    bool NearBoulder(int x, int y, int radius) const;
    bool NearItem(int x, int y, int radius) const;
    bool atItem(int x, int y, bool ice) const;
    GraphObject::Direction determineFirstMoveToExit(int x, int y);
    GraphObject::Direction determineFirstMoveToIceMan(int x, int y);

    //getter functions
    const int getCurrentHealth() {
        return (int)((iceman->getHitPoints() * 100) / 10);
    };
    const int getSquirtsLeftInSquirtGun() { return iceman->getWater(); };
    const int getPlayerGoldCount() { return iceman->getGold(); };
    const int getBarrelsRemaining() { return barrels; };
    void decBarrels() { barrels--; };
    const int getPlayerSonarChargeCount() { return iceman->getSonar(); };
    const int getTicks() const { return ticks; };

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
        s = "Lvl: " + (string)(level >= 10 ? "" : "_") + std::to_string(level) + "  Lives : "
            + std::to_string(lives) + "  Hlth : "
            + (health == 100 ? "" : "_") + to_string(health) + "%  Wtr : "
            + (squirts >= 10 ? "" : "_") + std::to_string(squirts) + "  Gld : "
            + (gold >= 10 ? "" : "_") + std::to_string(gold) + "  Oil Left : "
            + (barrelsLeft >= 10 ? "" : "_") + std::to_string(barrelsLeft) + "  Sonar : "
            + (sonar >= 10 ? "" : "_") + std::to_string(sonar) + "  Scr : "
            + std::string(6 - std::to_string(score).length(), '0') + std::to_string(score);
        GameWorld::setGameStatText(s); // in GameWorld.cpp
    }

private:
    Iceman* iceman = nullptr;
    std::vector<Agent*> agents; // Protesters & iceman
    std::vector<ActivatingObject*> aobj; // for barrels, gold, sonar, waterpools
    std::vector<Actor*> iceField; // ice, boulders, squirts
    int ticks = 1; // global variable to keep track of ticks
    int barrels = 0;
    int currentLevelNumber = 0;

    std::unordered_map<int, int> coords;

};


#endif // STUDENTWORLD_H_
