#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject{
public:
    
    //parametized constructor to allow each derived class to have unique position and size
    Actor(int ID, int x, int y, Direction dir, double siz, unsigned int dep);
    
    bool is_Alive();
    
    //virtual functions
    virtual void doSomething() = 0;
    virtual ~Actor();

private:
    bool m_isAlive; //alive or dead
};



class Iceman: public Actor{
public:
    
    //contructor declaration
    Iceman(StudentWorld* sp);
    virtual void doSomething();
    ~Iceman();
    
private:
    int m_hits = 10;
    int m_squirts = 5;
    int m_sonar = 1;
    int m_nuggets = 0;
    StudentWorld* sp = nullptr;
};


class Ice: public Actor{
public:
    
    //contructor declaration
    Ice(StudentWorld* sp);
    virtual void doSomething();
    ~Ice();
    
private:
    StudentWorld* sp = nullptr;
};



//******************************** Protestors *******************************
class Protestor: public Actor{
    
public:
    virtual void doSomething(){}
    
    
private:
    
};

class HardcoreProtestor: public Protestor{
    
public:
    virtual void doSomething(){}
    
    
private:
};


//******************************** Objects *******************************

class BarrelsOfOil: public Actor{
public:
    
private:
    
};

class Boulders: public Actor{
public:
    
private:
    
};

class GoldNuggets: public Actor{
public:
    
private:
    
};

class WaterRefills: public Actor{
public:
    
private:
    
};



#endif // ACTOR_H_

