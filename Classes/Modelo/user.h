#ifndef  _USER_H_
#define  _USER_H_

#include <vector>
#include <cassert>
#include "constant.h"
#include "card.h"
#include "strategy.h"

using namespace std;

class user{
protected:
  int nLife;
  int nBullet;
  int nZombie;
  int leftMoves;
  int leftZombieMoves;
  bool ismachine;

  vector<card> handCardVector;
  vector<card> tableCardVector;

  strategy stm;

public:
  /*
   * Create an empty user
   */
  user(bool b = false);
  /*
   * Return true if the user is a machine
   */
  bool isMachine();
  /*
   * Increase a life point
   */
  void increaseLife();
  /*
   * Decrease a life point
   */
  void decreaseLife();
  /*
   * Increaase a bullet point
   */
  void increaseBullet();
  /*
   * Decrease a bullet point
   */
  void decreaseBullet();
  /*
   * Increase a zombie point
   */
  void increaseZombie();
  /*
   * Decrease a zombie point
   */
  void decreaseZombie();
  /*
   * Return the number of life points
   */
  int getLife();
  /*
   * Return the number of bullet points
   */
  int getBullet();
  /*
   * Return the number of zombie points
   */
  int getZombie();
  /*
   * Push the card ca in the list of the player
   */
  void pushHandCard(card ca);
  /*
   * Get the card idCard
   */
  card& getHandCard(int idCard);
  /*
   * Erase the card idCard of the hand
   */
  bool eraseHandCard(int idCard);
  /*
   * Set x left moves
   */
  void setLeftMoves(int x);
  /*
   * Set x left moves
   */
  void setLeftZombieMoves(int x);
  /*
   * Get the number of left moves
   */
  int getLeftMoves();
  /*
   * Get the number of left moves
   */
  int getLeftZombieMoves();
  
  /*
   * Init the user
   */
  void init();
  /*
   * Init the user after get killed
   */
  void die();

  strategy& getStrategy();
  void selectStrategy();
};




#endif
