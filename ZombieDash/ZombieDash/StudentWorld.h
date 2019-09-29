#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>
#include <vector>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();

	bool spawnVomit(double x, double y, int dir);
	bool spawnZombie(double x, double y, int type);
	bool spawnFlame(double x, double y, int dir);
	bool spawnPit(double x, double y);
	bool spawnLandmine(double x, double y);
	bool spawnVaccine(double x, double y);



	bool canActorMove(double dest_x, double dest_y, baseClass* p);

	void calcCitDists(citizen* p, double x, double y);
	void smartCalcs(smartZombie* p, double x, double y);
	int sameRowCol(citizen* p);
	void twoDir(double x, double y, int* arr);

	int overlapAction(double x, double y, baseClass* p, double less);

	int returnNZombies();
	bool returnGameOver();
	int returnInven(string type);
	int getnCitizens();

	void modInven(int n, string type);
	void decCitizens();


private:
	//private member variables 
	list<baseClass*> actors;
	penelope* m_player;
	int nCitizens;
	int nZombies;
	int status;
	map<string, int> inventory;
	bool levelFinished;
	bool gameOver;


	//private member functions
};

#endif // STUDENTWORLD_H_
