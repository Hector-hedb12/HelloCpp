#include "user.h"
#include <cstdlib>

user::user(bool b){
  init();
  ismachine = b;
  if(ismachine){
	  selectStrategy();
  }
}

bool user::isMachine(){
	return this->ismachine;
}

void user::increaseLife(){
  if(nLife < MAX_LIFE){
    nLife++;
  }
}
void user::decreaseLife(){
  assert(nLife > 0);
  nLife--;
}
void user::increaseBullet(){
  nBullet++;
}
void user::decreaseBullet(){
  assert(nBullet > 0);
  nBullet--;
}
void user::increaseZombie(){
  nZombie++;
}
void user::decreaseZombie(){
  assert(nZombie > 0);
  nZombie--;
}
int user::getLife(){
  return nLife;
}
int user::getBullet(){
  return nBullet;
}
int user::getZombie(){
  return nZombie;
}


void user::pushHandCard(card ca){
  handCardVector.push_back(ca);
}
card& user::getHandCard(int idCard){
  assert(0 <= idCard && idCard < handCardVector.size());
  return handCardVector[idCard];
}
bool user::eraseHandCard(int idCard){
  assert(0 <= idCard && idCard < handCardVector.size());
  handCardVector.erase(handCardVector.begin()+idCard);
}
void user::setLeftMoves(int x){
  leftMoves = x;
}
int user::getLeftMoves(){
  return leftMoves;
}

void user::setLeftZombieMoves(int x){
  leftZombieMoves = x;
}
int user::getLeftZombieMoves(){
  return leftZombieMoves;
}

void user::init(){
  leftMoves = 0;
  nBullet = PLAYER_INIT_BULLET;
  nLife = PLAYER_INIT_LIFE;
  nZombie = PLAYER_INIT_ZOMBIES;
}
void user::die(){
  nBullet = PLAYER_INIT_BULLET;
  nLife = PLAYER_INIT_LIFE;
  nZombie = nZombie/2;
  if(this->ismachine){
	  selectStrategy();
  }
}

strategy& user::getStrategy(){
	return stm;
}



void user::selectStrategy(){
	int lc, bc, zc, pc, hc, dc;
	int maxst = 5;
	vector<int> vs;
	if(nLife < 5){
		vs.push_back(3);
	}else{
		vs.push_back(4);
	}
	vs.push_back(1);
	vs.push_back(2);

	this->stm = strategy(vs[rand()%vs.size()]);

}
