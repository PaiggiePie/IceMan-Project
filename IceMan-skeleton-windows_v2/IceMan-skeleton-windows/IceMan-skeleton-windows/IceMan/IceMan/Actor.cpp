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
		moveTo(x, y); // move to the specified location
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
            else if (getX() > 0) {
                moveTo(getX() - 1, getY());
                sp->clearIce(getX(), getY());
            }
            
            break;
        case KEY_PRESS_RIGHT:
            if (d != right) {
                setDirection(right);
            }
            else if (getX() < 61) {
                moveTo(getX() + 1, getY());
                sp->clearIce(getX(), getY());
            }
            
            break;
        case KEY_PRESS_UP:
            if (d != up) {
                setDirection(up);
            }
            else if (getY() < 61) {
                moveTo(getX(), getY() + 1);
                sp->clearIce(getX(), getY());
            }
            
            break;
        case KEY_PRESS_DOWN:
            if (d != down) {
                setDirection(down);
            }
            else if (getY() > 0) {
                moveTo(getX(), getY() - 1);
                sp->clearIce(getX(), getY());
            }
            
            break;
        case KEY_PRESS_TAB: //place gold nugget at current position
            cout << "tab" << endl;
            if (getGold() != 0) {
                m_gold--;
                GoldNugget* nugget = new GoldNugget(sp, getX(), getY(), true);
                nugget->doSomething();
                sp->addObj(nugget);
            }
            break;
        case KEY_PRESS_SPACE: //squirt water
            if (m_squirts > 0) {
                sp->playSound(SOUND_PLAYER_SQUIRT);
                if (d == up) {
                    m_squirts--;
                    Squirt* s = new Squirt(sp, getX(), getY() + 4, up);
                    s->doSomething();

                }
                else if (d == down) {
                    m_squirts--;
                    Squirt* s = new Squirt(sp, getX(), getY() - 4, down);
                    s->doSomething();

                }
                else if (d == right) {
                    m_squirts--;
                    Squirt* s = new Squirt(sp, getX() + 4, getY(), right);
                    s->doSomething();
                }
                else if (d == left) {
                    m_squirts--;
                    Squirt* s = new Squirt(sp, getX() - 4, getY(), left);
                    s->doSomething();
                }
            }
            break;
        case KEY_PRESS_ESCAPE:
            //sp->decLives();
            cout << "esc" << endl;
            setDead(); // set iceman to dead
            break;
        case 'Z':
            if (getSonar() > 0) { // if iceman has sonar
                m_sonar--; // decrement sonar
				sp->playSound(SOUND_SONAR); // play sonar sound
                getWorld()->revealAllNearbyObjects(getX(), getY(), 60); // reveal all nearby objects
            }
            //use sonar if available

            break;
        case 'z':
            //use sonar if available
            if (getSonar() > 0) { // if iceman has sonar
                m_sonar--; // decrement sonar
                sp->playSound(SOUND_SONAR); 
                getWorld()->revealAllNearbyObjects(getX(), getY(), 60); // reveal all nearby objects
            }
            break;
        }
    }
}


bool Iceman::annoy(unsigned int amount) {
    if (getHitPoints() >= 1) {
        Agent::annoy(amount);
        return true;
    }
    setDead();
    return false;
}

void Iceman::setDead() {
    if (sp) {
        if (sp->getLives() == 0) {
            m_isAlive = false;
            sp->playSound(SOUND_PLAYER_GIVE_UP);
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

void RegularProtester::doSomething() {

}

//******************************** Hardcore Protester Methods *******************************
HardcoreProtester::HardcoreProtester(StudentWorld* sp, int x, int y, int ID) :Protester(sp, x, y, IID_HARD_CORE_PROTESTER, 5, 0) {
    cout << "hardcore protester ctor" << endl;

}

void HardcoreProtester::doSomething() {

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
    cout << "Ice dtor" << endl;
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
	if (isAlive() == false) {
		return; // if not alive, do nothing
	}

    if (sp->canActorMoveTo(this, getX(), getY() - 1) &&
        sp->canActorMoveTo(this, getX() + 1, getY() - 1) &&
        sp->canActorMoveTo(this, getX() + 2, getY() - 1) &&
        sp->canActorMoveTo(this, getX() + 3, getY() - 1)) {
        // if boulder can fall, then it will fall after 30 ticks
        if (getWorld()->getTicks() % 30 == 0) {
            fallingState = true;
            sp->playSound(SOUND_FALLING_ROCK);
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

//Squirt::Squirt(StudentWorld* sp, int x, int y, Direction dir) : Actor(sp, IID_WATER_SPURT, getX(), getY(), right, 1.0, 1, true) {
//    setVisible(true);
//
//}

Squirt::Squirt(StudentWorld* sp, int x, int y, Direction dir) : Actor(sp, IID_WATER_SPURT, x, y, dir, 1.0, 1, false) {
    //let doSomething decide if it should appear
    sp->addActor(this);
    setVisible(true);
    cout << "squirt ctor" << endl;
}

void Squirt::doSomething() {
    setVisible(true);
	if (!isAlive()) {
		return; // if not alive, do nothing
	}

    if (sp->canActorMoveTo(this, getX(), getY() + 4) && getDirection() == up) {
        if (!sp->NearBoulder(getX(), getY() + 4, 3))
            moveTo(getX(), getY() + 4);
        else
            setDead();// if cannot move, set dead
    }
    else if (sp->canActorMoveTo(this, getX(), getY() - 4) && getDirection() == down) {
        if (!sp->NearBoulder(getX(), getY() - 4, 3))
            moveTo(getX(), getY() - 4);
        else
            setDead();
    }
    else if (sp->canActorMoveTo(this, getX() - 4, getY()) && getDirection() == left) {
        if (!sp->NearBoulder(getX() - 4, getY(), 3))
            moveTo(getX() - 4, getY());
        else
			setDead(); 
    }
    else if (sp->canActorMoveTo(this, getX() + 4, getY()) && getDirection() == right) {
        if (!sp->NearBoulder(getX() + 4, getY(), 3))
            moveTo(getX() + 4, getY());
        else
            setDead();
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
BarrelsOfOil::BarrelsOfOil(StudentWorld* sp) : ActivatingObject(sp, (std::rand() % 59), (std::rand() % 59), IID_BARREL, SOUND_FOUND_OIL, true, false, false) {
    getGraphObjects(2).insert(this);
    setVisible(false);
    cout << "Barrels of Oil ctor" << endl;
}

void BarrelsOfOil::doSomething() {
    if (isAlive()) {
        if (isVisible() == false && sp->findNearbyIceMan(this, 4) != nullptr) { // if not visible and iceman is in radius 4
            setVisible(true); // make visible
            return;
        }
        if (sp->findNearbyIceMan(this, 3) != nullptr) {
            cout << "barrel picked up" << endl;
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
    cout << "barrelofoil dtor" << endl;
}



//******************************** Gold Nuggets Methods *******************************
GoldNugget::GoldNugget(StudentWorld* sp, int x, int y, bool temporary) :
    ActivatingObject(sp, x, y, IID_GOLD, SOUND_GOT_GOODIE, true, true, false),
    temporary(temporary) // initialize temporary, pickup, and lifetime
{
    getGraphObjects(2).insert(this);
    setVisible(false);
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

    if (isVisible() == false && sp->findNearbyIceMan(this, 4) != nullptr) { // if not visible and iceman is in radius 4
        setVisible(true); // make visible
        return;
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
            sp->playSound(SOUND_GOT_GOODIE); // play sound
            sp->increaseScore(25); // increase score by 25
            return;
        }
    }

    if (temporary == true) {
        if (lifetime == -1) {
            setTicksToLive(); // set lifetime to 800 ticks
        }
        // if temporary, set dead after lifetime of ticks
        if (sp->getTicks() == lifetime) {
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
        if (sp->getTicks() == lifetime) {
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

SonarKit::~SonarKit() {
    setVisible(false);
    SonarKit::getGraphObjects(2).erase(this);
}

//******************************** Water Pool Methods *******************************
WaterPool::WaterPool(StudentWorld* sp)
    : ActivatingObject(sp, (std::rand() % 59), (std::rand() % 59), IID_WATER_POOL, SOUND_GOT_GOODIE, true, false, true) // initially active false -> true
{
    sp->playSound(SOUND_FALLING_ROCK);
	cout << "WaterPool ctor" << endl;
    getGraphObjects(2).insert(this);
    setVisible(true);
    //moveTo((std::rand() % 59), (std::rand() % 59)); // random position in the world

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


