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

bool Actor::annoy(unsigned int amt) {
    return false;
}

bool Actor::huntsIceMan() const {
    return true;
}

bool Actor::canDigThroughIce() const {
    return false;
}

bool Actor::canActorsPassThroughMe() const {
    return false;
}

bool Actor::canPickThingsUp() const {
    return false;
}

bool Actor::needsToBePickedUpToFinishLevel() const {
    return false;
}

bool Actor::moveToIfPossible(int x, int y) {
    if (getWorld()->canActorMoveTo(this, x, y)) {
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


bool Agent::canPickThingsUp() const {
    return true;
}

unsigned int Agent::getHitPoints() const {
    return hitPoints;
}
//******************************** IceMan Methods *******************************

Iceman::Iceman(StudentWorld* sp) : Agent(sp, IID_PLAYER, 30, 60, right, 10) { //contructor
    setWorld(sp);
    getGraphObjects(0).insert(this);
    
    setVisible(true);
    //cout << "iceman ctor" << endl;
}

void Iceman::doSomething() {
    StudentWorld* world = getWorld();
    if (world == nullptr) {
        cerr << "Error: StudentWorld pointer is null!" << endl;
        return;
    }

    Direction d = this->getDirection();
    int key;

    
    
    if (getWorld()->getKey(key) == true) {
        switch (key) {
            case KEY_PRESS_LEFT:
                if (d != left) {
                    setDirection(left);
                }
                else if (getX() > 0) {
                    moveTo(getX() - 1, getY());
                    world->clearIce(getX(), getY());
                }
                
                break;
            case KEY_PRESS_RIGHT:
                if (d != right) {
                    setDirection(right);
                }
                else if (getX() < 61) {
                    moveTo(getX() + 1, getY());
                    world->clearIce(getX(), getY());
                }
                
                break;
            case KEY_PRESS_UP:
                if (d != up) {
                    setDirection(up);
                }
                else if (getY() < 61) {
                    moveTo(getX(), getY() + 1);
                    world->clearIce(getX(), getY());
                }
                
                break;
            case KEY_PRESS_DOWN:
                if (d != down) {
                    setDirection(down);
                }
                else if (getY() > 0) {
                    moveTo(getX(), getY() - 1);
                    world->clearIce(getX(), getY());
                }
                
                break;
            case KEY_PRESS_TAB: //place gold nugget at current position
                //cout << "tab" << endl;
                if (getGold() != 0) {
                    m_gold--;
                    GoldNugget* nugget = new GoldNugget(sp, getX(), getY(), true);
                    world->addObj(nugget);
                }
                break;
            case KEY_PRESS_SPACE: //squirt water
                if (m_squirts > 0) {
                    world->playSound(SOUND_PLAYER_SQUIRT);
                    m_squirts--;
                    
                    int x = getX();
                    int y = getY();
                    
                    if(d == up && y + 4 < 64) {y += 4;}
                    else if(d == down && y - 4 >= 0) {y -= 4;}
                    else if(d == right && x + 4 < 60) {x += 4;}
                    else if(d == left && x - 4 >= 0) {x -= 4;}
                    else break;
                    
                    Squirt* s = new Squirt(sp, x, y, d,4);
                    world->addSquirt(s);
                }
                
                break;
            case KEY_PRESS_ESCAPE:
                //sp->decLives();
                this->annoy(hitPoints); // set iceman to dead
                break;
            case 'Z':
            case 'z':
                //use sonar if available
                if (getSonar() > 0) { // if iceman has sonar
                    m_sonar--; // decrement sonar
                    world->playSound(SOUND_SONAR);
                    world->revealAllNearbyObjects(getX(), getY(), 12  ); // reveal all nearby objects
                }
                break;
        }
    }
}



bool Iceman::annoy(unsigned int amount) {
    if (getHitPoints() >= 1) {
        Agent::annoy(amount);
        return false;
    }
    setDead();
    return false;
}

//void Iceman::setDead() {
//    if (sp) {
//        if (sp->getLives() == 0) {
//            m_isAlive = false;
//            sp->playSound(SOUND_PLAYER_GIVE_UP);
//        }
//    }
//}

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
        restingTicks = 0; // reset resting ticks
        return true;
    }
}

void Protester::addGold() {
    m_gold++;
    restingTicks = 0;
    setMustLeaveOilField(); // regulars must leave after being bribed
}

bool Protester::huntsIceMan() const {
    return true;
}

// Set state to having given up protest
void Protester::setMustLeaveOilField() {
    leave = true;
}

// Set number of ticks until next move
void Protester::setTicksToNextMove() {
    restingTicks = std::max(0, (int)(3 - sp->getLevel() / 4));
}

void Protester::setSquaresToMoveInCurrentDirection() {
    // 8 <= numSquaresToMoveInCurrentDirection <= 60
    // random number between 8 and 60
    numSquaresToMoveInCurrentDirection = (rand() % 60 + 8);
}


//******************************** Regular Protester Methods *******************************
RegularProtester::RegularProtester(StudentWorld* sp)
    : Protester(sp, IID_PROTESTER, 5, 0) {
    //cout << "regular protester ctor" << endl;

}

void RegularProtester::doSomething() {

    cout<< "perpen: " << perpenTicks << endl;
    cout<< "rest: " << restingTicks << endl;
    cout<< "shout:" << shoutTicks << endl;
    cout<< "prot doSomething" << endl;
    //dead or resting ->return
    if (!isAlive()){
        return;
    }
    
    if (shoutTicks < 15) shoutTicks++;
    if (perpenTicks < 200) perpenTicks++;
    
    if (restingTicks > 0)
    {
        restingTicks--;
        return;
    }
    
    int ticks = getWorld()->getTicks();
    if (restingTicks != 0 && (ticks % restingTicks == 0) && resting == true) {
        resting = false; // reset resting state
    }

    //leaving state actions
    if(leave == true){
        int x = getX();
        int y = getY();
        if (y == 60 && x == 60){
            setDead();
        }
        //find way to exit
        Direction d = sp->determineFirstMoveToExit(x, y);
        setDirection(d);
        switch(d){
            case right:
                moveTo( x+1, y);
                break;
            case left:
                moveTo( x-1, y);
                break;
            case down:
                moveTo( x, y-1 );
                break;
            case up:
                moveTo( x, y+1 );
                break;
            default:
                break;
        }
        return;
    }
    if (sp->facingTowardIceMan(this) && sp->isNearIceMan(this, 4)){
        if(shoutTicks >= 15){
            sp->playSound(SOUND_PROTESTER_YELL);
            sp->getIceman()->annoy(2);
            shoutTicks = 0;
            setTicksToNextMove();
            return;
        }
    }
    
    if(sp->lineOfSightToIceMan(this, false) && !sp->isNearIceMan(this, 4) && sp->canActorMoveTo(this, sp->getIceman()->getX(), sp->getIceman()->getY())){
        Direction toIceman = sp->determineFirstMoveToIceMan(sp->getIceman()->getX(), sp->getIceman()->getY());
        setDirection(toIceman);
        switch(toIceman){
            case right:
                moveTo( getX()+1, getY());
                break;
            case left:
                moveTo( getX()-1, getY());
                break;
            case down:
                moveTo( getX(), getY()-1 );
                break;
            case up:
                moveTo( getX(), getY()+1 );
                break;
            default:
                break;
        }
        numSquaresToMoveInCurrentDirection = 0;
        return;
    }
    // if finished wandering, set new direction
        if (numSquaresToMoveInCurrentDirection <= 0) {
            if (sp->canActorMoveTo(this, getX() - 1, getY()) && !(getDirection() == left)) {// check if can move left
                setDirection(left);
                moveTo(getX() - 1, getY());
                setTicksToNextMove();


        }
            else if (sp->canActorMoveTo(this, getX() + 1, getY()) && !(getDirection() == right)) {// check if can move right
                setDirection(right);
                moveTo(getX() + 1, getY());
                setTicksToNextMove();


            }
            else if (sp->canActorMoveTo(this, getX(), getY() + 1) && !(getDirection() == up)) {// check if can move up
                setDirection(up);
            moveTo(getX(), getY() + 1);
                setTicksToNextMove();


        }
            else if (sp->canActorMoveTo(this, getX(), getY() - 1) && !(getDirection() == down)) {// check if can move down
                setDirection(down);
                moveTo(getX(), getY() - 1);
                setTicksToNextMove();


            }
            else //last resort direction
                setDirection(sp->determineFirstMoveToIceMan(getX(), getY())); // if cannot move in current direction, change direction
            setSquaresToMoveInCurrentDirection(); // reset squares to move in current direction
            setTicksToNextMove();

        }
    
    // if no perpendicular turn in 200 non-resting ticks, change to a perpendicular direction if possible
    if (perpenTicks >= 200) {
        bool turned = false;
        
            if (getDirection() == left || getDirection() == right) { // if current direction is left or right
                if (sp->canActorMoveTo(this, getX(), getY() + 1)) { // if can move up
                    if(getY() < 60){
                        setDirection(up);
                        turned = true;
                    }
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
           if (turned){
               setSquaresToMoveInCurrentDirection();
               perpenTicks = 0;
           }
       }
    
    // if can't see, just move (wander oil field)
    // can move in current direction for numSquaresToMoveInCurrentDirection
        bool moved = false;

        switch (getDirection()) {
            case up:
                moved = moveToIfPossible(getX(), getY() + 1);
                setTicksToNextMove();
                break;
            case down:
                moved = moveToIfPossible(getX(), getY() - 1);
                setTicksToNextMove();

                break;
            case left:
                moved = moveToIfPossible(getX() - 1, getY());
                setTicksToNextMove();

                break;
            case right:
                moved = moveToIfPossible(getX() + 1, getY());
                setTicksToNextMove();

                break;
            default:
                break;
        }

        if (moved) {
            numSquaresToMoveInCurrentDirection--;
        } else {
            numSquaresToMoveInCurrentDirection = 0; // force a new direction next tick
        }

}




//******************************** Hardcore Protester Methods *******************************
HardcoreProtester::HardcoreProtester(StudentWorld* sp)
    : Protester(sp, IID_HARD_CORE_PROTESTER, 20, 0) {
    //cout << "hardcore protester ctor" << endl;
        if (!isAlive()) {
                setTicksToNextMove(); // set ticks to next move
                return; // do nothing
            }
}

void HardcoreProtester::doSomething() {
    
}

void HardcoreProtester::addGold() {
    // increasing score, sound added in gold func
    m_gold++;
    restingTicks = max(50, (int) (100 - sp->getLevel() * 10)); // time to stare before chasing again
}


//******************************** Ice Methods *******************************

//constructor
Ice::Ice(StudentWorld* sp, int x, int y) : Actor(sp, IID_ICE, x, y, right, 0.25, 3, true) {
    //cout << "Ice ctor" << endl;
    setVisible(true);
    // add to the set of graph objects
    getGraphObjects(3).insert(this);
}

void Ice::doSomething(){
    return;
}

//******************************** Boulder Methods *******************************
Boulder::Boulder(StudentWorld* sp, int x, int y) : Actor(sp, IID_BOULDER, x, y, down, 1.0, 1, true) {
    setVisible(true);
    //cout << "boulder ctor" << endl;

}
bool Boulder::canActorsPassThroughMe() const {
    return false;
}

void Boulder::doSomething() {
    // if there is not ice below the boulder in 2x2 square, it can fall ie.
    if (isAlive() == false) {
        return; // if not alive, do nothing
    }
    bool canFall =
    (sp->canActorMoveTo(this, getX(),     getY() - 1) &&
    sp->canActorMoveTo(this, getX() + 1, getY() - 1) &&
    sp->canActorMoveTo(this, getX() + 2, getY() - 1) &&
    sp->canActorMoveTo(this, getX() + 3, getY() - 1));
    
    if (!fallingState){
        if (canFall && fallingState == false) {
            // if boulder can fall, then it will fall after 30 ticks
            if (m_waitTicks == -1){
                m_waitTicks = 30;
            }
            else if (--m_waitTicks <= 0) {
                fallingState = true;
                sp->playSound(SOUND_FALLING_ROCK);
            }
        } else {
            m_waitTicks = -1; //
        }
    }
    
    if (fallingState == true) {
            if (canFall) {
                    moveTo(getX(), getY() - 1);
                    getWorld()->annoyAllNearbyActors(this, 100, 3); // annoy all nearby actors
            }
            else{
                setDead();
                fallingState = false;
            }
        }
}

//******************************** Squirt Methods *******************************

Squirt::Squirt(StudentWorld* sp, int x, int y, Direction dir, int m_distance) : Actor(sp, IID_WATER_SPURT, x, y, dir, 1.0, 1, true), m_distance(4) {
    //let doSomething decide if it should appear
    setVisible(true);
    //cout << "squirt ctor" << endl;
}

void Squirt::doSomething() {
    if (!isAlive()) {
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
        setDead(); // set squirt to dead
        return;
    }
}



//******************************** ActivatingObj Methods *******************************
ActivatingObject::ActivatingObject(StudentWorld* sp, int x, int y, int ID,
    int soundToPlay, bool activateOnPlayer, bool activateOnProtester, bool initallyActive) :
    Actor(sp, ID, x, y, right, 1.0, 2, true), m_soundToPlay(soundToPlay),
    m_activateOnPlayer(activateOnPlayer), m_activateOnProtester(activateOnProtester),
    m_initiallyActive(initallyActive)  // initialize the member variables
{
    //cout << "ActivatingObject ctor" << endl;
}

//will only be used by barrels of oil. all other objects use Actor::needsToBePickedUpToFinishLevel()
bool ActivatingObject::needsToBePickedUpToFinishLevel() const {
    return false;
}

bool ActivatingObject::canActorsPassThroughMe() const {
    return true;
}
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
    : ActivatingObject(sp, x, y, IID_BARREL, SOUND_FOUND_OIL, true, false, false) {
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
            setDead(); // if iceman is in radius 3, set dead
            return;
        }
    }
}

bool BarrelsOfOil::needsToBePickedUpToFinishLevel() const {
    return true;
}

BarrelsOfOil::~BarrelsOfOil() {
    setVisible(false);
    BarrelsOfOil::getGraphObjects(2).erase(this); // remove from the set of graph objects
    //cout << "barrelofoil dtor" << endl;
}



//******************************** Gold Nuggets Methods *******************************
GoldNugget::GoldNugget(StudentWorld* sp, int x, int y, bool temporary)
    : ActivatingObject(sp, x, y, IID_GOLD, SOUND_GOT_GOODIE, true, true, false),
    temporary(temporary) // initialize temporary, pickup, and lifetime
{
    lifetime = temporary ? 100 : -1;
    getGraphObjects(2).insert(this);
    setVisible(false);
    //cout << "gold ctor" << endl;
}



void GoldNugget::doSomething() {
    if (isAlive() == false) {
        return; // if not alive, do nothing
    }
    
    if (isVisible() == false && sp->findNearbyIceMan(this, 4) != nullptr) { // if not visible and iceman is in radius 4
        setVisible(true); // make visible
        return;
    }
    
    if (temporary) {
        if (--lifetime <= 0)
            setDead();
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

    if (temporary == true) {
        if (lifetime == -1) {
            setTicksToLive(); // set lifetime to 800 ticks
            trackTick = sp->getTicks();
        }
        // if temporary, set dead after lifetime of ticks
        if (sp->getTicks()- trackTick == lifetime) {
            setDead();
            lifetime = -1; // reset lifetime
            return;
        }
    }
}

void GoldNugget::setTicksToLive() {
    int ticks = sp->getTicks();

    if (sp->getTicks() < 0) {
        lifetime = 2000;
    }
    else {
        lifetime = ticks + 2000; // set lifetime to 800 ticks
    }
}

//******************************** SonarKit Methods *******************************
SonarKit::SonarKit(StudentWorld* sp) : ActivatingObject(sp, 0, 60, IID_SONAR, SOUND_GOT_GOODIE, true, false, false) {
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
            lifetime = -1; // reset lifetime
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


//******************************** Water Pool Methods *******************************
WaterPool::WaterPool(StudentWorld* sp, int x, int y)
    : ActivatingObject(sp, x, y, IID_WATER_POOL, SOUND_GOT_GOODIE, true, false, true) // initially active false -> true
{
    sp->playSound(SOUND_FALLING_ROCK);
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
        if (getWorld()->findNearbyIceMan(this, 3)) {
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
