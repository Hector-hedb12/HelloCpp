#include "state.h"
#include <cstdlib>
#include "cocos2d.h"

USING_NS_CC;

bool state::queryLife(position p){
  return world.getTile(p).hasLife();
}
bool state::queryBullet(position p){
  return world.getTile(p).hasBullet();
}
bool state::queryZombie(position p){
  return world.getTile(p).hasZombie();
}

bool state::queryLife(){
  return world.getTile(world.getPlayerPosition(currentPlayer)).hasLife();
}
bool state::queryBullet(){
  return world.getTile(world.getPlayerPosition(currentPlayer)).hasBullet();
}
bool state::queryZombie(){
  return world.getTile(world.getPlayerPosition(currentPlayer)).hasZombie();
}

void state::printWorld(){
  world.print();
}

vector<position> state::getPosibleMapCard(mapCard ma){
  return world.getPossibleMapCard(ma);
}

void state::putCardMap(mapCard ma, position pos){
  world.insertMapCard(ma, pos);
}

void state::addLife(){
  user &ap = playerVector[currentPlayer];
  ap.increaseLife();
  world.setLife(world.getPlayerPosition(currentPlayer),false);
}
void state::addBullet(){
  user &ap = playerVector[currentPlayer];
  ap.increaseBullet();
  world.setBullet(world.getPlayerPosition(currentPlayer),false);
}
void state::addZombie(){
  user &ap = playerVector[currentPlayer];
  ap.increaseZombie();
  world.setZombie(world.getPlayerPosition(currentPlayer),false);
}

void state::decreaseLife(){
  user &ap = playerVector[currentPlayer];
  ap.decreaseLife();
}
void state::decreaseBullet(int x){
  user &ap = playerVector[currentPlayer];
  assert(x >= 1);
  for(int i = 0; i < x; i++){
	  ap.decreaseBullet();
  }
}

void state::killcurrentPlayer(){
  user &ap = playerVector[currentPlayer];
  ap.die();
  world.killPlayer(currentPlayer);
  // MOVER JUGADOR AL INICIOO EN EL MAPA
}

void state::init(int nPlayers){

  playerVector.clear();
  for(int i = 0; i < nPlayers; i++){
    playerVector.push_back(user());
  }
  init();
}

void state::init(int na, int nb){
	playerVector.clear();
  for(int i = 0; i < na; i++){
    playerVector.push_back(user());
  }
  for(int i = 0; i < nb; i++){
	  playerVector.push_back(machine());
  }
  init();
}

void state::init(){
	srand(time(0));
	initmapVector();
	world.init(playerVector.size());
	currentPlayer = 0;
	movedZombies.clear();
	// LIMPIAR PILA: pile.clear();
}

void state::initmapVector(){
	mapVector.clear();
	for(int i = 0; i < MAP_CARD_MAX; i++){
		for(int j = 0; j < typeMapCards[i]; j++){
			mapVector.push_back(mapCard(i, i==1));
		}
	}
	random_shuffle(mapVector.begin(), mapVector.end());
}

void state::nextTurn(){
  currentPlayer = (currentPlayer + 1)% playerVector.size();
  movedZombies.clear();
}


mapCard &state::pickMapCard(){
  int idPlayer = currentPlayer;
  lastMapCard = mapVector.back();
  mapVector.pop_back();
  return getLastMapCard();
}

mapCard &state::getLastMapCard(){
  return lastMapCard;
}

mapCard &state::rotateRLastMapCard(){
	getLastMapCard().rotateR();
	return getLastMapCard();
}

mapCard &state::rotateLLastMapCard(){
	getLastMapCard().rotateL();
	return getLastMapCard();
}

void state::pickCard(int idPlayer = -1){
  if(idPlayer == -1) idPlayer = currentPlayer;
  assert(0 <= idPlayer && idPlayer < playerVector.size());
  playerVector[idPlayer].pushHandCard(pile.get());
}

void state::useCard(int idPlayer, int idCard){
  assert(0 <= idPlayer && idPlayer < playerVector.size());
  card ca = playerVector[idPlayer].getHandCard(idCard);
  playerVector[idPlayer].eraseHandCard(idCard);
  ca.use();
  // No siempre se hace
  pile.push(ca);
}

int state::rollPlayerDice(){
  blueDice.next();
  user &ap = playerVector[currentPlayer];
  ap.setLeftMoves(blueDice.recent()+1); // TRANSFORM[1..6]
  return blueDice.recent();
}

int state::rollZombieDice(){
  redDice.next();
  user &ap = playerVector[currentPlayer];
  ap.setLeftZombieMoves(redDice.recent()+1);// TRANSFORM[1..6]
  return redDice.recent();
}
int state::rollFightDice(){
	return yellowDice.next();
}

int state::getLastRollPlayerDice(){
	return blueDice.recent();
}

int state::getLastRollZombieDice(){
	return redDice.recent();
}

int state::getLastRollFightDice(){
	return yellowDice.recent();
}



pair<vector<position>, vector<position> > state::getPossibleMoves(){
  vector<position> r1, r2;
  user &ap = playerVector[currentPlayer];
  position current = world.getPlayerPosition(currentPlayer);
  int dist = ap.getLeftMoves();
  r1 = world.getPossibleMoves(current, dist, 1);
  r2 = world.getPossibleMoves(current, dist, 0); // no le para a los zombies

  return make_pair(r1,r2);
}

vector<position> state::getPossibleZombieMoves(position pos){
	return world.getPossibleZombieMoves(pos);
}

vector<position> state::queryMovePlayerTo(position to){
  vector<position> ve = world.queryMovePlayerTo(currentPlayer, to);
  return ve;
}

void state::movePlayerTo(position to){
  user &ap = playerVector[currentPlayer];
  world.movePlayerTo(currentPlayer, to);

  assert(ap.getLeftMoves() >= 1);
  ap.setLeftMoves(ap.getLeftMoves() - 1);
}
void state::moveZombieTo(position prev, position p){
	assert(isValidZombie(prev));
	world.moveZombie(prev, p);
	movedZombies.insert(p);
}

int state::getCurrentPlayerLife(){
	playerVector[currentPlayer].getLife();
}
int state::getCurrentPlayerBullet(){
	playerVector[currentPlayer].getBullet();
}

int state::getCurrentPlayerZombie(){
	playerVector[currentPlayer].getZombie();
}
vector<vector<position> > state::getAllPosibleMapCard(mapCard ma){
	vector<vector<position> > res;
	for(int i = 0; i < 4; i++){
		res.push_back(getPosibleMapCard(ma));
		ma.rotateR();
	}
	return res;
}

int state::getCurrentPlayer(){
	return currentPlayer;
}

position state::getCurrentPlayerPosition(){
	return world.getPlayerPosition(currentPlayer);
}
bool state::isValidZombie(position p){
	return (movedZombies.count(p) == 0 && queryZombie(p));
}

bool state::isCurrentPlayerMachine(){
	return playerVector[currentPlayer].isMachine();
}

user &state::getCurrentPlayerInstance(){
	return this->playerVector[this->currentPlayer];
}
bool state::mapStackEmpty(){
	return this->mapVector.empty();
}
bool state::currentPlayerOverHeliport(){
	return world.hasEndCard() && (world.endPosition == this->getCurrentPlayerPosition());
}

