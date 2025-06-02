#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

StudentWorld* sw;
int ticks = 0; // global variable to keep track of ticks
GameWorld* createStudentWorld(string assetDir)
{
	sw = new StudentWorld(assetDir); // create a new StudentWorld object
    return sw;
}

StudentWorld::StudentWorld(std::string assetDir)
    : GameWorld(assetDir) { //contructor


}

//functions to implement

StudentWorld* Actor::getWorld(StudentWorld*& sp) const {
	// This function returns the StudentWorld pointer associated with this Actor
	return sp = sw;
}

int StudentWorld::init() {
	StudentWorld* sp = nullptr;
    // make iceman
    Iceman* iceMan = new Iceman(this);
    actors.push_back(iceMan);

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

	for (auto& iceObj : iceField) {
        iceObj->setVisible(true);
	}

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
	//ticks++; // increment ticks for each move
	// Give each Actor a chance to do something

	for (auto& actor : actors) {
		if (actor->is_Alive()) {
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
		return GWSTATUS_PLAYER_DIED; // if player died
	}
	//if () {
		//playFinishedLevelSound();
		//return GWSTATUS_FINISHED_LEVEL;
	//}
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


