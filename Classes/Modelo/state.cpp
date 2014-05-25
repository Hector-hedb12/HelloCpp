
int state::getCurrentPlayerLife(){
	playerVector[currentPlayer].getLife();
}
int state::getCurrentPlayerBullet(){
	playerVector[currentPlayer].getBullet();
}
int state::getCurrentPlayerZombie(){
	playerVector[currentPlayer].getZombie();
}

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


state::state(int nPlayers){
  for(int i = 0; i < nPlayers; i++){
    playerVector.push_back(user());
  }
  for(int i = 0; i < MAP_CARD_MAX; i++){
	for(int j = 0; j < vNMapcard[i]; j++){
	  mapVector.push_back(mapCard(i));
	}
  }
  unsort(mapVector.begin(), mapVector.end());
  world.init(nPlayers);
  currentPlayer = 0;
}

void state::nextTurn(){
  currentPlayer = (currentPlayer + 1)% playerVector.size();
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
  ap.setLeftMoves(blueDice.recent());
  return blueDice.recent();
}

int state::rollZombieDice(){
  redDice.next();
  user &ap = playerVector[currentPlayer];
  ap.setLeftZombieMoves(redDice.recent());
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
  r2 = world.getPossibleMoves(current, dist, 0);
  return make_pair(r1,r2);
}

vector<position> state::queryMovePlayerTo(position to){
  vector<position> ve = world.queryMovePlayerTo(currentPlayer, to);
}

void state::movePlayerTo(position to){
  user &ap = playerVector[currentPlayer];
  world.movePlayerTo(currentPlayer, to, ap);

  assert(getLeftMoves() > 1);
  ap.setLeftMoves(ap.getLeftMoves() - 1);
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

