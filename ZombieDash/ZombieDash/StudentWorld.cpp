#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <cstdlib>
#include <math.h>
#include <sstream>
#include <iomanip>
using namespace std;


GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
	nCitizens = 0;
	nZombies = 0;
	status = GWSTATUS_CONTINUE_GAME;

	inventory["landmines"] = 0;				//set her inventory to contain no items to start off with 
	inventory["flamethrowers"] = 0;
	inventory["vaccines"] = 0;

	levelFinished = false;

}

StudentWorld::~StudentWorld() {
	cleanUp();
}

int StudentWorld::init()
{
	gameOver = false;
	Level lev(assetPath());
	ostringstream level;
	level.fill('0');
	level << "level" << setw(2) << getLevel() << ".txt";
	string file = level.str();
	
	Level::LoadResult result = lev.loadLevel(file);
	if (result == Level::load_fail_file_not_found || getLevel() > 99) {
		return GWSTATUS_PLAYER_WON;
	}
	if (result == Level::load_fail_bad_format) {
		return GWSTATUS_LEVEL_ERROR;
	}
	for (int i = 0; i < LEVEL_WIDTH; i++) {
		for (int j = 0; j < LEVEL_HEIGHT; j++) {
			Level::MazeEntry object = lev.getContentsOf(i, j);
			if (object == Level::player) {
				m_player = new penelope(i, j, this);
			}
			if (object == Level::wall) {
				actors.push_back(new wall(i, j, this));
			}
			if (object == Level::exit) {
				actors.push_back(new Exit(i, j, this));
			}
			if (object == Level::citizen) {
				actors.push_back(new citizen(i, j, this));
				nCitizens++;
			}
			if (object == Level::dumb_zombie) {
				actors.push_back(new dumbZombie(i, j, this));
				nZombies++;
			}
			if (object == Level::smart_zombie) {
				actors.push_back(new smartZombie(i, j, this));
				nZombies++;
			}
			if (object == Level::vaccine_goodie) {
				actors.push_back(new vacGoodie(i, j, this));
			}
			if (object == Level::gas_can_goodie) {
				actors.push_back(new gasGoodie(i, j, this));
			}
			if (object == Level::landmine_goodie) {
				actors.push_back(new mineGoodie(i, j, this));
			}
			if (object == Level::pit) {
				actors.push_back(new pit(i, j, this));
			}
		}
	}
	
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	ostringstream UI;
	UI.setf(ios::fixed);
	UI.precision(2);
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	for (list<baseClass*>::iterator it = actors.begin(); it != actors.end(); it++) {
		(**it).doSomething();
	}
	m_player->doSomething();

	list<baseClass*>::iterator it = actors.begin();
	while (it != actors.end()) {
		if (!(**it).getAlive()) {
			delete (*it);
			it = actors.erase(it);
			continue;
		}
		it++;
	}
	if (!m_player->getAlive()) {
		playSound(SOUND_PLAYER_DIE);
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	if (levelFinished) {
		levelFinished = false;
		playSound(SOUND_LEVEL_FINISHED);
		return GWSTATUS_FINISHED_LEVEL;
	}

	UI.fill('0');
	if (getScore() >= 0) {
		UI << "Score: " << setw(6) << getScore();
	}
	else {
		UI << "Score: -" << setw(5) << abs(getScore());
	}
	UI.fill(' ');
	UI << "  Level: " << setw(2) << getLevel();
	UI << "  Lives: " << setw(1) << getLives();
	UI << "  Vaccines: " << setw(2) << inventory["vaccines"];
	UI << "  Flames: " << setw(2) << inventory["flamethrowers"];
	UI << "  Mines: " << setw(2) << inventory["landmines"];
	UI << "  Infected: " << setw(1) << m_player->returnInfectionCount();
	string display = UI.str();
	setGameStatText(display);
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	gameOver = true;
	if (m_player != nullptr) {
		delete m_player;
		m_player = nullptr;
		for (list<baseClass*>::iterator it = actors.begin(); it != actors.end();) {
			delete *it;
			it = actors.erase(it);

		}
		nCitizens = 0;
		inventory["landmines"] = 0;				//set her inventory to contain no items to start off with 
		inventory["flamethrowers"] = 0;
		inventory["vaccines"] = 0;
	}
}

void StudentWorld::decCitizens() {
	nCitizens--;
}

bool StudentWorld::canActorMove(double dest_x, double dest_y, baseClass* p) {
	for (list<baseClass*>::iterator it = actors.begin(); it != actors.end(); it++) {
		if (*it == p) {
			continue;
		}
		if (abs(dest_x - (**it).getX()) < SPRITE_WIDTH && abs(dest_y - (**it).getY()) < SPRITE_HEIGHT && (**it).getBlocksMove()) {
			return false;
		}
	}
	if (p == m_player) {
		return true;
	}
	if (abs(dest_x - m_player->getX()) < SPRITE_WIDTH && abs(dest_y - m_player->getY()) < SPRITE_HEIGHT && m_player->getBlocksMove()) {
		return false;
	}
	return true;
}

int StudentWorld::getnCitizens() {
	return nCitizens;
}

void StudentWorld::calcCitDists(citizen* p, double x, double y) {
	double dist_z = 100000000;

	for (list<baseClass*>::iterator it2 = actors.begin(); it2 != actors.end(); it2++) {
		if ((**it2).getCanInfect()) {
			double temp = pow(x - (**it2).getX(), 2.0) + pow(y - (**it2).getY(), 2.0);
			if (temp < dist_z) {
				dist_z = temp;
			}
		}
	}
	double dist_p = pow(x - m_player->getX(), 2.0) + pow(y - m_player->getY(), 2.0);
	p->setDistP(dist_p);
	p->setDistZ(dist_z);
}

void StudentWorld::smartCalcs(smartZombie* p, double x, double y) {
	double dist_z = 100000000;
	baseClass* closest;
	int rand = 0;

	for (list<baseClass*>::iterator it2 = actors.begin(); it2 != actors.end(); it2++) {
		if ((**it2).getInfectable()) {
			double temp = pow(x - (**it2).getX(), 2.0) + pow(y - (**it2).getY(), 2.0);
			if (temp < dist_z) {
				dist_z = temp;
				closest = *it2;
			}
		}
	}
	double dist_p = pow(x - m_player->getX(), 2.0) + pow(y - m_player->getY(), 2.0);
	if (dist_p < dist_z) {
		dist_z = dist_p;
		closest = m_player;
	}
	int arr[2] = { 0,0 };
	if (dist_z > pow(80, 2.0)) {
		rand = randInt(1, 4); //1-up, 2-down, 3-left, 4-right
	}
	else {
		if (p->getX() == closest->getX()) {
			if (p->getY() > closest->getY()) {
				rand = 2;
			}
			else {
				rand = 1;
			}
		}
		else if (p->getY() == closest->getY()) {
			if (p->getX() > closest->getX()) {
				rand = 3;
			}
			else {
				rand = 4;
			}
		}
		if (y > closest->getY()) {
			arr[0] = 2;
		}
		else {
			arr[0] = 1;
		}
		if (x > closest->getX()) {
			arr[1] = 3;
		}
		else {
			arr[1] = 4;
		}
	}
	if (rand == 0) {
		rand = arr[randInt(0, 1)];
	}
	switch (rand) {
	case 1:
		p->setDirection(90);
		break;
	case 2:
		p->setDirection(270);
		break;
	case 3:
		p->setDirection(180);
		break;
	case 4:
		p->setDirection(0);
		break;
	}

}

int StudentWorld::returnNZombies() {
	return nZombies;
}

int StudentWorld::sameRowCol(citizen* p) {
	if (p->getX() == m_player->getX()) {
		if (p->getY() > m_player->getY()) {
			return 0;
		}
		else {
			return 1;
		}
	}
	if (p->getY() == m_player->getY()) {
		if (p->getX() > m_player->getX()) {
			return 2;
		}
		else {
			return 3;
		}
	}
	return -1;
}

void StudentWorld::twoDir(double x, double y, int* arr) {
	if (y > m_player->getY()) {
		arr[0] = 270;
	}
	else {
		arr[0] = 90;
	}
	if (x > m_player->getX()) {
		arr[1] = 180;
	}
	else {
		arr[1] = 0;
	}
}

int StudentWorld::overlapAction(double x, double y, baseClass* p, double less) {
	vector<baseClass*> check;
	for (list<baseClass*>::iterator it = actors.begin(); it != actors.end(); it++) {
		if (*it == p) {
			continue;
		}
		if ((pow(x - (**it).getX(), 2.0) + pow(y - (**it).getY(), 2.0)) < pow(less, 2.0)) {
			check.push_back(*it);
		}
	}
	if ((pow(x - m_player->getX(), 2.0) + pow(y - m_player->getY(), 2.0)) < pow(less, 2.0)) {
		check.push_back(m_player);
	}
	
	
	if (check.empty()) {
		return 10;
	}
	for (unsigned int i = 0; i < check.size(); i++) {
		if (p->getCanInfect() && p->getBlocksMove() && check[i]->getInfectable()) {
			return 5;
		}
		else if (check[i]->getInfectable() && check[i] != m_player && !p->getBlocksMove() && p->getBlocksFlame()) {
			increaseScore(500);
			playSound(SOUND_CITIZEN_SAVED);
			decCitizens();
			check[i]->setisAlive(false);
			return 1;
		}
		else if (check[i] == m_player && p->getBlocksFlame() && !p->getBlocksMove() && nCitizens <= 0) {
			levelFinished = true;
			return 7;
		}
		else if (check[i] == m_player && p->getCanInfect() && !p->getBlocksMove()) {
			m_player->setInfectionStatus(true);
		}
		else if (p->getInfectable() && p != m_player && check[i]->getCanInfect() && !check[i]->getBlocksMove()) {
			return 2;
		}
		else if (p->getFlammable() && !p->getInfectable() && check[i] == m_player) {
			return 5;
		}
		else if (check[i]->getFlammable() && !p->getBlocksFlame() && !p->getFlammable() && !p->getCanInfect()) {
			if (check[i] != m_player) {
				if (check[i]->getCanInfect()) {
					check[i]->setisAlive(false);
					playSound(SOUND_ZOMBIE_DIE);
					increaseScore(1000);
				}
				if (check[i]->getInfectable()) {
					check[i]->setisAlive(false);
					playSound(SOUND_CITIZEN_DIE);
					decCitizens();
					increaseScore(-1000);
				}
				else {
					return 5;
				}
			}
			
			check[i]->setisAlive(false);
		}
		else if (p == m_player && check[i]->getBlocksFlame()) {
			return 3;
		}
		else if (check[i]->getBlocksFlame() && !p->getBlocksMove()) {
			return 2;
		}
		else if (check[i]->getInfectable() && !p->getBlocksFlame()) {	//not landmine (actually maybe landmine i forget shit)
			return 1;
		}
		else if (check[i]->getCanInfect() && !p->getBlocksFlame()) {
			return 4;
		}
		else if (!check[i]->getBlocksFlame() && !check[i]->getFlammable() && !p->getBlocksFlame()) {
			return 1;
		}
		return 9;
	}
	return 10;
}

bool StudentWorld::spawnVomit(double x, double y, int dir) {
	actors.push_back(new vomit(x, y, dir, this));
	return true;
}

bool StudentWorld::spawnFlame(double x, double y, int dir) {
	actors.push_front(new flame(x, y, dir, this));
	return true;
}

bool StudentWorld::spawnPit(double x, double y) {
	actors.push_back(new pit(x / 16, y / 16, this));
	return true;
}

bool StudentWorld::spawnLandmine(double x, double y) {
	actors.push_back(new landmine(x / 16, y / 16, this));
	return true;
}

bool StudentWorld::spawnZombie(double x, double y, int type) {
	if (type == 0) {
		actors.push_back(new dumbZombie(x / 16, y / 16, this));
		return true;
	}
	else {
		actors.push_back(new smartZombie(x / 16, y / 16, this));
		return true;
	}
	return false;
}

void StudentWorld::modInven(int n, string type) {
	inventory[type] += n;
}

int StudentWorld::returnInven(string type) {
	return inventory[type];
}

bool StudentWorld::spawnVaccine(double x, double y) {
	actors.push_back(new vacGoodie(x/16, y/16, this));
	return true; 
}

bool StudentWorld::returnGameOver() {
	return gameOver;
}

