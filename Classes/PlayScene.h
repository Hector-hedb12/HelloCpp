/*
 * PlayScene.h
 *
 *  Created on: 03/04/2014
 *      Author: hector
 */

#ifndef PLAYSCENE_H_
#define PLAYSCENE_H_

#include "cocos2d.h"
#include "CCLayerPanZoom.h"
#include "Resources.h"
#include <vector>

using namespace std;

USING_NS_CC;

const int MAP_TAG = 1;

class Event {
public:
	char orientation;
	float movement;

	Event() {};
};

class PlayScene  : public cocos2d::CCLayer
{
	public:
	   virtual bool init();
	   static cocos2d::CCScene* scene();
	   CREATE_FUNC(PlayScene);

	   // Callbacks:
	   void mainMenuCallback(CCObject* pSender);
	   void moveSpriteMenuCallback(CCObject* pSender);

	private:
	   CCLayer        *_stayLayer;
	   CCLayerPanZoom *_moveLayer;
	   CCSprite * mSprite[NUM_OF_PLAYER];
	   CCSpriteBatchNode * spritebatch;
	   CCSprite * animatedSprite;
	   vector<Event> events;

	   int player;
	   int offset_x[NUM_OF_PLAYER];

	   // Para colocar elementos en los layers:
	   void setMenuBackMenu(CCLayer *mLayer);
	   void setMoveSpriteMenu(CCLayer *mLayer);
	   void setTileMap(CCLayer *mLayer);
	   void setLifeIcons(int num, CCLayer* mLayer);
	   void setBulletIcons(int num, CCLayer* mLayer);
	   void setSprites(CCLayer* mLayer);
	   void setMoveSprite(CCNode* sender, void* data);
	   void animateSprite(CCNode* sender, void* data);
	   void setStaticSprite(CCNode* sender, void* data);
	   //void setSpriteAnimationAndMove(CCLayer *mLayer, int x_0, int y_0, int x_1, int y_1);

	   // acciones:
	   void removeSprite();
};

#endif /* PLAYSCENE_H_ */
