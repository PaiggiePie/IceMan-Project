#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Actor : public GraphObject {
	Actor(int imageID, int startX, int startY,
		DIRECTION startDirection, float size,
		unsigned int depth);
	void setVisible(bool shouldIDisplay); 
	void getX() const;
	void moveTo(int x, int y);
	void doSomething();

protected: 
	size_t current_level_number; // starts at 1
	// int?
};

class Iceman : public Actor {
	// bonked by boulders

	Iceman(int imageID, int startX, int startY,
		DIRECTION startDirection, float size,
		unsigned int depth) : Actor(IID_PLAYER, 30, 60, "right", 1.0, 0);

	int getX() const;
	int getY() const; // uses GraphObject x, y member variables
	void moveTo(int x, int y);
	DIRECTION getDirection() const;
	void setDirection(DIRECTION d);

private:
	int water_count = 5;
	int hit_points = 10;
	int sonar_charge = 1;
	int gold_nuggets = 0;

};

class Protester : public Actor {
	// (60, 60)
	int probabilityOfHardcore = std::min(90, current_level_number * 10 + 30);
	int T = std::max(25, 200 – current_level_number); // # of tick to wait for new protester
	int P = std::min(15, 2 + current_level_number * 1.5); // # of Protesters that should be on field
};

class RegularProtesters : public Protester { 
	// annoyed by squirts of water
	// bonked by boulders

	void moveTo(int x, int y);
	// moveTo(getX()+1, y); 
	// move one square to the right
};

class HardcoreProtesters : public Protester {
	// annoyed by squirts of water
	// bonked by boulders
	

};

class StaticObject : public Actor {
	
};

class SquirtsOfWater : public DynamicObject {

};

class BarrelsOfOil : public StaticObject {

};

class Boulders : public DynamicObject {

};

class GoldNuggets : public StaticObject {

};

class SonarKits : public StaticObject {
	// (0, 60)
	// chance of spawn:  int G = current_level_number * 30 + 290

};

class DynamicObject : public Actor {

};

class Water : public DynamicObject { // water refills (water pools)
	// location: random ice-less spot in 4x4 grid
	// 4/5 chance
	// chance of spawn:  int G = current_level_number * 30 + 290
};

class Ice : public StaticObject {

};
#endif // ACTOR_H_
