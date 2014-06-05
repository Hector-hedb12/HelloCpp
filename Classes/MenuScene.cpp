/*
 * MenuScene.cpp
 *
 *  Created on: 02/04/2014
 *      Author: hector
 */

#include "MenuScene.h"

static const int LINE_SPACE = 40;
static CCPoint s_tCurPos = CCPointZero;

CCScene* MenuScene::scene()
{
    CCScene *scene = CCScene::create();

    MenuScene *layer = MenuScene::create();

    // agrega layer como un hijo de escena
    scene->addChild(layer);

    // retorna la escena
    return scene;
}

bool MenuScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCSprite* mSprite = CCSprite::create("bg.png");
    mSprite->setPosition( VisibleRect::center() );
    mSprite->setScaleX(VisibleRect::getVisibleRect().size.width / mSprite->getContentSize().width);
    mSprite->setScaleY(VisibleRect::getVisibleRect().size.height / mSprite->getContentSize().height);
    addChild(mSprite, 0);

    mSprite = CCSprite::create("logo_home.png");
    mSprite->setPosition( ccp( VisibleRect::top().x , VisibleRect::top().y - mSprite->getContentSize().height / 2 - 10 ) );
    addChild(mSprite, 0);

    // Crea el menu y agrega las opciones con su respectivo callback
	menuOptions = CCMenu::create();

	CCMenuItemImage * itemImage = CCMenuItemImage::create("boton_jugar.png","boton_jugar.png",this,menu_selector(MenuScene::menuCallBack));
	itemImage->setPosition( ccp( VisibleRect::center().x , VisibleRect::center().y - mSprite->getContentSize().height / 2) );
	menuOptions->addChild(itemImage, 1);

	itemImage = CCMenuItemImage::create("boton_creditos.png","boton_creditos.png",this,menu_selector(MenuScene::menuCallBack));
	itemImage->setPosition( ccp( VisibleRect::leftBottom().x + itemImage->getContentSize().width / 2 + 5, VisibleRect::leftBottom().y + itemImage->getContentSize().height / 2 + 5) );
	menuOptions->addChild(itemImage, 2);

	itemImage = CCMenuItemImage::create("boton_salir.png","boton_salir.png",this,menu_selector(MenuScene::menuCallBack));
	itemImage->setPosition( ccp( VisibleRect::rightBottom().x - itemImage->getContentSize().width / 2 - 5 , VisibleRect::rightBottom().y + itemImage->getContentSize().height / 2 + 5) );
	menuOptions->addChild(itemImage, 3);


//	for ( int i = 0;  i < MENU_OPTIONS_COUNT; ++ i) {
//		CCLabelTTF* label = CCLabelTTF::create(MENU_OPTIONS[i].c_str(), "Arial", FONT_SIZE);
//		CCMenuItemLabel* itemLabel = CCMenuItemLabel::create(label,this,menu_selector(MenuScene::menuCallBack));
//		menuOptions->addChild(itemLabel,i);
//		itemLabel->setPosition( ccp( VisibleRect::center().x , VisibleRect::center().y + ((MENU_OPTIONS_COUNT/2 + MENU_OPTIONS_COUNT%2 - (i+1)) * LINE_SPACE) ) );
//	}

	menuOptions->setContentSize(CCSizeMake(VisibleRect::getVisibleRect().size.width, (MENU_OPTIONS_COUNT + 1) * (LINE_SPACE)));
	menuOptions->setPosition(s_tCurPos);
	addChild(menuOptions);
	setTouchEnabled(true);

	return true;
}

void MenuScene::menuCallBack(CCObject * pSender)
{
	// obtiene la opcion del menu cliqueada por el usuario
	CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
	int nIdx = pMenuItem->getZOrder();

	// verifica a cual pantalla se enviara al usuario
	switch (nIdx)
	{
	case PLAY:
		play();
		break;
	case CREDITS:
		credits();
		break;
	case EXIT:
		close();
		break;
	default:
		break;
	}
}


void MenuScene::close()
{
	// Cierra la aplicacion

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");

	#else
		CCDirector::sharedDirector()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#endif
#endif
}

void MenuScene::credits()
{
	// Inicializa la escena de creditos y la muestra
	CCScene * creditsScene = CreditsScene::scene();

	if (creditsScene)
	{
		CCDirector::sharedDirector()->replaceScene(creditsScene);
	}
}

void MenuScene::play()
{
	// Inicializa la escena de Juego y la muestra
	CCScene * playScene = PlayScene::scene();

	if (playScene)
	{
		CCDirector::sharedDirector()->replaceScene(playScene);
	}
}
