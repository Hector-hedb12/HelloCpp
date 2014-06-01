/*
 * PlayScene.h
 *
 *  Created on: 03/04/2014
 *      Author: hector
 */

#ifndef _PLAYSCENE_H_
#define _PLAYSCENE_H_

#include "cocos2d.h"
#include "CCLayerPanZoom.h"
#include "Resources.h"
#include "Modelo/model.h"
#include "Modelo/decision.h"
#include "PopupLayer.h"
#include <vector>
#include <set>

using namespace std;
USING_NS_CC;

// TAGS utilizados para identificar elementos presente en el layer
static const int MAP_TAG            = 1;
static const int MAP_CARD_DECK = 1;
static const int MAP_CARD_REVERSE = 2;
static const int ROTATE_ICON= 3;
static const int MAP_CARD_SELECTED = 4;
static const int LIFES_LABEL_TAG    = 878;
static const int BULLETS_LABEL_TAG  = 879;
static const int ZOMBIES_LABEL_TAG  = 880;
static const int TITLE_LABEL_TAG    = 881;
static const int SUBTITLE_LABEL_TAG = 882;
static const int LIFE_ICON_TAG      = 883;
static const int BULLET_ICON_TAG    = 884;
static const int ZOMBIE_ICON_TAG    = 885;
static const int QUESTION_LABEL_TAG = 886;
static const int QUESTION_LIFE_ICON_TAG = 887;
static const int QUESTION_BULLET_ICON_TAG = 888;

static const int SELECTED_ZOMBIE_TAG = 889;
static const int SELECTED_PLAYER_TAG = 890;

static const int CONTINUAR_LABEL_TAG = 891;

// Dimensiones utilizadas para calculos del mapa y sus imagenes
static float PIXELS_MAP_CARD;
static float PIXELS_TILE;

/*
 * Pequeña clase auxiliar para para el movimiento de los jugadores
 * y de los zombies
 */
class Event {
public:
	char orientation;
	float movement;
	position end_point;

	Event() {};
};

/*
 * Clase auxiliar para modelar corazones, balas y zombies en el mapa,
 * para poder agregar y remover de la pantalla cuando sea oportuno
 */
class Element {
public:
	CCSprite * sprite;
	int mapCard_i, mapCard_j;
	int tile_i, tile_j;

	Element& operator=(Element const &rhs) {
        sprite = rhs.sprite;
        mapCard_i = rhs.mapCard_i;
        mapCard_j = rhs.mapCard_j;
        tile_i = rhs.tile_i;
        tile_j = rhs.tile_j;
        return *this;
    };
};

/*
 * Clase principal que contiene el flujo del juego y la mayor parte
 * de las funciones, tanto principales como auxiliares del juego
 */
class PlayScene : public cocos2d::CCLayer
{
	public:
	   // Funcion de inicializacion de la escena
	   virtual bool init();
	   // Funcion que crea e inicializa la escena del juego
	   static cocos2d::CCScene* scene();
	   CREATE_FUNC(PlayScene);

	   // Callbacks:
	   // Funciones que manejan
	   void skipMenuCallback(CCObject* pSender);
	   void mainMenuCallback(CCObject* pSender);
	   void moveSpriteMenuCallback(CCObject* pSender);
	   void incrementLifeMenuCallback(CCObject* pSender);
	   void decrementLifeMenuCallback(CCObject* pSender);
	   void incrementBulletMenuCallback(CCObject* pSender);
	   void decrementBulletMenuCallback(CCObject* pSender);
	   void redDiceCallback();
	   void blueDiceCallback();
	   void activateMapCardModeCallback(CCObject* pSender);

	private:
	   // Utilizado para mostrar al usario el Zombie/Player seleccionado:

	   CCSize boxTileSize;
	   CCSize boxMapCardSize;
	   CCLayerColor *boxTile;
	   vector<CCLayerColor*> boxTileAdded;

	   // FIN: Utilizado para mostrar al usario el Zombie/Player seleccionado:

	   // Estado del juego
	   state GameState;

	   CCLayer        *_stayLayer;
	   CCLayerPanZoom *_moveLayer;
	   CCSprite * mSprite[NUM_OF_PLAYER];
	   CCSpriteBatchNode * spritebatch;
	   CCSprite * animatedSprite;
	   vector<Event> events;

	   // Manejo de fases
	   int currFase, currSubFase;
	   bool fase[NUM_FASES];
	   string name[NUM_FASES][NUM_FASES];

	   // Relacionados a la batalla:
	   string msg;

	   // Referente a los toques en pantalla
	   float touchDistance;
	   bool singleTouch;
	   int touches;
	   int currCardRotation;

	   bool WAIT;
	   bool PUTMAPCARD;
	   bool LANZODADOAZUL; // provisional
	   bool HAY_BATALLA;
	   bool LANZODADOROJO; // provisional
	   bool HAY_PREGUNTA;
	   int NUM_OF_ZOMBIES_TO_MOVE; // provisional
	   CCPoint prevZombieLocation;
	   vector<vector<position> > allowedPositions;
	   vector<position> possibleMoves;

	   // Vidas, balas, zombies y dados
	   int lastRedDiceResult, lastBlueDiceResult;
	   vector<CCSprite*> blueDices, redDices;

	   // Para el manejo de los elementos (zombies, vidas y balas)
	   int zombieSpriteToMove;
	   vector<Element> zombies, lifes, bullets;

	   // Para colocar elementos en los layers:
	   // Estas funciones agregan elementos de interfaz y elementos
	   // estaticos y dinamicos del juego
	   void setMenuBackMenu(CCLayer *mLayer);
	   void setOptionsLifeMenu(CCLayer *mLayer);
	   void setOptionsBulletMenu(CCLayer *mLayer);
	   void setMap(CCLayer *mLayer);
	   void setPlayerInfo();
	   void setInterface();
	   void setBackgrounds();
	   void setMoveRedDice(CCNode* node);
	   void setMoveBlueDice(CCNode* node);
	   void setStaticRedDice(CCNode* node);
	   void setStaticBlueDice(CCNode* node);
	   void setSprites(CCLayer* mLayer);
	   void setMoveSprite(CCNode* sender, void* data);
	   void setStaticSprite(CCNode* sender, void* data);
	   void setMoveZombieSprite(CCNode* sender, void* data);
	   void setStaticZombieSprite(CCNode* sender, void* data);
	   void setDices(CCLayer * layer);
	   bool isAllowed(position location);
	   bool putMapCard(CCPoint location);
	   bool canMove(position location);
	   bool canMoveZombie(position prev_p, position p);
	   void removeZombieBox();
	   void showCurrPlayerBox();
	   void addPlayerBox();
	   void removePlayerBox();
	   void addMapCardBox();
	   void removeMapCardBox();

	   // Funciones utilizadas para modificar el estado del jugador
	   void modifyPlayerLifes(int num);
	   void modifyPlayerBullets(int num);
	   void modifyPlayerZombies();
	   void modifyScreenLifes();
	   void modifyScreenBullets();
	   void modifyScreenZombies();

	   // Funciones utilizadas para animar y mover los sprites del
	   // jugador y de los zombies
	   void animateSprite(CCNode* sender, void* data);
	   void animateZombieSprite(CCNode* sender, void* data);

	   // Funciones auxiliares para manejar las transformaciones del modelo del
	   // juego a la interfaz del usuario
	   position relativeTileToAbsoluteTile(int mx, int my, int tx, int ty);
	   CCPoint axisToMapCardMatrix(float x, float y);
	   CCPoint axisToTileMatrix(float x, float y);
	   CCPoint mapCardMatrixToAxis(int i, int j);
	   CCPoint tileMatrixToAxis(int i_mapCard, int j_mapCard, int i, int j);

	   // Funciones utilizadas para transformar puntos cartesianos en
	   // movimientos en los ejes para el movimientos de las criaturas
	   Event getEventZombieMove(CCPoint prev, CCPoint next);
	   void pointToEvent(position p);

	   // Funciones utilizadas para mantener el orden entre las fases
	   // que debe seguir el juego
	   void activateTouch(CCNode* sender, void* data);
	   void changePhase(CCNode* sender, void* data);
	   void changeSubPhase(CCNode* sender, void* data);

	   // Funciones estandares para manejar los toques del usuarios en la pantalla
	   void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	   void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	   void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	   void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	   // Metodos relacionados con las fases del turno
	   void firstPhase(CCPoint pto, CCPoint ptoConvertido);
	   void secondPhase(CCPoint pto, CCPoint ptoConvertido);
	   void thirdPhase(CCPoint pto, CCPoint ptoConvertido);
	   void showFirstPhase();
	   void hideFirstPhase();
	   void showSecondPhase();
	   void hideSecondPhase();
	   void showThirdPhase();
	   void hideThirdPhase();

	   // Metodos relacionados con elegir carta de mapa:
	   void setPickMapCard();
	   void initFlipXCallback(CCObject* pSender);
	   void show_mapCard_selected(CCNode* node);
	   void rotate_mapCard();

	   // Metodos relacionados con una batalla:
	   void putSubtitleInformation(CCNode* sender, void* data);
	   void checkBattle(CCNode* sender, void* data);
	   void checkLeftMoves(CCNode* sender, void* data);
	   void winBattle(CCNode* sender, void * data);
	   void removeSprite(CCNode* sender, void * data);
	   void checkLifeAndBullet(CCNode* sender, void* data);
	   void loseBattleAndDie(CCNode* sender);
	   void continueBattle(CCNode* sender);

	   // FIN DEL JUEGO
	   void gameOver();

	   // popup:
	   void popUpbuttonCallback(CCNode *pNode);
	   void popupLayer(CCNode* sender, void * data);

	   // LLamadas para la maquina
	   void callFirstPhase(CCNode* sender, void * data);
	   void callSecondPhase(CCNode* sender, void * data);
	   void callThirdPhase(CCNode* sender, void * data);

};

#endif /* PLAYSCENE_H_ */
