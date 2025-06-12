#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <utility>
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

void Actor::setWorld(StudentWorld*& sp) {
    sp = sw;
}

int StudentWorld::init() {
    // make iceman
    this->iceman = new Iceman(sw);
    if (getIceman() == nullptr)
        cout << "null" << endl;
    agents.push_back(iceman); // add iceman to agents vector
    
    //boulders code
    int B = min(currentLevelNumber / 2 + 2, 9); // everything must be 6 squares apart
    for (int i = 0; i < B; i++) {
        int x, y; // must be (0, 20) and (60, 56) inclusive
        do {
            x = rand() % 61; // random x coordinate
            y = rand() % 34 + 20 ; // random y coordinate
        } while (NearItem(x, y, 6) || (x >= 25
                                       
                                       && x <= 34 && y > 3)); // check if boulder is too close to another boulder
        Boulder* boulder = new Boulder(sw, x, y);
        iceField.push_back(boulder); // add boulder to aobj vector
    }

    //ice making code
    for (int i = 0; i < 65; i++) {
        for (int j = 0; j < 60; j++) {
            if (i > 29 && i < 34 && j > 3) {
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
    int L = min(2 + currentLevelNumber, 21); // barrels of oil number
    for (int i = 1; i < L; i++) {
        int x, y; // must be at (0,0) and (60, 56) inclusive
        do {
            x = rand() % 61; // random x coordinate
            y = rand() % 57; // random y coordinate
        } while (NearItem(x, y, 6) || (x >= 25 && x <= 34 && y > 3)); // check if boulder is too close to another boulder
        BarrelsOfOil* barrel = new BarrelsOfOil(this, x, y);
        aobj.push_back(barrel);
    }
    int K = max(5 - currentLevelNumber / 2, 2);
    for (int i = 0; i < K; i++) {
        int x, y; // must be at (0,0) and (60, 56) inclusive
        do {
            x = rand() % 61; // random x coordinate
            y = rand() % 57; // random y coordinate
        } while (NearItem(x, y, 6) || (x >= 25 && x <= 34 && y > 3)); // check if boulder is too close to another boulder
    
        GoldNugget* nugget = new GoldNugget(sw, x, y, false);
        aobj.push_back(nugget);
    }

    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move() {

    setDisplayText();

    currentLevelNumber = getLevel(); // get the current level number
    bool canAddP = false;
    
    int probabilityOfHardcore = min(90, currentLevelNumber * 10 + 30);
    // new protester every T ticks
    int T = max(25, 200 - currentLevelNumber); // new protester
    int P = min(15, (int)(2 + currentLevelNumber * 1.5)); // max number of protesters on field
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

    int random = (rand() % (G + 1));
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
            } while ((atItem(x, y, true)) || atItem(x + 3, y, true)
                || atItem(x, y + 3, true) || atItem(x + 3, y + 3, true)
                || NearItem(x, y, 6)); // check if boulder is too close to another boulder
            WaterPool* pool = new WaterPool(this, x, y); // random iceless spot
            aobj.push_back(pool);
        }
    }

    //if(iceman->isAlive())
    //    iceman->doSomething(); // ask iceman to do something

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

    // delete objects (protesters, boulders, gold, waterpools, squirts, sonarkits, barrels)

    for (unsigned int i = 0; i < agents.size();) {
        if (agents[i]->isAlive() == false) {
            //cout << "obj died" << endl;
            delete agents[i];
            agents[i]= nullptr;
            agents.erase(agents.begin() + i);
        }else {
            ++i;
        }
    }
    for (unsigned int i = 0; i < aobj.size();) {
        if (aobj[i]->isAlive() == false) {
            //cout << "obj died" << endl;
            delete aobj[i];
            aobj[i]= nullptr;
            aobj.erase(aobj.begin() + i);
        }else {
            ++i;
        }
    }

    for (unsigned int i = 0; i < iceField.size();) {
        if (iceField[i]->isAlive() == false) {
            //cout << "obj died" << endl;
            delete iceField[i];
            iceField[i]= nullptr;
            iceField.erase(iceField.begin() + i);
        }else {
            ++i;
        }
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
    for (auto it = iceField.begin(); it != iceField.end(); ++it) {
        auto& iceObj = *it;
        
        if (iceObj->getID() != IID_ICE || !iceObj->isVisible())
            continue;

        int ix = iceObj->getX();
        int iy = iceObj->getY();

        if (ix >= x && ix < x + 4 && iy >= y && iy < y + 4) {
            iceObj->setVisible(false);
            // if (ticks % 15 == 0)
            //     playSound(SOUND_DIG);
        }
    }
}



bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const {
    if (x < 0 || x >= 60 || y < 0 || y >= 64) {
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
        if (nearbyAnnoyed!=nullptr){
            if (nearbyAnnoyed->getID() == IID_PLAYER) { // if iceman is in radius
                iceman->annoy(points); // annoy iceman
                iceman->setDead(); // instant death
                return 1; // return 1 for iceman
            }
            else {
                nearbyAnnoyed->annoy(points); // annoy the nearby protester
                increaseScore(500); // 500 points for annoying protesters
                return 2; // return 2 for protester
            }
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
        if (radius > abs((a->getX() - agents[i]->getX())) && radius > abs((a->getY() - agents[i]->getY())) && agents[i]->canPickThingsUp() == true)
            return agents[i];
    }
    if (iceman && iceman->canPickThingsUp()) {
        if (radius > abs((a->getX() - iceman->getX())) && radius > abs((a->getY() - iceman->getY())) && iceman->canPickThingsUp() == true) {
            return iceman; // if iceman is in radius, return iceman
        }
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
        if (a->getDirection() == GraphObject::Direction::down && a->getX() > iceman->getX())
            return true; // actor is facing down above iceman
        if (a->getDirection() == GraphObject::Direction::up && a->getX() < iceman->getX()) {
            return true; // actor is facing up, below of iceman
        }
    }
    if (a->getY() == iceman->getY()) { // same y level && above iceman
        if (a->getDirection() == GraphObject::Direction::left && a->getY() < iceman->getY())
            return true; // actor is facing left, right of iceman
        if (a->getDirection() == GraphObject::Direction::right && a->getY() > iceman->getY()) {
            return true; // actor is facing right, left of iceman
        }
    }
    return false;
}


// If the Actor a has a clear line of sight to the IceMan, return (not blocked by ice/boulders
      // the direction to the IceMan, otherwise GraphObject::none.
GraphObject::Direction StudentWorld::lineOfSightToIceMan(Actor* a, bool facing) const {
    if (facingTowardIceMan(a) == false && facing) {
        return GraphObject::Direction::none; // if actor is not facing towards iceman, return none
    }
    bool blocked = false; // if there is an object in the way

        
    // Vertical line-of-sight (same X)
    if (a->getX() == iceman->getX()) {
            for (auto ice : iceField) {
                if (!ice->isVisible())
                    continue;

                if (ice->getX() >= a->getX() && ice->getX() < a->getX() + 4) {
                    if ((a->getY() < iceman->getY() && ice->getY() > a->getY() && ice->getY() < iceman->getY()) ||
                        (a->getY() > iceman->getY() && ice->getY() > iceman->getY() && ice->getY() < a->getY())) {
                        blocked = true;
                        break;
                    }
                }
            }
            if (blocked)
                return GraphObject::Direction::none;

            return (a->getY() < iceman->getY()) ? GraphObject::Direction::up : GraphObject::Direction::down;
        }
        // Horizontal line-of-sight (same Y)
        else if (a->getY() == iceman->getY()) {
            for (auto ice : iceField) {
                if (!ice->isVisible())
                    continue;

                if (ice->getY() >= a->getY() && ice->getY() < a->getY() + 4) {
                    if ((a->getX() < iceman->getX() && ice->getX() > a->getX() && ice->getX() < iceman->getX()) ||
                        (a->getX() > iceman->getX() && ice->getX() > iceman->getX() && ice->getX() < a->getX())) {
                        blocked = true;
                        break;
                    }
                }
            }
            if (blocked)
                return GraphObject::Direction::none;

            return (a->getX() < iceman->getX()) ? GraphObject::Direction::right : GraphObject::Direction::left;
        }

    return GraphObject::Direction::none;
}

bool StudentWorld::isNearIceMan(Actor* a, int radius) const {
    if (!iceman || !a)
        return false;
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

bool StudentWorld::NearItem(int x, int y, int radius) const {
    for (int i = 0; i < iceField.size(); i++) {
        if (iceField[i] != nullptr) {
            if (iceField[i]->getID() == IID_BOULDER) {
                if (radius > std::abs((x - iceField[i]->getX())) && radius > std::abs((y - iceField[i]->getY())))
                    return true; // if boulder is in radius, return true
            }
        }
    }
    for (int i = 0; i < aobj.size(); i++) {
        if (aobj[i] != nullptr) {
            if (radius > std::abs((x - aobj[i]->getX())) && radius > std::abs((y - aobj[i]->getY())))
                return true; // if boulder is in radius, return true
        }
    }
    return false;
}

bool StudentWorld::atItem(int x, int y, bool ice) const { // if you need to check ice, ice == true, boulders = false
    //bool onit = false;
    for (int i = 0; i < iceField.size(); i++) {
        if (iceField[i]->getX() >= x - 3 && iceField[i]->getX() <= x && iceField[i]->getY() >= y - 3 && iceField[i]->getY() <= y) {
                    if (iceField[i]->getID() == IID_ICE && !ice)
                        continue; // skip ice if not checking for it
                    return true; // object found in the region
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
        for (auto [dX, dY, dir] : directions) {
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



// Determine the direction of the first move a hardcore protester
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
        for (auto [dX, dY, dir] : directions) {
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
