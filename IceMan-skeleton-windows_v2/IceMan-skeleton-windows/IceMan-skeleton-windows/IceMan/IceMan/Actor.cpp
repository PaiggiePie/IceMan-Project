#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "GameController.h"


const int KEY_PRESS_Z = 'z'; //squirt key
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
//******************************** Actor Methods *******************************

//contructor
Actor::Actor(int ID, int x, int y, Direction dir, double siz, unsigned int dep) :
    GraphObject(ID, x, y, dir, siz, dep) {
    setVisible(true);
}

bool Actor::is_Alive() {
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

Iceman::Iceman(StudentWorld* sp) : Actor(IID_PLAYER, 30, 60, right, 1.0, 0) { //contructor
    cout << "Iceman ctor" << endl;

    setVisible(true);
}



void Iceman::doSomething() {
	cout << "iceman : doSomething()" << endl;

	Direction d = getDirection();
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
		//case KEY_PRESS_Z: //squirt
		//	cout << "squirt" << endl;
			//	//squirt code here
			//	break;
		//default:
		//	break;
		}
	}

}

Iceman::~Iceman() {
	setVisible(false);
	Ice::getGraphObjects(0).erase(this);
}


//******************************** Ice Methods *******************************

//constructor
Ice::Ice(StudentWorld* sp) : Actor(IID_ICE, getX(), getY(), right, 0.25, 3) {
    setVisible(false);
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
BarrelsOfOil::BarrelsOfOil(StudentWorld* sp) : Actor(IID_BARREL, 0, 0, right, 1.0, 2) {
}
void BarrelsOfOil::doSomething() {
	// If the player is next to the barrel, then
	// increase the player's score by 1000 points
	// and set the barrel to be invisible.
}
BarrelsOfOil::~BarrelsOfOil() {
	setVisible(false);
	BarrelsOfOil::getGraphObjects(2).erase(this); // remove from the set of graph objects
}
//******************************** G Boulder Methods *******************************
Boulders::Boulders(StudentWorld* sp) : Actor(IID_BOULDER, 0, 0, right, 1.0, 1) {
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
