#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
//******************************** Actor Methods *******************************
bool Actor::is_Alive(){
    return m_isAlive;
}



//******************************** IceMan Methods *******************************
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

//******************************** Ice Methods *******************************


//******************************** B.O.O. Methods *******************************


//******************************** G Boulder Methods *******************************

//******************************** Gold Nuggets Methods *******************************

//******************************** Water Refills Methods *******************************

//******************************** Hardcore Protestor Methods *******************************

//******************************** Regular Protestor Methods *******************************
