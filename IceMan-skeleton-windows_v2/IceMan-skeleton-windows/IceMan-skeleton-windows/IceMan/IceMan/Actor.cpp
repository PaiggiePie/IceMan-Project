#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <random>


//******************************** Actor Methods *******************************

//contructor
Actor::Actor(StudentWorld* sp, int ID, int x, int y, Direction dir, double siz, unsigned int dep, bool visible) :
    GraphObject(ID, x, y, dir, siz, dep) {
    setWorld(sp); // set the world pointer
    //cout << "Actor ctor" << endl;
	srand(time(0)); // seed the random number generator

}

Actor::~Actor() {
    cout << "Actor dtor" << endl;
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
		return true; // cannot move to the specified location
	}
    return false;
}




//******************************** Agent Methods *******************************
Agent::Agent(StudentWorld* sp, int ID, int x, int y, Direction dir,
    unsigned int hitPoints) : Actor(sp, ID, x, y, right, 1.0, 0, true) {
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

Iceman::Iceman(StudentWorld* sp) : Agent(sp, IID_PLAYER, 30, 60, right, 5) { //contructor
    getGraphObjects(0).insert(this);
    setVisible(true);
    cout << "iceman ctor" << endl;
}


void Iceman::doSomething() {
    setWorld(sp);
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
            }
            if (getX() > 0) {
                moveTo(getX() - 1, getY());
            }
            break;
        case KEY_PRESS_RIGHT:
            if (d != right) {
                setDirection(right);
            }
            if (getX() < 64) {
                moveTo(getX() + 1, getY());
            }
            break;
        case KEY_PRESS_UP:
            if (d != up) {
                setDirection(up);
            }
            if (getY() < 60) {
                moveTo(getX(), getY() + 1);
            }
            break;
        case KEY_PRESS_DOWN:
            if (d != down) {
                setDirection(down);
            }
            if (getY() > 0) {
                moveTo(getX(), getY() - 1);
            }
            break;
        case KEY_PRESS_TAB: //place gold nugget at current position
            if (getGold() > 0) {
                m_gold--;
                GoldNugget* nugget = new GoldNugget(sp, true);
                sp->addObj(nugget); // add gold nugget to the world
            }
            break;
        case KEY_PRESS_SPACE: //squirt water
            if (getWater() > 0) {
                if (m_squirts > 0) {
                    m_squirts--;
                    if (d == left && moveToIfPossible(getX() - 4, getY())) {
                        if (!sp->NearBoulder(getX() - 4, getY(), 3)) { // check if there is a boulder in the way
                            Squirt* s = new Squirt(sp, getX() - 4, getY(), left); // create squirt object
                            sp->addActor(s);
                        }
                        sp->playSound(SOUND_PLAYER_SQUIRT); // play squirt sound
                    }
                    else if (d == right && moveToIfPossible(getX() + 4, getY())) {
                        if (!sp->NearBoulder(getX() - 4, getY(), 3)) {
                            Squirt* s = new Squirt(sp, getX() - 4, getY(), right);
                            sp->addActor(s);
                        }
                        sp->playSound(SOUND_PLAYER_SQUIRT);
                    }
                    else if (d == up && moveToIfPossible(getX(), getY() - 4)) {
                        if (!sp->NearBoulder(getX(), getY() - 4, 3)) {
                            Squirt* s = new Squirt(sp, getX(), getY() - 4, up);
                            sp->addActor(s);
                        }
                        sp->playSound(SOUND_PLAYER_SQUIRT);
                    }
                    else if (d == down && moveToIfPossible(getX(), getY() + 4)) {
                        if (!sp->NearBoulder(getX(), getY() + 4, 3)) {
                            Squirt* s = new Squirt(sp, getX(), getY() + 4, down);
                            sp->addActor(s);
                        }
                        sp->playSound(SOUND_PLAYER_SQUIRT);
                    }
                }
                // else do nothing
                break;
        case KEY_PRESS_ESCAPE:
            sp->playSound(SOUND_PLAYER_GIVE_UP); // play give up sound
            setDead(); // set iceman to dead
            break;
        case 'Z':
            if (getSonar() > 0) { // if iceman has sonar
                m_sonar--; // decrement sonar
                getWorld()->revealAllNearbyObjects(getX(), getY(), 60); // reveal all nearby objects
            }
            //use sonar if available

            break;
        case 'z':
            //use sonar if available
            if (getSonar() > 0) { // if iceman has sonar
                m_sonar--; // decrement sonar
                getWorld()->revealAllNearbyObjects(getX(), getY(), 60); // reveal all nearby objects
            }
            break;
            }
            sp->clearIce(getX(), getY()); // clear ice at current position
        }
    }
}

bool Iceman::annoy(unsigned int amount) {
    if (getHitPoints() > 1) {
        Agent::annoy(amount);
        return true;
    }
    setDead();
    return false;
}

bool Iceman::canDigThroughIce() const {
    return true;
}

void Iceman::addGold() {
    m_gold++;
}


void Iceman::addSonar() {
    m_sonar += 2;
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
    cout << "iceman dtor" << endl;

}



//******************************** Protester Methods *******************************
Protester::Protester(StudentWorld* sp, int x, int y, int ID,
    unsigned int hitPoints, unsigned int score) : Agent(sp, ID, x, y, left, 5) {
    setVisible(true);
    cout << "protester ctor" << endl;

}
void Protester::doSomething() {

}

bool Protester::annoy(unsigned int amount) {
    return true;
}

void Protester::addGold() {
    m_gold++;
}

bool Protester::huntsIceMan() const {
    return true;
}

// Set state to having given up protest
void Protester::setMustLeaveOilField() {
    m_state = true;
}

// Set number of ticks until next move
void Protester::setTicksToNextMove() {
    int ticks = getWorld()->getTicks();

}


//******************************** Regular Protester Methods *******************************
RegularProtester::RegularProtester(StudentWorld* sp, int x, int y, int ID) : Protester(sp, x, y, IID_PROTESTER, 5, 0) {
    cout << "regular protester ctor" << endl;

}

void RegularProtester::doSomething() {}
void RegularProtester::addGold() {

}

//******************************** Hardcore Protester Methods *******************************
HardcoreProtester::HardcoreProtester(StudentWorld* sp, int x, int y, int ID) :Protester(sp, x, y, IID_HARD_CORE_PROTESTER, 5, 0) {
    cout << "hardcore protester ctor" << endl;

}

void HardcoreProtester::doSomething() {}
void HardcoreProtester::addGold() {

}



//******************************** Ice Methods *******************************

//constructor
Ice::Ice(StudentWorld* sp) : Actor(sp, IID_ICE, getX(), getY(), right, 0.25, 3, true) {
    cout << "Ice ctor" << endl;
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
Boulder::Boulder(StudentWorld* sp) : Actor(sp, IID_BOULDER, getX(), getY(), down, 1.0, 1, true) {
    setVisible(true);
    cout << "boulder ctor" << endl;

}
bool Boulder::canActorsPassThroughMe() const {
    return false;
}

void Boulder::doSomething() {
    // if there is not ice below the boulder in 2x2 square, it can fall ie. 
    if (getWorld()->canActorMoveTo(this, getX(), getY() - 1) &&
        getWorld()->canActorMoveTo(this, getX() + 1, getY() - 1) &&
        getWorld()->canActorMoveTo(this, getX() + 2, getY() - 1) &&
        getWorld()->canActorMoveTo(this, getX() + 3, getY() - 1)) {
        // if boulder can fall, then it will fall after 30 ticks
        if (getWorld()->getTicks() % 30 == 0) {
            fallingState = true;
            getWorld()->playSound(SOUND_FALLING_ROCK);
        }
        else {
            // if boulder cannot fall, then it will not do anything
            return;


        }

        
    }
    if (fallingState == true) {
        if (getWorld()->canActorMoveTo(this, getX(), getY() - 1)) {
            moveTo(getX(), getY() - 1);
			getWorld()->annoyAllNearbyActors(this, 100, 3); // annoy all nearby actors
        }
        else if (getY() == -1) {
            setDead();
        }
    }
}

//******************************** Squirt Methods *******************************

Squirt::Squirt(StudentWorld* sp, int x, int y, Direction dir) : Actor(sp, IID_WATER_SPURT, getX(), getY(), right, 1.0, 1, true) {
    setVisible(true);
    cout << "squirt ctor" << endl;

}

void Squirt::doSomething() {
	if (moveToIfPossible(getX(), getY() + 4) && getDirection() == up) {
        if (!sp->NearBoulder(getX(), getY() + 4, 3))
            moveTo(getX(), getY() + 4);
        sp->playSound(SOUND_PLAYER_SQUIRT);
	}
	else if (moveToIfPossible(getX(), getY() - 4) && getDirection() == down) {
        if (!sp->NearBoulder(getX(), getY() - 4, 3))
            moveTo(getX(), getY() - 4);
        sp->playSound(SOUND_PLAYER_SQUIRT);
	}
	else if (moveToIfPossible(getX() - 4, getY()) && getDirection() == left) {
        if (!sp->NearBoulder(getX() -4, getY(), 3))
            moveTo(getX() - 4, getY());
        sp->playSound(SOUND_PLAYER_SQUIRT);
	}
	else if (moveToIfPossible(getX() + 4, getY()) && getDirection() == right) {
		if (!sp->NearBoulder(getX() + 4, getY(), 3))
            moveTo(getX() + 4, getY());
        sp->playSound(SOUND_PLAYER_SQUIRT); 
	}
	else {
		setDead();
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
    setVisible(false);
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


};

//******************************** Oil Methods *******************************
BarrelsOfOil::BarrelsOfOil(StudentWorld* sp) : ActivatingObject(sp, (std::rand() % 59), (std::rand() % 59), IID_BARREL, SOUND_FOUND_OIL, true, false, false) {
    getGraphObjects(2).insert(this);
    //moveTo((std::rand() % 59), (std::rand() % 59)); // random position in the world
    cout << "Barrels of Oil ctor" << endl;
}

void BarrelsOfOil::doSomething() {
    if (isAlive()) {
        if (isVisible() == false && getWorld()->findNearbyIceMan(this, 4)) { // if not visible and iceman is in radius 4
            setVisible(true); // make visible
            return;
        }
        if (getWorld()->findNearbyIceMan(this, 3)) {
            this->setDead(); // if iceman is in radius 3, set dead
            sp->playSound(SOUND_FOUND_OIL); // play sound
            getWorld()->increaseScore(1000); // increase score by 10
            return;
        }
    }
    else
        return;
}

bool BarrelsOfOil::needsToBePickedUpToFinishLevel() const {
    return true;
}

BarrelsOfOil::~BarrelsOfOil() {
    setVisible(false);
    BarrelsOfOil::getGraphObjects(2).erase(this); // remove from the set of graph objects
}



//******************************** Gold Nuggets Methods *******************************
GoldNugget::GoldNugget(StudentWorld* sp, bool temporary) 
    : ActivatingObject(sp, (std::rand() % 59), (std::rand() % 59), IID_GOLD, SOUND_GOT_GOODIE, true, true, false)
	, temporary(temporary), pickup(true)  // initialize temporary, pickup, and lifetime
{
    getGraphObjects(2).insert(this);
    //moveTo((std::rand() % 59), (std::rand() % 59)); // random position in the world
    cout << "gold ctor" << endl;
}

GoldNugget::~GoldNugget() {
    setVisible(false);
    getGraphObjects(2).erase(this);
    cout << "gold dtor" << endl;
}

void GoldNugget::doSomething() {
    if (isAlive() == false) {
        return; // if not alive, do nothing
    }

    if (temporary == true) {
        if (lifetime == -1) {
            lifetime = getWorld()->getTicks() + 800; // set lifetime to 800 ticks
        }
        // if temporary, set dead after lifetime of ticks
        if (getWorld()->getTicks() == lifetime) {
            setDead();
            lifetime = -1; // reset lifetime
        }
    }

    if (isVisible() == false && getWorld()->findNearbyIceMan(this, 4)) { // if not visible and iceman is in radius 4
        setVisible(true); // make visible
        return;
    }

    Agent* a = getWorld()->findNearbyPickerUpper(this, 3); // find nearby protester or iceman
    if (a != nullptr) {
        if (a->getID() == IID_PLAYER && pickup == true) {
            setDead(); // if iceman is in radius 3, set dead
            sp->playSound(SOUND_GOT_GOODIE); // play sound
            a->addGold(); // add gold to iceman inventory
            getWorld()->increaseScore(10); // increase score by 10
            pickup = false; // set pickup to false
            return;
        }

        else if ((a->getID() == IID_PROTESTER || a->getID() == IID_HARD_CORE_PROTESTER) && pickup == false) {
            a->addGold(); // add gold to protester inventory
            this->setDead(); // if protester is in radius 3, set dead
            sp->playSound(SOUND_GOT_GOODIE); // play sound
            getWorld()->increaseScore(25); // increase score by 25
            return;
        }
    }

}

//******************************** Water Pool Methods *******************************
WaterPool::WaterPool(StudentWorld* sp) 
    : ActivatingObject(sp, (std::rand() % 59), (std::rand() % 59), IID_WATER_POOL, SOUND_GOT_GOODIE, true, false, true) // initially active false -> true
{
    getGraphObjects(2).insert(this);
    setVisible(true);
    moveTo((std::rand() % 59), (std::rand() % 59)); // random position in the world

}

void WaterPool::doSomething() {
    if (isAlive()) {
        if (lifetime == -1) {
            setTicksToLive(); // set lifetime if not set
        }
        if (getWorld()->getTicks() == lifetime) {
            setDead(); // set dead after lifetime
            lifetime = -1; // reset lifetime
        }
        if (getWorld()->findNearbyIceMan(this, 3)) {
            sp->playSound(SOUND_GOT_GOODIE); // play sound
            getWorld()->getIceman()->addWater(); // add water to iceman inventory
            getWorld()->increaseScore(100); // increase score by 50
            setDead(); // set dead
        }
    }
}

void WaterPool::setTicksToLive() {
	int T = std::max(100, (int) (300 - 19 * sp->getLevel())); // set lifetime to T ticks
	lifetime = getWorld()->getTicks() + T; // set lifetime to T ticks
}

//******************************** SonarKit Methods *******************************
SonarKit::SonarKit(StudentWorld* sp) : ActivatingObject(sp, 0, 60, IID_SONAR, SOUND_GOT_GOODIE, true, false, false) {
    getGraphObjects(2).insert(this);
}

void SonarKit::doSomething() {
    if (isAlive()) {
        if (lifetime == -1) {
            setTicksToLive();
        }
        if (getWorld()->getTicks() == lifetime) {
            setDead();
            lifetime = -1; // reset lifetime
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
	int currentLevelNumber = getWorld()->getLevel();  
    int T = max(100, 300 - 10 * currentLevelNumber);
	lifetime = getWorld()->getTicks() + T; // set lifetime to T ticks
}
