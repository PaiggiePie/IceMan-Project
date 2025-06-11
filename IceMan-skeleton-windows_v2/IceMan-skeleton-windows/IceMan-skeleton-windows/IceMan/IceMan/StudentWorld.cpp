#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <queue>
using namespace std;

class Actor;
StudentWorld* sw;
GameWorld* createStudentWorld(string assetDir)
{
	sw = new StudentWorld(assetDir); // create a new StudentWorld object
	return sw;
}

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir) { //contructor


}

//functions to implement

void Actor::setWorld(StudentWorld*& sp) const {
	sp = sw;
}

int StudentWorld::init() {
	// make iceman
	this->iceman = new Iceman(sw);
	//iceman->setWorld(sw);
	if (getIceman() == nullptr)
		cout << "null" << endl;
	agents.push_back(iceman); // add iceman to agents vector
	//random_device rd; // random number generator
	//mt19937 gen(rd());
	//uniform_int_distribution<> distrib(min, max);

	//ice making code
	for (int i = 0; i < 65; i++) {
		for (int j = 0; j < 60; j++) {
			if (i > 29 && i < 34 && j > 3) {
				continue;
			}
			else {
				Ice* iceObj = new Ice(sw);
				iceObj->moveTo(i, j);
				iceField.push_back(iceObj);
			}

		}
	}

	// make static objects
	int B = min(currentLevelNumber / 2 + 2, 9);
	for (int i = 0; i < B; i++) {
		BarrelsOfOil* barrel = new BarrelsOfOil(this);
		aobj.push_back(barrel);
	}
	int G = max(5 - currentLevelNumber / 2, 2);
	for (int i = 0; i < G; i++) {
		GoldNugget* nugget = new GoldNugget(sw, (std::rand() % 59), (std::rand() % 59), false);
		aobj.push_back(nugget);
	}
	/*
	Boulder* boulder = new Boulder(this);
	iceField.push_back(boulder);*/

	RegularProtester* prot = new RegularProtester(sw, 50, 60);
	agents.push_back(prot); // add protester to agents vector
	return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move() {
	//testing
		int X = iceman->getX();
		int Y = iceman->getY();
		cout << X << ", " << Y << endl;
	
	/*cout << "Score: " << getScore() << endl;
	cout << "Level: " << getLevel() << endl;

	cout << "tick: " << ticks << endl;*/

	setDisplayText();
	bool canAddP = false;
	int G = currentLevelNumber * 30 + 290; // 1 in G chance of water kit or sonar kit added
	int probabilityOfHardcore = min(90, currentLevelNumber * 10 + 30);
	currentLevelNumber = getLevel(); // get the current level number
	int ticksToWaitBetweenMoves = max(0, (3 - currentLevelNumber / 4));
	if (ticks % ticksToWaitBetweenMoves == 0) {
		canAddP = true;
	}
	if (canAddP == true) {
		//use probability of hardcore to determine if hardcore protester should be added
		// use probability of protester spawning
		// canAddP = false; // reset 
	}
	int random = (rand() % (G + 1)); // changed to (G + 1)
	if (G == random) {
		int random2 = (rand() % 2);
		if (random2 == 1) {
			WaterPool* pool = new WaterPool(this);
			aobj.push_back(pool);
		}
		else {
			SonarKit* kit = new SonarKit(this);
			aobj.push_back(kit);
		}
	}
	//if(iceman->isAlive())
	//	iceman->doSomething(); // ask iceman to do something

	// Give each Actor a chance to do something

	for (auto& agent : agents) {
		agent->doSomething();
	}
	for (auto& object : aobj) {
		object->doSomething();
	}
	for (auto& ice : iceField) {
		ice->doSomething();
	}


	//   int T = std::max(25, (200 - ((int) getLevel()))); // # of tick to wait for new protester
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

	// delete all objects
	int index = 0;
	for (auto& agent : agents) {
		if (agent->isAlive() == false) {
			cout << "agent died" << endl;
			delete agent;
			agents.erase(agents.begin() + index); // remove dead agent from vector
			index--;
		}
		index++;
	}
	index = 0;
	for (auto& object : aobj) {
		if (object->isAlive() == false) {
			cout << "obj died" << endl;
			delete object;
			aobj.erase((aobj.begin() + index));
			index--;
		}
		index++;
	}
	index = 0;
	for (auto& ice : iceField) {
		if (ice->isAlive() == false) {
			delete ice;
			iceField.erase((iceField.begin() + index));
			index--;
		}
		index++;
	}

	ticks++; // increment ticks for each move
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
	// after a player has lost a life (with more lives remaining) or has completed a level,
	// or lost all lives or game is over
	// delete all actors and ice objects 
	//delete this->iceman; // delete iceman
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

void StudentWorld::addObj(ActivatingObject* a) {
	aobj.push_back(a);
}

void StudentWorld::addActor(Actor* a) {
	iceField.push_back(a);
}

void StudentWorld::clearIce(int x, int y) {
	for (auto& iceObj : iceField) {
		if (iceObj->getID() == IID_ICE) {
			if (((iceObj->getX() == x) ||
				(iceObj->getX() == x + 1) ||
				(iceObj->getX() == x + 2) ||
				(iceObj->getX() == x + 3)) &&
				((iceObj->getY() == y) ||
					(iceObj->getY() == y + 1) ||
					(iceObj->getY() == y + 2) ||
					(iceObj->getY() == y + 3))
				&& iceObj->isVisible()) {
				iceObj->setVisible(false);
				/*if (ticks % 15 == 0)
					playSound(SOUND_DIG);*/
					//iceObj->setDead();
				cout << "clear ice" << endl;
				pair<int, int> coord = { make_pair(iceObj->getX(), iceObj->getY()) };
				coords.push_back(coord);
			}
		}
	}
}


bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const {
	if (x < 0 || x >= 64 || y < 0 || y >= 60) {
		return false; // out of bounds
	}
	for (auto& ice : iceField) {
		if (ice->getX() == x && ice->getY() == y && ice->isVisible()) {
			return false; // ice blocks and boulders
		}
	}
	return true;
}

int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius) {
	if (findNearbyPickerUpper(annoyer, radius) != nullptr) { // if there is a nearby iceman or protester that can pick things up
		Agent* nearbyAnnoyed = findNearbyPickerUpper(annoyer, radius);
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

void StudentWorld::revealAllNearbyObjects(int x, int y, int radius) {
	for (auto& actor : aobj) {
		if (isNearIceMan(actor, radius)) {
			actor->setVisible(true); // reveal the actor
		}
		for (auto& ice : iceField) {
			if (isNearIceMan(ice, radius)) {
				ice->setVisible(true); // reveal the ice
			}
		}
	}
}

// If the IceMan is within radius of a, return a pointer to the
	  // IceMan, otherwise null.
Iceman* StudentWorld::findNearbyIceMan(Actor* a, int radius) const {
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
/*void StudentWorld::annoyIceMan() {
	iceman->annoy(1); // by how much?
}*/

void StudentWorld::giveIceManSonar() {
	iceman->addSonar(); // give iceman sonar
}

void StudentWorld::giveIceManWater() {
	iceman->addWater();
}

// Is the Actor a facing toward the IceMan?
bool StudentWorld::facingTowardIceMan(Actor* a) const {
	if (a->getX() == iceman->getX()) { // same x level && right side of iceman
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
GraphObject::Direction StudentWorld::lineOfSightToIceMan(Actor* a, bool facing) const {
	if (facingTowardIceMan(a) == false && facing == true) {
		return GraphObject::Direction::none; // if actor is not facing towards iceman, return none
	}
	bool blocked = false; // if there is an object in the way

	if (a->getX() == iceman->getX()) { // same x level as iceman
		for (int i = a->getX(); i < iceman->getX(); i++) { //starting at actor, ending at iceman
			for (int h = 0; h < iceField.size(); h++) {// i = x coords  getY = y coords
				if (iceField[h]->getY() == a->getY() && iceField[h]->isVisible()) {
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
				if (iceField[t]->getX() == a->getX() && iceField[t]->isVisible()) { // if there is not an object in the path
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
	return GraphObject::Direction::none;
}

bool StudentWorld::isNearIceMan(Actor* a, int radius) const {
	if (radius > abs((a->getX() - iceman->getX())) && radius > abs((a->getY() - iceman->getY())))
		return true;
	return false;
}

bool StudentWorld::NearBoulder(int x, int y, int radius) const {
	for (int i = 0; i < iceField.size(); i++) {
		if (iceField[i] != nullptr) {
			if (iceField[i]->getID() == IID_BOULDER) { // if there is a boulder in the iceField
				if (radius > std::abs((x - iceField[i]->getX())) && radius > std::abs((y - iceField[i]->getY())))
					return true; // if boulder is in radius, return true
			}
		}
	}
	return false;
}


GraphObject::Direction StudentWorld::determineFirstMoveToExit(int x, int y) {
	return GraphObject::Direction::none; // placeholder, implement logic to determine first move to exit
}

GraphObject::Direction StudentWorld::determineFirstMoveToIceMan(int x, int y) {
	return GraphObject::Direction::none; // placeholder, implement logic to determine first move to exit

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


/*
int StudentWorld::move()
{
// Update the Game Status Line
updateDisplayText(); // update the score/lives/level text at screen top
// The term Actors refers to all Protesters, the player, Goodies,
// Boulders, Barrels of oil, Holes, Squirts, the Exit, etc.
// Give each Actor a chance to do something
for each of the actors in the game world
{
if (actor[i] is still active/alive)
{
// ask each actor to do something (e.g. move)
tellThisActorToDoSomething(actor[i]);
if (theplayerDiedDuringThisTick() == true)
return GWSTATUS_PLAYER_DIED;
if (theplayerCompletedTheCurrentLevel() == true)
{
return GWSTATUS_FINISHED_LEVEL;
}
}
}
// Remove newly-dead actors after each tick
removeDeadGameObjects(); // delete dead game objects
// return the proper result
if (theplayerDiedDuringThisTick() == true)
return GWSTATUS_PLAYER_DIED;
// If the player has collected all of the Barrels on the level, then
// return the result that the player finished the level
if (theplayerCompletedTheCurrentLevel() == true)
{
playFinishedLevelSound();
return GWSTATUS_FINISHED_LEVEL;
}
// the player hasnt completed the current level and hasnt died
// let them continue playing the current level
return GWSTATUS_CONTINUE_GAME;
}
*/