#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:

    //parametized constructor to allow each derived class to have unique position and size
    Actor(StudentWorld* sp, int ID, int x, int y, Direction dir, double siz, unsigned int dep);

    bool isAlive() const;
    void setDead();
    virtual bool annoy(unsigned int amt);
    virtual bool canActorsPassThroughMe() const;
    virtual bool canDigThroughIce() const;
    virtual bool canPickThingsUp() const;
    virtual bool huntsIceMan() const;
    virtual bool needsToBePickedUpToFinishLevel() const;
    bool moveToIfPossible(int x, int y);

    //virtual functions
    virtual void doSomething() = 0;
    virtual ~Actor();



    // getWorld function to return the StudentWorld pointer
	StudentWorld* setWorld(StudentWorld*& sp) const;
    virtual StudentWorld* getWorld() const {
		return sp;
    }

protected:
    // current level number, used for calculating ticks to wait between moves for protester
    //int currentLevelNumber = sp->getLevel(); 
    StudentWorld* sp = nullptr;
private:
    bool m_isAlive = true; //alive or dead

};

class Agent : public Actor {
public:
    // constructor
    Agent(StudentWorld* sp, int ID, int x, int y, Direction dir, double siz, unsigned int dep, unsigned int hitPoints) : Actor(ID, x, y, dir, siz, dep) {
        setVisible(true);
    }
    virtual void doSomething() = 0; // pure virtual function
    virtual ~Agent() {}

    virtual void addGold() = 0;
    unsigned int getHitPoints() const;
    virtual bool annoy(unsigned int amount);
    virtual bool canPickThingsUp() const;

private:
	unsigned int m_hitPoints; // hit points of the agent
	bool m_isAlive = true; // alive or dead

};

class Iceman : public Agent {
public:
    //contructor declaration
    Iceman(StudentWorld* sp);
    virtual void doSomething();
    ~Iceman();
	bool isAlive() const  // getter for m_isAlive
    {
        return m_isAlive;
    }

	//setter
    virtual bool annoy(unsigned int amount);
	virtual void addGold() { m_nuggets++; }
    virtual bool canDigThroughIce() const { return true; }
	virtual bool canActorsPassThroughMe() const { return true; }
	void addSonar() { m_sonar++; }
	void addWater() { m_squirts++; }
    
    //getter
	unsigned int getGold() const { return m_nuggets; }
	unsigned int getSonar() const { return m_sonar; }
	unsigned int getWater() const { return m_squirts; }




private:
    int m_hits = 10;
    int m_squirts = 5;
    int m_sonar = 1;
    int m_nuggets = 0;
    bool m_isAlive;
};


class Ice : public Actor {
public:

    //contructor declaration
    Ice(StudentWorld* sp);
    ~Ice();
    virtual void doSomething() override {
        // Ice does not perform any actions
    }
};



//******************************** Protester *******************************
class Protester : public Actor {
public:
    // constructor
    Protester(StudentWorld* sp) : Actor(IID_PROTESTER, 60, 60, left, 1.0, 0) {
        sp = sp;
        setVisible(true);
    }

    virtual void doSomething() {}


private:
    bool alive = true;
    //int ticksToWaitBetweenMoves = max(0, (3 - currentLevelNumber / 4));
};

class RegularProtester : public Protester {
public:
    RegularProtester(StudentWorld* sp);
    virtual void doSomething() override;

    bool is_Alive()
    {
        return alive;
    }

    bool is_LeavingField() {
        return leave_field;
    }

private:
    bool alive = true;
    int m_hits = 5;
    bool leave_field = false; // if true, protester will leave field after being hit
};

class HardcoreProtester : public Protester {
public:
    HardcoreProtester(StudentWorld* sp);
    virtual void doSomething() override;

    bool is_Alive() {
        return alive;
    }

    bool is_LeavingField() {
        return leave_field;
    }

private:
    bool alive = true;
    bool leave_field = false;
};


//******************************** Objects *******************************

class StaticObject : public Actor {
public:
	StaticObject(StudentWorld* sp) : Actor(IID_BARREL, 0, 0, right, 1.0, 2) {
		setVisible(true);
	}
	virtual void doSomething() {
		// Static objects do not perform any actions
	}
};

class DynamicObject : public Actor {
public:
	DynamicObject(StudentWorld* sp) : Actor(IID_BARREL, 0, 0, right, 1.0, 2) {
		setVisible(true);
	}
	virtual void doSomething() { }
};

class BarrelsOfOil : public StaticObject {
public:
    BarrelsOfOil(StudentWorld* sp);
    virtual void doSomething() override;

    bool is_Alive() {
        return alive;
    }

    ~BarrelsOfOil();

private:
    bool alive = true;
};

class Boulder : public DynamicObject {
public:
    Boulder(StudentWorld* sp);
    virtual void doSomething() override;
    bool is_Alive() {
        return alive;
    }

    //if boulder falls below ice, it is dead, (alive = false)

private:
    bool alive = true;

};

class GoldNugget : public StaticObject {
public:
    GoldNugget(StudentWorld* sp);
    virtual void doSomething() override;
    bool is_Alive() {
        return alive;
    }

private:
    bool alive = true;

};

class WaterPool : public StaticObject { //squirts
public:
    WaterPool(StudentWorld* sp);
    virtual void doSomething() override;
    bool is_Alive() {
        return alive;
    }

private:
    bool alive = true;
};

class Sonar : public StaticObject {
public:
    Sonar(StudentWorld* sp);
    virtual void doSomething() override;
    bool is_Alive() {
        return alive;
    }

private:
    bool alive = true;
};

#endif // ACTOR_H_

