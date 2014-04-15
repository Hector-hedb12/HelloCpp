/*
 * PlayScene.cpp
 *
 *  Created on: 03/04/2014
 *      Author: hector
 */

#include "PlayScene.h"
#include "MenuScene.h"
#include "VisibleRect.h"
#include "support/CCPointExtension.h"
#include <cmath>

CCScene* PlayScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    PlayScene *layer = PlayScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PlayScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    for (int i = 0; i < NUM_OF_PLAYER; i++)
    	mSprite[i] = CCSprite::create(PATH_PLAYER_SPRITE[i].c_str());

    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();

    for (int i = 0; i < NUM_OF_PLAYER; i++){
    	for (int j = 0; j < 4; j++)
    		cache->addSpriteFramesWithFile(("sprite/"+NAME_PLAYER_SPRITE_ANIMATION[i][j]+".plist").c_str(),
                                           ("sprite/"+NAME_PLAYER_SPRITE_ANIMATION[i][j]+".png").c_str());
    }

    offset_x[0] = -20;
    offset_x[1] = 20;

    // Inicia variables:
    _moveLayer = CCLayerPanZoom::create();
    _stayLayer = CCLayer::create();
    player = -1;

    // Coloca elementos sobre layer estatico
    setMenuBackMenu(_stayLayer);
    setMoveSpriteMenu(_stayLayer);
    setLifeIcons(3,_stayLayer);
    setBulletIcons(2,_stayLayer);

    // Coloca elementos sobre el layer dinamico
    setTileMap(_moveLayer);
    setSprites(_moveLayer);

    // Habilita eventos touch
    _moveLayer->setTouchEnabled( true );

    // Añade layers al layer principal
    addChild(_moveLayer);
    addChild(_stayLayer);

    return true;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *           Funciones Callbacks                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * */

void PlayScene::mainMenuCallback(CCObject* pSender)
{
	CCScene* pScene = MenuScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void PlayScene::moveSpriteMenuCallback(CCObject* pSender)
{
	player++;

	if (player >= NUM_OF_PLAYER )
		player = 0;

	// Obtenemos el "touch" del usuario y lo enviamos al Modelo para que nos de el
	// arreglo con los eventos
	events.clear();
	Event e;
	e.orientation = 'H';
	e.movement = 40.0;
	events.push_back(e);
	e.orientation = 'V';
	e.movement = 20.0;
	events.push_back(e);
	e.orientation = 'H';
	e.movement = -10.0;
	events.push_back(e);
	// FIN DEL SUPER CABLE

	CCArray *actions = CCArray::createWithCapacity(4 * events.size());
	CCFiniteTimeAction* single_action;

	for (int i = 0; i < events.size(); i++) {
		single_action = CCCallFuncND::create( this, callfuncND_selector(PlayScene::setMoveSprite), (void*) &events[i]);
		actions->addObject(single_action);

		single_action = CCCallFuncND::create( this, callfuncND_selector(PlayScene::animateSprite), (void*) &events[i]);
		actions->addObject(single_action);

		actions->addObject(CCDelayTime::create(abs(events[i].movement) / VELOCITY_SPRITE_MOVEMENT));

		single_action = CCCallFuncND::create( this, callfuncND_selector(PlayScene::setStaticSprite), (void*) &events[i]);
		actions->addObject(single_action);
	}

	CCSequence *sq = CCSequence::create(actions);

	mSprite[player]->runAction( sq );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * *
 * Funciones para colocar elementos en los layers  *
 * * * * * * * * * * * * * * * * * * * * * * * * * */

void PlayScene::setMenuBackMenu(CCLayer *mLayer)
{
	CCLabelTTF* label = CCLabelTTF::create(MAIN_MENU_STRING.c_str(), "Arial", FONT_SIZE);
	CCMenuItemLabel* itemLabel = CCMenuItemLabel::create(label,this,menu_selector(PlayScene::mainMenuCallback));

	itemLabel->setPosition(ccp(VisibleRect::rightBottom().x - itemLabel->getContentSize().width/2 -10,
                               VisibleRect::rightBottom().y + itemLabel->getContentSize().height/2 +10));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(itemLabel, NULL);
    pMenu->setPosition(CCPointZero);
    mLayer->addChild(pMenu, 1);
}

void PlayScene::setMoveSpriteMenu(CCLayer *mLayer)
{
	CCLabelTTF* label = CCLabelTTF::create("Mover Sprite", "Arial", FONT_SIZE);
	CCMenuItemLabel* itemLabel = CCMenuItemLabel::create(label,this,menu_selector(PlayScene::moveSpriteMenuCallback));

	itemLabel->setPosition(ccp(VisibleRect::rightBottom().x - itemLabel->getContentSize().width/2 -10,
                               VisibleRect::rightBottom().y + itemLabel->getContentSize().height/2 +50));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(itemLabel, NULL);
    pMenu->setPosition(CCPointZero);
    mLayer->addChild(pMenu, 2);
}

void PlayScene::setTileMap(CCLayer *mLayer)
{
	CCTMXTiledMap *map = CCTMXTiledMap::create("map/prueba.tmx");
	mLayer->addChild(map, 0, MAP_TAG);

    CCSize CC_UNUSED s = map->getContentSize();
    CCLOG("ContentSize: %f, %f", s.width,s.height);

    CCArray* pChildrenArray = map->getChildren();
    CCSpriteBatchNode* child = NULL;
    CCObject* pObject = NULL;

    CCARRAY_FOREACH(pChildrenArray, pObject)
    {
        child = (CCSpriteBatchNode*)pObject;

        if(!child)
            break;

        child->getTexture()->setAntiAliasTexParameters();
    }

    map->setScale(1.5f);
    map->setAnchorPoint( ccp(0.5f, 0.5f) );
}

void PlayScene::setLifeIcons(int num, CCLayer* mLayer)
{
	CCSprite* lifeIcon;
	int x_offset = 8;
	for (int i = 0; i < num; i++){
		lifeIcon = CCSprite::create("lifeIcon.png");
		lifeIcon->setPosition(ccp(VisibleRect::leftTop().x + x_offset + lifeIcon->getContentSize().width/2,
                                  VisibleRect::leftTop().y - lifeIcon->getContentSize().height/2 - 10));
		mLayer->addChild(lifeIcon, i);

		x_offset += lifeIcon->getContentSize().width + 8;
	}
}

void PlayScene::setBulletIcons(int num, CCLayer* mLayer)
{
	CCSprite* bulletIcon;
	int x_offset = 8;
	for (int i = 0; i < num; i++){
		bulletIcon = CCSprite::create("bulletIcon.png");
		bulletIcon->setPosition(ccp(VisibleRect::rightTop().x - x_offset - bulletIcon->getContentSize().width/2,
                                  VisibleRect::rightTop().y - bulletIcon->getContentSize().height/2 - 10));
		mLayer->addChild(bulletIcon, i);

		x_offset += bulletIcon->getContentSize().width + 8;
	}
}

void PlayScene::setSprites(CCLayer* mLayer)
{
	for(int i = 0; i < NUM_OF_PLAYER; i++){
		mSprite[i]->setPosition(ccp(VisibleRect::center().x + offset_x[i], VisibleRect::center().y));
		mLayer->addChild(mSprite[i]);
	}
}

void PlayScene::setMoveSprite(CCNode* sender, void* data) {
	CCLOG ("MOVE SPRITE");
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	Event *d =  (Event*) data;
	string name;

	if (d->orientation == 'H')
		if (d->movement > 0)
			name = NAME_PLAYER_SPRITE_ANIMATION[player][RIGHT];
		else
			name = NAME_PLAYER_SPRITE_ANIMATION[player][LEFT];
	else
		if (d->movement > 0)
			name = NAME_PLAYER_SPRITE_ANIMATION[player][UP];
		else
			name = NAME_PLAYER_SPRITE_ANIMATION[player][DOWN];

	animatedSprite = CCSprite::createWithSpriteFrameName( (name +"_1.png").c_str() );
	animatedSprite->setPosition( ccp( mSprite[player]->getPositionX(), mSprite[player]->getPositionY()) );
	mSprite[player]->setVisible(false);

	spritebatch = CCSpriteBatchNode::create( ("sprite/" +name +".png" ).c_str());
	spritebatch->addChild(animatedSprite);

	_moveLayer->addChild(spritebatch);

	CCArray* animFrames = CCArray::createWithCapacity(10);

	char str[250] = {0};
	for(int i = 1; i < 10; i++)
	{
		sprintf(str, "%s_%d.png", name.c_str(), i);
		CCSpriteFrame* frame = cache->spriteFrameByName( str );
		animFrames->addObject(frame);
	}

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
	animatedSprite->runAction( CCRepeatForever::create( CCAnimate::create(animation) ) );

	animatedSprite->setFlipX(false);
	animatedSprite->setFlipY(false);
}

void PlayScene::animateSprite(CCNode* sender, void* data) {
	CCLOG ("ANIMATE SPRITE");
	CCFiniteTimeAction* actionMove;
	Event *d =  (Event*) data;
	float offX = 0, offY = 0;

	CCLOG("movmiento: %f orientacion: %c", d->movement, d->orientation);

	if (d->orientation== 'H')
		offX += d->movement;
	else
		offY += d->movement;

	actionMove = CCMoveTo::create( ( abs(d->movement)) / VELOCITY_SPRITE_MOVEMENT,
									ccp(mSprite[player]->getPositionX() + offX, mSprite[player]->getPositionY() +offY));

	CCLOG("desde punto (x,y): (%f,%f) ", mSprite[player]->getPositionX(), mSprite[player]->getPositionY());

	mSprite[player]->setPosition(ccp(mSprite[player]->getPositionX() + offX, mSprite[player]->getPositionY()+ offY));

	CCLOG("al punto (x,y): (%f,%f) ", mSprite[player]->getPositionX() + offX, mSprite[player]->getPositionY()+ offY);

	animatedSprite->runAction(actionMove);
}

void PlayScene::setStaticSprite(CCNode* sender, void* data) {
	CCLOG ("SET STATIC");
	if (spritebatch != NULL) {
		_moveLayer->removeChild(spritebatch);
		spritebatch = NULL;
		animatedSprite = NULL;
	}

	mSprite[player]->setVisible(true);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *     Funciones para llevar a cabo acciones       *
 * * * * * * * * * * * * * * * * * * * * * * * * * */

void PlayScene::removeSprite()
{
//    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
//    for (int i = 0; i < NUM_OF_PLAYER; i++){
//    	cache->removeSpriteFramesFromFile(PATH_PLAYER_SPRITE_ANIMATION[i][0].c_str()+".plist");
//    	cache->removeSpriteFramesFromFile(PATH_PLAYER_SPRITE_ANIMATION[i][1].c_str()+".plist");
//    	cache->removeSpriteFramesFromFile(PATH_PLAYER_SPRITE_ANIMATION[i][2].c_str()+".plist");
//    	cache->removeSpriteFramesFromFile(PATH_PLAYER_SPRITE_ANIMATION[i][3].c_str()+".plist");
//    }
}
