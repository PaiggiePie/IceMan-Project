#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <random>


//******************************** Actor Methods *******************************

//contructor
Actor::Actor(StudentWorld* sp, int ID, int x, int y, Direction dir, double siz, unsigned int dep, bool visible) :
    GraphObject(ID, x, y, dir, siz, dep), sp(sp) {
    //setWorld(sp); // set the world pointer
    //cout << "Actor ctor" << endl;
    //srand(time(0)); // seed the random number generator

}

Actor::~Actor() {
    //cout << "Actor dtor" << endl;
    setVisible(false);
    m_isAlive = false; // set alive to false when actor is destroyed
    // remove from the set of graph objects
    getGraphObjects(getAnimationNumber()).erase(this);
}

bool Actor::isAlive() const {
    return m_isAlive;
}

void Actor::setDead() {
    m_isAlive = false;
}

//bool Actor::annoy(unsigned int amt) {
//    return false;
//}

bool Actor::huntsIceMan() const {
   return false;
}
//
bool Actor::canDigThroughIce() const {
    return false;
}
//
//bool Actor::canActorsPassThroughMe() const {
//    return false;
//}
//
//bool Actor::canPickThingsUp() const {
//    return false;
//}
//
//bool Actor::needsToBePickedUpToFinishLevel() const {
//    return false;
//}

bool Actor::moveToIfPossible(int x, int y) {
    if (sp->canActorMoveTo(this, x, y) == true) {
        moveTo(x, y); // move to the specified location
        return true; // cannot move to the specified location
    }
    return false;
}




//******************************** Agent Methods *******************************
Agent::Agent(StudentWorld* sp, int ID, int x, int y, Direction dir,
    unsigned int hitPoints) : Actor(sp, ID, x, y, dir, 1.0, 0, true) , hitPoints(hitPoints) {
    //cout << "Agent ctor" << endl;

}

bool Agent::annoy(unsigned int amount) {
    hitPoints = hitPoints - amount;
    if (hitPoints == 0) {
        setDead();
        return false;
    }
    return true;
}

bool Agent::canDigThroughIce() const {
   return false;
}

void Agent::setMustLeaveOilField() {
    leave = true;
}


unsigned int Agent::getHitPoints() const {
    return hitPoints;
}
//******************************** IceMan Methods *******************************

Iceman::Iceman(StudentWorld* sp) : Agent(sp, IID_PLAYER, 30, 60, right, 10) { //contructor
    getGraphObjects(0).insert(this);
    setVisible(true);
    //cout << "iceman ctor" << endl;
}

void Iceman::doSomething() {
    if (getWorld() == nullptr) {
        cerr << "Error: StudentWorld pointer is null!" << endl;
        return; // handle the error as needed
    }
    Direction d = this->getDirection();
    int key;

    if (getWorld()->getKey(key) == true) {
        switch (key) {
        case KEY_PRESS_LEFT:
            if (d != left) {
                setDirection(left);
            } // no boulder at (x-1,y) and (x-1, y+3)
            else if (getX() > 0 && (!sp->atItem(getX() - 1, getY(), false) ||
                    !sp->atItem(getX() - 1, getY() + 3, false) ||
                    !sp->atItem(getX() - 1, getY() + 2, false) ||
                    !sp->atItem(getX() - 1, getY() + 1, false))) {
                moveTo(getX() - 1, getY());
                sp->clearIce(getX(), getY());
            }
            break;
        case KEY_PRESS_RIGHT:
            if (d != right) {
                setDirection(right);
            }
            else if (getX() < 61 &&(!sp->atItem(getX() + 3, getY() + 3, false) ||
                    !sp->atItem(getX() + 3, getY(), false))) {
                moveTo(getX() + 1, getY());
                sp->clearIce(getX(), getY());
            }
            break;
        case KEY_PRESS_UP:
            if (d != up) {
                setDirection(up);
            }
            else if (getY() < 60 && (!sp->atItem(getX(), getY() + 3, false) ||
                !sp->atItem(getX() + 3, getY() + 3, false))) {
                moveTo(getX(), getY() + 1);
                sp->clearIce(getX(), getY());
            }
            break;
        case KEY_PRESS_DOWN:
            if (d != down) {
                setDirection(down);
            }
            else if (getY() > 0 && (!sp->atItem(getX(), getY() - 1, false) ||
                !sp->atItem(getX() + 3, getY() - 1, false))) {
                moveTo(getX(), getY() - 1);
                sp->clearIce(getX(), getY());
            }
            break;
        case KEY_PRESS_TAB: //place gold nugget at current position
            //cout << "tab" << endl;
            if (getGold() != 0) {
                m_gold--;
                GoldNugget* nugget = new GoldNugget(sp, getX(), getY(), true);
                sp->addObj(nugget);
            }
            break;
        case KEY_PRESS_SPACE: //squirt water
            if (m_squirts > 0) {
                sp->playSound(SOUND_PLAYER_SQUIRT);
                m_squirts--;

                int x = getX();
                int y = getY();
                
                if (d == up && y + 4 < 64) { y += 2; }
                else if (d == down && y - 4 >= 0) { y -= 2; }
                else if (d == right && x + 4 < 60) { x += 2; }
                else if (d == left && x - 4 >= 0) { x -= 2; }
                else break;
                if (!getWorld()->canActorMoveTo(nullptr, x, y) || getWorld()->NearBoulder(x, y, 3)) {
                        return; // Blocked! Don’t spawn the squirt
                    }
                Squirt* s = new Squirt(sp, x, y, d, 4);
                sp->addActor(s);
            }
            break;
        case KEY_PRESS_ESCAPE:
            //sp->decLives();
            setDead(); // set iceman to dead
            break;
        case 'Z':
        case 'z':
            //use sonar if available
            if (getSonar() > 0) { // if iceman has sonar
                m_sonar--; // decrement sonar
                sp->playSound(SOUND_SONAR);
                sp->revealAllNearbyObjects(getX(), getY(), 12); // reveal all nearby objects
            }
            break;
        }
    }
}


bool Iceman::canDigThroughIce() const {
    return true;
}

void Iceman::addGold() {
    m_gold++;
}


void Iceman::addSonar() {
    m_sonar += 1;
}

// getter functions
// Pick up water.
void Iceman::addWater() {
    m_squirts += 5;
}

// Get amount of gold
unsigned int Iceman::getGold() const {
    return m_gold;
}

// Get amount of sonar charges
unsigned int Iceman::getSonar() const {
    return m_sonar;
}

// Get amount of water
unsigned int Iceman::getWater() const {
    return m_squirts;
}

bool Iceman::huntsIceMan() const{
    return false;
}


Iceman::~Iceman() {
    setVisible(false);
    Iceman::getGraphObjects(0).erase(this);
    //cout << "iceman dtor" << endl;

}

//******************************** Protester Methods *******************************
Protester::Protester(StudentWorld* sp, int ID, unsigned int hitPoints, unsigned int score)
    : Agent(sp, ID, 60, 60, left, hitPoints) {
    setVisible(true);
    //cout << "protester ctor" << endl;

}
void Protester::doSomething() {
}

bool Protester::annoy(unsigned int amount) {
    if (this->getHitPoints() > amount) { // if hit points are greater amount (will equal at least 1)
        sp->playSound(SOUND_PROTESTER_ANNOYED); // play sound
        restingTicks = std::max(50, 100 - (int)(sp->getLevel() * 10)); // set resting ticks
        hitPoints -= amount; // reduce hit points by amount
        return false; // false if not dead yet?
    }
    else {
        hitPoints = 0; // reduce hit points by amount
        sp->playSound(SOUND_PROTESTER_GIVE_UP); // play sound
        setMustLeaveOilField(); // leave == true
        restingTicks = 0; // immediately leave field
        return true;
    }
}

void Protester::addGold() {
    m_gold++;
    restingTicks = 0;
    setMustLeaveOilField(); // regulars must leave after being bribed
}


bool Protester::leavingOilField() {
    if (leave == true){
        setMustLeaveOilField();
        return true;
    }
    return false;
}

void Protester::setDead(){
    setMustLeaveOilField();
}

// Set number of ticks until next move
void Protester::setTicksToNextMove() {
    restingTicks = std::max(0, (int)(3 - sp->getLevel() / 4));
}

void Protester::setSquaresToMoveInCurrentDirection() {
    // 8 <= numSquaresToMoveInCurrentDirection <= 60
    // random number between 8 and 60
    numSquaresToMoveInCurrentDirection = (8 + rand()% 53);
}

bool Protester::huntsIceMan() const {
    return true;
}


//******************************** Regular Protester Methods *******************************
RegularProtester::RegularProtester(StudentWorld* sp)
    : Protester(sp, IID_PROTESTER, 5, 0) {
    //cout << "regular protester ctor" << endl;

}

void RegularProtester::doSomething() {
    if (!isAlive()) {
        return;
    }
    
    if (hitPoints == 0 && !leave) {
        setMustLeaveOilField();
    }
    
    shoutTicks--;
    perpenTicks++;
    
    if (restingTicks > 0)
    {
        restingTicks--;
        return;
    }
    
    Direction dp = getDirection();
    // Leaving the oil field
    if (leave) {
        int x = getX();
        int y = getY();
        if (x == 60 && y == 60) {
            setVisible(false);
            setDead();
            return;
        }
        Direction d = sp->determineFirstMoveToExit(x, y);
        setDirection(d);
        if (d == left) moveTo(getX() - 1, getY());
        else if (d == right) moveTo(getX() + 1, getY());
        else if (d == up) moveTo(getX(), getY() + 1);
        else if (d == down) moveTo(getX(), getY() - 1);
        return;
    }
    
    
    // Shout if Iceman is in front and within 4
    if (sp->isNearIceMan(this, 4) && sp->facingTowardIceMan(this)) {
        if (shoutTicks < 0) {
            sp->playSound(SOUND_PROTESTER_YELL);
            sp->getIceman()->annoy(2);
            shoutTicks = 15;
            setTicksToNextMove();
        }
        return; //return immediately
    }
    
    Direction d = sp->lineOfSightToIceMan(this, false);
    if (d != none && !sp->isNearIceMan(this, 4)) {
        //cout << "sees iceman" << endl;
        setDirection(d);
        switch (d) {
            case right:
                moveTo(getX() + 1, getY());
                break;
            case left:
                moveTo(getX() - 1, getY());
                break;
            case down:
                moveTo(getX(), getY() - 1);
                break;
            case up:
                moveTo(getX(), getY() + 1);
                break;
            default:
                break;
        }
        if (((dp == left || dp == right) && (getDirection() == up || getDirection() == down)) ||
            ((dp == up || dp == down) && (getDirection() == left || getDirection() == right))) {
            perpenTicks = 0; // reset perpendicular turn ticks
        }
        numSquaresToMoveInCurrentDirection = 60;
        setTicksToNextMove();
        return;
    }
    
    if (numSquaresToMoveInCurrentDirection <= 0) {
        //cout << "change direction" << endl;
        setSquaresToMoveInCurrentDirection(); // reset squares to move in current direction
        setTicksToNextMove();
        int pickDirection = rand() % 4;
        switch (pickDirection) {
            case 0:
                if (sp->canActorMoveTo(this, getX(), getY() + 1) && !(getDirection() == up)) {
                    setDirection(up);
                    moveTo(getX(), getY() + 1);
                    setTicksToNextMove();
                }
                break;
            case 1:
                if (sp->canActorMoveTo(this, getX(), getY() - 1) && !(getDirection() == down)) {
                    setDirection(down);
                    moveTo(getX(), getY()-1);
                    setTicksToNextMove();
                }
                break;
            case 2: //left
                if (sp->canActorMoveTo(this, getX() - 1, getY()) && getDirection() != left) {
                    setDirection(left);
                    moveTo(getX() - 1, getY());
                    setTicksToNextMove();
                }
                break;
            case 3: //right
                if (sp->canActorMoveTo(this, getX() + 1, getY()) && getDirection() != right) {
                    setDirection(right);
                    moveTo(getX() + 1, getY());
                    setTicksToNextMove();
                }
                break;
        }
    }
    
    
    // Check for perpendicular turn every 200 non-resting ticks
    if (perpenTicks >= 200) {
        bool turned = false;
        if (getDirection() == left || getDirection() == right) {
            if (sp->canActorMoveTo(this, getX(), getY() + 1)) {
                setDirection(up);
                turned = true;
            } else if (sp->canActorMoveTo(this, getX(), getY() - 1)) {
                setDirection(down);
                turned = true;
            }
        } else {
            if (sp->canActorMoveTo(this, getX() - 1, getY())) {
                setDirection(left);
                turned = true;
            } else if (sp->canActorMoveTo(this, getX() + 1, getY())) {
                setDirection(right);
                turned = true;
            }
        }
        
        if (turned) {
            setSquaresToMoveInCurrentDirection();
            perpenTicks = 0;
        }
    }
    
    // Try to move in current direction
    bool moved = false;
    switch (getDirection()) {
        case up:    moved = moveToIfPossible(getX(), getY() + 1); break;
        case down:  moved = moveToIfPossible(getX(), getY() - 1); break;
        case left:  moved = moveToIfPossible(getX() - 1, getY()); break;
        case right: moved = moveToIfPossible(getX() + 1, getY()); break;
        default: break;
    }
    
    if (moved) {
        numSquaresToMoveInCurrentDirection--;
        setTicksToNextMove();
        return;
    } else {
        numSquaresToMoveInCurrentDirection = 0;
    }
}

//******************************** Hardcore Protester Methods *******************************
HardcoreProtester::HardcoreProtester(StudentWorld* sp)
    : Protester(sp, IID_HARD_CORE_PROTESTER, 20, 0) {
    //cout << "hardcore protester ctor" << endl;

}

void HardcoreProtester::doSomething() {
    if (!isAlive()) {
        return;
    }

    if (hitPoints == 0 && !leave) {
        setMustLeaveOilField();
    }

    shoutTicks--;
    perpenTicks++;

    if (restingTicks > 0)
    {
        restingTicks--;
        return;
    }

    Direction dp = getDirection();
    //leaving state actions
    if (leave == true) {
        int x = getX();
        int y = getY();
        if (y == 60 && x == 60) {
            setDead();
        }
        //find way to exit
        Direction d = sp->determineFirstMoveToExit(x, y);
        setDirection(d);
        if (d == left) moveTo(getX() - 1, getY());
        else if (d == right) moveTo(getX() + 1, getY());
        else if (d == up) moveTo(getX(), getY() + 1);
        else if (d == down) moveTo(getX(), getY() - 1);
        return;
    }

    // if iceman is inline of sight, facing and within 4 squares = shout
    if (sp->isNearIceMan(this, 4) && sp->facingTowardIceMan(this)) {
        if (shoutTicks < 0) {
            //cout << "\t\t\t\t\tshouted" << endl;
            sp->playSound(SOUND_PROTESTER_YELL);
            sp->getIceman()->annoy(2);
            shoutTicks = 15;
            restingTicks = 0;
        }
        return; //return immediately
    }

    // if within M squares, hunt iceman down via phone signal
    int M = 16 + (sp->getLevel()) * 2;
    if (sp->findNearbyIceMan(this, M)) {// find nearby iceman within M steps
        Direction d = sp->determineFirstMoveToIceMan(getX(), getY());
        if (d == left) {
            setDirection(left);
            moveToIfPossible(getX() - 1, getY()); // move to exit
        }
        else if (d == right) {
            setDirection(right);
            moveToIfPossible(getX() + 1, getY()); // move to exit
        }
        else if (d == up) {
            setDirection(up);
            moveToIfPossible(getX(), getY() + 1); // move to exit
        }
        else if (d == down) {
            setDirection(down);
            moveToIfPossible(getX(), getY() - 1); // move to exit
        }
        numSquaresToMoveInCurrentDirection = 0; // new direction
        return;
    }

    // if in line of sight, but not facing & more than 4 units away, turn to face iceman & move 1 square in that direction
    Direction d = sp->lineOfSightToIceMan(this, false);
    if (d != none && !sp->isNearIceMan(this, 4)) {
        //cout << "sees iceman" << endl;
        setDirection(d);
        switch (d) {
        case right:
            moveTo(getX() + 1, getY());
            break;
        case left:
            moveTo(getX() - 1, getY());
            break;
        case down:
            moveTo(getX(), getY() - 1);
            break;
        case up:
            moveTo(getX(), getY() + 1);
            break;
        default:
            break;
        }
        if (((dp == left || dp == right) && (getDirection() == up || getDirection() == down)) ||
            ((dp == up || dp == down) && (getDirection() == left || getDirection() == right))) {
            perpenTicks = 0; // reset perpendicular turn ticks
        }
        numSquaresToMoveInCurrentDirection = 60;
        restingTicks = 0;
        return;
    }

    // if finished wandering, set new direction
    if (numSquaresToMoveInCurrentDirection <= 0) {
        //cout << "change direction" << endl;
        setSquaresToMoveInCurrentDirection(); // reset squares to move in current direction
        setTicksToNextMove();
        int pickDirection = rand() % 5;
        switch (pickDirection) {
            case 1:
                setDirection(right);
                if (sp->canActorMoveTo(this, getX() - 1, getY()) && !(getDirection() == left)) {
                    setDirection(left);
                    moveTo(getX() - 1, getY());
                    setTicksToNextMove();
                    
                }
                break;
            case 2:
                if (sp->canActorMoveTo(this, getX() + 1, getY()) && !(getDirection() == right)) {
                    setDirection(right);
                    moveTo(getX() + 1, getY());
                    setTicksToNextMove();
                }
                break;
            case 3:
                if (sp->canActorMoveTo(this, getX(), getY() + 1) && !(getDirection() == up)) {
                    setDirection(up);
                    moveTo(getX(), getY() + 1);
                    setTicksToNextMove();
                }
                break;
            case 4:
                if (sp->canActorMoveTo(this, getX(), getY() - 1) && !(getDirection() == down)) {
                    setDirection(down);
                    moveTo(getX(), getY()-1);
                    setTicksToNextMove();
                }
                break;
            default:
                setDirection(sp->determineFirstMoveToIceMan(getX(), getY()));
        }
    }
    
    // if no perpendicular turn in 200 non-resting ticks, change to a perpendicular direction if possible
    if (perpenTicks >= 200) {
        bool turned = false;
        //cout << "perpendicular turn" << endl;
        if (getDirection() == left || getDirection() == right) { // if current direction is left or right
            if (sp->canActorMoveTo(this, getX(), getY() + 1)) { // if can move up
                setDirection(up);
                turned = true;
            }
            else if (sp->canActorMoveTo(this, getX(), getY() - 1)) { // if can move down
                setDirection(down);
                turned = true;
            }
        }
        else if (getDirection() == up || getDirection() == down) { // if current direction is up or down
            if (sp->canActorMoveTo(this, getX() - 1, getY())) { // if can move left
                setDirection(left);
                turned = true;
            }
            else if (sp->canActorMoveTo(this, getX() + 1, getY())) { // if can move right
                setDirection(right);
                turned = true;
            }
            setTicksToNextMove();
        }
        if (turned) {
            setSquaresToMoveInCurrentDirection();
            perpenTicks = 0;
        }
    }

    // if can't see iceman, just move (wander oil field)
    // can move in current direction for numSquaresToMoveInCurrentDirection
    bool moved = false;
    switch (getDirection()) {
        case up:    moved = moveToIfPossible(getX(), getY() + 1); break;
        case down:  moved = moveToIfPossible(getX(), getY() - 1); break;
        case left:  moved = moveToIfPossible(getX() - 1, getY()); break;
        case right: moved = moveToIfPossible(getX() + 1, getY()); break;
        default: break;
    }

    if (moved == true) {
        //cout << "wandering" << endl;
        numSquaresToMoveInCurrentDirection--;
        setTicksToNextMove();
    }
    else {
        numSquaresToMoveInCurrentDirection = 0; // force a new direction next tick
    }
}

void HardcoreProtester::addGold() {
    // increasing score, sound added in gold func
    m_gold++;
    restingTicks = max(50, (int)(100 - sp->getLevel() * 10)); // time to stare before chasing again
}


//******************************** Ice Methods *******************************

//constructor
Ice::Ice(StudentWorld* sp, int x, int y) : Actor(sp, IID_ICE, x, y, right, 0.25, 3, true) {
    //cout << "Ice ctor" << endl;
    setVisible(true);
    // add to the set of graph objects
    getGraphObjects(3).insert(this);
}

Ice::~Ice() {
    //cout << "Ice dtor" << endl;
    setVisible(false);
    Ice::getGraphObjects(3).erase(this); // remove from the set of graph objects
}

void Ice::doSomething() {
    // Ice does not perform any actions
}

//******************************** Boulder Methods *******************************
Boulder::Boulder(StudentWorld* sp, int x, int y) : Actor(sp, IID_BOULDER, x, y, down, 1.0, 1, true) {
    setVisible(true);
    //cout << "boulder ctor" << endl;

}
//bool Boulder::canActorsPassThroughMe() const {
//    return false;
//}

void Boulder::doSomething() {
    // if there is not ice below the boulder in 2x2 square, it can fall ie.
    if (isAlive() == false) {
        return; // if not alive, do nothing
    }

    if(fallingState == false && sp->canActorMoveTo(this, getX(), getY() - 1) &&
        sp->canActorMoveTo(this, getX() + 1, getY() - 1) &&
        sp->canActorMoveTo(this, getX() + 2, getY() - 1) &&
        sp->canActorMoveTo(this, getX() + 3, getY() - 1)) {
        // if boulder can fall, then it will fall after 30 ticks
        if (restingTicks == -1)
            restingTicks = sp->getTicks() + 30;
        else if (sp->getTicks() == restingTicks) {
            fallingState = true;
            sp->playSound(SOUND_FALLING_ROCK);
        }
    }
    if (fallingState == true) {
        if (sp->canActorMoveTo(this, getX(), getY() - 1) &&
            sp->canActorMoveTo(this, getX() + 1, getY() - 1) &&
            sp->canActorMoveTo(this, getX() + 2, getY() - 1) &&
            sp->canActorMoveTo(this, getX() + 3, getY() - 1)) {
            moveTo(getX(), getY() - 1);
            sp->annoyAllNearbyActors(this,100, 3); // annoy all nearby actors
        }
        else {
            setVisible(false);
            setDead();
        }
    }
}

//******************************** Squirt Methods *******************************

Squirt::Squirt(StudentWorld* sp, int x, int y, Direction dir, int m_distance)
    : Actor(sp, IID_WATER_SPURT, x, y, dir, 1.0, 1, true) {
    //let doSomething decide if it should appear
    setVisible(true);
    //cout << "squirt ctor" << endl;
}

void Squirt::doSomething() {
    if (!isAlive()) {
        setVisible(false);
        return; // if not alive, do nothing
    }
    setVisible(true);
    
    if (m_distance <= 0){
        setDead();
        return;
    }
    
    sp = getWorld();
    
    int x = getX();
    int y = getY();
    
    switch(getDirection()){
        case up:    y++; break;
        case down:  y--; break;
        case left:  x--; break;
        case right: x++; break;
        default:
            break;
    }
    if (!sp->canActorMoveTo(this, x, y) || sp->NearBoulder(x, y, 3)) {
        setDead();
        return;
    }
    
    moveTo(x, y);
    m_distance--;
    
    // if is a nearby protester gets hit, annoy them
    Agent* nearby =sp->findNearbyPickerUpper(this, 3);
    if (nearby != nullptr && !nearby->leavingOilField()) {
        // if not in leaving state, annoy
        nearby->annoy(2); // annoy protester by 2
        sp->increaseScore(100);
        if (nearby->getID() == IID_HARD_CORE_PROTESTER)
            sp->increaseScore(250); // 250 points for hardcore protester
        sp->increaseScore(150);
        setDead();// set squirt to dead
        return;
    }
}




//******************************** ActivatingObj Methods *******************************
ActivatingObject::ActivatingObject(StudentWorld* sp, int x, int y, int ID) :
    Actor(sp, ID, x, y, right, 1.0, 2, true)  // initialize the member variables
{
    //cout << "ActivatingObject ctor" << endl;
}

//will only be used by barrels of oil. all other objects use Actor::needsToBePickedUpToFinishLevel()
//bool ActivatingObject::needsToBePickedUpToFinishLevel() const {
//    return false;
//}
//
//bool ActivatingObject::canActorsPassThroughMe() const {
//    return true;
//}
void ActivatingObject::setTicksToLive() {
    int math = 300 - (10 * sp->getLevel());
    ticksToLive = max(100, math);
};

ActivatingObject::~ActivatingObject() {
    setVisible(false);
    getGraphObjects(getAnimationNumber()).erase(this);
}

//******************************** Oil Methods *******************************
BarrelsOfOil::BarrelsOfOil(StudentWorld* sp, int x, int y)
    : ActivatingObject(sp, x, y, IID_BARREL) {
    getGraphObjects(2).insert(this);
    setVisible(false);
    //cout << "Barrels of Oil ctor" << endl;
}

void BarrelsOfOil::doSomething() {
    if (isAlive()) {
        if (isVisible() == false && sp->findNearbyIceMan(this, 4) != nullptr) { // if not visible and iceman is in radius 4
            setVisible(true); // make visible
            return;
        }
        if (sp->findNearbyIceMan(this, 3) != nullptr) {
            //cout << "barrel picked up" << endl;
            sp->playSound(SOUND_FOUND_OIL); // play sound
            sp->increaseScore(1000); // increase score by 10
            //setVisible(false);
            sp->decBarrels();
            setDead(); // if iceman is in radius 3, set dead
            return;
        }
    }
}

//bool BarrelsOfOil::needsToBePickedUpToFinishLevel() const {
//    return true;
//}

BarrelsOfOil::~BarrelsOfOil() {
    setVisible(false);
    BarrelsOfOil::getGraphObjects(2).erase(this); // remove from the set of graph objects
    //cout << "barrelofoil dtor" << endl;
}



//******************************** Gold Nuggets Methods *******************************
GoldNugget::GoldNugget(StudentWorld* sp, int x, int y, bool temporary)
    : ActivatingObject(sp, x, y, IID_GOLD),
    temporary(temporary) // initialize temporary, pickup, and lifetime
{
    lifetime = temporary ? 90 : -1;
    getGraphObjects(2).insert(this);
    setVisible(false);
    if (temporary) {
        trackTick = sp->getTicks();
        lifetime = 90; // expire after 90 ticks from now
    } else {
        lifetime = -1;
    }
}

GoldNugget::~GoldNugget() {
    setVisible(false);
    getGraphObjects(2).erase(this);
    //cout << "gold dtor" << endl;
}

void GoldNugget::doSomething() {
    if (isAlive() == false) {
        return; // if not alive, do nothing
    }

    if (isVisible() == false && sp->findNearbyIceMan(this, 4) != nullptr) { // if not visible and iceman is in radius 4
        setVisible(true); // make visible
        return;
    }
    
   //disappear after lifetime ticks have passed
    if (temporary && isVisible()) {
        if (--lifetime <= 0) {
            setDead();
            return;
        }
    }
    
    if (temporary) {
        if (sp->getTicks() - trackTick >= lifetime) {
            setDead();
            return;
        }
    }

    Agent* a = sp->findNearbyPickerUpper(this, 3); // find nearby protester or iceman
    if (a != nullptr) {
        if (a->getID() == IID_PLAYER && temporary == false) {
            setVisible(false); // delete this later
            setDead(); // if iceman is in radius 3, set dead
            sp->playSound(SOUND_GOT_GOODIE); // play sound
            a->addGold(); // add gold to iceman inventory
            getWorld()->increaseScore(10); // increase score by 10
            return;
        }

        else if ((a->getID() == IID_PROTESTER || a->getID() == IID_HARD_CORE_PROTESTER)
            && temporary == true) {
            a->addGold(); // add gold to protester inventory
            this->setDead(); // if protester is in radius 3, set dead
            sp->playSound(SOUND_PROTESTER_FOUND_GOLD); // play sound
            sp->increaseScore(25); // increase score by 25
            if (a->getID() == IID_HARD_CORE_PROTESTER)
                sp->increaseScore(25); // 50 points for hardcore
            return;
        }
    }

}




//******************************** SonarKit Methods *******************************
SonarKit::SonarKit(StudentWorld* sp) : ActivatingObject(sp, 0, 60, IID_SONAR) {
    getGraphObjects(2).insert(this);
    setVisible(true);
}

void SonarKit::doSomething() {
    if (isAlive()) {
        if (lifetime == -1) {
            setTicksToLive();
        }
        if (sp->getTicks() >= lifetime) {
            setDead();
            lifetime = -1;
            return;
            }
        
        if (sp->findNearbyIceMan(this, 3)) {
            sp->playSound(SOUND_GOT_GOODIE);
            sp->getIceman()->addSonar(); // add sonar to iceman inventory
            sp->increaseScore(75); // increase score by 75
            setDead();
        }
    }
    else {
        return; // if not alive, do nothing
    }
}

void SonarKit::setTicksToLive() {
    int currentLevelNumber = sp->getLevel();
    int T = max(100, 300 - 10 * currentLevelNumber);
    lifetime = sp->getTicks() + T; // set lifetime to T ticks
}

SonarKit::~SonarKit() {
    setVisible(false);
    SonarKit::getGraphObjects(2).erase(this);
}

//******************************** Water Pool Methods *******************************
WaterPool::WaterPool(StudentWorld* sp, int x, int y)
    : ActivatingObject(sp, x, y, IID_WATER_POOL) // initially active false -> true
{
    //cout << "WaterPool ctor" << endl;
    getGraphObjects(2).insert(this);
    setVisible(true);
}

void WaterPool::doSomething() {
    if (isAlive()) {
        if (lifetime == -1) {
            setTicksToLive(); // set lifetime if not set
        }
        if (sp->getTicks() == lifetime) {
            setDead(); // set dead after lifetime
            lifetime = -1; // reset lifetime
        }
        if (sp->findNearbyIceMan(this, 3)) {
            sp->playSound(SOUND_GOT_GOODIE); // play sound
            sp->getIceman()->addWater(); // add water to iceman inventory
            sp->increaseScore(100); // increase score by 50
            setDead(); // set dead
        }
    }
}

void WaterPool::setTicksToLive() {
    int T = std::max(100, (int)(300 - 19 * sp->getLevel())); // set lifetime to T ticks
    lifetime = sp->getTicks() + T; // set lifetime to T ticks
}

WaterPool::~WaterPool() {
    setVisible(false);
    WaterPool::getGraphObjects(2).erase(this);
}
