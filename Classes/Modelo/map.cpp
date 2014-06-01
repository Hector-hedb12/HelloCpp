#include <map>
#include "map.h"

position mapGrid::getPlayerPosition(int x){
  return playerVector[x];
}

void mapGrid::init(int nPlayers){
	this->hasHeliport = false;
	this->idCounter = 0;
  for(int i = 0; i < MAPMAX; i++){
	  for(int j = 0; j < MAPMAX; j++){
		  grid[i][j] = tile();
	  }
  }
  this->freeMapCardPosition.clear();
  this->lifeSet.clear();
  this->bulletSet.clear();
  this->zombieSet.clear();
  playerVector.clear();


  for(int i = -1; i <= 1; i++){
    tile &t = getTile(i,0);
    t.setValid(1);
  }
  for(int j = -1; j <= 1; j++){
    tile &t = getTile(0,j);
    t.setValid(1);
  }
  tile &t = getTile(0,0);
  for(int i = 0; i < nPlayers; i++){
    t.addPlayer(i);
    playerVector.push_back(position(0,0));
  }
  for(int i = 0; i < 4; i++){
    freeMapCardPosition.insert(position(vx3[i], vy3[i]));
  }

  idCounter = 0;
  for(int i = -1; i <= 1; i++){
	  for(int j = -1; j <= 1; j++){
		  getTile(i,j).setId(idCounter);
	  }
  }
  idCounter = 1;
  hasHeliport = false;

}


void mapGrid::killPlayer(int x){
	playerVector[x] = position(0,0);
}

void mapGrid::movePlayerTo(int x, position end){
  position u = playerVector[x];
  playerVector[x] = end;
  tile &t1 = getTile(u);
  tile &t2 = getTile(end);
  t1.removePlayer(x);
  t2.addPlayer(x);
}

bool mapGrid::isValid(position p){
  int x = p.x + MAPMID;
  int y = p.y + MAPMID;
  return (0 <= x && x < MAPMAX && 0 <= y && y <= MAPMAX);
}

void mapGrid::insertMapCard(mapCard ma, position pos){
  assert(freeMapCardPosition.count(pos) > 0);
  freeMapCardPosition.erase(pos);
  
  
  for(int i = -1; i <= 1; i++){
    for(int j = -1; j <= 1; j++){
      position x = pos + position(i,j);
      setTile(x, ma.getTile(i,j));
    }
  }
  
  for(int i = -1; i <= 1; i++){
    for(int j = -1; j <= 1; j++){
      position u = pos + position(i,j);
      if(getTile(u).hasZombie()) zombieSet.insert(u);
      if(getTile(u).hasLife()) lifeSet.insert(u);
      if(getTile(u).hasBullet()) bulletSet.insert(u);
      getTile(u).setId(idCounter);
    }
  }
  idCounter++;
  
  for(int k = 0; k < 4; k++){
    position x1 = position(vx1[k], vy1[k]) + pos;
    position x2 = position(vx3[k], vy3[k]) + pos;
    if(!getTile(x1).isStreet()) continue;
    if(!isValid(x2) || getTile(x2).isValid()) continue;
    freeMapCardPosition.insert(x2);
  }
  if(ma.isEndCard()){
	  this->hasHeliport = true;
	  endPosition = pos;
  }
  
  
  
}

vector<position> mapGrid::getPossibleMapCard(mapCard ma){
  bool p[4];
  position w;
  for(int k = 0; k < 4; k++){
    w = position(vx1[k], vy1[k]);
    
    p[k] = ma.getTile(w).isStreet();
  }
  /*for(int i = -1; i <= 1; i++){
    for(int j = -1; j <= 1; j++){
      cout << (ma.getTile(i,j).isStreet()?'*':'#');
    }cout << endl;
  }*/
  
  vector<position> res;
  position u,v;
  
  for(set<position>::iterator it = freeMapCardPosition.begin(); it != freeMapCardPosition.end(); it++){
    u = *it;
    if(!isValid(u) && getTile(u).isValid()) continue;
    
    bool q = true;
    int num = 0;
    for(int k = 0; k < 4; k++){
      
      v = position(u.x+vx3[k]+vx1[vi[k]], u.y+vy3[k]+vy1[vi[k]]);
      w = position(u.x+vx3[k], u.y+vy3[k]);
      if(!getTile(w).isValid()) continue; // if center is invalid no map card
      if(getTile(v).isStreet() != p[k]){
        q = false;
        break;
      }
      num++; 
    }
    if(num != 0 && q) res.push_back(u);
  }
  return res;
}

void mapGrid::setTile(position p, tile t){
  grid[p.x+MAPMID][p.y+MAPMID] = t;
}
tile& mapGrid::getTile(int x, int y){
  return grid[x+MAPMID][y+MAPMID];
}
tile& mapGrid::getTile(position p){
  return grid[p.x+MAPMID][p.y+MAPMID];
}
void mapGrid::setLife(position p, bool b){
  getTile(p).setLife(b);
  if(b){
	  this->lifeSet.insert(p);
  }else{
	  this->lifeSet.erase(p);
  }
}
void mapGrid::setBullet(position p, bool b){
  getTile(p).setBullet(b);
  	if(b){
  	  this->bulletSet.insert(p);
    }else{
  	  this->bulletSet.erase(p);
    }
}
void mapGrid::setZombie(position p, bool b){
  getTile(p).setZombie(b);
  if(b){
  	  this->zombieSet.insert(p);
    }else{
  	  this->zombieSet.erase(p);
    }
}

bool mapGrid::isValidMove(position u, int i){
  return isValidMove(u,u.next(i));
}
bool mapGrid::isValidMove(position u, position v){
  tile tu,tv;
  tu = getTile(u); tv = getTile(v);
  if((tu.isBuilding() != tv.isBuilding()) && !tu.isDoor() && !tv.isDoor()) return false;
  if(!tv.isValid()) return false;
  if(tu.isBuilding() && tv.isBuilding() && tu.getId() != tv.getId()) return false;
  return true;
}

vector<position> mapGrid::getPossibleMoves(position p, int nMoves, bool zomb){
  position u, v;
  int du, dv;
  vector<position> res;
  set<position> visit;
  queue<pair<position, int> > Q;
  
  Q.push(make_pair(p,0));
  visit.insert(p);
  
  while(!Q.empty()){
    u = Q.front().first;
    du = Q.front().second;
    Q.pop();
    
    dv = du + 1;
    if(dv > nMoves) continue;
    for(int i = 0; i < 4; i++){
      v = u.next(i);
      if(!isValidMove(u,v) || visit.count(v) > 0) continue;
      res.push_back(v);
      visit.insert(v);

      if((zomb && getTile(v).hasZombie()))continue;
      Q.push(make_pair(v, dv));
    }
  }
  return res;
}
vector<position> mapGrid::queryMovePlayerTo(int x, position end){
	vector<position> res;
	set<position> visit;
	map<position, position> pred;
	queue<position> Q;
	position u, v;
	v = playerVector[x];
	Q.push(v);
	visit.insert(v);
	bool found = false;
	while(!Q.empty()){
		u = Q.front();
		Q.pop();
		if(u == end){
		  found = true;
		  break;
		}
		for(int i = 0; i < 4; i++){
			v = u.next(i);
			if(!isValidMove(u,v) || visit.count(v) > 0) continue;
			visit.insert(v);
			pred[v] = u;
			Q.push(v);
		}
	}
	assert(found);
	if(!found) return res;

	v = end;
	while(v != playerVector[x]){
		res.push_back(v);
		v = pred[v];
	}
	res.push_back(v);
	reverse(res.begin(), res.end());
	return res;
}

vector<position> mapGrid::getPossibleZombieMoves(position pos){
	position v;
	vector<position> res;
	for(int i = 0; i < 4; i++){
		v = pos.next(i);
		if(!isValidMove(pos,v) || this->getTile(v).hasZombie()) continue;
		res.push_back(v);
	}
	return res;
}

vector<position> mapGrid::getPosibleObjectPositions(){
  position u, v;
  u = position(0,0);
  vector<position> res;
  set<position> visit;
  stack<position> Q;
  while(!Q.empty()){
    u = Q.top();
    Q.pop();
    if(!getTile(u).hasBullet() && !getTile(u).hasLife() && getTile(u).isBuilding()){
      res.push_back(u);
    }
    for(int i = 0; i < 4; i++){
      v = u.next(i);
      if(!getTile(v).isValid() || visit.count(v) > 0) continue;
      Q.push(v);
    }
  }
  return res;
}

bool mapGrid::moveZombie(position u, position v){
  assert(getTile(u).hasZombie() && !getTile(v).hasZombie());
  getTile(u).setZombie(false);
  getTile(v).setZombie(true);
  return true;
}

bool mapGrid::hasEndCard(){
  return hasHeliport;
}


map<position,int> mapGrid::getDistances(position p){
	vector<int> res;
	set<position> visit;
	map<position, int> dist;
	queue<position> Q;
	position u, v;
	int du;
	v = p;
	Q.push(v);
	visit.insert(v);
	dist[v] = 0;
	while(!Q.empty()){
		u = Q.front();
		du = dist[u];
		Q.pop();

		for(int i = 0; i < 4; i++){
			v = u.next(i);
			if(!isValidMove(u,v) || visit.count(v) > 0) continue;
			visit.insert(v);
			dist[v] = du+1;
			Q.push(v);
		}
	}

	return dist;
}
