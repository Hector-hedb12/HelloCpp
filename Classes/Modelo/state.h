/*
 * Class state
 */
class state{
  vector<user> playerVector;
  vector<mapCard> mapVector;
  mapCard lastMapCard;
  mapGrid world;
  cardHeap pile;
  dice blueDice, redDice, yellowDice;
  int currentPlayer;
public:
  state(){
  }
/*
 * Create a state of nPlayers
 */
  state(int);
/*
 * Print World
 */  
  void printWorld();
/*
 * Next Player's turn
 */  
  void nextTurn();
/*
 * Current player picks a map card.
 */
  mapCard &pickMapCard();
/*
 * Return the last map card picked from the map stack
 */
  mapCard &getLastMapCard();
 /*
  * Rotate R last Map Card
  */
  mapCard &rotateRLastMapCard();
 /*
  * Rotate R last Map Card
  */
  mapCard &rotateLLastMapCard();
/*
 * Current player picks a card.
 */
  void pickCard(int);
/*
 * Use the card idCard of the idPlayer
 */
  void useCard(int, int);
/*
 * Roll the blue dice. (move player)
 */
  int rollPlayerDice();
/*
 * Roll the red dice. (move zombies)
 */
  int rollZombieDice();
/*
 * Roll the yellow dice. (fight with zombies)
 */
  int rollFightDice();
/*
 * Return the last roll player dice
 */
  int getLastRollPlayerDice();
/*
 * Return the last roll zombie dice
 */
  int getLastRollZombieDice();
 /*
  * Return the last roll fight dice
  */
  int getLastRollFightDice();

/*
 * Obtain the posible moves from the current position. It assumes that
 * it is called after the dice rolled
 */
  pair<vector<position>, vector<position> > getPossibleMoves();

  vector<position> getPossibleZombieMoves(position current_position);
/*
 * Move the current player to position to.
 */
  void movePlayerTo(position);
/*
 * Return the event vector of the movement without moving the player
 */
  vector<position> queryMovePlayerTo(position to);
/*
 * Return the number of lifes of the current player
 */
  int getCurrentPlayerLife();
/*
 * Return the number of bullets of the current player
 */
  int getCurrentPlayerBullet();
/*
 * Return the number of zombies of the current player
 */
  int getCurrentPlayerZombie();
/*
 * Add Life point to current player and delete the life
 * in the world if exists
 */  
  void addLife();
/*
 * Add Bullet to current player and delete the bullet
 * in the world if exists
 */  
  void addBullet();
/*
 * Add Zombie point to current player and delete the zombie
 * int the world if exists
 */
  void addZombie();
/*
 * Returns true if exist a life in the position p
 */  
  bool queryLife(position p);
/*
 * Returns true if exist a bullet in the position p
 */
  bool queryBullet(position p);
/*
 * Returns true if exist a zombie in the position p
 */
  bool queryZombie(position p);

  bool isValidZombie(position p);


  /*
   * Returns true if exist a life in the position of the current player
   */
    bool queryLife();
  /*
   * Returns true if exist a bullet in the position  of the current player
   */
    bool queryBullet();
  /*
   * Returns true if exist a zombie in the position  of the current player
   */
    bool queryZombie();
/*
 * Add MapCard ma to the world in the position pos
 */
  void putCardMap(mapCard ma, position pos);
/*
 * Get the positions where is able to put the map card
 */
  vector<position> getPosibleMapCard(mapCard ma);
/*
 * Get a vector with four vectors with all the posible positions
 * where is able to put the map card with every posible rotation of ma
 */
  vector<vector<position> > getAllPosibleMapCard(mapCard ma);
/*
 * Decrease a point of Life of the current player
 */
  void decreaseLife();
/*
* Decrease a point of Bullet of the current player
*/
  void decreaseBullet(int x = 1);
  /*
   * kill the current player
   */
  void killcurrentPlayer();
  
  /*
   * Return the current player id
   */
  int getCurrentPlayer();

  /*
   * Return the position of the current player
   */
  position getCurrentPlayerPosition();

};
