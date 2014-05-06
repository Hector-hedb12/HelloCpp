#ifndef  _CONSTANT_H_
#define  _CONSTANT_H_

#include <string>
using namespace std;

namespace constant{
  const int CREP = 50;
  const int MAPMAX = 100;
  const int MAPMID = 50;
  const int MAPNUMPOS = 3;
  const int MAX_LIFE = 5;
  const int PLAYER_INIT_BULLET = 3;
  const int PLAYER_INIT_LIFE = 3;
  const int PLAYER_INIT_ZOMBIES = 0;

  const int MAP_CARD_MAX = 5;
  int vx3[4] = {-3,0,3,0};
  int vy3[4] = {0,-3,0,3};
  int vx1[4] = {-1,0,1,0};
  int vy1[4] = {0,-1,0,1};
  int vi[4] = {2,3,0,1};
  
  
  /*
   * Especification of the map card
   * {nZombie, nLife, nBullet}
   */
  int veMapCards[][3] = 
  {
  {0,0,0},
  {9,0,0},
  {2,0,0},
  {2,0,0},
  {3,0,0}
  };
  
  /*
   * Map Cards constantns
   * * => free position
   * # => blocked position
   * D => building (enter)
   * B => building
   */
  char vMapCards[][3][4] =
  { 
    {
	  "#*#",
	  "***",
	  "#*#"
    },
    {
    "***",
    "***",
    "***"
    },
    {
    "#*#",
    "#*#",
    "#*#"
    },
    {
    "#*#",
    "**#",
    "###"
    },
    {
    "#*#",
    "**#",
    "#*#"
    }
    
    
  };

/*
 * Number of mapcards in the map stack
 */
  int vNMapcard[] = {
	  0,1,2,2,2
  };
/*
 * 0 => not end map card
 * 1 => end map card
 */
  int vEndMapcard[] = {
	0,1,0,0,0
  };

  string mapPath[] = {
    "map/center.png",
	"map/center.png",
	"map/center.png",
	"map/center.png",
	"map/center.png"
  };
  
};

#endif
