#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;


/*GameWorld* createStudentWorld(string assetDir){
    return new StudentWorld(assetDir);
}*/

StudentWorld* sw;
int ticks = 0; // global variable to keep track of ticks

GameWorld* createStudentWorld(string assetDir)
{
    sw = new StudentWorld(assetDir); // create a new StudentWorld object
    return sw;
}

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir){ //contructor

}

StudentWorld::~StudentWorld(){
    for (auto& actor : actors) {
                delete actor; // delete all actors
            }
            actors.clear();
            for (auto& iceObj : iceField) {
                delete iceObj; // delete all ice objects
            }
            iceField.clear();
            //cout << "StudentWorld dtor" << endl;
}

//functions to implement

StudentWorld* Actor::getWorld(StudentWorld*& sp) const {
    // This function returns the StudentWorld pointer associated with this Actor
    return sp = sw;
}

int StudentWorld::init(){
    
    //StudentWorld* sp = nullptr;

    Iceman* iceMan = new Iceman(sw);
    actors.push_back(iceMan);
        
    for (int i = 0; i < 65; i++){
        for (int j = 0; j < 60; j++){
            if(i > 29 && i < 34 && j > 3){
                continue;
            }
            else{
                Ice* iceObj = new Ice(sw);
                iceObj->moveTo(i, j);
                iceField.push_back(iceObj);
            }
            
        }
    }
    
    for (auto& iceObj : iceField) {
            iceObj->setVisible(true);
        }
    
    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move(){
    //ticks++; // increment ticks for each move
    // Give each Actor a chance to do something
    
    for (auto& actor : actors) {
        if (actor->isAlive()) {
            actor->doSomething();
            //actor->animate(); // animate the actor
        }
        else {
            delete actor; // delete dead actors
        }

    }
    //   int T = std::max(25, (200 - ((int) getLevel()))); // # of tick to wait for new protester
       //int P = std::min(15.0, (2 + ((int) getLevel()) * 1.5)); // # of Protesters that should be on field
       //int probabilityOfHardcore = std::min(90, ((int) getLevel() * 10 + 30));

       //if (probabilityOfHardcore)
     
     if (isGameOver() == true) {
     return GWSTATUS_PLAYER_DIED; // if player died
     }
     //if () {
     //playFinishedLevelSound();
     //return GWSTATUS_FINISHED_LEVEL;
     //}
     return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
    // after a player has lost a life (with more lives remaining) or has completed a level,
    // or lost all lives or game is over
    // delete all actors and ice objects

    for (auto& actor : actors) {
        delete actor; // delete all actors
    }
    actors.clear();
    for (auto& iceObj : iceField) {
        delete iceObj; // delete all ice objects
    }
    iceField.clear();

}
void StudentWorld::addActor(Actor* a){
    actors.push_back(a);
}


void StudentWorld::clearIce(int x, int y){
    for (auto& iceObj : iceField) {
        if ((iceObj->getX() == x      ||
            iceObj->getX() == x + 1   ||
            iceObj->getX() == x + 2   ||
            iceObj->getX() == x + 3)  &&
            (iceObj->getY() == y      ||
            iceObj->getY() == y + 1   ||
            iceObj->getY() == y + 2   ||
            iceObj->getY() == y + 3)){
            iceObj->setVisible(false);
            //playSound(SOUND_DIG);
            iceObj->setDead();
        }
    }
}

bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const{
    for (auto& actor : actors ){
        if (actor->getX()== x && actor->getY() == y){
            return false;
        }
    }
    return true;
}

int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius){
    //TODO
    return 0;
}

void StudentWorld::revealAllNearbyObjects(int x, int y, int radius){
    //TODO
    
}


