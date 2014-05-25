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
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

CCScene* PlayScene::scene()
{
    CCScene *scene = CCScene::create();
    PlayScene *layer = PlayScene::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool PlayScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    GameState = state(NUM_OF_PLAYER);

    CCSprite * mapCardSprite = CCSprite::create(mapPath[0].c_str());
    PIXELS_MAP_CARD = mapCardSprite->getTextureRect().size.height;
    PIXELS_TILE = PIXELS_MAP_CARD/3.0;

    for (int i = 0; i < NUM_OF_PLAYER; i++)
    	mSprite[i] = CCSprite::create(PATH_PLAYER_SPRITE[i].c_str());

    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();

    for (int i = 0; i < NUM_OF_PLAYER; i++){
    	for (int j = 0; j < 4; j++)
    		cache->addSpriteFramesWithFile(("sprite/"+NAME_PLAYER_SPRITE_ANIMATION[i][j]+".plist").c_str(),
                                           ("sprite/"+NAME_PLAYER_SPRITE_ANIMATION[i][j]+".png").c_str());
    }

    for (int i = 0; i < 4; i++){
		cache->addSpriteFramesWithFile(("sprite/"+NAME_ZOMBIE_SPRITE_ANIMATION[i]+".plist").c_str(),
									   ("sprite/"+NAME_ZOMBIE_SPRITE_ANIMATION[i]+".png").c_str());
    }

    // Agrega dados a cache
    cache->addSpriteFramesWithFile("sprite/movedicered.plist", "sprite/movedicered.png");
    cache->addSpriteFramesWithFile("sprite/movediceblue.plist", "sprite/movediceblue.png");

    // Inicializa Nombre de las rondas
    name[0][0] = "Agarra Carta del Mazo";
    name[0][1] = "Coloca la carta de mapa obtenida";
    name[1][0] = "Lanza el dado Azul para Moverte";
    name[1][1] = "Mueve tu personaje";
	name[2][0] = "Lanza el dado Rojo para mover Zombies";
	name[2][1] = "Mueve los zombies";

    // Inicia variables:
    _moveLayer = CCLayerPanZoom::create();
    _stayLayer = CCLayer::create();

    touches = 0;
    lastRedDiceResult = 5;
    lastBlueDiceResult = 5;
    WAIT = false;
    PUTMAPCARD = false;
    currFase = 0;
    currSubFase = 0;
    fase[0] = true;
    currCardRotation = 0;

    for (int i = 1; i < NUM_FASES; i++) fase[i] = false;

    // Coloca elementos sobre layer estatico
    setMenuBackMenu(_stayLayer);
//    setOptionsLifeMenu(_stayLayer); // provisional
//    setOptionsBulletMenu(_stayLayer); // provisional
    setDices(_stayLayer);
    setInterface();
    setPlayerInfo();
    setBackgrounds();
    setPickMapCard();

    // Coloca elementos sobre el layer dinamico
    setMap(_moveLayer);
    setSprites(_moveLayer);

    // Habilita eventos touch
    this->setTouchEnabled( true );

    CCPoint centro = mapCardMatrixToAxis(MAX_MAP_DIM/2,MAX_MAP_DIM/2);
    centro.x -= VisibleRect::center().x;
    centro.y -= VisibleRect::center().y;
    _moveLayer->setPosition(-centro);

	_moveLayer->setAnchorPoint(ccp(0.0, 0.0));
	_moveLayer->setContentSize(CCSize((MAX_MAP_DIM+2)*PIXELS_MAP_CARD, (MAX_MAP_DIM+2)*PIXELS_MAP_CARD));

	_moveLayer->setPanBoundsRect(CCRectMake(VisibleRect::getVisibleRect().origin.x,
							     	 	 	VisibleRect::getVisibleRect().origin.y,
							     	 	 	VisibleRect::getVisibleRect().size.width,
							     	 	 	VisibleRect::getVisibleRect().size.height));

    // Añade layers al layer principal
    addChild(_moveLayer);
    addChild(_stayLayer);

    // Se prepara la primera fase del turno
    showFirstPhase();

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

void PlayScene::incrementLifeMenuCallback(CCObject* pSender)
{
	modifyPlayerLifes(1);
}

void PlayScene::decrementLifeMenuCallback(CCObject* pSender)
{
	modifyPlayerLifes(-1);
}

void PlayScene::incrementBulletMenuCallback(CCObject* pSender)
{
	modifyPlayerBullets(1);
}

void PlayScene::decrementBulletMenuCallback(CCObject* pSender)
{
	modifyPlayerBullets(-1);
}

void PlayScene::activateMapCardModeCallback(CCObject* pSender)
{
	if (fase[0]) PUTMAPCARD = true;
}

void PlayScene::redDiceCallback()
{
	int result;

	CCArray * actions = CCArray::createWithCapacity(7);
	CCFiniteTimeAction * single_action;

	single_action = CCCallFuncN::create( this, callfuncN_selector(PlayScene::setMoveRedDice));
	actions->addObject(single_action);

	actions->addObject(CCDelayTime::create(1.5)); // 1.5 seg

	single_action = CCCallFuncN::create( this, callfuncN_selector(PlayScene::setStaticRedDice));
	actions->addObject(single_action);

	// desactivar el wait
	single_action = CCCallFuncND::create( this, callfuncND_selector(PlayScene::activateTouch), NULL);
	actions->addObject(single_action);

	if ( !HAY_BATALLA )
	{
		 result = GameState.rollZombieDice(); // [0,5]

		// Cambiar la subfase
		single_action = CCCallFuncND::create( this, callfuncND_selector(PlayScene::changeSubPhase), NULL);
		actions->addObject(single_action);

		NUM_OF_ZOMBIES_TO_MOVE = result + 1;
	} else {
		result = GameState.rollFightDice(); // [0,5]

		if ( result < 3 ) {

			if (GameState.getCurrentPlayerLife() == 0 && (result + GameState.getCurrentPlayerBullet()) < 3){
				single_action = CCCallFuncN::create( this, callfuncN_selector(PlayScene::loseBattleAndDie));
				actions->addObject(single_action);

			} else {
				// Jugador debe tomar una decision: perder vidas o balas
				single_action = CCCallFuncN::create( this, callfuncN_selector(PlayScene::continueBattle));
				actions->addObject(single_action);
			}

		} else {
			// Jugador ha ganado
			single_action = CCCallFuncND::create( this, callfuncND_selector(PlayScene::winBattle), NULL);
			actions->addObject(single_action);
		}
	}

	CCSequence *sq = CCSequence::create(actions);

	redDices[lastRedDiceResult]->setVisible(false);
	lastRedDiceResult = result;
	redDices[lastRedDiceResult]->runAction( sq );
}

void PlayScene::setMoveRedDice(CCNode* node)
{
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();

	animatedSprite = CCSprite::createWithSpriteFrameName( "movedicered_1.png" );
	animatedSprite->setPosition( ccp( redDices[0]->getPosition().x, redDices[0]->getPosition().y) );

	spritebatch = CCSpriteBatchNode::create("sprite/movedicered.png");
	spritebatch->addChild(animatedSprite);

	_stayLayer->addChild(spritebatch);

	CCArray* animFrames = CCArray::createWithCapacity(16);

	char str[70] = {0};
	for(int i = 1; i < 16; i++)
	{
		sprintf(str, "movedicered_%d.png", i);
		CCSpriteFrame* frame = cache->spriteFrameByName( str );
		animFrames->addObject(frame);
	}

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1f);
	animatedSprite->runAction( CCRepeatForever::create( CCAnimate::create(animation) ) );

	animatedSprite->setFlipX(false);
	animatedSprite->setFlipY(false);
}

void PlayScene::setStaticRedDice(CCNode* node) {
	if (spritebatch != NULL) {
		_stayLayer->removeChild(spritebatch);
		spritebatch = NULL;
		animatedSprite = NULL;
	}

	redDices[lastRedDiceResult]->setVisible(true);
}

void PlayScene::blueDiceCallback()
{
	int result = GameState.rollPlayerDice();

	CCArray * actions = CCArray::createWithCapacity(4);
	CCFiniteTimeAction * single_action;

	single_action = CCCallFuncN::create( this, callfuncN_selector(PlayScene::setMoveBlueDice));
	actions->addObject(single_action);

	actions->addObject(CCDelayTime::create(1.5)); // 1.5 seg

	single_action = CCCallFuncN::create( this, callfuncN_selector(PlayScene::setStaticBlueDice));
	actions->addObject(single_action);

	// desactivar el wait ( **OJO**)
	single_action = CCCallFuncND::create( this, callfuncND_selector(PlayScene::activateTouch), NULL);
	actions->addObject(single_action);

	// Cambiar la subfase
	single_action = CCCallFuncND::create( this, callfuncND_selector(PlayScene::changeSubPhase), NULL);
	actions->addObject(single_action);

	CCSequence *sq = CCSequence::create(actions);

	blueDices[lastBlueDiceResult]->setVisible(false);
	lastBlueDiceResult = result;
	blueDices[lastBlueDiceResult]->runAction( sq );
}

void PlayScene::setMoveBlueDice(CCNode* node)
{
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();

	animatedSprite = CCSprite::createWithSpriteFrameName( "movediceblue_1.png" );
	animatedSprite->setPosition( ccp( blueDices[0]->getPosition().x, blueDices[0]->getPosition().y) );

	spritebatch = CCSpriteBatchNode::create("sprite/movediceblue.png");
	spritebatch->addChild(animatedSprite);

	_stayLayer->addChild(spritebatch);

	CCArray* animFrames = CCArray::createWithCapacity(16);

	char str[70] = {0};
	for(int i = 1; i < 16; i++)
	{
		sprintf(str, "movediceblue_%d.png", i);
		CCSpriteFrame* frame = cache->spriteFrameByName( str );
		animFrames->addObject(frame);
	}

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1f);
	animatedSprite->runAction( CCRepeatForever::create( CCAnimate::create(animation) ) );

	animatedSprite->setFlipX(false);
	animatedSprite->setFlipY(false);
}

void PlayScene::setStaticBlueDice(CCNode* node) {
	if (spritebatch != NULL) {
		_stayLayer->removeChild(spritebatch);
		spritebatch = NULL;
		animatedSprite = NULL;
	}

	blueDices[lastBlueDiceResult]->setVisible(true);
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

void PlayScene::setOptionsLifeMenu(CCLayer *mLayer)
{
	// +++++++++++++++++++++++++++++++++
	CCLabelTTF* label = CCLabelTTF::create("+", "Arial", FONT_SIZE + 10);
	CCMenuItemLabel* itemLabel = CCMenuItemLabel::create(label,this,menu_selector(PlayScene::incrementLifeMenuCallback));

	itemLabel->setPosition(ccp(VisibleRect::rightBottom().x - itemLabel->getContentSize().width/2 -30,
                               VisibleRect::rightBottom().y + itemLabel->getContentSize().height/2 +50));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(itemLabel, NULL);
    pMenu->setPosition(CCPointZero);
    mLayer->addChild(pMenu);

    // ----------------------------------
	label = CCLabelTTF::create("-", "Arial", FONT_SIZE +10);
	itemLabel = CCMenuItemLabel::create(label,this,menu_selector(PlayScene::decrementLifeMenuCallback));

	itemLabel->setPosition(ccp(VisibleRect::rightBottom().x - itemLabel->getContentSize().width/2 -10,
                               VisibleRect::rightBottom().y + itemLabel->getContentSize().height/2 +50));

    // create menu, it's an autorelease object
    pMenu = CCMenu::create(itemLabel, NULL);
    pMenu->setPosition(CCPointZero);
    mLayer->addChild(pMenu);
}

void PlayScene::setOptionsBulletMenu(CCLayer *mLayer)
{
	// +++++++++++++++++++++++++++++++++
	CCLabelTTF* label = CCLabelTTF::create("+", "Arial", FONT_SIZE + 10);
	CCMenuItemLabel* itemLabel = CCMenuItemLabel::create(label,this,menu_selector(PlayScene::incrementBulletMenuCallback));

	itemLabel->setPosition(ccp(VisibleRect::rightBottom().x - itemLabel->getContentSize().width/2 -30,
                               VisibleRect::rightBottom().y + itemLabel->getContentSize().height/2 +100));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(itemLabel, NULL);
    pMenu->setPosition(CCPointZero);
    mLayer->addChild(pMenu);

    // ----------------------------------
	label = CCLabelTTF::create("-", "Arial", FONT_SIZE +10);
	itemLabel = CCMenuItemLabel::create(label,this,menu_selector(PlayScene::decrementBulletMenuCallback));

	itemLabel->setPosition(ccp(VisibleRect::rightBottom().x - itemLabel->getContentSize().width/2 -10,
                               VisibleRect::rightBottom().y + itemLabel->getContentSize().height/2 +100));

    // create menu, it's an autorelease object
    pMenu = CCMenu::create(itemLabel, NULL);
    pMenu->setPosition(CCPointZero);
    mLayer->addChild(pMenu);
}

void PlayScene::setDices(CCLayer * layer)
{
	CCSprite* icon;

	char str[70] = {0};
	for (int i = 1; i <= 6; i++){

		sprintf(str, "sprite/diceblue_%d.png", i);
		icon = CCSprite::create(str);
		icon->setPosition(ccp(VisibleRect::leftBottom().x + icon->getContentSize().width/2 + 5,
							  VisibleRect::leftBottom().y + icon->getContentSize().height/2 + 5));
		icon->setVisible(false);
		layer->addChild(icon);
		blueDices.push_back(icon);

		sprintf(str, "sprite/dicered_%d.png", i);
		icon = CCSprite::create(str);
		icon->setPosition(ccp(VisibleRect::leftBottom().x + 3*icon->getContentSize().width/2 + 10 ,
							  VisibleRect::leftBottom().y + icon->getContentSize().height/2 + 5));
		icon->setVisible(false);
		layer->addChild(icon);
		redDices.push_back(icon);
	}

}

void PlayScene::setMap(CCLayer *mLayer)
{
	CCSprite* mapCardSprite = CCSprite::create("map/empty_card2.png");

	for (int i = 0; i < MAX_MAP_DIM; i++)
		for (int j = 0; j < MAX_MAP_DIM; j++){
			mapCardSprite->setPosition(mapCardMatrixToAxis(i,j));
			mLayer->addChild(mapCardSprite);
		}

	mapCardSprite = CCSprite::create(mapPath[0].c_str()); // Plaza Central
	mapCardSprite->setPosition(mapCardMatrixToAxis(MAX_MAP_DIM/2, MAX_MAP_DIM/2));
	mLayer->addChild(mapCardSprite);
}

void PlayScene::setInterface()
{
	CCSprite*   icon;
	CCLabelTTF* label;
	int off_y = 5;
	int off_x = 8;

	icon = CCSprite::create("lifeIcon.png");
	icon->setPosition(ccp(VisibleRect::leftTop().x + off_x + icon->getContentSize().width/2,
						  VisibleRect::leftTop().y - icon->getContentSize().height/2 - off_y));
	icon->setTag(LIFE_ICON_TAG);


	label = CCLabelTTF::create("x 0", "Arial", FONT_SIZE);
	label->setTag(LIFES_LABEL_TAG);
	label->setPosition(ccp(VisibleRect::leftTop().x + icon->getContentSize().width + off_x + 7 + label->getContentSize().width/2,
						   VisibleRect::leftTop().y - icon->getContentSize().height/2 - off_y));

	off_y += icon->getContentSize().height + 5;

	_stayLayer->addChild(icon, 2);
	_stayLayer->addChild(label, 2);

	icon = CCSprite::create("bulletIcon.png");
	icon->setPosition(ccp(VisibleRect::leftTop().x + off_x + icon->getContentSize().width/2,
						  VisibleRect::leftTop().y - icon->getContentSize().height/2 - off_y));
	icon->setTag(BULLET_ICON_TAG);

	label = CCLabelTTF::create("x 0", "Arial", FONT_SIZE);
	label->setTag(BULLETS_LABEL_TAG);
	label->setPosition(ccp(VisibleRect::leftTop().x + icon->getContentSize().width + 7 + off_x + label->getContentSize().width/2,
						   VisibleRect::leftTop().y - icon->getContentSize().height/2 - off_y));

	_stayLayer->addChild(icon, 2);
	_stayLayer->addChild(label, 2);

	off_x = 8;
	off_y = 5;

	icon = CCSprite::create("zombie.png");
	icon->setPosition(ccp(VisibleRect::rightTop().x - off_x - icon->getContentSize().width/2,
						  VisibleRect::rightTop().y - icon->getContentSize().height/2 - off_y));
	icon->setTag(ZOMBIE_ICON_TAG);

	off_x += icon->getContentSize().width;

	label = CCLabelTTF::create("0 x", "Arial", FONT_SIZE);
	label->setTag(ZOMBIES_LABEL_TAG);
	label->setPosition(ccp(VisibleRect::rightTop().x - off_x - 5 - label->getContentSize().width/2,
						   VisibleRect::rightTop().y - icon->getContentSize().height/2 - off_y));

	_stayLayer->addChild(icon, 2);
	_stayLayer->addChild(label, 2);

	label = CCLabelTTF::create("Player 1", "Arial", FONT_SIZE + 8);
	label->setTag(TITLE_LABEL_TAG);
	label->setPosition(ccp(VisibleRect::top().x,
						   VisibleRect::top().y - label->getContentSize().height/2));

	_stayLayer->addChild(label, 2);
	off_y = label->getContentSize().height + 5;

	label = CCLabelTTF::create(name[currFase][currSubFase].c_str(), "Arial", FONT_SIZE);
	label->setTag(SUBTITLE_LABEL_TAG);
	label->setPosition(ccp(VisibleRect::top().x,
						   VisibleRect::top().y - off_y));

	_stayLayer->addChild(label, 2);


	icon = CCSprite::create("lifeIcon.png");
	icon->setPosition(ccp(VisibleRect::bottom().x - icon->getContentSize().width,
						  VisibleRect::bottom().y + icon->getContentSize().height/2 + 5));
	icon->setTag(QUESTION_LIFE_ICON_TAG);
	icon->setVisible(false);
	_stayLayer->addChild(icon, 2);

	off_y = icon->getContentSize().height + 15;

	icon = CCSprite::create("bulletIcon.png");
	icon->setPosition(ccp(VisibleRect::bottom().x + icon->getContentSize().width,
						  VisibleRect::bottom().y + icon->getContentSize().height/2 + 5));
	icon->setTag(QUESTION_BULLET_ICON_TAG);
	icon->setVisible(false);
	_stayLayer->addChild(icon, 2);


	label = CCLabelTTF::create("¿Qué desea utilizar?", "Arial", FONT_SIZE);
	label->setTag(QUESTION_LABEL_TAG);
	label->setPosition(ccp(VisibleRect::bottom().x,
						   VisibleRect::bottom().y + off_y));
	label->setVisible(false);
	_stayLayer->addChild(label, 2);

}

void PlayScene::setPlayerInfo()
{
	CCLabelTTF *label = (CCLabelTTF *) _stayLayer->getChildByTag(TITLE_LABEL_TAG);
	char info[45];
	sprintf(info, "Jugador %d", GameState.getCurrentPlayer() + 1);
	label->setString(info);

	modifyScreenLifes();
	modifyScreenBullets();
	modifyScreenZombies();
}

void PlayScene::modifyPlayerLifes(int num)
{
	if (num == 1) GameState.addLife();
	else if (num == -1) GameState.decreaseLife();

	modifyScreenLifes();
}

void PlayScene::modifyPlayerBullets(int num)
{
	if (num == 1) GameState.addBullet();
	else if (num < 0) GameState.decreaseBullet(-num);

	modifyScreenBullets();
}

void PlayScene::modifyPlayerZombies()
{
	GameState.addZombie();

	modifyScreenZombies();
}

void PlayScene::modifyScreenLifes()
{
	CCLabelTTF *label = (CCLabelTTF *) _stayLayer->getChildByTag(LIFES_LABEL_TAG);
	char info[10];
	sprintf(info, "x %d", GameState.getCurrentPlayerLife());

	label->setString(info);
}

void PlayScene::modifyScreenBullets()
{
	CCLabelTTF *label = (CCLabelTTF *) _stayLayer->getChildByTag(BULLETS_LABEL_TAG);
	char info[10];
	sprintf(info, "x %d", GameState.getCurrentPlayerBullet());

	label->setString(info);
}

void PlayScene::modifyScreenZombies()
{
	CCLabelTTF *label = (CCLabelTTF *) _stayLayer->getChildByTag(ZOMBIES_LABEL_TAG);
	char info[10];
	sprintf(info, "%d x", GameState.getCurrentPlayerZombie());

	label->setString(info);
}

void PlayScene::setSprites(CCLayer* mLayer)
{
	CCPoint center = mapCardMatrixToAxis(MAX_MAP_DIM/2, MAX_MAP_DIM/2);

	center.y += (mSprite[0]->getContentSize().height / 2);

	for(int i = 0; i < NUM_OF_PLAYER; i++){
		if (i == 0)
			center.x += (mSprite[i]->getContentSize().width / 2);
		else
			center.x -= (mSprite[i]->getContentSize().width / 2);

		mSprite[i]->setPosition(center);
		mLayer->addChild(mSprite[i], 5);

		if (i == 0)
			center.x -= (mSprite[i]->getContentSize().width / 2);
		else
			center.x += (mSprite[i]->getContentSize().width / 2);
	}
}

void PlayScene::setMoveSprite(CCNode* sender, void* data) {
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	Event *d =  (Event*) data;
	string name;

	if (d->orientation == 'H')
		if (d->movement > 0)
			name = NAME_PLAYER_SPRITE_ANIMATION[GameState.getCurrentPlayer()][RIGHT];
		else
			name = NAME_PLAYER_SPRITE_ANIMATION[GameState.getCurrentPlayer()][LEFT];
	else
		if (d->movement > 0)
			name = NAME_PLAYER_SPRITE_ANIMATION[GameState.getCurrentPlayer()][UP];
		else
			name = NAME_PLAYER_SPRITE_ANIMATION[GameState.getCurrentPlayer()][DOWN];

	animatedSprite = CCSprite::createWithSpriteFrameName( (name +"_1.png").c_str() );
	animatedSprite->setPosition( ccp( mSprite[GameState.getCurrentPlayer()]->getPositionX(), mSprite[GameState.getCurrentPlayer()]->getPositionY()) );
	mSprite[GameState.getCurrentPlayer()]->setVisible(false);

	spritebatch = CCSpriteBatchNode::create( ("sprite/" +name +".png" ).c_str());
	spritebatch->addChild(animatedSprite);

	_moveLayer->addChild(spritebatch, 5);

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

	CCFiniteTimeAction* actionMove;
	Event *d =  (Event*) data;
	float offX = 0, offY = 0;

	if (d->orientation== 'H')
		offX += d->movement;
	else
		offY += d->movement;

	actionMove = CCMoveTo::create( ( abs(d->movement)) / VELOCITY_SPRITE_MOVEMENT,
									ccp(mSprite[GameState.getCurrentPlayer()]->getPositionX() + offX, mSprite[GameState.getCurrentPlayer()]->getPositionY() +offY));


	mSprite[GameState.getCurrentPlayer()]->setPosition(ccp(mSprite[GameState.getCurrentPlayer()]->getPositionX() + offX, mSprite[GameState.getCurrentPlayer()]->getPositionY()+ offY));

	animatedSprite->runAction(actionMove);
}

void PlayScene::setStaticSprite(CCNode* sender, void* data) {
	Event *d =  (Event*) data;

	GameState.movePlayerTo(d->end_point);

	if ( GameState.queryZombie(p) ) {

		if (spritebatch != NULL ) {
			_moveLayer->removeChild(spritebatch);
			spritebatch = NULL;
			animatedSprite = NULL;
		}

		mSprite[GameState.getCurrentPlayer()]->setVisible(true);
	}
}








void PlayScene::setMoveZombieSprite(CCNode* sender, void* data) {
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	Event *d =  (Event*) data;
	string name;

	if (d->orientation == 'H')
		if (d->movement > 0)
			name = NAME_ZOMBIE_SPRITE_ANIMATION[RIGHT];
		else
			name = NAME_ZOMBIE_SPRITE_ANIMATION[LEFT];
	else
		if (d->movement > 0)
			name = NAME_ZOMBIE_SPRITE_ANIMATION[UP];
		else
			name = NAME_ZOMBIE_SPRITE_ANIMATION[DOWN];

	animatedSprite = CCSprite::createWithSpriteFrameName( (name +"_1.png").c_str() );
	animatedSprite->setPosition( prevZombieLocation );
	zombies[zombieSpriteToMove].sprite->setVisible(false);

	// Posicion previa del zombie no se necesita mas
	prevZombieLocation.x = prevZombieLocation.y = -1;

	spritebatch = CCSpriteBatchNode::create( ("sprite/" +name +".png" ).c_str());
	spritebatch->addChild(animatedSprite);

	_moveLayer->addChild(spritebatch, 5);

	CCArray* animFrames = CCArray::createWithCapacity(4);

	char str[250] = {0};
	int i;
	for(i = 1; i < 4; i++)
	{
		sprintf(str, "%s_%d.png", name.c_str(), i);
		CCSpriteFrame* frame = cache->spriteFrameByName( str );
		animFrames->addObject(frame);
	}

	if (d->orientation == 'V'){
		sprintf(str, "%s_%d.png", name.c_str(), i);
		CCSpriteFrame* frame = cache->spriteFrameByName( str );
		animFrames->addObject(frame);
	}

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
	animatedSprite->runAction( CCRepeatForever::create( CCAnimate::create(animation) ) );

	animatedSprite->setFlipX(false);
	animatedSprite->setFlipY(false);
}

void PlayScene::animateZombieSprite(CCNode* sender, void* data) {

	CCFiniteTimeAction* actionMove;
	Event *d =  (Event*) data;
	float offX = 0, offY = 0;

	if (d->orientation == 'H')
		offX += d->movement;
	else
		offY += d->movement;

	actionMove = CCMoveTo::create( ( abs(d->movement)) / VELOCITY_SPRITE_MOVEMENT,
									ccp(zombies[zombieSpriteToMove].sprite->getPositionX() + offX, zombies[zombieSpriteToMove].sprite->getPositionY() +offY));

	zombies[zombieSpriteToMove].sprite->setPosition(ccp(zombies[zombieSpriteToMove].sprite->getPositionX() + offX, zombies[zombieSpriteToMove].sprite->getPositionY()+ offY));

	animatedSprite->runAction(actionMove);
}

void PlayScene::setStaticZombieSprite(CCNode* sender, void* data) {
	if (spritebatch != NULL) {
		_moveLayer->removeChild(spritebatch);
		spritebatch = NULL;
		animatedSprite = NULL;
	}

	zombies[zombieSpriteToMove].sprite->setVisible(true);
}





/*
 * Verifica si la posicion p, es valida para la carta
 * con la rotacion actual
 */

bool PlayScene::isAllowed(position p) {

	for (int i = 0; i < allowedPositions[currCardRotation].size(); i++) {
		if (allowedPositions[currCardRotation][i] == p)
			return true;
	}

	return false;
}


bool PlayScene::putMapCard(CCPoint location, int id)
{
	CCPoint actualLocation = axisToMapCardMatrix(location.x, location.y);
	position p((int) actualLocation.x, (int) actualLocation.y);
	p.invT();

	allowedPositions.clear();
	allowedPositions = GameState.getAllPosibleMapCard(GameState.getLastMapCard());

	if (!isAllowed(p)) {
		return false;
	}

	CCSprite* sprite;
	// mapCards[id].path
	/* provisional DEL MAPA */
	int fila = (int) actualLocation.x, columna = (int) actualLocation.y;

	GameState.putCardMap(GameState.getLastMapCard(), p);

	// Se coloca la carta de mapa
	sprite = CCSprite::create(GameState.getLastMapCard().getPath().c_str());
	sprite->setPosition(mapCardMatrixToAxis(actualLocation.x,actualLocation.y));
	_moveLayer->addChild(sprite, 1);

	float bv_offset_x, bv_offset_y, z_offset_y;

	//Zombie
	sprite = CCSprite::create("sprite/zombie_down_1.png");
	z_offset_y = - sprite->getContentSize().height/4;

	// vida y bala
	sprite = CCSprite::create("lifeIcon.png");
	bv_offset_x = -PIXELS_TILE/2 + sprite->getContentSize().width/2 + 5;
	bv_offset_y = -PIXELS_TILE/2 + sprite->getContentSize().height/2 + 5;

	Element element;
	element.mapCard_i = fila;
	element.mapCard_j = columna;

	// Se colocan los elementos que tiene (zombies, vidas, balas):
	for (int i = 0; i < MAPNUMPOS; i++){
		element.tile_i = i;
		for (int j = 0; j < MAPNUMPOS; j++){
			actualLocation = tileMatrixToAxis(fila, columna, i, j);
			tile currTile = GameState.getLastMapCard().getTile(i-1, j-1);
			element.tile_j = j;
			if (currTile.hasZombie()) {
				element.sprite = CCSprite::create("sprite/zombie_down_1.png");
				element.sprite->setPosition(ccp(actualLocation.x, actualLocation.y + z_offset_y));
				_moveLayer->addChild(element.sprite,4);
				zombies.push_back(element);
			}

			if (currTile.hasBullet()) {
				element.sprite = CCSprite::create("bulletIcon.png");
				element.sprite->setPosition(ccp(actualLocation.x + bv_offset_x, actualLocation.y + bv_offset_y));
				_moveLayer->addChild(element.sprite,1);
				bullets.push_back(element);
			}

			if (currTile.hasLife()) {
				element.sprite = CCSprite::create("lifeIcon.png");
				element.sprite->setPosition(ccp(actualLocation.x + bv_offset_x, actualLocation.y + bv_offset_y));
				_moveLayer->addChild(element.sprite,1);
				lifes.push_back(element);
			}
		}
	}

	PUTMAPCARD = false;

	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *     Funciones para llevar a cabo acciones       *
 * * * * * * * * * * * * * * * * * * * * * * * * * */

CCPoint PlayScene::axisToMapCardMatrix(float x, float y){
//	float aux_x = abs( - (MAX_MAP_DIM/2 * PIXELS_MAP_CARD + PIXELS_MAP_CARD / 2) - x);
//	float aux_y = abs(   (MAX_MAP_DIM/2 * PIXELS_MAP_CARD + PIXELS_MAP_CARD / 2) - y);

	float aux_x = abs( PIXELS_MAP_CARD - x );
	float aux_y = abs( PIXELS_MAP_CARD + MAX_MAP_DIM * PIXELS_MAP_CARD - y );

	return ccp(aux_y/PIXELS_MAP_CARD, aux_x/PIXELS_MAP_CARD);
}

CCPoint PlayScene::axisToTileMatrix(float x, float y){
//	float aux_x = abs( - (MAX_MAP_DIM/2 * PIXELS_MAP_CARD + PIXELS_MAP_CARD / 2) - x);
//	float aux_y = abs(   (MAX_MAP_DIM/2 * PIXELS_MAP_CARD + PIXELS_MAP_CARD / 2) - y);
	float aux_x = abs( PIXELS_MAP_CARD - x );
	float aux_y = abs( PIXELS_MAP_CARD + MAX_MAP_DIM * PIXELS_MAP_CARD - y );

	return ccp( (int)(aux_y/PIXELS_TILE) % 3, (int)(aux_x/PIXELS_TILE) % 3);
}

CCPoint PlayScene::mapCardMatrixToAxis(int i, int j){
//	float origen_x = (MAX_MAP_DIM/2)*(-PIXELS_MAP_CARD);
//	float origen_y = (MAX_MAP_DIM/2)* PIXELS_MAP_CARD;

	float origen_x = PIXELS_MAP_CARD + PIXELS_MAP_CARD/2;
	float origen_y = PIXELS_MAP_CARD + MAX_MAP_DIM* PIXELS_MAP_CARD - PIXELS_MAP_CARD/2;

	return ccp(origen_x + j * PIXELS_MAP_CARD, origen_y + (-i * PIXELS_MAP_CARD) );
}

CCPoint PlayScene::tileMatrixToAxis(int i_mapCard, int j_mapCard, int i, int j){
	CCPoint center = mapCardMatrixToAxis(i_mapCard,j_mapCard);

	return ccp(center.x + PIXELS_TILE * (j-1), center.y - PIXELS_TILE * (i-1) );
}

Event PlayScene::getEventZombieMove(CCPoint prev, CCPoint next){
	int prev_x = abs( PIXELS_MAP_CARD - prev.x) / PIXELS_TILE;
	int prev_y = abs( (PIXELS_MAP_CARD + MAX_MAP_DIM * PIXELS_MAP_CARD) - prev.y) / PIXELS_TILE;

	int next_x = abs( PIXELS_MAP_CARD - next.x) / PIXELS_TILE;
	int next_y = abs( (PIXELS_MAP_CARD + MAX_MAP_DIM * PIXELS_MAP_CARD) - next.y) / PIXELS_TILE;

	Event evento;
	CCPoint point = axisToMapCardMatrix(next.x, next.y);
	point = tileMatrixToAxis((int)point.x, (int)point.y, next_y % 3, next_x % 3);

	if ( abs(next_x - prev_x) == 1 && abs(next_y - prev_y) == 0 ){
		evento.orientation = 'H';
		evento.movement = point.x - prev.x;
		return evento;
	}

	if ( abs(next_x - prev_x) == 0 && abs(next_y - prev_y) == 1 ){
		evento.orientation = 'V';
		evento.movement = point.y - prev.y;
		return evento;
	}

	evento.orientation = '-';
	return evento;
}

void PlayScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch*) (*pTouches->begin());
	CCPoint pto = _moveLayer->convertTouchToNodeSpace(touch);
	CCLOG ("%f %f", pto.x, pto.y);

	_moveLayer->ccTouchesBegan(pTouches, pEvent);
	touches++;
	if (touches == 1) singleTouch = true;
	else singleTouch = false;
}

void PlayScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	_moveLayer->ccTouchesMoved(pTouches, pEvent);

    if (touches != 1)
    	return;

	CCTouch *touch = (CCTouch*) (*pTouches->begin());
	CCPoint curTouchPosition = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	CCPoint prevTouchPosition = CCDirector::sharedDirector()->convertToGL(touch->getPreviousLocationInView());

	touchDistance += ccpDistance(curTouchPosition, prevTouchPosition);
}

void PlayScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){

	_moveLayer->ccTouchesEnded(pTouches, pEvent);

	if (touches == 1 && singleTouch && touchDistance <= EPSILON_DISTANCE && !WAIT) {

    	CCTouch * touch = (CCTouch *) (*pTouches->begin());
    	CCPoint ptoConvertido = _moveLayer->convertTouchToNodeSpace(touch);

    	if      (currFase == 0) firstPhase(touch->getLocation(), ptoConvertido);
    	else if (currFase == 1) secondPhase(touch->getLocation(), ptoConvertido);
    	else if (currFase == 2) thirdPhase(touch->getLocation(), ptoConvertido);

	}

    touchDistance = 0.0;
    if (touches > 0) touches--;
}

void PlayScene::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	_moveLayer->ccTouchesCancelled(pTouches, pEvent);

	if (touches > 0) touches--;
	if (touches == 0) {
		touchDistance = 0.0;
	}
}

void PlayScene::pointToEvent(CCPoint prevPoint, CCPoint nextPoint, position p)
{
	// Obtenemos el "touch" del usuario y lo enviamos al Modelo para que nos de el
	// arreglo con los eventos. provisional

	vector<position> road = GameState.queryMovePlayerTo(p);

	events.clear();
	Event e;
	e.movement = PIXELS_TILE;

	for (int i = 1; i < road.size(); i++){
		if ( road[i-1].x != road[i].x )
			e.orientation = 'H';
		else
			e.orientation = 'V';

		e.end_point = road[i];
		events.push_back(e);
	}
}

void PlayScene::activateTouch(CCNode* sender, void* data)
{
	WAIT = false;
}

void PlayScene::changePhase(CCNode* sender, void* data)
{
	currFase = (currFase + 1) % NUM_FASES;
	currSubFase = 0;

	msg = name[currFase][currSubFase];
	putSubtitleInformation(NULL,NULL);

	if (currFase == 0) {
		// Cambio de Jugador
		GameState.nextTurn();
		setPlayerInfo();

		// Retornar el sprite de la carta a su forma original
		currCardRotation = 0;
		CCSprite * sprite = (CCSprite *) _stayLayer->getChildByTag(MAP_CARD_SELECTED);
		sprite->setRotation(0);

		hideThirdPhase();
		showFirstPhase();
	}

	if (currFase == 1) {
		possibleMoves.clear();
		LANZODADOAZUL = false; // provisional
		HAY_BATALLA = false;
		HAY_PREGUNTA = false;
		hideFirstPhase();
		showSecondPhase();
		checkBattle(NULL,NULL);
	}

	if (currFase == 2) {
		LANZODADOROJO = false; // provisional
		hideSecondPhase();
		showThirdPhase();
	}
}

void PlayScene::changeSubPhase(CCNode* sender, void* data)
{
	currSubFase++;
	CCLabelTTF * label = (CCLabelTTF *) _stayLayer->getChildByTag(SUBTITLE_LABEL_TAG);
	label->setString(name[currFase][currSubFase].c_str());
}

void PlayScene::putSubtitleInformation(CCNode* sender, void* data)
{
	CCLabelTTF * label = (CCLabelTTF *) _stayLayer->getChildByTag(SUBTITLE_LABEL_TAG);
	label->setString(msg.c_str());
}

// FASES
void PlayScene::firstPhase(CCPoint pto, CCPoint ptoConvertido)
{
	CCSprite * sprite = (CCSprite *) _stayLayer->getChildByTag(MAP_CARD_DECK);

	if (!PUTMAPCARD && sprite->boundingBox().containsPoint(pto)) {
		WAIT = true;
		initFlipXCallback(NULL);
	}

	if (PUTMAPCARD)
	{
		// actualizar rotacion de la carta (variable) para
		// chequear correcta lista de posibles posiciones
		sprite = (CCSprite *) _stayLayer->getChildByTag(ROTATE_ICON);
		if (sprite->boundingBox().containsPoint(pto)) {
			WAIT = true;
			rotate_mapCard();
		} else
			if (putMapCard(ptoConvertido, 0)) // provisional 0 debe ser id de carta
				changePhase(NULL, NULL);
	}
}

bool PlayScene::canMove(position p) {
	for (int i = 0; i < possibleMoves.size(); i++) {
		if (possibleMoves[i] == p)
			return true;
	}

	return false;
}

void PlayScene::secondPhase(CCPoint pto, CCPoint ptoConvertido)
{
	// El dado azul fue cliqueado
	if ( !HAY_PREGUNTA && !LANZODADOAZUL && !HAY_BATALLA  && blueDices[0]->boundingBox().containsPoint(pto))
	{
		// desactiva touch
		WAIT = true;
		blueDiceCallback();
		LANZODADOAZUL = true;
		return;
	}

	CCPoint index_cardMap = axisToMapCardMatrix(ptoConvertido.x, ptoConvertido.y);
	position p((int) index_cardMap.x, (int) index_cardMap.y);
	p.invT();

	if (possibleMoves.size() == 0) possibleMoves = GameState.getPossibleMoves().first;

	if (canMove(p) && LANZODADOAZUL && !HAY_PREGUNTA && !HAY_BATALLA)
	{
		// desactiva touch
		WAIT = true;

		CCPoint index_tile = axisToTileMatrix(ptoConvertido.x,ptoConvertido.y);
		CCPoint point = tileMatrixToAxis(index_cardMap.x, index_cardMap.y, index_tile.x, index_tile.y);

		if (GameState.getCurrentPlayer() == 0)
			point.x += (mSprite[GameState.getCurrentPlayer()]->getContentSize().width / 2);
		else
			point.x -= (mSprite[GameState.getCurrentPlayer()]->getContentSize().width / 2);

		point.y += (mSprite[0]->getContentSize().height / 2);

		pointToEvent(mSprite[GameState.getCurrentPlayer()]->getPosition(), point, p);

		CCArray *actions = CCArray::createWithCapacity(4 * events.size() + 2);
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

		// activar touch
		single_action = CCCallFuncND::create( this, callfuncND_selector(PlayScene::activateTouch), NULL);
		actions->addObject(single_action);

		/*
		 * Quedamos en que el arreglo de eventos tendria eventos hasta encontrarse con un zombie para
		 * batallar o hasta llegar al punto final del movimiento (en caso de que no haya batalla)
		 */

		single_action = CCCallFuncND::create( this, callfuncND_selector(PlayScene::checkBattle), NULL);
		actions->addObject(single_action);

		CCSequence *sq = CCSequence::create(actions);

		mSprite[GameState.getCurrentPlayer()]->runAction( sq );
	}

	if ( HAY_BATALLA && !HAY_PREGUNTA && redDices[0]->boundingBox().containsPoint(pto))
	{
		// desactiva touch
		WAIT = true;
		redDiceCallback();
		return;
	}

	CCSprite * life, * bullet;
	life = (CCSprite * ) _stayLayer->getChildByTag(QUESTION_LIFE_ICON_TAG);
	bullet = (CCSprite * ) _stayLayer->getChildByTag(QUESTION_BULLET_ICON_TAG);

	if ( HAY_PREGUNTA ) {
		if (life->boundingBox().containsPoint(pto) && GameState.getCurrentPlayerLife() > 0) {
			modifyPlayerLifes(-1);

			HAY_PREGUNTA = false;
			_stayLayer->getChildByTag(QUESTION_LABEL_TAG)->setVisible(false);
			_stayLayer->getChildByTag(QUESTION_LIFE_ICON_TAG)->setVisible(false);
			_stayLayer->getChildByTag(QUESTION_BULLET_ICON_TAG)->setVisible(false);

		} else if (bullet->boundingBox().containsPoint(pto) && GameState.getCurrentPlayerBullet() + lastRedDiceResult >= 3) {
			modifyPlayerBullets(lastRedDiceResult - 3);
			redDices[lastRedDiceResult]->setVisible(false);
			lastRedDiceResult = 3;
			redDices[lastRedDiceResult]->setVisible(true);

			winBattle(NULL, NULL);

			HAY_PREGUNTA = false;
			_stayLayer->getChildByTag(QUESTION_LABEL_TAG)->setVisible(false);
			_stayLayer->getChildByTag(QUESTION_LIFE_ICON_TAG)->setVisible(false);
			_stayLayer->getChildByTag(QUESTION_BULLET_ICON_TAG)->setVisible(false);
		}

	}
}

void PlayScene::thirdPhase(CCPoint pto, CCPoint ptoConvertido)
{
	// El dado rojo fue cliqueado
	if (!LANZODADOROJO && redDices[0]->boundingBox().containsPoint(pto))
	{
		// desactiva touch
		WAIT = true;
		redDiceCallback();
		LANZODADOROJO = true;
		prevZombieLocation.x = prevZombieLocation.y = -1;
		return;
	}

	WAIT = true;
	CCPoint location, tileLocation;

	// Se verifica si el toque es para seleccionar el zombie a mover:
	if (LANZODADOROJO && prevZombieLocation.x == -1 && prevZombieLocation.y == -1)
	{
		location = axisToMapCardMatrix(ptoConvertido.x,ptoConvertido.y);
		tileLocation = axisToTileMatrix(ptoConvertido.x,ptoConvertido.y);

		CCPoint index_cardMap = axisToMapCardMatrix(ptoConvertido.x, ptoConvertido.y);
		position p((int) index_cardMap.x, (int) index_cardMap.y);
		p.invT();

		if ( GameState.queryZombie(p) && GameState.isValidZombie(p) )
		{
			WAIT = false;
			return;
		}

		prevZombieLocation = tileMatrixToAxis(location.x, location.y, tileLocation.x, tileLocation.y);

		WAIT = false;

	} else { // Va a seleccionar a donde lo quiere mover

		CCPoint nLocation = axisToMapCardMatrix(ptoConvertido.x,ptoConvertido.y);
		CCPoint nTileLocation = axisToTileMatrix(ptoConvertido.x,ptoConvertido.y);

		events.clear();
		events.push_back( getEventZombieMove(prevZombieLocation, ptoConvertido) );

		// Si hay zombie en el nuevo lugar o no es valido el movimiento --> no se mueve
		if (mapCardMatrix[(int)nLocation.x][(int)nLocation.y].grid[(int)nTileLocation.x][(int)nTileLocation.y] == ZOMBIE ||
			mapCardMatrix[(int)nLocation.x][(int)nLocation.y].grid[(int)nTileLocation.x][(int)nTileLocation.y] == ZOMBIE_BULLET ||
			mapCardMatrix[(int)nLocation.x][(int)nLocation.y].grid[(int)nTileLocation.x][(int)nTileLocation.y] == ZOMBIE_LIFE ||
			!mapCardMatrix[(int)nLocation.x][(int)nLocation.y].isOnMap ||
			events[0].orientation == '-'  )
		{
			WAIT = false;
			return;
		}

		//provisional update
		if (mapCardMatrix[(int)nLocation.x][(int)nLocation.y].grid[(int)nTileLocation.x][(int)nTileLocation.y] == BULLET)
			mapCardMatrix[(int)nLocation.x][(int)nLocation.y].grid[(int)nTileLocation.x][(int)nTileLocation.y] = ZOMBIE_BULLET;
		else if (mapCardMatrix[(int)nLocation.x][(int)nLocation.y].grid[(int)nTileLocation.x][(int)nTileLocation.y] == LIFE)
			mapCardMatrix[(int)nLocation.x][(int)nLocation.y].grid[(int)nTileLocation.x][(int)nTileLocation.y] = ZOMBIE_LIFE;
		else
			mapCardMatrix[(int)nLocation.x][(int)nLocation.y].grid[(int)nTileLocation.x][(int)nTileLocation.y] = ZOMBIE;

		// FIN provisional

		// El movimiento es valido:

		location = axisToMapCardMatrix(prevZombieLocation.x,prevZombieLocation.y);
		tileLocation = axisToTileMatrix(prevZombieLocation.x,prevZombieLocation.y);

		// provisional UPDATE
		if (mapCardMatrix[(int)location.x][(int)location.y].grid[(int)tileLocation.x][(int)tileLocation.y] == ZOMBIE_BULLET)
			mapCardMatrix[(int)location.x][(int)location.y].grid[(int)tileLocation.x][(int)tileLocation.y] = BULLET;
		else if (mapCardMatrix[(int)location.x][(int)location.y].grid[(int)tileLocation.x][(int)tileLocation.y] == ZOMBIE_LIFE)
			mapCardMatrix[(int)location.x][(int)location.y].grid[(int)tileLocation.x][(int)tileLocation.y] = LIFE;
		else
			mapCardMatrix[(int)location.x][(int)location.y].grid[(int)tileLocation.x][(int)tileLocation.y] = 0;

		// Se busca el sprite
		for (int i = 0; i < zombies.size(); i ++){
			if ( zombies[i].mapCard_i == (int) location.x     && zombies[i].mapCard_j == (int) location.y      &&
				 zombies[i].tile_i    == (int) tileLocation.x && zombies[i].tile_j    == (int) tileLocation.y)
			{
				zombieSpriteToMove = i;
				break;
			}
		}

		// Se actualiza el vector
		zombies[zombieSpriteToMove].mapCard_i = (int) nLocation.x;
		zombies[zombieSpriteToMove].mapCard_j = (int) nLocation.y;
		zombies[zombieSpriteToMove].tile_i = (int) nTileLocation.x;
		zombies[zombieSpriteToMove].tile_j = (int) nTileLocation.y;

		prevZombieLocation = zombies[zombieSpriteToMove].sprite->getPosition();

		// Se mueve el sprite. Aqui, evento != NULL
		CCFiniteTimeAction* animate_zombie_action, *move_zombie_action, *stop_zombie_move_action, *activate_touch_action, *delay;
		CCSequence *sq;

		NUM_OF_ZOMBIES_TO_MOVE--;

		animate_zombie_action = (CCFiniteTimeAction*) CCCallFuncND::create( this, callfuncND_selector(PlayScene::setMoveZombieSprite), (void*) &events[0]);
		move_zombie_action = (CCFiniteTimeAction*) CCCallFuncND::create( this, callfuncND_selector(PlayScene::animateZombieSprite), (void*) &events[0]);
		delay = CCDelayTime::create(abs(events[0].movement) / VELOCITY_SPRITE_MOVEMENT);
		stop_zombie_move_action = (CCFiniteTimeAction*) CCCallFuncN::create( this, callfuncN_selector(PlayScene::setStaticZombieSprite));
		activate_touch_action = (CCFiniteTimeAction*) CCCallFuncND::create( this, callfuncND_selector(PlayScene::activateTouch), NULL);

		sq = (CCSequence *) CCSequence::create(animate_zombie_action,
                                               move_zombie_action,
                                               delay,
                                               stop_zombie_move_action,
                                               activate_touch_action,
                                               (!NUM_OF_ZOMBIES_TO_MOVE? CCCallFuncND::create( this, callfuncND_selector(PlayScene::changePhase), NULL) : NULL),
                                               NULL);

		// SIEMPRE DEBE SUCEDER que zombieSpriteToMove != null
		zombies[zombieSpriteToMove].sprite->runAction( sq );
	}

}

void PlayScene::showFirstPhase()
{
	_stayLayer->getChildByTag(MAP_CARD_DECK)->setVisible(true);
	_stayLayer->getChildByTag(MAP_CARD_REVERSE)->setVisible(false);
}

void PlayScene::hideFirstPhase()
{
	_stayLayer->getChildByTag(MAP_CARD_SELECTED)->setVisible(false);
	_stayLayer->getChildByTag(ROTATE_ICON)->setVisible(false);

	CCSprite * sprite = (CCSprite *) _stayLayer->getChildByTag(MAP_CARD_DECK);
	sprite->setVisible(false);
	sprite->runAction(CCFadeIn::create(1));
}

void PlayScene::showSecondPhase()
{
	redDices[lastRedDiceResult]->setVisible(true);
	blueDices[lastBlueDiceResult]->setVisible(true);
}

void PlayScene::hideSecondPhase()
{
	blueDices[lastBlueDiceResult]->setVisible(false);
	redDices[lastRedDiceResult]->setVisible(false);
}

void PlayScene::showThirdPhase()
{
	redDices[lastRedDiceResult]->setVisible(true);
	blueDices[lastBlueDiceResult]->setVisible(true);
}

void PlayScene::hideThirdPhase()
{
	blueDices[lastBlueDiceResult]->setVisible(false);
	redDices[lastRedDiceResult]->setVisible(false);
}


void PlayScene::setPickMapCard()
{
	// Mazo
	CCSprite* sprite;
	sprite = CCSprite::create("mazo.png");
	sprite->setPosition(ccp(VisibleRect::leftBottom().x + sprite->getContentSize().width/2,
							  VisibleRect::leftBottom().y + sprite->getContentSize().height/2 ));
	sprite->setTag(MAP_CARD_DECK);
	sprite->setVisible(false);
	_stayLayer->addChild(sprite);

	// reversed mapCard
	sprite = CCSprite::create("reverse_map_card.png");
	sprite->setPosition(ccp(VisibleRect::leftBottom().x + sprite->getContentSize().width/2 + 10,
							VisibleRect::leftBottom().y + sprite->getContentSize().height/2 + 10));
	sprite->setTag(MAP_CARD_REVERSE);
	sprite->setVisible(false);
	_stayLayer->addChild(sprite);

	// Rotate
	int offset_x = sprite->getContentSize().width / 2 + 8;
	int offset_y = sprite->getPositionY();

	sprite = CCSprite::create("rotate.png");
	sprite->setPosition(ccp(VisibleRect::bottom().x + sprite->getContentSize().width/2 + offset_x,
							offset_y));
	sprite->setTag(ROTATE_ICON);
	sprite->setVisible(false);
	_stayLayer->addChild(sprite);

}

void PlayScene::initFlipXCallback(CCObject* target)
{
	CCSprite * reversed_sprite = (CCSprite *) _stayLayer->getChildByTag(MAP_CARD_REVERSE);
	// Aqui el chino deberia dar el id de la carta seleccionada (generada random)
	// y sacar con el id el path: arreglo[id].path
	mapCard card = GameState.pickMapCard();

	reversed_sprite->setVisible(true);
	CCSprite * sprite = CCSprite::create(card.getPath().c_str());

    // 110 y 300
	sprite->setScale(reversed_sprite->getContentSize().width/PIXELS_MAP_CARD);

	sprite->setPosition(ccp(VisibleRect::bottom().x,
			                VisibleRect::bottom().y + reversed_sprite->getContentSize().height/2));
	sprite->setTag(MAP_CARD_SELECTED);
	sprite->setVisible(false);
	_stayLayer->addChild(sprite);

	CCActionInterval*  moveToBottom = CCMoveTo::create(1, sprite->getPosition());
    CCOrbitCamera* camera = CCOrbitCamera::create(1, 1, 0, 0, 90, 0, 0);
    CCHide* hide = CCHide::create();
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(PlayScene::show_mapCard_selected));
    CCActionInterval* action = (CCActionInterval*) CCSequence::create(moveToBottom, camera, hide, func, NULL);
    reversed_sprite->runAction(action);
}

void PlayScene::show_mapCard_selected(CCNode* node)
{
	CCSprite * icon = (CCSprite *) _stayLayer->getChildByTag(ROTATE_ICON);
	icon->setVisible(true);

	CCSprite * sprite = (CCSprite *) _stayLayer->getChildByTag(MAP_CARD_SELECTED);
    CCShow* show = CCShow::create();
    CCOrbitCamera* camera = CCOrbitCamera::create(1, 1, 0, 270, 90, 0, 0);
    CCActionInterval* action = (CCActionInterval*) CCSequence::create(show, camera, NULL);
    sprite->runAction(action);

    camera = CCOrbitCamera::create(1, 1, 0, 0, 0, 0, 0);
    sprite = (CCSprite * ) _stayLayer->getChildByTag(MAP_CARD_REVERSE);
	sprite->setPosition(ccp(VisibleRect::leftBottom().x + sprite->getContentSize().width/2 + 10,
							VisibleRect::leftBottom().y + sprite->getContentSize().height/2 + 10));
	sprite->runAction(CCSequence::create(camera,
			CCCallFuncND::create( this, callfuncND_selector(PlayScene::activateTouch), NULL),
			CCCallFuncND::create( this, callfuncND_selector(PlayScene::changeSubPhase), NULL),
			NULL));

	sprite = (CCSprite *) _stayLayer->getChildByTag(MAP_CARD_DECK);
	sprite->runAction(CCFadeOut::create(1));

    PUTMAPCARD = true;
}

void PlayScene::rotate_mapCard()
{
	currCardRotation = (currCardRotation + 1) % 4;

	CCSprite * sprite = (CCSprite *) _stayLayer->getChildByTag(MAP_CARD_SELECTED);
	CCRotateTo* rotate_action = CCRotateTo::create(2, 90 * currCardRotation);
	sprite->runAction(CCSequence::create(rotate_action,
			CCCallFuncND::create( this, callfuncND_selector(PlayScene::activateTouch), NULL),
			NULL));
}

void PlayScene::setBackgrounds()
{
	float h;
	CCLabelTTF * label = (CCLabelTTF *) _stayLayer->getChildByTag(TITLE_LABEL_TAG);
	h = label->getContentSize().height;

	label = (CCLabelTTF *) _stayLayer->getChildByTag(SUBTITLE_LABEL_TAG);
	h += label->getContentSize().height + 3;

    CCSize boxSize = CCSizeMake(VisibleRect::getVisibleRect().size.width, h);

    CCLayerColor *box = CCLayerColor::create(ccc4(0, 0, 0, 0));
    box->setPosition(ccp(VisibleRect::top().x-boxSize.width/2, VisibleRect::top().y-h));
    box->setContentSize(boxSize);
    box->setOpacity((GLubyte)(140));
    _stayLayer->addChild(box, 1);
}

void PlayScene::checkBattle(CCNode* sender, void* data)
{
	CCPoint point = mSprite[GameState.getCurrentPlayer()]->getPosition();
	CCPoint mapCard_player_location = axisToMapCardMatrix( (int)point.x, (int)point.y );
	CCPoint tile_player_location = axisToTileMatrix( (int)point.x, (int)point.y );
	int content = mapCardMatrix[(int)mapCard_player_location.x][(int)mapCard_player_location.y].grid[(int)tile_player_location.x][(int)tile_player_location.y];

	if ( content == ZOMBIE || content == ZOMBIE_LIFE || content == ZOMBIE_BULLET )
	{
		HAY_BATALLA = true;
		msg = "A batallar ! Lanza el dado rojo";
		putSubtitleInformation(NULL, NULL);
	} else if ( LANZODADOAZUL ) {
		checkLifeAndBullet(NULL, NULL);
	}
}

void PlayScene::checkLifeAndBullet(CCNode* sender, void* data)
{
	CCPoint point;
	CCPoint ij_cardMap = axisToMapCardMatrix(mSprite[GameState.getCurrentPlayer()]->getPositionX(), mSprite[GameState.getCurrentPlayer()]->getPositionY());
	CCPoint ij_tile = axisToTileMatrix(mSprite[GameState.getCurrentPlayer()]->getPositionX(), mSprite[GameState.getCurrentPlayer()]->getPositionY());

	int content = mapCardMatrix[(int)ij_cardMap.x][(int)ij_cardMap.y].grid[(int)ij_tile.x][(int)ij_tile.y];

	if ( content == LIFE )
	{
		for (int i = 0; i < lifes.size(); i++) {
			if ( lifes[i].mapCard_i == (int)ij_cardMap.x &&  lifes[i].mapCard_j == (int)ij_cardMap.y &&
					lifes[i].tile_i == (int)ij_tile.x &&  lifes[i].tile_j == (int)ij_tile.y) {

				point = _moveLayer->convertToNodeSpace( _stayLayer->getChildByTag(LIFE_ICON_TAG)->getPosition() );

				lifes[i].sprite->runAction(CCSequence::create(CCMoveTo::create(1, point),
						                      CCCallFuncND::create( this, callfuncND_selector(PlayScene::changePhase), NULL),
	                                          CCCallFuncN::create( this, callfuncN_selector(PlayScene::removeSprite)),
						                      NULL));

				lifes[i] = lifes[lifes.size()-1];
				break;
			}
		}

		// provisional
		mapCardMatrix[(int)ij_cardMap.x][(int)ij_cardMap.y].grid[(int)ij_tile.x][(int)ij_tile.y] = 0;
		modifyPlayerLifes(1);
		lifes.pop_back();
		// FIN provisional

	} else if ( content == BULLET ) {

		for (int i = 0; i < bullets.size(); i++) {
			if ( bullets[i].mapCard_i == (int)ij_cardMap.x &&  bullets[i].mapCard_j == (int)ij_cardMap.y &&
					bullets[i].tile_i == (int)ij_tile.x &&  bullets[i].tile_j == (int)ij_tile.y) {
				point = _moveLayer->convertToNodeSpace( _stayLayer->getChildByTag(BULLET_ICON_TAG)->getPosition() );

				bullets[i].sprite->runAction(CCSequence::create(CCMoveTo::create(1, point),
						                        CCCallFuncND::create( this, callfuncND_selector(PlayScene::changePhase), NULL),
	                                            CCCallFuncN::create( this, callfuncN_selector(PlayScene::removeSprite)),
						                        NULL));

				bullets[i] = bullets[bullets.size()-1];
				break;
			}
		}

		// provisional
		mapCardMatrix[(int)ij_cardMap.x][(int)ij_cardMap.y].grid[(int)ij_tile.x][(int)ij_tile.y] = 0;
		modifyPlayerBullets(1);
		bullets.pop_back();
		// FIN provisional

	} else {
		changePhase(NULL,NULL);
	}
}

void PlayScene::winBattle(CCNode* sender, void * data)
{
	msg = "Has ganado...";
	putSubtitleInformation(NULL, NULL);

	HAY_BATALLA = false;

	CCPoint ij_cardMap = axisToMapCardMatrix(mSprite[GameState.getCurrentPlayer()]->getPositionX(), mSprite[GameState.getCurrentPlayer()]->getPositionY());
	CCPoint ij_tile = axisToTileMatrix(mSprite[GameState.getCurrentPlayer()]->getPositionX(), mSprite[GameState.getCurrentPlayer()]->getPositionY());
	CCPoint point;
	CCArray * actions = CCArray::createWithCapacity(7);
	CCSprite * sprite;

	for (int i = 0; i < zombies.size(); i++) {
		if ( zombies[i].mapCard_i == (int)ij_cardMap.x &&  zombies[i].mapCard_j == (int)ij_cardMap.y &&
		     zombies[i].tile_i == (int)ij_tile.x &&  zombies[i].tile_j == (int)ij_tile.y) {

			sprite = zombies[i].sprite;
			zombies[i] = zombies[zombies.size()-1];
			break;
		}
	}

	for (int i = 0; i < lifes.size(); i++) {
		if ( lifes[i].mapCard_i == (int)ij_cardMap.x &&  lifes[i].mapCard_j == (int)ij_cardMap.y &&
				lifes[i].tile_i == (int)ij_tile.x &&  lifes[i].tile_j == (int)ij_tile.y) {

			point = _moveLayer->convertToNodeSpace( _stayLayer->getChildByTag(LIFE_ICON_TAG)->getPosition() );

			lifes[i].sprite->runAction(CCSequence::create(CCMoveTo::create(1, point),
                                          CCCallFuncN::create( this, callfuncN_selector(PlayScene::removeSprite)),
					                      NULL));

			lifes[i] = lifes[lifes.size()-1];
			break;
		}
	}

	for (int i = 0; i < bullets.size(); i++) {
		if ( bullets[i].mapCard_i == (int)ij_cardMap.x &&  bullets[i].mapCard_j == (int)ij_cardMap.y &&
				bullets[i].tile_i == (int)ij_tile.x &&  bullets[i].tile_j == (int)ij_tile.y) {
			point = _moveLayer->convertToNodeSpace( _stayLayer->getChildByTag(BULLET_ICON_TAG)->getPosition() );

			bullets[i].sprite->runAction(CCSequence::create(CCMoveTo::create(1, point),
                                          CCCallFuncN::create( this, callfuncN_selector(PlayScene::removeSprite)),
					                      NULL));

			bullets[i] = bullets[bullets.size()-1];
			break;
		}
	}

	if ( GameState.queryZombie() && GameState.queryBullet() ) {
		modifyPlayerBullets(1);
		bullets.pop_back();
	} else if ( GameState.queryZombie() && GameState.queryLife() ) {
		modifyPlayerLifes(1);
		lifes.pop_back();
	}

	point = _moveLayer->convertToNodeSpace( _stayLayer->getChildByTag(ZOMBIE_ICON_TAG)->getPosition() );
	actions->addObject(CCMoveTo::create(1, point));

	if (GameState.getCurrentPlayerZombie() + 1 == ZOMBIES_TO_WIN) {
		actions->addObject(CCCallFunc::create( this, callfunc_selector(PlayScene::gameOver)));
	} else {
		if ( LANZODADOAZUL ) {
			actions->addObject(CCCallFuncND::create( this, callfuncND_selector(PlayScene::changePhase), NULL));
		} else {
			msg = name[1][0];
			actions->addObject(CCCallFuncND::create( this, callfuncND_selector(PlayScene::putSubtitleInformation), NULL));
		}

		zombies.pop_back();
		modifyPlayerZombies();

		actions->addObject(CCCallFuncN::create( this, callfuncN_selector(PlayScene::removeSprite)));
	}

	sprite->runAction(CCSequence::create(actions));
}

void PlayScene::removeSprite(CCNode* sender, void * data)
{
	_moveLayer->removeChild(sender);
}

void PlayScene::loseBattleAndDie(CCNode* sender)
{
	msg = "Has perdido ...";
	putSubtitleInformation(NULL, NULL);
	HAY_BATALLA = false;

	CCPoint point = tileMatrixToAxis(MAX_MAP_DIM/2,MAX_MAP_DIM/2,1,1);

	point.y += (mSprite[0]->getContentSize().height / 2);
	if (GameState.getCurrentPlayer() == 0)
		point.x += (mSprite[GameState.getCurrentPlayer()]->getContentSize().width / 2);
	else
		point.x -= (mSprite[GameState.getCurrentPlayer()]->getContentSize().width / 2);

	float distance = mSprite[GameState.getCurrentPlayer()]->getPosition().getDistance(point);
	mSprite[GameState.getCurrentPlayer()]->runAction(CCSequence::create(CCMoveTo::create(distance/VELOCITY_SPRITE_MOVEMENT, point),
			CCCallFuncN::create( this, callfuncN_selector(PlayScene::changePhase)),
			CCCallFuncN::create( this, callfuncN_selector(PlayScene::changePhase)),
			NULL));

//	_moveLayer->runAction(CCFollow::create(mSprite[GameState.getCurrentPlayer()],
//			  CCRectMake(0,0,_moveLayer->getContentSize().width, _moveLayer->getContentSize().height)));

	GameState.killcurrentPlayer();

	modifyScreenLifes();
	modifyScreenBullets();
	modifyScreenZombies();
}

void PlayScene::continueBattle(CCNode* sender)
{
	msg = "Toma una decisión ...";
	putSubtitleInformation(NULL, NULL);

	_stayLayer->getChildByTag(QUESTION_LABEL_TAG)->setVisible(true);

	if (GameState.getCurrentPlayerLife() > 0)
		_stayLayer->getChildByTag(QUESTION_LIFE_ICON_TAG)->setVisible(true);

	if (GameState.getCurrentPlayerBullet() + lastRedDiceResult >= 3)
		_stayLayer->getChildByTag(QUESTION_BULLET_ICON_TAG)->setVisible(true);

	HAY_PREGUNTA = true;
}

void PlayScene::gameOver() {

	this->removeChild(_moveLayer);
	this->removeChild(_stayLayer);

	CCLayer * _gameOverLayer = CCLayer::create();
	char endMsg[30];
	sprintf(endMsg, "Ganó el jugador %d", GameState.getCurrentPlayer() + 1);
	CCLabelTTF * label = CCLabelTTF::create("GAME OVER", "Arial", FONT_SIZE + 20);
	label->setPosition(ccp(VisibleRect::center().x, VisibleRect::center().y + 20));

	int off_y = label->getContentSize().height/2 + 10;
	_gameOverLayer->addChild(label);

	label = CCLabelTTF::create(endMsg, "Arial", FONT_SIZE + 10);
	label->setPosition(ccp(VisibleRect::center().x, VisibleRect::center().y - off_y + 20));
	_gameOverLayer->addChild(label);

	setMenuBackMenu(_gameOverLayer);

	this->addChild(_gameOverLayer);
}
