#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <utility>
#include <tuple>
#include <queue>
#include <stdlib.h>
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

//void Actor::setWorld(StudentWorld*& sp) const {
//	sp = sw;
//}

int StudentWorld::init() {
	// make iceman
	this->iceman = new Iceman(sw);
	if (getIceman() == nullptr)
		cout << "null" << endl;
	agents.push_back(iceman); // add iceman to agents vector

	currentLevelNumber = getLevel();
	//boulders code
	int B = min(currentLevelNumber / 2 + 2, 9); // everything must be 6 squares apart
	for (int i = 0; i < B; i++) {
		int x, y; // must be (0, 20) and (60, 56) inclusive
		do {
			x = rand() % 61; // random x coordinate
			y = rand() % (53 - 20 + 1) + 20; // random y coordinate
		} while (NearItem(x, y, 6) || (x > 26 && x < 34 && y > 3) || y > 60); // check if boulder is too close to another boulder
		Boulder* boulder = new Boulder(sw, x, y);
		iceField.push_back(boulder); // add boulder to aobj vector
	}

	//ice making code
	iceField.reserve(61 * 60); // reserve space for ice objects
	for (int i = 0; i < 65; i++) {
		for (int j = 0; j < 60; j++) {
			if (i > 29 && i < 34 && j > 3) {
				coords.emplace(i, j); // add tunnel coordinates to coords vector
				continue;
			}
			else {
				if (!atItem(i, j, false)) {
					Ice* iceObj = new Ice(sw, i, j);
					iceField.push_back(iceObj);
				}
			}
		}
	}

	// make static objects
	// Barrels of Oil
	int L = min(2 + currentLevelNumber, 21); // barrels of oil number
	for (int i = 0; i < L; i++) {
		int x, y; // must be at (0,0) and (60, 56) inclusive
		do {
			x = rand() % 60; // random x coordinate
			y = rand() % 55; // random y coordinate
		} while (NearItem(x, y, 6)); // check if boulder is too close to another boulder
		BarrelsOfOil* barrel = new BarrelsOfOil(sw, x, y);
		aobj.push_back(barrel);
		barrels = L;
	}

	// Gold Nuggets
	int K = max(5 - currentLevelNumber / 2, 2);
	for (int i = 0; i < K; i++) {
		int x, y; // must be at (0,0) and (60, 56) inclusive
		do {
			x = rand() % 61; // random x coordinate
			y = rand() % 57; // random y coordinate
		} while (NearItem(x, y, 6)); // check if boulder is too close to another boulder
		GoldNugget* nugget = new GoldNugget(sw, x, y, false);
		aobj.push_back(nugget);
	}
	//start with 1 protester
	RegularProtester* rp = new RegularProtester(sw);
	agents.push_back(rp); // add protester to agents vector
	return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move() {
	//testing
		/*int X = iceman->getX();
		int Y = iceman->getY();
		cout << X << ", " << Y << endl;

	/*cout << "Score: " << getScore() << endl;
	cout << "Level: " << getLevel() << endl;

	cout << "tick: " << ticks << endl;*/

	setDisplayText();
	sw = this;

	// Protester spawning code
	currentLevelNumber = getLevel(); // get the current level number
	bool canAddP = false;
	int probabilityOfHardcore = min(90, currentLevelNumber * 10 + 30);
	// new protester every T ticks
	int T = std::max(25, 200 - currentLevelNumber); // new protester
	int P = std::min(15, (int)(2 + currentLevelNumber * 1.5)); // max number of protesters on field
	if (ticks % T == 0) {
		canAddP = true;
	}
	if (canAddP == true) {
		if (P > agents.size() - 1) { // if number of protesters is less than max (-1 for iceman)
			if (probabilityOfHardcore == (rand() % probabilityOfHardcore + 1)) { // if hardcore protester
				HardcoreProtester* hp = new HardcoreProtester(sw);
				agents.push_back(hp); // add hardcore protester to agents vector
			}
			else {
				RegularProtester* rp = new RegularProtester(sw);
				agents.push_back(rp); // add protester to agents vector
			}

		}
	}

	// randomly add water pool or sonar kit
	int G = currentLevelNumber * 30 + 290; // 1 in G chance of water kit or sonar kit added

	int random = (rand() % G + 1);
	if (G == random) {
		int random2 = (rand() % 6);
		if (random2 == 1) { // 1 in 5 chance of sonar kit
			SonarKit* kit = new SonarKit(this);
			aobj.push_back(kit);
		}
		else { // 4 in 5 chance of water pool
			int x, y; // 
			do {
				x = rand() % 57; // random x coordinate
				y = rand() % 57; // random y coordinate
			} while ((atItem(x, y, true)) || atItem(x + 3, y, true) ||
				atItem(x, y + 3, true) || atItem(x + 3, y + 3, true) || NearItem(x, y, 6)); // random iceless spot
			WaterPool* pool = new WaterPool(this, x, y);
			aobj.push_back(pool);
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

	// delete objects (protesters, boulders, gold, waterpools, squirts, sonarkits, barrels, ice)
	int index = 0;
	for (auto& agent : agents) {
		if (agent->isAlive() == false) {
			//cout << "agent died" << endl;
			delete agent;
			agents.erase(agents.begin() + index); // remove dead agent from vector
			index--;
		}
		index++;
	}
	index = 0;
	for (auto& object : aobj) {
		if (object->isAlive() == false) {
			//cout << "obj died" << endl;
			delete object;
			aobj.erase((aobj.begin() + index));
			index--;
		}
		index++;
	}
	index = 0;
	/*for (auto& ice : iceField) {
		if (ice->isAlive() == false) {
			delete ice;
			iceField.erase((iceField.begin() + index));
			index--;
		}
		index++;
	}*/

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
				iceObj->setDead(); // set ice object to dead
				//coords.emplace(iceObj->getX(), iceObj->getY());
				/*if (ticks % 15 == 0)
					playSound(SOUND_DIG);*/
					//cout << "clear ice" << endl;
			}
		}
	}
}


bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const {
	// applies to protesters
	if (a->canPickThingsUp() && (x < 0 || x >= 61 || y < 0 || y > 60)) 
		return false;
	else if (x < 0 || x >= 61 || y < 0 || y > 64) { // squirts mainly
		return false; // out of bounds
	}
	if (a->canPickThingsUp()) {
		// if there is ice or boulder at x, y
		/* (x, y+3)     (x+3, y+3)
		*  
		*  (x, y)       (x+3, y)
		*/
		if (atItem(x, y + 3, true) || atItem(x + 3, y + 3, true) ||
			atItem(x, y, true) || atItem(x + 3, y, true)) {
			return false;
		}
	}
	else {
		for (auto& iceObj : iceField) {
			if (iceObj->isVisible() && iceObj->getX() == x && iceObj->getY() == y) {
				return false; // ice blocks and boulders
			}
		}
	}
	return true;
}

int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius) {
	if (findNearbyPickerUpper(annoyer, radius) != nullptr) { // if there is a nearby iceman or protester that can pick things up
		Agent* nearbyAnnoyed = findNearbyPickerUpper(annoyer, radius);
		if (nearbyAnnoyed->getID() == IID_PLAYER && nearbyAnnoyed->isAlive()) { // if iceman is in radius
			iceman->annoy(points); // annoy iceman
			iceman->setDead(); // instant death
			return 1; // return 1 for iceman
		}
		else if (nearbyAnnoyed->isAlive()) {
			nearbyAnnoyed->annoy(points); // annoy the nearby protester
			increaseScore(500); // 500 points for annoying protesters
			nearbyAnnoyed->setDead();
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
			if (isNearIceMan(ice, radius) && ice->isVisible() == true) {
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
		if (radius > abs((a->getX() - agents[i]->getX())) && radius > abs(a->getY() - agents[i]->getY()))
			return agents[i];
	}
	//if (radius > abs((a->getX() - iceman->getX())) && radius > abs(a->getY() - iceman->getY())) {
	//	return iceman; // if iceman is in radius, return iceman
	//}
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
	if (a->getDirection() == GraphObject::Direction::left && a->getX() > iceman->getX())
		return true; // actor is facing left towards iceman
	else if (a->getDirection() == GraphObject::Direction::right && a->getX() < iceman->getX()) {
		return true; // actor is facing right away from iceman
	}

	if (a->getDirection() == GraphObject::Direction::up && a->getY() < iceman->getY())
		return true; // actor is facing up towards iceman
	else if (a->getDirection() == GraphObject::Direction::down && a->getY() > iceman->getY()) {
		return true; // actor is facing down away from iceman
	}
	return false;
}


// If the Actor a has a clear line of sight to the IceMan, return (not blocked by ice/boulders
	  // the direction to the IceMan, otherwise GraphObject::none.
GraphObject::Direction StudentWorld::lineOfSightToIceMan(Actor* a, bool facing) const {
	if (facingTowardIceMan(a) == false && facing == true) {
		return GraphObject::Direction::none; // if actor is not facing towards iceman, return none
	}
	int actorX = a->getX();
	int actorY = a->getY();
	int icemanX = iceman->getX();
	int icemanY = iceman->getY();
	// Vertical line-of-sight (same X)
	if (a->getX() == icemanX) {
		for (auto& ice : iceField) {
			if (!ice->isVisible())
				continue;

			if (ice->getX() >= actorX && ice->getX() < actorX + 4) {
				if ((actorY < icemanY && ice->getY() > actorY && ice->getY() < icemanY) ||
					(actorY > icemanY && ice->getY() > icemanY && ice->getY() < actorY)) {
					return GraphObject::Direction::none;
				}
			}
		}

		return (actorY < icemanY) ? GraphObject::Direction::up : GraphObject::Direction::down;
	}
	// Horizontal line-of-sight (same Y)
	else if (actorY == icemanY) {
		for (auto& ice : iceField) {
			if (!ice->isVisible())
				continue;

			if (ice->getY() >= actorY && ice->getY() < actorY + 4) {
				if ((actorX < icemanX && ice->getX() > actorX && ice->getX() < icemanX) ||
					(actorX > icemanX && ice->getX() > icemanX && ice->getX() < actorX)) {
					return GraphObject::Direction::none;
				}
			}
		}

		return (actorX < icemanX) ? GraphObject::Direction::right : GraphObject::Direction::left;
	}

	return GraphObject::Direction::none;
}


// if this actor is within a radius of iceman
bool StudentWorld::isNearIceMan(Actor* a, int radius) const {
	if (!iceman || !a)
		return false;
	if (radius > abs(a->getX() - iceman->getX()) && radius > abs(a->getY() - iceman->getY()))
		return true;
	return false;
}

// if within radius of boulder
bool StudentWorld::NearBoulder(int x, int y, int radius) const {
	for (auto& ice : iceField) {
		if (ice != nullptr && ice->getID() == IID_BOULDER) { // if there is a boulder in the iceField
			if (radius > std::abs(x - ice->getX()) && radius > std::abs(y - ice->getY()))
				return true; // if boulder is in radius, return true
		}
	}
	return false;
}

//within a certain radius of boulder, gold, water pool, sonar kit, and/or oil barrels
bool StudentWorld::NearItem(int x, int y, int radius) const {
	if (NearBoulder(x, y, radius)) { // if there is a boulder within radius
		return true;
	}

	for (auto& aobj : aobj) {
		if (aobj != nullptr) {
			if (radius > std::abs(x - aobj->getX()) && radius > std::abs(y - aobj->getY()))
				return true; // if boulder is in radius, return true
		}
	}
	return false;
}

// checks the iceField for boulder and if ice == true, for ice
bool StudentWorld::atItem(int x, int y, bool ice) const { // if you need to check ice, ice == true, boulders = false

	if (ice == true) {
		for (auto& iceObj : iceField) {
			if (iceObj->isVisible() && (iceObj->getX() == x && iceObj->getY() == y)) {
				return true;
			}
		}
	}
	for (auto& iceObj : iceField) { // only boulder
		if (iceObj->isVisible() && iceObj != nullptr &&
					iceObj->getID() == IID_BOULDER &&
					((iceObj->getX() == x || iceObj->getX() + 1 == x 
					|| iceObj->getX() + 2 == x || iceObj->getX() + 3 == x) && 
					(iceObj->getY() == y || iceObj->getY() + 1 == y || 
					iceObj->getY() + 2 == y || iceObj->getY() + 3 == y))) {
			return true;
		}
	}
	return false;
}

struct TreeNode {
	GraphObject::Direction fromParent; // The direction taken to get to this node from the parent
	TreeNode* parent;
	int x, y;
	//int steps;
	TreeNode(int x, int y, GraphObject::Direction dir, TreeNode* parent = nullptr)
		: x(x), y(y), fromParent(dir), parent(parent) {
	}
};


// returns direction of shortest path to exit
GraphObject::Direction StudentWorld::determineFirstMoveToExit(int x, int y) {
	const int goalX = 60;
	const int goalY = 60;

	std::queue<TreeNode*> q;
	vector<TreeNode*> allNodes;
	bool visited[64][64] = { false };

	// Root node: the protester's current location
	TreeNode* root = new TreeNode(x, y, GraphObject::none);
	//root to track exit
	TreeNode* exitNode = nullptr;
	visited[x][y] = true;
	q.push(root);
	allNodes.push_back(root);

	// stores 4 directions
	vector<tuple<int, int, GraphObject::Direction>> directions = { {0, 1, GraphObject::up   },
																	{0, -1, GraphObject::down},
																	{-1, 0, GraphObject::left},
																	{1, 0, GraphObject::right} };
	while (!q.empty()) {
		//root to track current/nodes in the queue
		TreeNode* curr = q.front();
		q.pop();

		int cX = curr->x;
		int cY = curr->y;

		//at exit
		if (cX == goalX && cY == goalY) {
			exitNode = curr;
			break;
		}

		//check if valid for each direction for current place
		for (auto& [dX, dY, dir] : directions) {
			int nextX = cX + dX;
			int nextY = cY + dY;

			if (canActorMoveTo(iceman, nextX, nextY) && !visited[nextX][nextY]) {
				//if valid put on queue
				visited[nextX][nextY] = true;
				TreeNode* child = new TreeNode(nextX, nextY, dir, curr);
				q.push(child);
				allNodes.push_back(child);
			}
		}
	}

	if (exitNode != nullptr) {
		TreeNode* path = exitNode;
		GraphObject::Direction dirToMove = GraphObject::none;
		//for length of path, go through path
		while (path->parent != nullptr && path->parent != root) {
			path = path->parent;
		}
		if (path->parent == root)
			dirToMove = path->fromParent;
		return dirToMove;
	}
	for (TreeNode* node : allNodes) {
		delete node;
	}
	return GraphObject::none;
}



// Determine the direction of the first move a protester
// makes to approach the IceMan.
GraphObject::Direction StudentWorld::determineFirstMoveToIceMan(int x, int y) {
	const int goalX = iceman->getX();
	const int goalY = iceman->getY();

	std::queue<TreeNode*> q;
	vector<TreeNode*> allNodes;
	bool visited[64][64] = { false };

	// Root node: the protester's current location
	TreeNode* root = new TreeNode(x, y, GraphObject::none);
	//root to track exit
	TreeNode* exitNode = nullptr;
	visited[x][y] = true;
	q.push(root);
	allNodes.push_back(root);

	// stores 4 directions
	vector<tuple<int, int, GraphObject::Direction>> directions = { {0, 1, GraphObject::up   },
																	{0, -1, GraphObject::down},
																	{-1, 0, GraphObject::left},
																	{1, 0, GraphObject::right} };
	while (!q.empty()) {
		//root to track current/nodes in the queue
		TreeNode* curr = q.front();
		q.pop();

		int cX = curr->x;
		int cY = curr->y;

		//at exit
		if (cX == goalX && cY == goalY) {
			exitNode = curr;
			break;
		}

		//check if valid for each direction for current place
		for (auto& [dX, dY, dir] : directions) {
			int nextX = cX + dX;
			int nextY = cY + dY;

			if (canActorMoveTo(iceman, nextX, nextY) && !visited[nextX][nextY]) {
				//if valid put on queue
				visited[nextX][nextY] = true;
				TreeNode* child = new TreeNode(nextX, nextY, dir, curr);
				q.push(child);
				allNodes.push_back(child);
			}
		}
	}

	if (exitNode != nullptr) {
		TreeNode* path = exitNode;
		GraphObject::Direction dirToMove = GraphObject::none;
		//for length of path, go through path
		while (path->parent != nullptr && path->parent != root) {
			path = path->parent;
		}
		if (path->parent == root)
			dirToMove = path->fromParent;
		return dirToMove;
	}
	for (TreeNode* node : allNodes) {
		delete node;
	}
	return GraphObject::none;
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
