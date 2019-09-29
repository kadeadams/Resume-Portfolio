#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

#include <map>
#include <string>


class StudentWorld;
using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//base class for all actors
class baseClass: public GraphObject {
public:
	baseClass(int imageID, double startX, double startY, Direction dir, int depth, double size, 
		bool bmove, bool bFlame, bool bVomit, bool infect, bool flame, bool canInfect, StudentWorld* p, bool alive);
	virtual void doSomething() = 0;
	virtual ~baseClass();
	bool getBlocksMove();
	bool getBlocksFlame();
	bool getInfectable();
	bool getCanInfect();
	bool getFlammable();
	bool getAlive();
	void setisAlive(bool alive);
	StudentWorld* returnStudentWorld();
private:

	//booleans to determine object properties and in some cases object type 
	bool blocksMove;		
	bool blocksFlame;
	bool blocksVomit;
	bool infectable;		//essentially whether or not an object can be affected by vomit
	bool flammable;			//essentially whether or not an object can be affected by flame
	bool canInfect;
	bool isAlive;
	StudentWorld* m_StudentWorld;

	//might add further identifier booleans later 
};

//all terrain classes
class terrain : public baseClass {
public:
	terrain(int imageID, double levelX, double levelY, int depth,
		bool bmove, bool bFlame, bool bVomit, StudentWorld* p);
	virtual ~terrain();
	virtual void doSomething();
private:
};

class wall : public terrain {
public:
	wall(double levelX, double levelY, StudentWorld* p);
	virtual ~wall();
	virtual void doSomething();
private:
};

class Exit : public terrain {
public:
	Exit(double levelX, double levelY, StudentWorld* p);
	virtual ~Exit();
private:
};

class pit : public terrain {
public:
	pit(double levelX, double levlY, StudentWorld* p);
	virtual ~pit();
private:
};

//human base class for penelope and citzens
class human : public baseClass {
public:
	human(int imageID, double levelX, double levelY, StudentWorld* p);
	virtual ~human();
	void setInfectionStatus(bool temp);
	virtual void doSomething();
	int returnInfectionCount();
	void modifyInfectionCount(int n);
	bool getInfectionStatus();
protected:
	virtual void onFullInfect() = 0;
	virtual void uniqueAction() = 0;
private:
	int infectionCount;
	bool infectionStatus;
};

class penelope : public human {
public:
	penelope(double levelX, double levelY, StudentWorld* p);
	virtual ~penelope();
protected:
	virtual void onFullInfect();
	virtual void uniqueAction();
private:
	void flamethrower(int dir);
};

class citizen : public human {
public:
	citizen(double levelX, double levvelY, StudentWorld* p);
	virtual ~citizen();
	void setDistP(double n);
	void setDistZ(double n);
protected:
	virtual void onFullInfect();
	virtual void uniqueAction();
private:
	bool paralysis;
	double dist_p;
	double dist_z;

};

//zombie class for smart and dumb zombies
class zombie : public baseClass {
public:
	zombie(double levelX, double levelY, StudentWorld* p);
	virtual ~zombie();
	virtual void doSomething();
	void modifyPlanDist(int n);
	int returnPlanDist();
protected:
	virtual void zombieMove() = 0;
private:
	void vomit(double x, double y, int dir);
	int movePlanDist;
	bool isParalyzed;
};

class dumbZombie : public zombie {
public:
	dumbZombie(double levelX, double levelY, StudentWorld* p);
	virtual ~dumbZombie();
protected:
	virtual void zombieMove();
private:
};

class smartZombie : public zombie {
public:
	smartZombie(double levelX, double levelY, StudentWorld* p);
	virtual ~smartZombie();
	//void modClosDist(double n);
protected:
	virtual void zombieMove();
private:
	//double closestDist;

};

//projectile class for flames and vomit
class projectile : public baseClass {
public:
	projectile(int imageID, double levelX, double levelY, int dir,bool caninfect, StudentWorld* p);
	virtual ~projectile();
	virtual void doSomething();
	virtual void uniqueAction() = 0;
private:
	int lifeSpan;
};

class vomit : public projectile {
public:
	vomit(double levelX, double levelY, int dir, StudentWorld* p);
	virtual ~vomit();
protected:
	virtual void uniqueAction();
private:
};

class flame : public projectile {
public:
	flame(double levelX, double levelY, int dir, StudentWorld* p);
	virtual ~flame();
	virtual void uniqueAction();
private:
};

//goodie class for all goodies 
class goodie : public baseClass {
public:
	goodie(int imageID, double levelX, double levelY, StudentWorld* p);
	virtual ~goodie();
	virtual void doSomething();
protected:
	virtual void uniqueAction() = 0;
private:
};

class vacGoodie : public goodie {
public:
	vacGoodie(double levelX, double levelY, StudentWorld* p);
	virtual ~vacGoodie();
protected:
	virtual void uniqueAction();
private:
};

class gasGoodie : public goodie {
public:
	gasGoodie(double levelX, double levelY, StudentWorld* p);
	virtual ~gasGoodie();
protected:
	virtual void uniqueAction();
private:
};

class mineGoodie : public goodie {
public:
	mineGoodie(double levelX, double levelY, StudentWorld* p);
	virtual ~mineGoodie();
protected:
	virtual void uniqueAction();
private:
};

class landmine : public goodie {
public:
	landmine(double levelX, double levelY, StudentWorld* p);
	virtual ~landmine();
protected:
	virtual void uniqueAction();
private:
	void explode();
	int safetyTicks;
	bool active;
};
#endif // ACTOR_H_
