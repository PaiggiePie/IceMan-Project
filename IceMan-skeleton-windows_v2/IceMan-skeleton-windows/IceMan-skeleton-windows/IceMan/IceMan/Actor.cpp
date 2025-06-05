#include "Actor.h"
#include "StudentWorld.h"


//******************************** Actor Methods *******************************

//contructor
Actor::Actor(StudentWorld* sp, int ID, int x, int y, Direction dir, double siz, unsigned int dep, bool visible):
GraphObject(ID, x, y, dir, siz, dep){
    
}

/*StudentWorld* Actor::getWorld(){
    return sp;
}*/

Actor::~Actor() {
    //cout << "Actor dtor" << endl;
    setVisible(false);
    m_isAlive = false; // set alive to false when actor is destroyed
    // remove from the set of graph objects
    getGraphObjects(getAnimationNumber()).erase(this);
}

bool Actor::isAlive() const{
    return m_isAlive;
}

void Actor::setDead(){
    
    m_isAlive = false;
}

bool Actor::annoy(unsigned int amt){
    return false;
}

bool Actor::huntsIceMan() const{
    return true;

}
bool Actor::canDigThroughIce() const{
    return false;
}

bool Actor::canActorsPassThroughMe() const{
    return false;
}

bool Actor::canPickThingsUp() const{
    return false;
}

bool Actor::needsToBePickedUpToFinishLevel() const{
    return false;
}

bool Actor::moveToIfPossible(int x, int y){
    return true;
}




//******************************** Agent Methods *******************************
Agent::Agent(StudentWorld* sp, int ID, int x, int y, Direction dir,
             unsigned int hitPoints) : Actor(sp, IID_PLAYER, 30, 60, right, 1.0, 0, true){
    
}

bool Agent::annoy(unsigned int amount){
    hitPoints--;
    if (hitPoints == 0){
        setDead();
        return false;
    }
    return true;
}

bool Agent::canDigThroughIce() const{
    return false;
}


bool Agent::canPickThingsUp() const{
    return true;
}

unsigned int Agent::getHitPoints() const{
    return hitPoints;
}
//******************************** IceMan Methods *******************************

Iceman::Iceman(StudentWorld* sp): Agent(sp, IID_PLAYER, 30, 60, right, 5){ //contructor
    getGraphObjects(0).insert(this);
    setVisible(true);
}


void Iceman::doSomething(){
        
    Direction d = this->getDirection();
    int key;
    if (getWorld(sp) == nullptr) {
            cerr << "Error: StudentWorld pointer is null!" << endl;
            return; // handle the error as needed
        }
    if (getWorld(sp)->getKey(key) == true) {
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
                if (getX() < 64) {
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
            case KEY_PRESS_TAB:
                //add gold into field if iceman has gold
                break;
            case KEY_PRESS_SPACE:
                //create squirt water if available
                //else nothing
                break;
            case KEY_PRESS_ESCAPE:
                setDead();
                if(isAlive()){
                    //restart level
                    //else end game
                }
                break;
            case 'Z':
                //use sonar if available
                
                break;
            case 'z':
                //use sonar if available

                break;
        }
        
    }
}

bool Iceman::annoy(unsigned int amount){
    if(getHitPoints() >= 1){
        Agent::annoy(amount);
        return true;
    }
    setDead();
    return false;
}

bool Iceman::canDigThroughIce() const{
    return true;
}

void Iceman::addGold(){
    m_gold++;
}


void Iceman::addSonar(){
    m_sonar+=2;
}

  // Pick up water.
void Iceman::addWater(){
    m_squirts+=5;
}

  // Get amount of gold
unsigned int Iceman::getGold() const{
    return m_gold;
}

  // Get amount of sonar charges
unsigned int Iceman::getSonar() const{
    return m_sonar;
}

  // Get amount of water
unsigned int Iceman::getWater() const{
    return m_squirts;
}

Iceman::~Iceman(){
    setVisible(false);
    Iceman::getGraphObjects(0).erase(this);
}



//******************************** Protester Methods *******************************
Protester::Protester(StudentWorld* sp, int x, int y, int ID,
                     unsigned int hitPoints, unsigned int score): Agent(sp, IID_PROTESTER, x, y , left, 5){
    setVisible(true);
}
void Protester::doSomething(){
    
}

bool Protester::annoy(unsigned int amount){
    return true;
}

void Protester::addGold(){
    m_gold++;
}

bool Protester::huntsIceMan() const{
    return true;
}

  // Set state to having given up protest
void Protester::setMustLeaveOilField(){
    m_state = true;
}

  // Set number of ticks until next move
void Protester::setTicksToNextMove(){
    
}


//******************************** Regular Protester Methods *******************************
RegularProtester::RegularProtester(StudentWorld* sp, int x, int y, int ID): Protester(sp, x, y, IID_PROTESTER, 5, 0){
    
}

void RegularProtester::doSomething(){}
void RegularProtester::addGold(){
    
}

//******************************** Hardcore Protester Methods *******************************
HardcoreProtester::HardcoreProtester(StudentWorld* sp, int x, int y, int ID):Protester(sp, x, y, IID_HARD_CORE_PROTESTER, 5, 0){
    
}

void HardcoreProtester::doSomething(){}
void HardcoreProtester::addGold(){
    
}



//******************************** Ice Methods *******************************

//constructor
Ice::Ice(StudentWorld* sp): Actor(sp, IID_ICE, getX(), getY(), right, 0.25, 3, true){
    //cout << "Ice ctor" << endl;
    setVisible(true);
    // add to the set of graph objects
    getGraphObjects(3).insert(this);
}

Ice::~Ice(){
    //cout << "Ice dtor" << endl;
    setVisible(false);
    Ice::getGraphObjects(3).erase(this); // remove from the set of graph objects
}

void Ice::doSomething(){
        // Ice does not perform any actions
}

//******************************** Boulder Methods *******************************
Boulder::Boulder(StudentWorld* sp): Actor(sp, IID_BOULDER, getX(), getY(), down, 1.0, 1, true){
    setVisible(true);
}
bool Boulder::canActorsPassThroughMe() const{
    return false;
}

void Boulder::doSomething(){
    
}


//******************************** Squirt Methods *******************************

Squirt::Squirt(StudentWorld* sp, int x, int y, Direction dir): Actor(sp, IID_WATER_SPURT, getX(), getY(), right, 1.0, 1, true){
    setVisible(true);
}

void Squirt::doSomething(){
    
}



//******************************** ActivatingObj Methods *******************************
ActivatingObject::ActivatingObject(StudentWorld* sp, int x, int y, int ID,
    int soundToPlay, bool activateOnPlayer, bool activateOnProtester, bool initallyActive):
Actor(sp, IID_PLAYER, 0, 0, right, 1.0, 2, true)
{}

//will only be used by barrels of oil. all other objects use Actor::needsToBePickedUpToFinishLevel()
bool ActivatingObject::needsToBePickedUpToFinishLevel() const{
    return false;
}

bool ActivatingObject::canActorsPassThroughMe() const{
    return true;
}
void ActivatingObject::setTicksToLive(){
    
    
};

//******************************** Oil Methods *******************************
BarrelsOfOil::BarrelsOfOil(StudentWorld* sp): ActivatingObject(sp, (rand() % 64), (rand() % 59), IID_BARREL, SOUND_FOUND_OIL, true, false, false){
    
}
void BarrelsOfOil::doSomething(){
    if (isAlive()){
        //if()
    }
    else{
        return;
    }
}

bool BarrelsOfOil::needsToBePickedUpToFinishLevel() const{
    return true;
}

BarrelsOfOil::~BarrelsOfOil() {
    setVisible(false);
    BarrelsOfOil::getGraphObjects(2).erase(this); // remove from the set of graph objects
}



//******************************** Gold Nuggets Methods *******************************
GoldNugget::GoldNugget(StudentWorld* sp, bool temporary): ActivatingObject(sp, (rand() % 64), (rand() % 59), IID_GOLD, SOUND_GOT_GOODIE, true, true, false){
    
}
void GoldNugget::doSomething(){
    
}

//******************************** Water Pool Methods *******************************
WaterPool::WaterPool(StudentWorld* sp): ActivatingObject(sp, (rand() % 64), (rand() % 59), IID_WATER_POOL, SOUND_GOT_GOODIE, true, false, false){
    
}
void WaterPool::doSomething(){
    
}

//******************************** SonarKit Methods *******************************
SonarKit::SonarKit(StudentWorld* sp): ActivatingObject(sp, 0, 60, IID_SONAR, SOUND_GOT_GOODIE, true, false, false){
    
}
void SonarKit::doSomething(){
    
}
