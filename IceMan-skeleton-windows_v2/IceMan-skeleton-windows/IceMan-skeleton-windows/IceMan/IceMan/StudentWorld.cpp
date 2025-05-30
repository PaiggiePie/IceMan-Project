#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
<<<<<<< Updated upstream
	return new StudentWorld(assetDir);

=======
    return new StudentWorld(assetDir);
}
int StudentWorld::init(){
    int i = getLives();
    //cout << "Lives:" << i << endl;
    
    Iceman* I = new Iceman;
    I->setVisible(true);
    
    return GWSTATUS_CONTINUE_GAME;
    
}
>>>>>>> Stashed changes

}


int StudentWorld::move()
{
	// Update the Game Status Line 
	setDisplayText();
	// update the score/lives/level text at screen top 
	vector <Actor> Actors;
	Actors.push_back(Iceman player);
	// The term “Actors” refers to all Protesters, the player, Goodies, 
	// Boulders, Barrels of oil, Holes, Squirts, the Exit, etc. 
	// Give each Actor a chance to do something 
	for (int i = 0; i < Actors.size(); i++)// each of the actors in the game world
	{
		if (actor[i].is_alive() == true)  // is still active / alive
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

void StudentWorld::cleanUp() {

}