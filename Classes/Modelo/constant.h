#ifndef  _CONSTANT_H_
#define  _CONSTANT_H_

#include <string>
using namespace std;

  const int MAPMAX = 180;
  const int MAPMID = 90;
  const int MAX_MAP_DIM = 60;
  const int MAPNUMPOS = 3;
  const int MAX_LIFE = 5;
  const int PLAYER_INIT_BULLET = 3;
  const int PLAYER_INIT_LIFE = 3;
  const int PLAYER_INIT_ZOMBIES = 0;

  const int MAP_CARD_MAX = 1;
  const int vx3[4] = {-3,0,3,0};
  const int vy3[4] = {0,-3,0,3};
  const int vx1[4] = {-1,0,1,0};
  const int vy1[4] = {0,-1,0,1};
  const int vi[4] = {2,3,0,1};
  
  
  /*
   * Especification of the map card
   * {nZombie, nLife, nBullet}
   */
  const int veMapCards[][3] =
  {
  {0,0,0}, // Plaza Central
  {9,0,0}, // Helipuerto
  {2,0,2}, // Tienda de Excedentes Militares
  {3,0,1}, // Tienda de Monopatines
  {8,4,0}, // Hospital
  {6,2,4}, // Comisaria de Policia
  {3,1,2}, // Gasolinera
  {3,1,1}, // Jugueteria
  {3,3,0}, // Farmacia
  {3,1,1}, // Floristeria
  {6,4,2}, // Parque de Bomberos
  {3,1,2}, // Ferreteria
  {6,2,4}, // Tienda de Deportes
  {6,2,3}, // Casa y Jardin
  {2,0,0}, // Esquina
  {4,0,0}, // Cruz
  {3,0,0}, // T
  {2,0,0}  // Horizontal (Recta)
  };
  
  const int typeMapCards[] =  { 10,1,1,1,1,1,1,1,1,1,1,1,1,1,4,4,4,4 };

  /*
   * Map Cards constantns
   * * => free position
   * # => blocked position
   * D => building (enter)
   * B => building
   */
  const char vMapCards[][3][4] =
  {
    { // Plaza Central
      "#*#",
      "***",
      "#*#"
    },
    { // Helipuerto
      "***",
      "***",
      "***"
    },
    { // Tienda de Excedentes Militares
      "BDB",
      "***",
      "###"
    },
    { // Tienda de Monopatines
      "BDB",
      "***",
      "#*#"
    },
    { // Hospital
      "BBB",
      "BDB",
      "B*B"
    },
    { // Comisaria de Policia
      "BBB",
      "BDB",
      "#*#"
    },
    { // Gasolinera
      "BDB",
      "***",
      "#*#"
    },
    { // Jugueteria
      "BDB",
      "D**",
      "B*#"
    },
    { // Farmacia
      "BDB",
      "***",
      "###"
    },
    { // Floristeria
      "BDB",
      "***",
      "#*#"
    },
    { // Parque de Bomberos
      "BBB",
      "BDB",
      "#*B"
    },
    { // Ferreteria
      "BDB",
      "D**",
      "B*#"
    },
    { // Tienda de Deportes
      "BBB",
      "BDB",
      "#*B"
    },
    { // Casa y Jardin
      "BBB",
      "BDB",
      "B*B"
    },
    { // Esquina
      "###",
      "#**",
      "#*#"
    },
    { // Cruz
      "#*#",
      "***",
      "#*#"
    },
    { // T
      "###",
      "***",
      "#*#"
    },
    { // Horizontal (Recta)
      "###",
      "***",
      "###"
    }
  };

  const string mapPath[] = {
    "map/center.png",
    "map/center.png",
    "map/center.png",
    "map/center.png",
    "map/center.png",
    "map/center.png",
    "map/center.png",
    "map/center.png",
    "map/center.png",
    "map/center.png",
    "map/center.png",
    "map/center.png",
    "map/center.png",
    "map/center.png",
    "map/center.png",
    "map/center.png",
    "map/center.png",
    "map/center.png"
  };

#endif
