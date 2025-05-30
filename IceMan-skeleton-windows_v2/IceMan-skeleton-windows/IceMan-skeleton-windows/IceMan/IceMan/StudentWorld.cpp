#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir){ //contructor

    
}

//functions to implement

int StudentWorld::init(){
    
    //int i = getLives();
    //cout << "Lives:" << i << endl;
    
    Iceman* iceMan = new Iceman(this);
    actors.push_back(iceMan);

    //ice making code
    for (int i = 0; i < 65; i++){
        for (int j = 0; j < 60; j++){
            if(i > 29 && i < 34 && j > 3){
                continue;
            }
            else{
                Ice* iceObj = new Ice(this);
                iceObj->moveTo(i, j);
                iceField.push_back(iceObj);
            }
            
        }
    }
        
        
    return GWSTATUS_CONTINUE_GAME;
    
}

int StudentWorld::move(){
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
    
}

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


