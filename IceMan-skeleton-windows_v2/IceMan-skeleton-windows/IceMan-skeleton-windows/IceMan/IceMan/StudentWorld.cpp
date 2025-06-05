#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
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

StudentWorld* Actor::setWorld(StudentWorld*& sp) const {
	sp = sw;
}

int StudentWorld::init() {
	StudentWorld* sp = nullptr;
	// make iceman
	Iceman* iceman = new Iceman(sw);
	agents.push_back(iceman);

	//ice making code
	for (int i = 0; i < 65; i++) {
		for (int j = 0; j < 60; j++) {
			if (i > 29 && i < 34 && j > 3) {
				continue;
			}
			else {
				Ice* iceObj = new Ice(this);
				iceObj->moveTo(i, j);
				iceField.push_back(iceObj);
			}

		}
	}

	/*for (auto& iceObj : iceField) {
		iceObj->setVisible(true);
	}*/

	// make static objects
	/*BarrelsOfOil* barrel = new BarrelsOfOil(this);
	GoldNuggets* nugget = new GoldNuggets(this);
	Boulders* boulder = new Boulders(this);
	actors.push_back(barrel);
	actors.push_back(nuggets);
	actors.push_back(boulders);*/

	return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move() {
	//updateDisplayText();
	ticks++; // increment ticks for each move
	bool canAddP = false;
	int G = currentLevelNumber * 30 + 290; // 1 in G chance of water kit or sonar kit added
	int probabilityOfHardcore = min(90, currentLevelNumber * 10 + 30);
	currentLevelNumber = getLevel(); // get the current level number
	int ticksToWaitBetweenMoves = max(0, (3 - currentLevelNumber / 4));
	if (ticks % ticksToWaitBetweenMoves == 0) {
		canAddP == true;
	}
	if (canAddP == true) {
		//use probability of hardcore to determine if hardcore protester should be added
		// use probability of protester spawning
	}
	// Give each Actor a chance to do something
	for (auto& agent : agents) {
		if (agent->isAlive()) {
			agent->doSomething();
			//agent->animate(); // animate the agent
		}
		else {
			delete agent; // delete dead agents
		}
	}
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
		playSound(SOUND_PLAYER_GIVE_UP);
		return GWSTATUS_PLAYER_DIED; // if player died
	}
	
	if (getBarrelsRemaining() == 0) {
		playSound(SOUND_FINISHED_LEVEL);
		 currentLevelNumber++;
		return GWSTATUS_FINISHED_LEVEL;
	}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
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

void StudentWorld::addActor(Actor* a) {

}

void StudentWorld::clearIce(int x, int y) {

}

bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const {

}

int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius) {

}

void StudentWorld::revealAllNearbyObjects(int x, int y, int radius) {

}

Actor* StudentWorld::findNearbyIceMan(Actor* a, int radius) const {

}

Actor* StudentWorld::findNearbyPickerUpper(Actor* a, int radius) const {

}

void StudentWorld::annoyIceMan() {

}

void StudentWorld::giveIceManSonar() {

}

void StudentWorld::giveIceManWater() {

}

bool StudentWorld::facingTowardIceMan(Actor* a) const {

}

GraphObject::Direction StudentWorld::lineOfSightToIceMan(Actor* a) const {
	bool block = false; 
	if (a->getX() == agents[0]->getX()) {
		for (int i = a->getX(); i < agents[0]->getX(); i++) {
			int each = 0;
			for (auto& obj : this->aobj<ActivatingObject*>) {// i = x coords  getY = y coords
				if (obj[each]->getY == a->getY()) {
					if (obj[each]->getX() == i) {
						block = true;
					}


				}
				each++;
			}
		}
		if (block == false) { // if there is not an object in the path
			if (a->getX() > agents[0]->getX()) {
				return GraphObject::Direction::left;
			}
			else if (a->getX() < agents[0]->getX()) {
				return GraphObject::Direction::right;
			}
		}
	}
	else if (a->getY() == agents[0]->getY()) {
		for (int i = a->getY(); i < agents[0]->getY(); i++) { //loop through each coord between iceman and actor
			int each = 0;
			bool block = false;
			for (auto& obj : this->aobj<ActivatingObject*>) {// getX = x coords  i = y coords
				if (obj[each]->getX == a->getX()) { // if there is not an object in the path
					if (obj[each]->getX() == i)
						block = true;
					if (block == false) { // if there is not an object in the path
						if (a->getX() > agents[0]->getX()) {
							return GraphObject::Direction::left;
						}
						else if (a->getX() < agents[0]->getX()) {
							return GraphObject::Direction::right;
						}
					}
					each++;

				}
			}
		}
	}
	return GraphObject::Direction::none;
}

bool StudentWorld::isNearIceMan(Actor* a, int radius) const {
	if (radius > (a->getX() - agents[0]->getX()) && radius > (a->getY() - agents[0]->getY()))
		return true;
	return false;
}

GraphObject::Direction determineFirstMoveToExit(int x, int y) {

}
GraphObject::Direction determineFirstMoveToIceMan(int x, int y) {

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
// The term “Actors” refers to all Protesters, the player, Goodies,
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
// the player hasn’t completed the current level and hasn’t died
// let them continue playing the current level
return GWSTATUS_CONTINUE_GAME;
}
*/


