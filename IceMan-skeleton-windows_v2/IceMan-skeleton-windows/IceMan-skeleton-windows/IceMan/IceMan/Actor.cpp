#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "GameController.h"


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
//******************************** Actor Methods *******************************

//contructor
Actor::Actor(StudentWorld* sp, int ID, int x, int y, Direction dir, double siz, unsigned int dep) :
	GraphObject(ID, x, y, dir, siz, dep) {
	setVisible(true);
}
void Actor::setDead() {
	m_isAlive = false; // set alive to false when actor is dead
}
bool Actor::isAlive() const {
	return m_isAlive;
}

Actor::~Actor() {
	cout << "Actor dtor" << endl;
	setVisible(false);
	m_isAlive = false; // set alive to false when actor is destroyed
	// remove from the set of graph objects
	getGraphObjects(getAnimationNumber()).erase(this);
}


//******************************** IceMan Methods *******************************

Iceman::Iceman(StudentWorld* sp) : Agent(sp, IID_PLAYER, 30, 60, right, 1.0, 0) { //contructor
	cout << "Iceman ctor" << endl;
	getGraphObjects(0).insert(this);
	setVisible(true);

}



void Iceman::doSomething() {
	cout << "iceman : doSomething()" << endl;
	Direction d = getDirection();
	int key;
	if (getWorld() == nullptr) {
		cerr << "Error: StudentWorld pointer is null!" << endl;
		return; // handle the error as needed
	}
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
			if (getX() < 60) {
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
			//case KEY_PRESS_SPACE: //squirt
			//	//squirt code here
			//	break;
			//case KEY_PRESS_TAB: //sonar
			//	//sonar code here
			//	break;
			//case KEY_PRESS_ENTER: //nugget
			//	//nugget code here
			//	break;
			//case KEY_PRESS_ESCAPE: //quit game
			//	getWorld()->decLives();
			//	m_isAlive = false;
			//	return;
			//case "z": //squirt
			//	cout << "squirt" << endl;
				//	//squirt code here
				//	break;
			// case "Z": //sonar
			//	//squirt code here
				//	break;
			//default:
			//	break;
		}
	}

}

Iceman::~Iceman() {
	setVisible(false);
	Iceman::getGraphObjects(0).erase(this);
}

bool Iceman::annoy(unsigned int amount)



//******************************** Ice Methods *******************************

//constructor
Ice::Ice(StudentWorld* sp) : Actor(IID_ICE, 0, 0, right, 0.25, 3) {
	//setVisible(true);
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


//******************************** Regular Protester Methods *******************************
RegularProtester::RegularProtester(StudentWorld* sp) : Protester(sp) {
	setVisible(true);
}
void RegularProtester::doSomething()
{
	// copy all to Hardcore protester after finished
	vector<Actor*> actors = getWorld()->getActors(); // get the actors in the world

	if (this->getY() == actors[1]->getY()) // if Iceman is on same y axis
	{
		// if in line of sight
		// shout at the Iceman to annoy him
		cout << "Shouting at Iceman!" << endl;
		return;
	}
	else if (this->getX() == actors[1]->getX()) // if Iceman is next to protester y
	{
		// shout at the Iceman to annoy him
		cout << "Shouting at Iceman!" << endl;
		return;
	}
	Direction d = getDirection(); // get the current direction of the protester
	for (int i = 0; i < actors.size(); i++) {
		if (this->getX() == actors[i]->getX() - 1 && d == left) // if object is to the left of the protester
		{
			this->setDirection(up);
			if (this->getY() == actors[i]->getY() - 1) {
				setDirection(down);
				moveTo(getX(), getY() + 1);
			}
			else {
				moveTo(getX(), getY() - 1);
				return;
			}
		}
		if (this->getX() == actors[i]->getX() + 1 && d == right) { // object to right
			setDirection(down);
			if (this->getY() == actors[i]->getY() + 1) {
				setDirection(up);
				moveTo(getX(), getY() - 1);
			}
			else {
				moveTo(getX(), getY() + 1);
				return;
			}
		}
		if (this->getY() == actors[i]->getY() - 1 && d == up) { // object above protester
			setDirection(right);
			if (this->getX() == actors[i]->getX() + 1) {
				setDirection(left);
				moveTo(getX() - 1, getY());
				return;
			}
			else {
				moveTo(getX() + 1, getY());
				return;
			}
		}

		else if (this->getY() == actors[i]->getY() + 1 && d == down) // if object is below protester
		{
			this->setDirection(left);
			if (this->getX() == actors[i]->getX() - 1) {
				setDirection(right);
				moveTo(getX() + 1, getY());
			}
			else {
				moveTo(getX(), getY() + 1);
				return;
			}
		}
	}


	/*If I am facing the Iceman and he is next to me, then
		Shout at the Iceman(to annoy him)
	Else if, the Iceman is visible via direct line of sight
		then, Switch direction to face the Iceman
		Move one square in this direction
	Else if, I’m about to run into an obstacle, then
		Pick a new direction
		Move one square in this direction
	Else
		Move one square in my current direction
}*/

};
//******************************** Hardcore Protester Methods *******************************
HardcoreProtester::HardcoreProtester(StudentWorld* sp) : Protester(sp) {
	setVisible(true);
}
void HardcoreProtester::doSomething()
{
	/*If I am facing the Iceman and he is next to me, then
		Shout at the Iceman(to annoy him)
	Else if, the Iceman is visible via direct line of sight
		then, Switch direction to face the Iceman
		Move one square in this direction
	Else if, I’m about to run into an obstacle, then
		Pick a new direction
		Move one square in this direction
	Else
		Move one square in my current direction
}*/

};
//******************************** B.O.O. Methods *******************************
BarrelsOfOil::BarrelsOfOil(StudentWorld* sp) : StaticObject(IID_BARREL, 0, 0, right, 1.0, 2) {
}
void BarrelsOfOil::doSomething() {
	vector<Actor*> actors = getWorld()->getActors(); // get the actors in the world
	
	if (this->getX() == actors[1]->getX() + 1 || this->getX() == actors[1]->getX() - 1
		|| this->getY() == actors[1]->getY() + 1 || this->getY() == actors[1]->getY() - 1) {
		getWorld()->increaseScore(1000); // increase player's score by 1000 points
		setVisible(false); // set the barrel to be invisible
	}
}
BarrelsOfOil::~BarrelsOfOil() {
	setVisible(false);
	BarrelsOfOil::getGraphObjects(2).erase(this); // remove from the set of graph objects
}
//******************************** G Boulder Methods *******************************
Boulders::Boulders(StudentWorld* sp) : DynamicObject(IID_BOULDER, 0, 0, right, 1.0, 1) {
}

void Boulders::doSomething() {
	// If the player is next to the boulder, then
	// decrease the player's health by 10 points
	// and set the boulder to be invisible.
}
//******************************** Gold Nuggets Methods *******************************
GoldNuggets::GoldNuggets(StudentWorld* sp) : Actor(IID_GOLD, 0, 0, right, 1.0, 2) {
}
void GoldNuggets::doSomething() {
	// If the player is next to the gold nugget, then
	// increase the player's gold count by 1
	// and set the gold nugget to be invisible.
}
//******************************** Water Refills Methods *******************************
WaterRefills::WaterRefills(StudentWorld* sp) : Actor(IID_WATER_POOL, 0, 0, right, 1.0, 2) {
}

void WaterRefills::doSomething() {
	// If the player is next to the water refill, then
	// increase the player's squirts by 5
	// and set the water refill to be invisible.
}

//************************************** Sonar Methods ************************************
Sonar::Sonar(StudentWorld* sp) : Actor(IID_SONAR, 0, 0, right, 1.0, 2) {
}

void Sonar::doSomething() {
	// If the player is next to the sonar, then
	// increase the player's sonar charge by 1
	// and set the sonar to be invisible.
}