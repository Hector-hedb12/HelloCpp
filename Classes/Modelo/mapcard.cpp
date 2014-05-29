#include "mapcard.h"

mapCard::mapCard(){
  nLife = nBullet = nZombie = 0;
  isEnd = 0;
}

mapCard::mapCard(int x, bool end){
  bool isB = false;
  for(int i = 0; i < MAPNUMPOS; i++){
    for(int j = 0; j < MAPNUMPOS; j++){
      if(vMapCards[x][i][j] == 'B' || vMapCards[x][i][j] == 'D') isB = true;
    }
  }
  
  for(int i = 0; i < MAPNUMPOS; i++){
    for(int j = 0; j < MAPNUMPOS; j++){
      grid[i][j].setValid(vMapCards[x][i][j] != '#');
      grid[i][j].setBuilding(vMapCards[x][i][j] == 'B' || vMapCards[x][i][j] == 'D');
      grid[i][j].setDoor(vMapCards[x][i][j] == 'D');
    }
  }
  isEnd = end;
  isBuilding = isB;
  grid[1][1].setEnd(end);
  nZombie = veMapCards[x][0];
  nLife = veMapCards[x][1];
  nBullet = veMapCards[x][2];
  
  if(isEnd){
	  for(int i= 0; i < MAPNUMPOS; i++){
		  for(int j = 0; j < MAPNUMPOS; j++){
			  grid[i][j].setZombie(true);
		  }
	  }
  }else if(!isB){
    int x, y;
    for(int k = 0; k < 4; k++){
      x = 1 + vx1[k];
      y = 1 + vy1[k];
      if(!grid[x][y].isValid()) continue;
      grid[x][y].setZombie(grid[x][y].isStreet());
    }
  }else{
    insertRandomZombie(nZombie);
    insertRandomLB(nLife, nBullet);
  }

  path = mapPath[x];
}

void mapCard::insertRandomLB(int x, int y){
  vector<position> ve;
  for(int i = 0; i < MAPNUMPOS; i++){
    for(int j = 0; j < MAPNUMPOS; j++){
      if(grid[i][j].isValid()) ve.push_back(position(i,j));
    }
  }
  random_shuffle(ve.begin(), ve.end());
  int xy = x+y;
  for(int i = 0; i < xy; i++){
    if(i < x){
      grid[ve[i].x][ve[i].y].setLife(true);
    }else{
      grid[ve[i].x][ve[i].y].setBullet(true);
    }
  }
}

void mapCard::insertRandomZombie(int n){
  vector<position> ve;
  for(int i = 0; i < MAPNUMPOS; i++){
    for(int j = 0; j < MAPNUMPOS; j++){
      if(grid[i][j].isValid()) ve.push_back(position(i,j));
    }
  }
  random_shuffle(ve.begin(), ve.end());
  for(int i = 0; i < n; i++){
    grid[ve[i].x][ve[i].y].setZombie(true);
  }
}

bool mapCard::isBuildingCard(){
  return isBuilding;
}

tile &mapCard::getTile(position p){
  return grid[p.x+1][p.y+1];
}
tile &mapCard::getTile(int x, int y){
  return grid[x+1][y+1];
}

string mapCard::getPath(){
  return path;
}

void mapCard::rotateL(){
  tile g2[MAPNUMPOS][MAPNUMPOS];
  int n = MAPNUMPOS;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      g2[n-1-j][i] = grid[i][j];
    }
  }
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      grid[i][j] =  g2[i][j];
    }
  }
}
void mapCard::rotateR(){
  rotateL();
  rotateL();
  rotateL();
}
void mapCard::print(){
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      cout << (grid[i][j].isStreet()?"*":"#");
    }cout << endl;
  }
}

