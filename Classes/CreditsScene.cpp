/*
 * Jose Gregorio Piñero
 * Hector Dominguez
 * Jose Sanchez
 *
 */

#include "CreditsScene.h"
#include "MenuScene.h"
#include "Resources.h"
#include "VisibleRect.h"
#include "support/CCPointExtension.h"

USING_NS_CC;

CCScene* CreditsScene::scene()
{
    CCScene *scene = CCScene::create();
    
    CreditsScene *layer = CreditsScene::create();

    // agrega layer como hijo de scene
    scene->addChild(layer);

    // retorna la escena
    return scene;
}

bool CreditsScene::init()
{
    //////////////////////////////
    // 1. super init primero
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    // ** Back Menu **

	CCLabelTTF* label = CCLabelTTF::create(MAIN_MENU_STRING.c_str(), "Arial", FONT_SIZE);
	CCMenuItemLabel* itemLabel = CCMenuItemLabel::create(label,this,menu_selector(CreditsScene::mainMenuCallback));
    
	itemLabel->setPosition(ccp(VisibleRect::rightBottom().x - itemLabel->getContentSize().width/2 -10,
                               VisibleRect::rightBottom().y + itemLabel->getContentSize().height/2 +10));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(itemLabel, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    
    // ** FIN: Back Menu **

    // ** Titulo **
    CCLabelTTF* pLabel = CCLabelTTF::create("Zombies!! el juego", "Arial", FONT_SIZE);
    
    // coloca el sprite en el centro de la pantalla
    pLabel->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y - pLabel->getContentSize().height - 10));

    // agrega el label como un hijo del layer principal
    this->addChild(pLabel, 1);

    // ** FIN Titulo **

    CCSprite* pSprite = CCSprite::create("ugr_logo.png");

    // coloca el sprite en el centro de la pantalla
    pSprite->setPosition(ccp(VisibleRect::center().x, VisibleRect::center().y));

    // agrega el sprite como un hijo del layer principal
    this->addChild(pSprite, 0);
    
    return true;
}

void CreditsScene::mainMenuCallback(CCObject* pSender)
{
	CCScene* pScene = MenuScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}
