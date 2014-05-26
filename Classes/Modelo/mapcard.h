#ifndef  _MAPCARD_H_
#define  _MAPCARD_H_

#include <iostream>
#include <algorithm>
#include <vector>
#include "constant.h"
#include "tile.h"
#include "position.h"

using namespace std;

/*
 * Class Map Card
 */
class mapCard{
  bool isEnd; // is end
  bool isBuilding; // is Building
  int nZombie; // Number of Zombies
  int nLife; // Number of lifes
  int nBullet; // Number of bullets
  tile grid[MAPNUMPOS][MAPNUMPOS];
  string path;
  public:
  
  /*
   * Create an empty map card
   */
  mapCard();
  /*_CONSTANT_H_
   * Return the x constant map card.
   * If end is true then it is the last card
   */
  mapCard(int x, bool end = false);
  /*
   * Return the path of the image
   */
  string getPath();
  /*
   * Insert randomly x life points and y bullet points
   */
  void insertRandomLB(int x, int y);
  /*
   * Insert randomly n zombies
   */
  void insertRandomZombie(int n);
  /*
   * Returns true if the map card is a building card
   */
  bool isBuildingCard();
  /*
   * Returns the tile at position p
   */
  tile &getTile(position p);
  /*
   * Returns the tile at position (x,y)
   */
  tile &getTile(int x, int y);

  /*
   * Rotate the map card to the right
   */
  void rotateR();
  /*
   * Rotate the map card to the left
   */
  void rotateL();
  /*
   * Print the map card
   */
  void print();
};


#endif
