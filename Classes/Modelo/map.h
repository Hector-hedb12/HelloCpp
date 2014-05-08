#ifndef  _MAP_H_
#define  _MAP_H_

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include "tile.h"
#include "position.h"
#include "constant.h"
#include "user.h"
#include "mapcard.h"

using namespace constant;
using namespace std;

/*
 * Class that represent the map
 */
class mapGrid{
  tile grid[MAPMAX][MAPMAX]; // map Grid matrix
  set<position> freeMapCardPosition; // Set of free positions for map cards
  set<position> lifeSet; // Life points positions set
  set<position> bulletSet; // Bullet positions set
  set<position> zombieSet; // Zombies positions set
  vector<position> playerVector; // players positions vector
  
  /*
   * DFS auxiliar function
   */
  vector<position> dfsMoveRes;
  set<position> dfsMoveVisit;
  bool dfsMove(position u, position end){
    if(u == end){
      return true;
    }

    position v;
    for(int i = 0; i < 4; i++){
      v = u.next(i);
      if(!getTile(v).isValid() || dfsMoveVisit.count(v) > 0) continue;
      dfsMoveRes.push_back(v);
      if(dfsMove(v, end, us, b)){
        return true;
      }
      dfsMoveRes.pop_back();
    }
    return false;
  }
  
public:
  
  void print(){
    for(int i = -6; i <= 6; i++){
      for(int j = -6; j <= 6; j++){
        cout << (getTile(i,j).isStreet()?"*":"#");
      }cout << endl;
    }
  }
  
  /*
   * Return the vector of positions without moving the player
   */
  vector<position> queryMovePlayerTo(int x, position end);
  
  /*
   * Move player x to position end and return a vector of positionss
   */
  vector<position> movePlayerTo(int x, position end, user &us);
  /*
   * Returns true if the position p is inside the table
   */
  bool isValid(position p);
  /*
   * Insert the map card ma at position pos assuming that it is posible
   */
  void insertMapCard(mapCard ma, position pos);
  /*
   * Get a list of positions where is able to put the map card ma
   */
  vector<position> getPossibleMapCard(mapCard ma);
  /*
   * Set tile t at position p
   */
  void setTile(position p, tile t);
  /*
   * Get Tile at position (x,y)
   */
  tile& getTile(int x, int y);
  /*
   * Get Tile at position p
   */
  tile& getTile(position p);
  /*
   * Set Life
   */
  void setLife(position p, bool b);
  /*
   * Set Bullet
   */
  void setBullet(position p, bool b);
  /*
   * Set Zombie
   */
  void setZombie(position p, bool b);
  /*
   * Returns true if a criature is movable from u to the next position i.
   */
  bool isValidMove(position u, int i);
  /*
   * Returns true if a criature is movable from u to the position v.
   */
  bool isValidMove(position u, position v);
  /*
   * Generate a vector with the posible positions that a criature can move
   * using nMoves of steps.
   * if zomb is false it will evade zombies. In other case it wont.
   */
  vector<position> getPossibleMoves(position p, int nMoves, bool zomb = false);
  /*
   * Get the positions where is able to put objects
   */
  vector<position> getPosibleObjectPositions();
  bool moveZombie(position u, position v);
  /*
   * Init the world with the initial mapCard given the number of players
   */
  void init(int nPlayers);
  /*
   * Returns the player num position
   */
  position getPlayerPosition(int num);
  
};

#endif
