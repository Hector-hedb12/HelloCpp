
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
}
void state::addBullet(){
  user &ap = playerVector[currentPlayer];
  ap.increaseBullet();
}
void state::addZombie(){
  user &ap = playerVector[currentPlayer];
  ap.increaseZombie();
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

void state::rollPlayerDice(){
  blueDice.next();
  user &ap = playerVector[currentPlayer];
  ap.setLeftMoves(blueDice.recent());
}

void state::rollZombieDice(){
  redDice.next();
  user &ap = playerVector[currentPlayer];
  ap.setLeftZombieMoves(blueDice.recent());
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

vector<event> state::queryMovePlayerTo(position to){
  vector<event> ve = world.queryMovePlayerTo(currentPlayer, to);
}

vector<event> state::movePlayerTo(position to){
  user &ap = playerVector[currentPlayer];
  vector<event> ve = world.movePlayerTo(currentPlayer, to, ap);
  assert(ve.size() <= ap.getLeftMoves());
  ap.setLeftMoves(ap.getLeftMoves() - ve.size());
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
