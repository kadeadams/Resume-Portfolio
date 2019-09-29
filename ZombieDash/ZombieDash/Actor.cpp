#include "StudentWorld.h"
#include "Actor.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//baseClass for all actors 
baseClass::baseClass(int imageID, double startX, double startY, Direction dir, int depth, double size = 1.0,
	bool bmove = false , bool bFlame = false, bool bVomit = false, bool infect = false, bool flame = false, bool caninfect = false, StudentWorld* p = nullptr, bool alive = true)
:GraphObject(imageID, startX, startY, dir, depth, size)
{
	blocksMove = bmove;
	blocksFlame = bFlame;
	blocksVomit = bVomit;
	infectable = infect;
	flammable = flame;
	setDirection(dir);
	canInfect = caninfect;
	isAlive = alive;
	m_StudentWorld = p;
	
	
}

baseClass::~baseClass() {
	return;
}
bool baseClass::getBlocksMove() {
	return blocksMove;
}
bool baseClass::getInfectable() {
	return infectable;
}
bool baseClass::getFlammable() {
	return flammable;
}
bool baseClass::getCanInfect() {
	return canInfect;
}
bool baseClass::getBlocksFlame() {
	return blocksFlame;
}
bool baseClass::getAlive() {
	return isAlive;
}
void baseClass::setisAlive(bool alive) {
	isAlive = alive;
}
StudentWorld* baseClass::returnStudentWorld() {
	return m_StudentWorld;
}

//terrain class
terrain::terrain(int imageID, double levelX, double levelY, int depth,
	bool bmove, bool bFlame, bool bVomit, StudentWorld* p)
:baseClass(imageID, SPRITE_WIDTH * levelX, SPRITE_HEIGHT * levelY, right, depth, 1, bmove, bFlame, bVomit, false, false, false, p, true)
{

}
terrain::~terrain() {
	return; 
}
void terrain::doSomething() {
	returnStudentWorld()->overlapAction(getX(), getY(), this, 10);
}

//wall class
wall::wall(double levelX, double levelY, StudentWorld* p)
	:terrain(IID_WALL, levelX, levelY, 0, true, true, true, p) {
	return;
}
wall::~wall() {
	return;
}
void wall::doSomething() {
	return;
}

//exit class
Exit::Exit(double levelX, double levelY, StudentWorld* p)
	:terrain(IID_EXIT, levelX, levelY, 1, false, true, false, p) {
	return;
}
Exit::~Exit() {
	return;
}


//pit class
pit::pit(double levelX, double levelY, StudentWorld* p)
	:terrain(IID_PIT, levelX, levelY, 0, false, false, false, p) {

}
pit::~pit() {

}


//human class
human::human(int imageID, double levelX, double levelY, StudentWorld* p)
:baseClass(imageID, SPRITE_WIDTH * levelX, SPRITE_HEIGHT * levelY, right, 0, 1, true, false, false, true, true, false, p, true)
{
	infectionCount = 0;
	infectionStatus = false;
}
human::~human() {
	return;
}
void human::setInfectionStatus(bool temp) {
	infectionStatus = temp;
}
bool human::getInfectionStatus() {
	return infectionStatus;
}
void human::doSomething() {
	if (!getAlive()) {
		return;
	}
	if (infectionStatus) {
		infectionCount++;
		if (infectionCount == 500) {
			onFullInfect();
		}
	}
	uniqueAction();
}
int human::returnInfectionCount() {
	return infectionCount;
}
void human::modifyInfectionCount(int n) {
	infectionCount = n;
}



//penelope class
penelope::penelope(double levelX, double levelY, StudentWorld* p)
	:human(IID_PLAYER, levelX, levelY, p)
{
	
}
penelope::~penelope() {
	return;
}
void penelope::flamethrower(int dir) {
	returnStudentWorld()->modInven(-1, "flamethrowers");
	returnStudentWorld()->playSound(SOUND_PLAYER_FIRE);
	for (int i = 1; i < 4; i++) {
		double dest_x;
		double dest_y;
		switch (dir) {
		case right:
			dest_x = getX() + (i * SPRITE_WIDTH);
			dest_y = getY();
			break;
		case left:
			dest_x = getX() - (i * SPRITE_WIDTH);
			dest_y = getY();
			break;
		case up:
			dest_x = getX(); 
			dest_y = getY() + (i * SPRITE_WIDTH);
			break;
		case down:
			dest_x = getX();
			dest_y = getY() - (i * SPRITE_WIDTH);
			break;
		}
		if (returnStudentWorld()->overlapAction(dest_x, dest_y, this, 10) == 3) {
			break;
		}
		else {
			returnStudentWorld()->spawnFlame(dest_x, dest_y, dir);
		}
	}
}
void penelope::onFullInfect() {
	setisAlive(false);
	returnStudentWorld()->spawnZombie(getX(), getY(), 0);
}
void penelope::uniqueAction() {
	int temp;
	returnStudentWorld()->getKey(temp);
	if (temp == KEY_PRESS_SPACE && returnStudentWorld()->returnInven("flamethrowers")) {
		flamethrower(getDirection());
	}
	if (temp == KEY_PRESS_TAB && returnStudentWorld()->returnInven("landmines")) {
		returnStudentWorld()->spawnLandmine(getX(), getY());
		returnStudentWorld()->modInven(-1, "landmines");
	}
	if (temp == KEY_PRESS_ENTER && returnStudentWorld()->returnInven("vaccines")) {
		setInfectionStatus(false);
		modifyInfectionCount(0);
		returnStudentWorld()->modInven(-1, "vaccines");
	}
	if (temp >= KEY_PRESS_LEFT && temp <= KEY_PRESS_DOWN) {		//check to see if the player is presing a directional key
		double dest_x;
		double dest_y;
		switch (temp) {
		case KEY_PRESS_DOWN:
			setDirection(down);			//set direction corresponding to key pressed 
			dest_x = getX();
			dest_y = getY() - 4;
			break;
		case KEY_PRESS_LEFT:
			setDirection(left);
			dest_x = getX() - 4;
			dest_y = getY();
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			dest_x = getX() + 4;
			dest_y = getY();
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			dest_x = getX();
			dest_y = getY() + 4;
			break;
		default:
			break;
		}
		if (returnStudentWorld()->canActorMove(dest_x, dest_y, this)) {
			moveTo(dest_x, dest_y);
		}
	}
}


//citizen class
citizen::citizen(double levelX, double levelY, StudentWorld* p)
	:human(IID_CITIZEN, levelX, levelY, p) {
	paralysis = false;
	
}
citizen::~citizen() {
	return;
}
void citizen::onFullInfect() {
	setisAlive(false);
	returnStudentWorld()->playSound(SOUND_ZOMBIE_BORN);
	returnStudentWorld()->increaseScore(-1000);
	int chance = randInt(0, 9);
	if (chance < 3) {
		returnStudentWorld()->spawnZombie(getX(), getY(), 1);
	}
	else {
		returnStudentWorld()->spawnZombie(getX(), getY(), 0);
	}
	returnStudentWorld()->decCitizens();
	return;
}
void citizen::uniqueAction() {
	if (!getAlive() || paralysis) {
		paralysis = false;
		return;
	}
	else {
		paralysis = true;
	}
	if (returnStudentWorld()->overlapAction(getX(), getY(), this, 10) == 2) {
		if (!getInfectionStatus()) {
			returnStudentWorld()->playSound(SOUND_CITIZEN_INFECTED);
		}
		setInfectionStatus(true);
	}
	returnStudentWorld()->calcCitDists(this, getX(), getY());
	if ((dist_p < dist_z || returnStudentWorld()->returnNZombies() == 0) && dist_p < pow(80, 2.0)) {
		switch (returnStudentWorld()->sameRowCol(this)) {
		case 0:
			if (returnStudentWorld()->canActorMove(getX(), getY() - 2, this)) {
				setDirection(down);
				moveTo(getX(), getY() - 2);
				return;
			}
			break;
		case 1:
			if (returnStudentWorld()->canActorMove(getX(), getY() + 2, this)) {
				setDirection(up);
				moveTo(getX(), getY() + 2);
				return;
			}
			break;
		case 2:
			if (returnStudentWorld()->canActorMove(getX() - 2, getY(), this)) {
				setDirection(left);
				moveTo(getX() - 2, getY());
				return;
			}
			break;
		case 3:
			if (returnStudentWorld()->canActorMove(getX() + 2, getY(), this)) {
				setDirection(right);
				moveTo(getX() + 2, getY());
				return;
			}
			break;
		case -1:
			int arr[2];
			returnStudentWorld()->twoDir(getX(), getY(), arr);
			int num = randInt(0, 1);
			double dest_x;
			double dest_y;
			for (int i = 0; i < 2; i++) {
				switch (arr[num]) {
				case down:
					dest_x = getX();
					dest_y = getY() - 2;
					break;
				case left:
					dest_x = getX() - 2;
					dest_y = getY();
					break;
				case right:
					dest_x = getX() + 2;
					dest_y = getY();
					break;
				case up:
					dest_x = getX();
					dest_y = getY() + 2;
					break;
				default:
					break;
				}
				if (returnStudentWorld()->canActorMove(dest_x, dest_y, this)) {
					setDirection(arr[num]);
					moveTo(dest_x, dest_y);
					return;
				}
				else if (num == 1) {
					num = 0;
				}
				else {
					num = 1;
				}
			}
			break;
		}
	}
	
	map<double, int> dists;
	if (dist_z < pow(80, 2.0)) {
		double temp = dist_z;
		if (returnStudentWorld()->canActorMove(getX(), getY() + 2, this)) {
			returnStudentWorld()->calcCitDists(this, getX(), getY() + 2);
			dists[dist_z] = up;
		}
		if (returnStudentWorld()->canActorMove(getX(), getY() - 2, this)) {
			returnStudentWorld()->calcCitDists(this, getX(), getY() - 2);
			dists[dist_z] = down;
		}
		if (returnStudentWorld()->canActorMove(getX() + 2, getY(), this)) {
			returnStudentWorld()->calcCitDists(this, getX() + 2, getY());
			dists[dist_z] = right;
		}
		if (returnStudentWorld()->canActorMove(getX() - 2, getY(), this)) {
			returnStudentWorld()->calcCitDists(this, getX() - 2, getY() + 2);
			dists[dist_z] = left;
		}
		if (dists.empty()) {
			return;
		}
		map<double, int>::iterator it = dists.end();
		it--;
		double dest_x;
		double dest_y;
		if (it->first <= temp) {
			return;
		}
		else {
			switch (it->second) {
			case down:
				dest_x = getX();
				dest_y = getY() - 2;
				break;
			case left:
				dest_x = getX() - 2;
				dest_y = getY();
				break;
			case right:
				dest_x = getX() + 2;
				dest_y = getY();
				break;
			case up:
				dest_x = getX();
				dest_y = getY() + 2;
				break;
			default:
				break;
			}
			if (returnStudentWorld()->canActorMove(dest_x, dest_y, this)) {
				setDirection(it->second);
				moveTo(dest_x, dest_y);
				return;
			}
		}
	}
	
}
void citizen::setDistP(double n) {
	dist_p = n;
}
void citizen::setDistZ(double n) {
	dist_z = n;
}

//zombie class
zombie::zombie(double levelX, double levelY, StudentWorld* p)
	:baseClass(IID_ZOMBIE, SPRITE_WIDTH * levelX, SPRITE_HEIGHT * levelY, right, 0, 1, true, false, true, false, true, true, p, true) {
	movePlanDist = 0;
	isParalyzed = false;
}
zombie::~zombie() {
	return;
}
void zombie::vomit(double x, double y, int dir) {
	returnStudentWorld()->spawnVomit(x, y, dir);
}
int zombie::returnPlanDist() {
	return movePlanDist;
}
void zombie::doSomething() {
	if (!getAlive() || isParalyzed) {
		isParalyzed = false;
		return;
	}
	switch (getDirection()) {
	case right:
		if (returnStudentWorld()->overlapAction(getX() + SPRITE_WIDTH, getY(), this, 10) == 5){
			if (randInt(0, 2) == 2) {
				vomit(getX() + SPRITE_WIDTH, getY(), right);
				isParalyzed = true;
				return;
			}
		}
		break;
	case left:
		if (returnStudentWorld()->overlapAction(getX() - SPRITE_WIDTH, getY(), this, 10) == 5) {
			if (randInt(0, 2) == 2) {
				vomit(getX() - SPRITE_WIDTH, getY(), left);
				isParalyzed = true;
				return;
			}
		}
		break;
	case up:
		if (returnStudentWorld()->overlapAction(getX(), getY() + SPRITE_HEIGHT, this, 10) == 5) {
			if (randInt(0, 2) == 2) {
				vomit(getX(), getY() + SPRITE_HEIGHT, up);
				isParalyzed = true;
				return;
			}
		}
		break;
	case down:
		if (returnStudentWorld()->overlapAction(getX(), getY() - SPRITE_HEIGHT, this, 10) == 5) {
			if (randInt(0, 2) == 2) {
				vomit(getX(), getY() - SPRITE_HEIGHT, down);
				isParalyzed = true;
				return;
			}
		}
		break;
	default:
		break;
	}
	if (movePlanDist == 0) {
		zombieMove();
	}

	double dest_x;
	double dest_y;
	switch (getDirection()) {
	case left:			//set direction corresponding to key pressed 
		dest_x = getX() - 1;
		dest_y = getY();
		break;
	case right:
		dest_x = getX() + 1;
		dest_y = getY();
		break;
	case up:
		dest_x = getX();
		dest_y = getY() + 1;
		break;
	case down:
		dest_x = getX();
		dest_y = getY() - 1;
		break;
	default:
		break;
	}
	if (returnStudentWorld()->canActorMove(dest_x, dest_y, this)) {
		moveTo(dest_x, dest_y);
		modifyPlanDist(returnPlanDist() - 1);
	}
	else {
		modifyPlanDist(0);
	}

	isParalyzed = true;
}
void zombie::modifyPlanDist(int n) {
	movePlanDist = n;
}

//dumb zombie class
dumbZombie::dumbZombie(double levelX, double levelY, StudentWorld* p)
	:zombie(levelX, levelY, p) {
	return;
}
dumbZombie::~dumbZombie() {
	if (!returnStudentWorld()->returnGameOver()) {
		if (randInt(0, 1) == 1) {
			vector<int> temp;
			temp.push_back(0);
			temp.push_back(90);
			temp.push_back(180);
			temp.push_back(270);
			int rand = randInt(0, temp.size() - 1);
			switch (temp[rand]) {
			case 0:
				if (returnStudentWorld()->overlapAction(getX() + SPRITE_WIDTH, getY(), this, 10) == 10) {
					returnStudentWorld()->spawnVaccine(getX() + SPRITE_WIDTH, getY());
				}
				break;
			case 90:
				if (returnStudentWorld()->overlapAction(getX(), getY() + SPRITE_HEIGHT, this, 10) == 10) {
					returnStudentWorld()->spawnVaccine(getX(), getY() + SPRITE_HEIGHT);
				}
				break;
			case 180:
				if (returnStudentWorld()->overlapAction(getX() - SPRITE_WIDTH, getY(), this, 10) == 10) {
					returnStudentWorld()->spawnVaccine(getX() - SPRITE_WIDTH, getY());
				}
				break;
			case 270:
				if (returnStudentWorld()->overlapAction(getX(), getY() - SPRITE_HEIGHT, this, 10) == 10) {
					returnStudentWorld()->spawnVaccine(getX(), getY() - SPRITE_HEIGHT);
				}
				break;
			}
		}
	}
}
void dumbZombie::zombieMove() {
	modifyPlanDist(randInt(3, 10));
	switch (randInt(1, 4)) {
	case 1:
		setDirection(left);
		break;
	case 2:
		setDirection(right);
		break;
	case 3:
		setDirection(up);
		break;
	case 4:
		setDirection(down);
		break;
	default:
		break;
	}
}

//smart zombie class
smartZombie::smartZombie(double levelX, double levelY, StudentWorld* p)
	:zombie(levelX, levelY, p) {
	return;
}
smartZombie::~smartZombie() {
	return;
}
void smartZombie::zombieMove() {
	modifyPlanDist(randInt(3, 10));
	returnStudentWorld()->smartCalcs(this, getX(), getY());
}

//projectile class
projectile::projectile(int imageID, double levelX, double levelY, int dir, bool caninfect, StudentWorld* p)
	:baseClass(imageID, levelX, levelY, dir, 0, 1, false, false, false, false, false, caninfect, p, true) {
	lifeSpan = 0;
}
projectile::~projectile() {
	return;
}
void projectile::doSomething() {
	if (!getAlive()) {
		return;
	}
	if (lifeSpan >= 2) {
		setisAlive(false);
		return;
	}
	uniqueAction();

	lifeSpan++;
}

//vomit class
vomit::vomit(double levelX, double levelY, int dir, StudentWorld* p)
	:projectile(IID_VOMIT, levelX, levelY, dir, true, p) {

}
vomit::~vomit() {
	return;
}
void vomit::uniqueAction() {
	returnStudentWorld()->overlapAction(getX(), getY(), this, 10);
}

//flame class
flame::flame(double levelX, double levelY, int dir, StudentWorld* p)
	:projectile(IID_FLAME, levelX, levelY, dir, false, p) {

}
flame::~flame() {
	return;
}
void flame::uniqueAction() {
	returnStudentWorld()->overlapAction(getX(), getY(), this, 10);
}

//goodie class
goodie::goodie(int imageID, double levelX, double levelY, StudentWorld* p)
	:baseClass(imageID, levelX * SPRITE_WIDTH, levelY * SPRITE_HEIGHT, right, 1, 1, false, false, false, false, true, false, p, true) {

}
goodie::~goodie() {

}
void goodie::doSomething() {
	if (!getAlive()) {
		return;
	}
	uniqueAction();
}

//vaccine goodie class
vacGoodie::vacGoodie(double levelX, double levelY, StudentWorld* p)
	:goodie(IID_VACCINE_GOODIE, levelX, levelY, p) {

}
vacGoodie::~vacGoodie() {

}
void vacGoodie::uniqueAction() {
	if (returnStudentWorld()->overlapAction(getX(), getY(), this, 10) == 5 && getAlive()) {
		setisAlive(false);
		returnStudentWorld()->playSound(SOUND_GOT_GOODIE);
		returnStudentWorld()->increaseScore(50);
		returnStudentWorld()->modInven(1, "vaccines");
	}
}

//gas can goodie class
gasGoodie::gasGoodie(double levelX, double levelY, StudentWorld* p)
	:goodie(IID_GAS_CAN_GOODIE, levelX, levelY, p) {

}
gasGoodie::~gasGoodie() {

}
void gasGoodie::uniqueAction() {
	if (returnStudentWorld()->overlapAction(getX(), getY(), this, 10) == 5 && getAlive()) {
		setisAlive(false);
		returnStudentWorld()->playSound(SOUND_GOT_GOODIE);
		returnStudentWorld()->increaseScore(50);
		returnStudentWorld()->modInven(5, "flamethrowers");
	}
}

//landmine goodie class
mineGoodie::mineGoodie(double levelX, double levelY, StudentWorld* p)
	:goodie(IID_LANDMINE_GOODIE, levelX, levelY, p) {

}
mineGoodie::~mineGoodie() {

}
void mineGoodie::uniqueAction() {
	if (returnStudentWorld()->overlapAction(getX(), getY(), this, 10) == 5 && getAlive()) {
		setisAlive(false);
		returnStudentWorld()->playSound(SOUND_GOT_GOODIE);
		returnStudentWorld()->increaseScore(50);
		returnStudentWorld()->modInven(2, "landmines");
	}
}

//landmine class
landmine::landmine(double levelX, double levelY, StudentWorld* p)
	:goodie(IID_LANDMINE, levelX, levelY, p) {
	safetyTicks = 30;
	active = false;
}
landmine::~landmine() {

}
void landmine::explode() {
	setisAlive(false);
	returnStudentWorld()->playSound(SOUND_LANDMINE_EXPLODE);
	if (returnStudentWorld()->overlapAction(getX(), getY(), this, 10) != 2) {
		returnStudentWorld()->spawnFlame(getX(), getY(), up);
	}
	if (returnStudentWorld()->overlapAction(getX() + SPRITE_WIDTH, getY(), this, 10) != 2) {
		returnStudentWorld()->spawnFlame(getX() + SPRITE_WIDTH, getY(), up);
	}
	if (returnStudentWorld()->overlapAction(getX() - SPRITE_WIDTH, getY(), this, 10) != 2) {
		returnStudentWorld()->spawnFlame(getX() - SPRITE_WIDTH, getY(), up);
	}
	if (returnStudentWorld()->overlapAction(getX(), getY() + SPRITE_HEIGHT, this, 10) != 2) {
		returnStudentWorld()->spawnFlame(getX(), getY() + SPRITE_HEIGHT, up);
	}
	if (returnStudentWorld()->overlapAction(getX(), getY() - SPRITE_HEIGHT, this, 10) != 2) {
		returnStudentWorld()->spawnFlame(getX(), getY() - SPRITE_HEIGHT, up);
	}
	if (returnStudentWorld()->overlapAction(getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT, this, 10) != 2) {
		returnStudentWorld()->spawnFlame(getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up);
	}
	if (returnStudentWorld()->overlapAction(getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT, this, 10) != 2) {
		returnStudentWorld()->spawnFlame(getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up);
	}
	if (returnStudentWorld()->overlapAction(getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT, this, 10) != 2) {
		returnStudentWorld()->spawnFlame(getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up);
	}
	if (returnStudentWorld()->overlapAction(getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT, this, 10) != 2) {
		returnStudentWorld()->spawnFlame(getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up);
	}

	returnStudentWorld()->spawnPit(getX(), getY());
}
void landmine::uniqueAction() {
	if (!active) {
		safetyTicks--;
		if (safetyTicks == 0) {
			active = true;
		}
		return;
	}

	if (returnStudentWorld()->overlapAction(getX(), getY(), this, 10) == 1 || returnStudentWorld()->overlapAction(getX(), getY(), this, 10) == 5 || returnStudentWorld()->overlapAction(getX(), getY(), this, 10) == 4) {
		explode();
	}
}
