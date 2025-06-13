#ifndef ACTOR_H_
#define ACTOR_H_
#include <cstdlib>
#include <cmath>
#include <random>
#include "GraphObject.h"
class StudentWorld;

using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
    //parametized constructor
    Actor(StudentWorld* sp, int ID, int x, int y, Direction dir, double siz, unsigned int dep, bool visible);
    virtual void doSomething() = 0;

    virtual ~Actor(); // virtual destructor

    //returns true if actor is alive
    bool isAlive() const;

    // getWorld function to return the StudentWorld pointer
    //void setWorld(StudentWorld*& sp) const;
    // Get this actor's world
    StudentWorld* getWorld() const { return sp; };

    // Mark this actor as dead.
    virtual void setDead();

    // Annoy this actor.
    //virtual bool annoy(unsigned int amt);

    //// Can this actor dig through Ice?
    //virtual bool canDigThroughIce() const;

    //// Can other actors pass through this actor?
    //virtual bool canActorsPassThroughMe() const;

    //// Can this actor pick items up?
    //virtual bool canPickThingsUp() const;

    //// Does this actor hunt the IceMan?
    //virtual bool huntsIceMan() const;

    //// Can this actor need to be picked up to finish the level?
    //virtual bool needsToBePickedUpToFinishLevel() const;

    // Move this actor to x,y if possible, and return true; otherwise,
    // return false without moving.
    bool moveToIfPossible(int x, int y);

protected:
    // current level number, used for calculating ticks to wait between moves for protester
        //int currentLevelNumber = sp->getLevel();
    StudentWorld* sp = nullptr;

private:
    bool m_isAlive = true; // alive or dead
};

class Agent : public Actor {
public:
    Agent(StudentWorld* sp, int ID, int x, int y, Direction dir,
        unsigned int hitPoints);
    virtual bool annoy(unsigned int amount);
    //void setDead() { m_isAlive = false; }
    // Pick up a gold nugget.
    virtual void addGold() = 0;
    //virtual bool canDigThroughIce() const;
    // How many hit points does this actor have left?
    unsigned int getHitPoints() const;
    virtual bool leavingOilField() { return false; }
    //virtual bool canPickThingsUp() const;

protected:
    unsigned int hitPoints;
    bool m_isAlive = true;
};



class Iceman : public Agent {
public:

    //contructor declaration
    Iceman(StudentWorld* sp);
    void doSomething();

    //bool annoy(unsigned int amount); // does it need to be virtual 
    //void setDead() override;
    //virtual bool canDigThroughIce() const;
    void addGold();
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
    unsigned int m_hits = 10;
    unsigned int m_squirts = 5;
    unsigned int m_sonar = 1;
    unsigned int m_gold = 0;
    //StudentWorld* sp = nullptr;
    bool m_isAlive = true;

};


class Ice : public Actor {
public:

    //contructor declaration
    Ice(StudentWorld* sp, int x, int y);
    virtual void doSomething() override;
    ~Ice();

};

//******************************** Protestors *******************************
class Protester : public Agent {

public:
    Protester(StudentWorld* sp, int ID, unsigned int hitPoints, unsigned int score);
    virtual void doSomething();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    unsigned int getGold() const { return m_gold; } // getter for gold
    //virtual bool huntsIceMan() const;

    // Set state to having gien up protest
    void setMustLeaveOilField();
	bool leavingOilField() const { return leave; } // getter for leave state

    // Set number of ticks until next move
    void setTicksToNextMove();
    void setSquaresToMoveInCurrentDirection();

protected:
    bool leave = false; // starts off not leaving
    bool shouted = false;
    int shoutTicks = 0;
    bool resting = false; // starts off not resting
    int restingTicks = 0;
    int perpenTicks = 80; // number of ticks since last perpendicular turn
    int numSquaresToMoveInCurrentDirection = 35;
    unsigned int m_gold = 0;
};


class RegularProtester : public Protester {
public:
    RegularProtester(StudentWorld* sp);
    void doSomething() override;

};

class HardcoreProtester : public Protester {

public:
    HardcoreProtester(StudentWorld* sp);
    void doSomething() override;
    void addGold() override;
};


class Boulder : public Actor {
public:
    Boulder(StudentWorld* sp, int x, int y);
    //virtual bool canActorsPassThroughMe() const;
    void doSomething() override;

private:
    bool fallingState = false; // starts off not falling
    int restingTicks = -1; 
};

class Squirt : public Actor
{
public:
    Squirt(StudentWorld* sp, int x, int y, Direction dir, int m_distance);
    void doSomething() override;

private:
    int m_distance = 4;
};


//******************************** Activating Objects *******************************

class ActivatingObject : public Actor
{
public:
    ActivatingObject(StudentWorld* sp, int x, int y, int ID);
    ~ActivatingObject();
    //bool needsToBePickedUpToFinishLevel() const;
    // Set number of ticks until this object dies
    //bool canActorsPassThroughMe() const;
    virtual void setTicksToLive();
    virtual void doSomething() = 0;
    // Getters for member variables
    /*bool activateOnPlayer() const { return m_activateOnPlayer; }
    bool activateOnProtester() const { return m_activateOnProtester; }
    bool isInitiallyActive() const { return m_initiallyActive; }*/

protected:
    int lifetime = -1;
    int trackTick = -1;
private:
    int ticksToLive = 0; // number of ticks until this object dies
    //int m_soundToPlay = SOUND_NONE; // sound to play when this object is activated
    //bool m_activateOnPlayer; // true if this object activates on player, false otherwise
    //bool m_activateOnProtester; // true if this object activates on protester, false otherwise
    //bool m_initiallyActive; // true if this object is initially active, false otherwise
};

class BarrelsOfOil : public ActivatingObject {
public:
    BarrelsOfOil(StudentWorld* sp, int x, int y);
    void doSomething() override;
    //virtual bool needsToBePickedUpToFinishLevel() const;
    ~BarrelsOfOil();
};

class GoldNugget : public ActivatingObject { // ghost block
public:
    GoldNugget(StudentWorld* sp, int x, int y, bool temporary);
    ~GoldNugget();
    void doSomething() override;
    void setTicksToLive();

private:
    bool temporary = false; // true if temporary, false if permanent

};


class SonarKit : public ActivatingObject
{
public:
    SonarKit(StudentWorld* sp);
    ~SonarKit();
    void doSomething() override;
    void setTicksToLive();
};

class WaterPool : public ActivatingObject
{
public:
    WaterPool(StudentWorld* sp, int x, int y);
    ~WaterPool();
    void doSomething() override;
    void setTicksToLive();
};



#endif // ACTOR_H_
