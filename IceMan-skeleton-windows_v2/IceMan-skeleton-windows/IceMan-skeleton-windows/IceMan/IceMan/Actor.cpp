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

Iceman::Iceman(StudentWorld* sp) : Agent(sp, IID_PLAYER, 30, 60, right, 5) { //contructor
    getGraphObjects(0).insert(this);
    setVisible(true);
    //cout << "iceman ctor" << endl;
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
            //cout << "tab" << endl;
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
            setDead(); // set iceman to dead
            break;
        case 'Z':
            if (getSonar() > 0) { // if iceman has sonar
                m_sonar--; // decrement sonar
                sp->playSound(SOUND_SONAR); // play sonar sound
                getWorld()->revealAllNearbyObjects(getX(), getY(), 12); // reveal all nearby objects
            }
            //use sonar if available

            break;
        case 'z':
            //use sonar if available
            if (getSonar() > 0) { // if iceman has sonar
                m_sonar--; // decrement sonar
                sp->playSound(SOUND_SONAR);
                getWorld()->revealAllNearbyObjects(getX(), getY(), 12); // reveal all nearby objects
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
void Protester::doSomething() { }

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
    restingTicks = std::max(0, (int)(3 - sp->getLevel() / 4));}

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
    if (!isAlive()) {
        return; // do nothing
    }

    // check if resting (if resting at 0, ignore this part)
    int ticks = getWorld()->getTicks();
    if (restingTicks != 0 && (ticks % restingTicks == 0) && resting == true) {
        resting = false; // reset resting state
    }
    if (resting == true)
        return;

    // if at exit, set dead
    if (getX() == 60 && getY() == 60 && leave == true) {
        setDead();
        return;
    }

    Direction dp = getDirection(); // get current direction

    // if hit points are 0, set to leave oil field
    if (hitPoints == 0)
        this->setMustLeaveOilField(); // leave == true

    // if leaving, determine first move to exit (move 1 square at a time)
    if (leave == true) {
        Direction d = sp->determineFirstMoveToExit(getX(), getY());
        if (d == left) {
            setDirection(left);
            moveTo(getX() - 1, getY()); // move to exit
        }
        else if (d == right) {
            setDirection(right);
            moveTo(getX() + 1, getY()); // move to exit
        }
        else if (d == up) {
            setDirection(up);
            moveTo(getX(), getY() + 1); // move to exit
        }
        else if (d == down) {
            setDirection(down);
            moveTo(getX(), getY() - 1); // move to exit
        }
        return;
    }

    // if iceman is inline of sight, facing and within 4 squares = shout
    if (sp->findNearbyIceMan(this, 4) != nullptr) {
        if (sp->facingTowardIceMan(this)) {
            if (shouted == false)
                sp->playSound(SOUND_PROTESTER_YELL);
            sp->getIceman()->annoy(2); // annoy iceman 2 points
            shouted = true;
        }
    }

    // if in line of sight, but not facing & more than 4 units away, turn to face iceman & move 1 square in that direction
    if (sp->lineOfSightToIceMan(this, false)) {
        if (sp->isNearIceMan(this, 4) == false) {
            // turn to face iceman & move 1 forward
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
    }

    // if finished wandering, set new direction
    if (numSquaresToMoveInCurrentDirection <= 0) {
        if (sp->canActorMoveTo(this, getX() - 1, getY()) && !(getDirection() == left)) // check if can move left
            setDirection(left);
        else if (sp->canActorMoveTo(this, getX() + 1, getY()) && !(getDirection() == right)) // check if can move right
            setDirection(right);
        else if (sp->canActorMoveTo(this, getX(), getY() + 1) && !(getDirection() == up)) // check if can move up
            setDirection(up);
        else if (sp->canActorMoveTo(this, getX(), getY() - 1) && !(getDirection() == down)) // check if can move down
            setDirection(down);
        else //last resort direction
            setDirection(sp->determineFirstMoveToIceMan(getX(), getY())); // if cannot move in current direction, change direction
        setSquaresToMoveInCurrentDirection(); // reset squares to move in current direction
    }


	// set perpendicular turn ticks back to zero if perpen turn is made, else increment
    if (((dp == left || dp == right) && (getDirection() == up || getDirection() == down)) ||
        ((dp == up || dp == down) && (getDirection() == left || getDirection() == right))) {
        perpenTicks = 0; // reset perpendicular turn ticks
    }
    else {
        perpenTicks++; // increment perpendicular turn ticks
    }

    // if no perpendicular turn in 200 non-resting ticks, change to a perpendicular direction if possible
    if (perpenTicks >= 200) {
		if (getDirection() == left || getDirection() == right) { // if current direction is left or right
			if (sp->canActorMoveTo(this, getX(), getY() + 1)) { // if can move up
				setDirection(up);
                perpenTicks = 0; // reset perpendicular turn ticks
			}
			else if (sp->canActorMoveTo(this, getX(), getY() - 1)) { // if can move down
				setDirection(down);
                perpenTicks = 0; // reset perpendicular turn ticks
			}
		}
		else if (getDirection() == up || getDirection() == down) { // if current direction is up or down
			if (sp->canActorMoveTo(this, getX() - 1, getY())) { // if can move left
				setDirection(left);
                perpenTicks = 0; // reset perpendicular turn ticks
			}
			else if (sp->canActorMoveTo(this, getX() + 1, getY())) { // if can move right
				setDirection(right);
                perpenTicks = 0; // reset perpendicular turn ticks
			}
		}
        
    }

    if (sp->getTicks() % 15 == 0 && shouted == true) { // reset shout after 15 ticks
        shouted = false;
    }

    // if can't see, just move (wander oil field)
// can move in current direction for numSquaresToMoveInCurrentDirection
    if (moveToIfPossible(getX(), getY() + 1) && getDirection() == up) {
        numSquaresToMoveInCurrentDirection--;
    }
    else if (moveToIfPossible(getX(), getY() - 1) && getDirection() == down) {
        numSquaresToMoveInCurrentDirection--;
    }
    else if (moveToIfPossible(getX() - 1, getY()) && getDirection() == left) {
        numSquaresToMoveInCurrentDirection--;
    }
    else if (moveToIfPossible(getX() + 1, getY()) && getDirection() == right) {
        numSquaresToMoveInCurrentDirection--;
    }
    else {
        // if cannot move, change direction
        numSquaresToMoveInCurrentDirection = 0; // set squares to move in current direction
    }

}



//******************************** Hardcore Protester Methods *******************************
HardcoreProtester::HardcoreProtester(StudentWorld* sp) 
    : Protester(sp, IID_HARD_CORE_PROTESTER, 20, 0) {
    //cout << "hardcore protester ctor" << endl;

}

void HardcoreProtester::doSomething() {
    if (!isAlive()) {
        setTicksToNextMove(); // set ticks to next move
        return; // do nothing
    }

    // check if resting (if resting at 0, ignore this part)
    int ticks = getWorld()->getTicks();
    if (restingTicks != 0 && (ticks % restingTicks == 0) && resting == true) {
        resting = false; // reset resting state
    }
    if (resting == true)
        return;

    // if at exit, set dead
    if (getX() == 60 && getY() == 60) {
        setDead();
        return;
    }

    Direction dp = getDirection(); // get current direction

    // if hit points are 0, set to leave oil field
    if (hitPoints == 0)
        this->setMustLeaveOilField(); // leave == true

    // if leaving, determine first move to exit (move 1 square at a time)
    if (leave == true) {
        Direction d = sp->determineFirstMoveToExit(getX(), getY());
        if (d == left) {
            setDirection(left);
            moveTo(getX() - 1, getY()); // move to exit
        }
        else if (d == right) {
            setDirection(right);
            moveTo(getX() + 1, getY()); // move to exit
        }
        else if (d == up) {
            setDirection(up);
            moveTo(getX(), getY() + 1); // move to exit
        }
        else if (d == down) {
            setDirection(down);
            moveTo(getX(), getY() - 1); // move to exit
        }
        return;
    }

    // if iceman is inline of sight, facing and within 4 squares = shout
    if (sp->findNearbyIceMan(this, 4) != nullptr) {
        if (sp->facingTowardIceMan(this)) {
            if (shouted == false) {
                sp->playSound(SOUND_PROTESTER_YELL);
                sp->getIceman()->annoy(2); // annoy iceman 2 points
            }
            shouted = true;
        }
        return;
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


	// if in line of sight, but not facing & more than 4 units away, turn to face iceman & move 1 square in that direction// if in line of sight, but not facing & more than 4 units away, turn to face iceman & move 1 square in that direction
    if (sp->lineOfSightToIceMan(this, false)) { //has line of sight to iceman, but not correct direction
        if (sp->isNearIceMan(this, 4) == false && sp->lineOfSightToIceMan(this, false)) { // greater than 4 squares away
            // turn to face iceman & move 1 forward
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
    }

    // if finished wandering, set new direction
    if (numSquaresToMoveInCurrentDirection <= 0) {
        if (sp->canActorMoveTo(this, getX() - 1, getY()) && !(getDirection() == left)) // check if can move left
            setDirection(left);
        else if (sp->canActorMoveTo(this, getX() + 1, getY()) && !(getDirection() == right)) // check if can move right
            setDirection(right);
        else if (sp->canActorMoveTo(this, getX(), getY() + 1) && !(getDirection() == up)) // check if can move up
            setDirection(up);
        else if (sp->canActorMoveTo(this, getX(), getY() - 1) && !(getDirection() == down)) // check if can move down
            setDirection(down);
        else //last resort direction
            setDirection(sp->determineFirstMoveToIceMan(getX(), getY())); // if cannot move in current direction, change direction
        setSquaresToMoveInCurrentDirection(); // reset squares to move in current direction
    }



    // set perpendicular turn ticks back to zero if perpen turn is made, else increment
    if (((dp == left || dp == right) && (getDirection() == up || getDirection() == down)) ||
        ((dp == up || dp == down) && (getDirection() == left || getDirection() == right))) {
        perpenTicks = 0; // reset perpendicular turn ticks
    }
    else {
        perpenTicks++; // increment perpendicular turn ticks
    }

    // if no perpendicular turn in 200 non-resting ticks, change to a perpendicular direction if possible
    if (perpenTicks >= 200) {
        if (getDirection() == left || getDirection() == right) { // if current direction is left or right
            if (sp->canActorMoveTo(this, getX(), getY() + 1)) { // if can move up
                setDirection(up);
                setSquaresToMoveInCurrentDirection();
                perpenTicks = 0; // reset perpendicular turn ticks
            }
            else if (sp->canActorMoveTo(this, getX(), getY() - 1)) { // if can move down
                setDirection(down);
                setSquaresToMoveInCurrentDirection();
                perpenTicks = 0; // reset perpendicular turn ticks
            }
        }
        else if (getDirection() == up || getDirection() == down) { // if current direction is up or down
            if (sp->canActorMoveTo(this, getX() - 1, getY())) { // if can move left
                setDirection(left);
                setSquaresToMoveInCurrentDirection();
                perpenTicks = 0; // reset perpendicular turn ticks
            }
            else if (sp->canActorMoveTo(this, getX() + 1, getY())) { // if can move right
                setDirection(right);
                setSquaresToMoveInCurrentDirection();
                perpenTicks = 0; // reset perpendicular turn ticks
            }
        }
        
    }

    if (sp->getTicks() % 15 == 0 && shouted == true) { // reset shout after 15 ticks
        shouted = false;
    }

    // if can't see, just move (wander oil field)
// can move in current direction for numSquaresToMoveInCurrentDirection
    if (moveToIfPossible(getX(), getY() + 1) && getDirection() == up) {
        numSquaresToMoveInCurrentDirection--;
    }
    else if (moveToIfPossible(getX(), getY() - 1) && getDirection() == down) {
        numSquaresToMoveInCurrentDirection--;
    }
    else if (moveToIfPossible(getX() - 1, getY()) && getDirection() == left) {
        numSquaresToMoveInCurrentDirection--;
    }
    else if (moveToIfPossible(getX() + 1, getY()) && getDirection() == right) {
        numSquaresToMoveInCurrentDirection--;
    }
    else {
        // if cannot move, change direction
        numSquaresToMoveInCurrentDirection = 0; // set squares to move in current direction
    }

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
        sp->canActorMoveTo(this, getX() + 3, getY() - 1) && fallingState == false) {
        // if boulder can fall, then it will fall after 30 ticks
        if (getWorld()->getTicks() % 30 == 0) {
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
            getWorld()->annoyAllNearbyActors(this, 100, 3); // annoy all nearby actors
        }
        else {
            setDead();
        }
    }
}

//******************************** Squirt Methods *******************************

Squirt::Squirt(StudentWorld* sp, int x, int y, Direction dir) : Actor(sp, IID_WATER_SPURT, x, y, dir, 1.0, 1, false) {
    //let doSomething decide if it should appear
    sp->addActor(this);
    setVisible(true);
    //cout << "squirt ctor" << endl;
}

void Squirt::doSomething() {
    setVisible(true);
    if (!isAlive()) {
        return; // if not alive, do nothing
    }


    if (sp->canActorMoveTo(this, getX(), getY() + 1) && getDirection() == up) {
        if (!sp->NearBoulder(getX(), getY() + 4, 3))
            moveTo(getX(), getY() + 1);
        else
            setDead();// if cannot move, set dead
    }
    else if (sp->canActorMoveTo(this, getX(), getY() - 1) && getDirection() == down) {
        if (!sp->NearBoulder(getX(), getY() - 4, 3))
            moveTo(getX(), getY() - 1);
        else
            setDead();
    }
    else if (sp->canActorMoveTo(this, getX() - 1, getY()) && getDirection() == left) {
        if (!sp->NearBoulder(getX() - 4, getY(), 3))
            moveTo(getX() - 1, getY());
        else
            setDead();
    }
    else if (sp->canActorMoveTo(this, getX() + 1, getY()) && getDirection() == right) {
        if (!sp->NearBoulder(getX() + 4, getY(), 3))
            moveTo(getX() + 1, getY());
        else
            setDead();
    }
    else {
        setDead();
        return;
    }

    // if is a nearby protester gets hit, annoy them
	if (sp->findNearbyPickerUpper(this, 3) != nullptr) {
		Agent* nearby = sp->findNearbyPickerUpper(this, 3);
        if (!nearby->leavingOilField()) { // if not in leaving state, annoy
            nearby->annoy(2); // annoy protester by 2
            sp->increaseScore(100); 
            if (nearby->getID() == IID_HARD_CORE_PROTESTER)
				sp->increaseScore(150); // 250 points for hardcore protester
            setDead(); // set squirt to dead
            return;
        }
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
    getGraphObjects(2).insert(this);
    setVisible(false);
    //cout << "gold ctor" << endl;
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