#ifndef ACTOR_H_
#define ACTOR_H_
#include <cstdlib>
#include <ctime>
#include <random>
#include "GraphObject.h"
class StudentWorld;

using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:

    virtual void doSomething() = 0;
    //parametized constructor
    Actor(StudentWorld* sp, int ID, int x, int y, Direction dir, double siz, unsigned int dep, bool visible);

    virtual ~Actor();

    //returns true if actor is alive
    bool isAlive() const;

    // getWorld function to return the StudentWorld pointer
    virtual void setWorld(StudentWorld*& sp) const;
    // Get this actor's world
    StudentWorld* getWorld() const { return sp; };

    // Mark this actor as dead.
    void setDead();

    // Annoy this actor.
    virtual bool annoy(unsigned int amt);

    // Can this actor dig through Ice?
    virtual bool canDigThroughIce() const;

    // Can other actors pass through this actor?
    virtual bool canActorsPassThroughMe() const;

    // Can this actor pick items up?
    virtual bool canPickThingsUp() const;

    // Does this actor hunt the IceMan?
    virtual bool huntsIceMan() const;

    // Can this actor need to be picked up to finish the level?
    virtual bool needsToBePickedUpToFinishLevel() const;

    // Move this actor to x,y if possible, and return true; otherwise,
    // return false without moving.
    bool moveToIfPossible(int x, int y);

protected:
    // current level number, used for calculating ticks to wait between moves for protester
        //int currentLevelNumber = sp->getLevel();
    StudentWorld* sp = nullptr;

private:
    bool m_isAlive{ true };//alive or dead
};

class Agent : public Actor {
public:
    Agent(StudentWorld* sp, int ID, int x, int y, Direction dir,
        unsigned int hitPoints);
    virtual bool annoy(unsigned int amount);

    // Pick up a gold nugget.
    virtual void addGold() = 0;
    virtual bool canDigThroughIce() const;
    // How many hit points does this actor have left?
    unsigned int getHitPoints() const;

    virtual bool canPickThingsUp() const;

private:
    unsigned int hitPoints = 10;
};



class Iceman : public Agent {
public:

    //contructor declaration
    Iceman(StudentWorld* sp);
    virtual void doSomething();

    virtual bool annoy(unsigned int amount);
    virtual bool canDigThroughIce() const;
    virtual void addGold();
    // Pick up a sonar kit.
    void addSonar();

    // Pick up water.
    void addWater();

    // Get amount of gold
    unsigned int getGold() const;

    // Get amount of sonar charges
    unsigned int getSonar() const;

    // Get amount of water
    unsigned int getWater() const;
    ~Iceman();
private:
    int m_hits = 10;
    int m_squirts = 5;
    int m_sonar = 1;
    int m_gold = 0;
    //StudentWorld* sp = nullptr;
    bool m_isAlive = true;

};


class Ice : public Actor {
public:

    //contructor declaration
    Ice(StudentWorld* sp);
    virtual void doSomething() override;
    ~Ice();

private:
    //StudentWorld* sp = nullptr;

};



//******************************** Protestors *******************************
class Protester : public Agent {

public:
    Protester(StudentWorld* sp, int x, int y, int ID,
        unsigned int hitPoints, unsigned int score);
    virtual void doSomething();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool huntsIceMan() const;

    // Set state to having gien up protest
    void setMustLeaveOilField();

    // Set number of ticks until next move
    void setTicksToNextMove();
private:
    int m_gold = 0;
    bool m_state = false; //starts off not giving up protest
};


class RegularProtester : public Protester {
public:
    RegularProtester(StudentWorld* sp, int x, int y, int ID);
    virtual void doSomething();
    virtual void addGold();
	int getGold() const { return gold; } // getter for gold

private:
	int gold = 0; // number of gold nuggets collected by this protester
};

class HardcoreProtester : public Protester {

public:
    HardcoreProtester(StudentWorld* sp, int x, int y, int ID);
    virtual void doSomething();
    virtual void addGold();
    int getGold() const { return gold; } // getter for gold


private:
    int gold = 0; // number of gold nuggets collected by this protester
};


class Boulder : public Actor {
public:
    Boulder(StudentWorld* sp);
    virtual bool canActorsPassThroughMe() const;
    virtual void doSomething();

private: 
	bool fallingState = false; // starts off not falling
};

class Squirt : public Actor
{
public:
    Squirt(StudentWorld* sp, int x, int y, Direction dir);
    virtual void doSomething();
};


//******************************** Activating Objects *******************************

class ActivatingObject : public Actor
{
public:

    ActivatingObject(StudentWorld* sp, int x, int y, int ID,
        int soundToPlay, bool activateOnPlayer,
        bool activateOnProtester, bool initallyActive);

     bool needsToBePickedUpToFinishLevel() const;
    // Set number of ticks until this object dies
    bool canActorsPassThroughMe() const;
    void setTicksToLive();

private:
	//int m_ticksToLive = 0; // number of ticks until this object dies
	//int m_soundToPlay; // sound to play when this object is activated
	//bool m_activateOnPlayer; // true if this object activates on player, false otherwise
	//bool m_activateOnProtester; // true if this object activates on protester, false otherwise
	//bool m_initiallyActive; // true if this object is initially active, false otherwise
};

class BarrelsOfOil : public ActivatingObject {
public:
    BarrelsOfOil(StudentWorld* sp);
    virtual void doSomething();
    virtual bool needsToBePickedUpToFinishLevel() const;
    ~BarrelsOfOil();
};

class GoldNugget : public ActivatingObject { // ghost block
public:
    GoldNugget(StudentWorld* sp, bool temporary);
    virtual void doSomething();

private:
    bool pickup = true; // true for iceman; false for protester
	bool temporary = false; // true if temporary, false if permanent

};


class SonarKit : public ActivatingObject
{
public:
    SonarKit(StudentWorld* sp);
    virtual void doSomething();
};

class WaterPool : public ActivatingObject
{
public:
    WaterPool(StudentWorld* sp);
    virtual void doSomething();
};



#endif // ACTOR_H_

