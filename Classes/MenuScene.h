/*
 * MenuScene.h
 *
 *  Created on: 02/04/2014
 *      Author: hector
 */

#ifndef MENUSCENE_H_
#define MENUSCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class MenuScene : public CCLayer
{
public:
	virtual bool init();

	static CCScene* scene();

	void menuCallBack(CCObject * pSender);

	CREATE_FUNC(MenuScene);

private:
	CCMenu* menuOptions;
	void close();
	void play();
	void credits();
};

#endif /* MENUSCENE_H_ */
