#include "CreditsScene.h"
#include "MenuScene.h"
#include "Resources.h"
#include "VisibleRect.h"
#include "support/CCPointExtension.h"

USING_NS_CC;

CCScene* CreditsScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CreditsScene *layer = CreditsScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CreditsScene::init()
{
    //////////////////////////////
    // 1. super init first
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
    CCLabelTTF* pLabel = CCLabelTTF::create("Granadenses CardGame", "Arial", FONT_SIZE);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y - pLabel->getContentSize().height - 10));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // ** FIN Titulo **

    CCSprite* pSprite = CCSprite::create("ugr_logo.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(VisibleRect::center().x, VisibleRect::center().y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    return true;
}

void CreditsScene::mainMenuCallback(CCObject* pSender)
{
	CCScene* pScene = MenuScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}
