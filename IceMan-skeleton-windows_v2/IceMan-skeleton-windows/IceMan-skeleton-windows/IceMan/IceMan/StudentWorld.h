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
    StudentWorld(std::string assetDir); //contructor1

    ~StudentWorld() {
        // destructor to clean up memory
        for (auto& object : aobj) {
            delete object; // delete all actors
        }
        aobj.clear();
        for (auto& iceObj : iceField) {
            delete iceObj; // delete all ice objects
        }
        iceField.clear();
        cout << "StudentWorld dtor" << endl;

    };

    virtual int init();

    virtual int move();

    virtual void cleanUp();

    void addActor(Actor* a);
    void clearIce(int x, int y);
    bool canActorMoveTo(Actor* a, int x, int y) const;
    int annoyAllNearbyActors(Actor* annoyer, int points, int radius);
    void revealAllNearbyObjects(int x, int y, int radius);
    Actor* findNearbyIceMan(Actor* a, int radius) const;
    Actor* findNearbyPickerUpper(Actor* a, int radius) const;
    void annoyIceMan();
    void giveIceManSonar();
    void giveIceManWater();
    bool facingTowardIceMan(Actor* a) const;
    GraphObject::Direction lineOfSightToIceMan(Actor* a) const;
    bool isNearIceMan(Actor* a, int radius) const;

    GraphObject::Direction determineFirstMoveToExit(int x, int y);
    GraphObject::Direction determineFirstMoveToIceMan(int x, int y);




    //getter functions
    virtual const int getCurrentHealth() { return iceman->getHitPoints(); };
    virtual const int getSquirtsLeftInSquirtGun() { return iceman->getWater(); };
    virtual const int getPlayerGoldCount() { return iceman->getGold(); };
    virtual const int getBarrelsRemaining() {
        for (int i = 0; i < aobj.size(); i++) {
            if (aobj[i]->getID() == IID_BARREL) {
                return aobj[i]->needsToBePickedUpToFinishLevel();
            }
        }
        return 0; // if no barrels, return 0
    };
    virtual const int getPlayerSonarChargeCount() { return iceman->getSonar(); };
	virtual const int getTicks() const { return ticks; };

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
        s = "Lvl: " + (string)(level > 10 ? "" : "_") + std::to_string(level) + "  Lives : "
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
    Iceman* iceman;
	std::vector<Squirt*> squirts; // for squirts
    std::vector<Protester*> protesters; // Protesters
    std::vector<ActivatingObject*> aobj; // for barrels, gold, sonar, waterpools
    std::vector<Ice*> iceField; // ice, boulders
    int ticks = 0; // global variable to keep track of ticks
    int currentLevelNumber;

};


#endif // STUDENTWORLD_H_