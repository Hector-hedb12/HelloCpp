#ifndef  _TILE_H_
#define  _TILE_H_

#include <set>
using namespace std;

/*
 * Class Tile
 */
class tile{
  bool bullet;
  bool life;
  bool zombie;
  bool valid;
  bool building;
  bool door;
  bool end;
  int id;
  set<int> players;
public:
  /*
   * Create an empty tile
   */
  tile();
  /*
   * Set Bullet to b
   */
  void setBullet(bool b);
  /*
   * Set Life to b
   */
  void setLife(bool b);
  /*
   * Set Zombie to b
   */
  void setZombie(bool b);
  /*
   * Set Valid to b
   */
  void setValid(bool b);
  /*
   * Set Building to b
   */
  void setBuilding(bool b);
  /*
   * Set door to b
   */
  void setDoor(bool b);
  /*
   * Set end to b
   */
  bool setEnd(bool b);
  /*
   * return true if the tile is the end
   */
  bool isEnd();
  /*
   * return true if the tile is valid
   */
  bool isValid();
  /*
   * return true if the tile has a zombie
   */
  bool hasZombie();
  /*
   * return true if the tile has a life
   */
  bool hasLife();
  /*
   * return true if the tile has a bullet
   */
  bool hasBullet();
  /*
   * return true if the tile is a building
   */
  bool isBuilding();
  /*
   * return true if the tile is a door builing
   */
  bool isDoor();
  /*
   * return true if the tile is street
   */
  bool isStreet();
  /*
   * add player p into the tile
   */
  void addPlayer(int p);
  /*
   * remove the player p from the tile
   */
  void removePlayer(int p);
  /*
   * Returns the tile id
   */
  int getId();
  /*
   * Set the tile Id
   */
  void setId(int i);

};

#endif
