#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:

    //parametized constructor to allow each derived class to have unique position and size
    Actor(int ID, int x, int y, Direction dir, double siz, unsigned int dep);

    bool is_Alive();

    //virtual functions
    virtual void doSomething() = 0;
    virtual ~Actor();
    
    
	
	// getWorld function to return the StudentWorld pointer
    virtual StudentWorld* getWorld(StudentWorld*& sp) const;

protected:
    // current level number, used for calculating ticks to wait between moves for protester
    //int currentLevelNumber = sp->getLevel(); 
    StudentWorld* sp = nullptr;
private:
    bool m_isAlive = true; //alive or dead
    
};



class Iceman : public Actor {
public:
    //contructor declaration
    Iceman(StudentWorld* sp);
    virtual void doSomething();
    ~Iceman();
    bool is_Alive()
    {
        return m_isAlive;
    }
    //virtual StudentWorld* getWorld() const {
    //    StudentWorld* world = sp;
    //    return world;
    //};

private:
    int m_hits = 10;
    int m_squirts = 5;
    int m_sonar = 1;
    int m_nuggets = 0;
    //StudentWorld* sp = nullptr;
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

private:
    //StudentWorld* sp = nullptr;
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
    //StudentWorld* sp = nullptr;
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
    //StudentWorld* sp = nullptr;
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
    //StudentWorld* sp = nullptr;
    bool leave_field = false;
};


//******************************** Objects *******************************

class BarrelsOfOil : public Actor {
public:
	BarrelsOfOil(StudentWorld* sp);
    virtual void doSomething() override;

    bool is_Alive() {
        return alive;
    }

    ~BarrelsOfOil();

private:
    //StudentWorld* sp = nullptr;
    bool alive = true;
};

class Boulders : public Actor {
public:
	Boulders(StudentWorld* sp);
    virtual void doSomething() override;
    bool is_Alive() {
        return alive;
    }

    //if boulder falls below ice, it is dead, alive = false

private:
    bool alive = true;
    //StudentWorld* sp = nullptr;

};

class GoldNuggets : public Actor {
public:
	GoldNuggets(StudentWorld* sp);
    virtual void doSomething() override;
    bool is_Alive() {
        return alive;
    }

private:
    bool alive = true;
    //StudentWorld* sp = nullptr;

};

class WaterRefills : public Actor {
public:
	WaterRefills(StudentWorld* sp);
    virtual void doSomething() override;
    bool is_Alive() {
        return alive;
    }

private:
    bool alive = true;
    //StudentWorld* sp = nullptr;
};

class Sonar : public Actor {
public:
    Sonar(StudentWorld* sp);
    virtual void doSomething() override;
    bool is_Alive() {
        return alive;
    }

private:
    bool alive = true;
    //StudentWorld* sp = nullptr;
};

#endif // ACTOR_H_

