/*
 * MenuScene.h
 *
 *  Created on: 02/04/2014
 *      Author: hector
 */

#ifndef _MENUSCENE_H_
#define _MENUSCENE_H_

#include "cocos2d.h"
#include "CreditsScene.h"
#include "PlayScene.h"
#include "VisibleRect.h"
#include "support/CCPointExtension.h"

USING_NS_CC;

class MenuScene : public CCLayer
{
public:
	/*
	 * Metodo utilizado para inicializar todos los componentes que poseerá la
	 * escena del menu principal
	 */
	virtual bool init();

	/*
	 * Esta sera la funcion a llamar cada vez que sea necesaria la creacion de
	 * la escena del menu principal. Este método se apoya en el método init.
	 */
	static CCScene* scene();

	/*
	 * Llamada encargada de manejar la navegacion a las pantallas del juego, los
	 * creditos y para salir de la aplicacion
	 */
	void menuCallBack(CCObject * pSender);

	CREATE_FUNC(MenuScene);

private:
	CCMenu* menuOptions;
	/*
	 * Funciones auxiliares utilizadas por el callback del menu para la navegacion
	 */
	void close();
	void play();
	void credits();
};

#endif /* MENUSCENE_H_ */
