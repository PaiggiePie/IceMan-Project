#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <cassert>
using namespace std;


class Actor;

StudentWorld* sw;

GameWorld* createStudentWorld(string assetDir)
{
    sw = new StudentWorld(assetDir); // create a new StudentWorld object
    return sw;
}

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir){ //contructor

}

void StudentWorld::setWorld(StudentWorld*& sp){
    sp = sw;
}

StudentWorld::~StudentWorld(){
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
}

//functions to implement


StudentWorld* Actor::getWorld(StudentWorld*& sp) const {
    // This function returns the StudentWorld pointer associated with this Actor
    return sp = sw;
}

int StudentWorld::init(){
    
    //StudentWorld* sp = nullptr;
    //unique_ptr<Iceman> iceman = make_unique<Iceman>(sw);
    this->iceman = new Iceman(sw);
    //iceman->setWorld(sw);
    if (getIceman() == nullptr)
        cout << "null" << endl;
    agents.push_back(iceman); // add iceman to agents vector
    //random_device rd; // random number generator
    //mt19937 gen(rd());
    //uniform_int_distribution<> distrib(min, max);    // use iceman.get() for unique
    
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
    // make static objects
        BarrelsOfOil* barrel = new BarrelsOfOil(this);
        aobj.push_back(barrel);
    
    //Adding obejcts to the field test code
        GoldNugget* nugget = new GoldNugget(this, 0, 60, true);
        Boulder* boulder = new Boulder(this);
        aobj.push_back(nugget);
        iceField.push_back(boulder);

   /* for (auto& iceObj : iceField) {
            iceObj->setVisible(true);
        }
    */
    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move(){

    //cout << "Score: " << getScore() << endl;
    //cout << "Level: " << getLevel() << endl;

    //cout << "tick: " << ticks << endl;

    setDisplayText();
    bool canAddP = false;
    int G = currentLevelNumber * 30 + 290; // 1 in G chance of water kit or sonar kit added
    int probabilityOfHardcore = min(90, currentLevelNumber * 10 + 30);
    currentLevelNumber = getLevel(); // get the current level number
    int ticksToWaitBetweenMoves = max(0, (3 - currentLevelNumber / 4));
    if ((ticks - ticksToWaitBetweenMoves) %2 == 0) {
        canAddP = true;
    }
    if (canAddP == true) {
        //use probability of hardcore to determine if hardcore protester should be added
        // use probability of protester spawning
        // canAddP = false; // reset
     }
    
    
    //if(iceman->isAlive())
    //    iceman->doSomething(); // ask iceman to do something
    
    // Give each Actor a chance to do something
    int agenti = 0;
    for (auto& agent : agents) {
        if (agent->isAlive()) {
            agent->doSomething();
            if (agent->isAlive() == false) {
                cout << "agent died" << endl;
                agents.erase(agents.begin() + agenti); // remove dead agent from vector
            }
        }
        else {
            delete agent; // delete dead protesters
        }
        agenti++;
    }
    int aobji = 0; // index for aobj vector
    for (auto& object : aobj) {
        if (object->isAlive() && object) {
            object->doSomething();
             // increment index for next object
            if (object->isAlive() == false) {
                //cout << "died" << endl;
                //cout << aobj.size() << endl;
                aobj.erase((aobj.begin() + aobji));
                //cout << aobj.size() << endl;
                delete object;
            }
        }
        aobji++;
    }
    for (auto& ice : iceField)
        ice->doSomething();

       //int T = std::max(25, (200 - ((int) getLevel()))); // # of tick to wait for new protester
       //int P = std::min(15.0, (2 + ((int) getLevel()) * 1.5)); // # of Protesters that should be on field
       //int probabilityOfHardcore = std::min(90, ((int) getLevel() * 10 + 30));

       //if (probabilityOfHardcore)
    if (iceman->isAlive() == false) {
        playSound(SOUND_PLAYER_GIVE_UP);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }


    if (isGameOver() == true) {
        playSound(SOUND_PLAYER_GIVE_UP);
        return GWSTATUS_PLAYER_DIED; // if player died
    }

    if (getBarrelsRemaining() == 0) {
        playSound(SOUND_FINISHED_LEVEL);
        currentLevelNumber++;
        return GWSTATUS_FINISHED_LEVEL;
    }
    ticks++;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
    // after a player has lost a life (with more lives remaining) or has completed a level,
    // or lost all lives or game is over
    // delete all actors and ice objects
        for (auto& agent : agents) {
            delete agent; // delete all protesters
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

}

const int StudentWorld::getTicks() const{
    return ticks;
}

void StudentWorld::addObj(ActivatingObject* a){
    aobj.push_back(a);
    a->doSomething();
}

void StudentWorld::addActor(Actor* a) {
    iceField.push_back(a);
    a->doSomething();
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
            if (ticks % 5 == 0)
                playSound(SOUND_DIG);
            iceObj->setDead();
        }
    }
}

//make Iceman retrievable from StudentWorld
void StudentWorld::setIceman(Iceman* a){
    if (a == nullptr){
        cout << "ERROR: parameter null" << endl;
        return;
    }
    
    if (iceman != nullptr){
        cout << "Iceman already set, overwriting ..." << endl;
    }
    assert(a != nullptr);
    iceman = a;
}

Iceman* StudentWorld::getIceman() const{
    if(iceman)
        return iceman;
    return nullptr;
}

bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const{
    if (x < 0 || x >= 64 || y < 0 || y >= 60) {
            return false; // out of bounds
        }
    for (auto& ice : iceField) {
        if (ice->getX() == x && ice->getY() == y) {
            return false; // ice blocks and boulders
        }
    }
    for (auto& actor : aobj ){ //for barrels, gold, sonar, waterpools
        if (actor->getX()== x && actor->getY() == y){
            return false;// another actor is in the way
        }
    }
    return true;
}

// Annoy all other actors within radius of annoyer, returning the
// number of actors annoyed.
int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius){
    //TODO
    if (findNearbyPickerUpper(annoyer, radius) != nullptr) { // if there is a nearby iceman or protester that can pick things up
        Actor* nearbyAnnoyed = findNearbyPickerUpper(annoyer, radius);
        if (nearbyAnnoyed->getID() == IID_PLAYER) { // if iceman is in radius
            iceman->annoy(points); // annoy iceman
            return 1; // return 1 for iceman
        }
        else {
            nearbyAnnoyed->annoy(points); // annoy the nearby protester
            return 2; // return 2 for protester
        }
    }
    return 0;
}


// Reveal all objects within radius of x,y.
void StudentWorld::revealAllNearbyObjects(int x, int y, int radius){
    //TODO
    for (auto& actor : aobj) {
        if (isNearIceMan(actor, radius)) {
            actor->setVisible(true); // reveal the actor
        }
    }
}


// If the IceMan is within radius of a, return a pointer to the
// IceMan, otherwise null.
Actor* StudentWorld::findNearbyIceMan(Actor* a, int radius) const {
    if (iceman == nullptr || a == nullptr)
        return nullptr;

    if (iceman->isAlive()) {
        if (radius > std::abs((a->getX() - iceman->getX())) && radius > std::abs((a->getY() - iceman->getY())))
            return iceman;
    }
    return nullptr;
}

// If at least one actor that can pick things up is within radius of a,
// return a pointer to one of them, otherwise null.

Agent* StudentWorld::findNearbyPickerUpper(Actor* a, int radius) const {
    for (int i = 0; i < agents.size(); i++) {
        if (radius > abs((a->getX() - agents[i]->getX())) && radius > abs((a->getY() - agents[i]->getY())) && agents[i]->canPickThingsUp() == true)
            return agents[i];
    }
    if (radius > abs((a->getX() - iceman->getX())) && radius > abs((a->getY() - iceman->getY())) && iceman->canPickThingsUp() == true) {
        return iceman; // if iceman is in radius, return iceman
    }
    return nullptr; // if no actors in radius, return null
}

  // Annoy the IceMan.
void StudentWorld::annoyIceMan(){
    playSound(SOUND_PROTESTER_YELL);
    //health -= 20;
}

  // Give IceMan some sonar charges.
void StudentWorld::giveIceManSonar(){
    playSound(SOUND_GOT_GOODIE);
    iceman->addSonar();
}

  // Give IceMan some water.
void StudentWorld::giveIceManWater(){
    playSound(SOUND_GOT_GOODIE);
    iceman->addGold();
}

  // Is the Actor a facing toward the IceMan?
bool StudentWorld::facingTowardIceMan(Actor* a) const{
    //might need to switch orientations
    if (a->getX() == iceman->getX() ) { // same x level && right side of iceman
            if (a->getDirection() == GraphObject::Direction::left && a->getX() > iceman->getX())
                return true; // actor is facing left towards iceman
            else if (a->getDirection() == GraphObject::Direction::right && a->getX() < iceman->getX()) {
                return true; // actor is facing right away from iceman
            }
        }
        if (a->getY() == iceman->getY()) { // same y level && above iceman
            if (a->getDirection() == GraphObject::Direction::up && a->getY() < iceman->getY())
                return true; // actor is facing up towards iceman
            else if (a->getDirection() == GraphObject::Direction::down && a->getY() > iceman->getY()) {
                return true; // actor is facing down away from iceman
            }
        }
        return false;
}

  // If the Actor a has a clear line of sight to the IceMan, return
  // the direction to the IceMan, otherwise GraphObject::none.
GraphObject::Direction StudentWorld::lineOfSightToIceMan(Actor* a) const{
    if (facingTowardIceMan(a) == false) {
            return GraphObject::Direction::none; // if actor is not facing towards iceman, return none
        }
        bool blocked = false; // if there is an object in the way

        if (a->getX() == iceman->getX()) { // same x level as iceman
            for (int i = a->getX(); i < iceman->getX(); i++) { //starting at actor, ending at iceman
                for (int h = 0; h < iceField.size(); h++) {// i = x coords  getY = y coords
                    if (iceField[h]->getY() == a->getY()) {
                        if (iceField[h]->getX() == i) {
                            blocked = true;
                        }


                    }
                }
            }
            if (blocked == false) { // if there is not an object in the path
                if (a->getX() > iceman->getX()) {
                    return GraphObject::Direction::left;
                }
                else if (a->getX() < iceman->getX()) {
                    return GraphObject::Direction::right;
                }
            }
        }

    // same y level as iceman
        else if (a->getY() == iceman->getY()) {
            for (int i = a->getY(); i < iceman->getY(); i++) { //loop through each coord between iceman and actor
                for (int t = 0; t < iceField.size(); t++) {// getX = x coords  i = y coords
                    if (iceField[t]->getX() == a->getX()) { // if there is not an object in the path
                        if (iceField[t]->getY() == i)
                            blocked = true;

                    }

                }
            }
            if (blocked == false) { // if there is not an object in the path
                if (a->getY() > iceman->getY()) {
                    return GraphObject::Direction::up;
                }
                else if (a->getY() < iceman->getY()) {
                    return GraphObject::Direction::down;
                }
            }
        }
    return GraphObject::none;
}

  // Return whether the Actor a is within radius of IceMan.
bool StudentWorld::isNearIceMan(Actor* a, int radius) const{
    if (radius > abs((a->getX() - iceman->getX())) && radius > abs((a->getY() - iceman->getY())))
        return true;
    return false;
}


bool StudentWorld::nearBoulder(int x, int y, int radius) const {
    for (int i = 0; i < iceField.size(); i++) {
        if (iceField[i]->getID() == IID_BOULDER) { // if there is a boulder in the iceField
            if (radius > abs((x - iceField[i]->getX())) && radius > abs((y - iceField[i]->getY())))
                return true; // if boulder is in radius, return true
        }
    }
    return false;
}

  // Determine the direction of the first move a quitting protester
  // makes to leave the oil field.
GraphObject::Direction determineFirstMoveToExit(int x, int y){
    return GraphObject::none;
}

  // Determine the direction of the first move a hardcore protester
  // makes to approach the IceMan.
GraphObject::Direction determineFirstMoveToIceMan(int x, int y){
    ActivatingObject::Direction d{};
    return d;
}

// useable functions
//unsigned int getLives() const;
//void decLives();
//void incLives();
//unsigned int getScore() const;
//unsigned int getLevel() const;
//void increaseScore(unsigned int howMuch);
//void setGameStatText(string text);
//bool getKey(int& value);
//void playSound(int soundID);

