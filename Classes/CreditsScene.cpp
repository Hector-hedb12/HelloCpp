/*
 * Jose Gregorio Piñero
 * Hector Dominguez
 * Jose Sanchez
 *
 */

#include "CreditsScene.h"

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
    
    setMenuBackMenu();

    // ** Titulo **
    CCLabelTTF* pLabel = CCLabelTTF::create("Zombies!!! El Juego", "Arial", FONT_SIZE);
    pLabel->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y - pLabel->getContentSize().height - 10));
    addChild(pLabel, 1);
    // ** FIN Titulo **

    // ** Logo **
    CCSprite* pSprite = CCSprite::create("ugr_logo.png");
    pSprite->setPosition( ccp(VisibleRect::center().x, VisibleRect::center().y + 10) );
    addChild(pSprite, 0);
    // ** FIN Logo **
    
    // ** Creadores **
    pLabel = CCLabelTTF::create("Artica Media, C.A.", "Arial", FONT_SIZE - 5);
    pLabel->setPosition(ccp(VisibleRect::bottom().x, VisibleRect::bottom().y + pLabel->getContentSize().height - 5));
    addChild(pLabel, 1);

    pLabel = CCLabelTTF::create("José Sánchez", "Arial", FONT_SIZE - 5);
    pLabel->setPosition(ccp(VisibleRect::bottom().x, VisibleRect::bottom().y + pLabel->getContentSize().height*2 - 5*2));
    addChild(pLabel, 1);

    pLabel = CCLabelTTF::create("Héctor Domínguez", "Arial", FONT_SIZE - 5);
    pLabel->setPosition(ccp(VisibleRect::bottom().x, VisibleRect::bottom().y + pLabel->getContentSize().height*3 - 5*3));
    addChild(pLabel, 1);

    pLabel = CCLabelTTF::create("José Piñero", "Arial", FONT_SIZE - 5);
    pLabel->setPosition(ccp(VisibleRect::bottom().x, VisibleRect::bottom().y + pLabel->getContentSize().height*4 - 5*4));
    addChild(pLabel, 1);

    pLabel = CCLabelTTF::create("Creadores:", "Arial", FONT_SIZE);
    pLabel->setPosition(ccp(VisibleRect::bottom().x, VisibleRect::bottom().y + pLabel->getContentSize().height*5 - 5*8));
    addChild(pLabel, 1);
    // ** FIN Creadores **

    return true;
}

void CreditsScene::mainMenuCallback(CCObject* pSender)
{
	CCScene* pScene = MenuScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void CreditsScene::setMenuBackMenu()
{
	CCMenuItemImage * itemImage = CCMenuItemImage::create("Menu.png","Menu.png",this,menu_selector(CreditsScene::mainMenuCallback));

	itemImage->setPosition(ccp(VisibleRect::rightBottom().x - itemImage->getContentSize().width/2,
                               VisibleRect::rightBottom().y + itemImage->getContentSize().height/2));

    CCMenu* pMenu = CCMenu::create(itemImage, NULL);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu, 1);
}
