#include "Actor.h"
#include "StudentWorld.h"



// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
//******************************** Actor Methods *******************************

//contructor
Actor::Actor(int ID, int x, int y, Direction dir, double siz, unsigned int dep):
GraphObject(ID, x, y, dir, siz, dep){setVisible(true);}

bool Actor::is_Alive(){
    return m_isAlive;
}

Actor::~Actor(){ }



//******************************** IceMan Methods *******************************

Iceman::Iceman(StudentWorld* sp): Actor(IID_PLAYER, 30, 60, right, 1.0, 0){ //contructor
    cout << "Iceman ctor" << endl;

    setVisible(true);
}



void Iceman::doSomething(){
   // if (this->is_Alive()){
        // if overlapping ice
        
        //check if key was pressed
        
            //different actions for different keys}
    Direction d = getDirection();
    
    setDirection(d);
    int x = getX();
    int y = getY();
    moveTo(x, y);
    return;
    
}

Iceman::~Iceman(){
    
}



//******************************** Ice Methods *******************************

//constructor
Ice::Ice(StudentWorld* sp): Actor(IID_ICE, getX(), getY(), right, 0.25, 3){
}

Ice::~Ice(){
    //delete sp->actors;
    cout << "Ice dtor" << endl;
    delete this;
}

void Ice::doSomething(){
  /*  Direction d = getDirection();
    setDirection(d);
    int x = getX();
    int y = getY();
    moveTo(x, y);
    return;*/
}

//******************************** B.O.O. Methods *******************************


//******************************** G Boulder Methods *******************************

//******************************** Gold Nuggets Methods *******************************

//******************************** Water Refills Methods *******************************

//******************************** Hardcore Protestor Methods *******************************

//******************************** Regular Protestor Methods *******************************
