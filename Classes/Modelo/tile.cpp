
tile::tile(){
  players.clear();
  door = building = valid = zombie = life = bullet = false;
}
void tile::setBullet(bool b){
  bullet = b;
}
void tile::setLife(bool b){
  life = b;
}
void tile::setZombie(bool b){
  zombie = b;
}
void tile::setValid(bool b){
  valid = b;
}
void tile::setBuilding(bool b){
  building = b;
}
void tile::setDoor(bool b){
  door = b;
}
bool tile::setEnd(bool b){
  end = b;
}

bool tile::isEnd(){
  return end;
}
bool tile::isValid(){
  return valid;
}
bool tile::hasZombie(){
  return zombie;
}
bool tile::hasLife(){
  return life;
}
bool tile::hasBullet(){
  return bullet;
}
bool tile::isBuilding(){
  return building;
}
bool tile::isDoor(){
  return door;
}
bool tile::isStreet(){
  return !isBuilding() && valid;
}
void tile::addPlayer(int p){
  players.insert(p);
}
void tile::removePlayer(int p){
  players.erase(p);
}

