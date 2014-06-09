#ifndef __CREDITS_SCENE_H__
#define __CREDITS_SCENE_H__

#include "cocos2d.h"
#include "MenuScene.h"
#include "Resources.h"
#include "VisibleRect.h"
#include "support/CCPointExtension.h"

class CreditsScene : public cocos2d::CCLayer
{
public:
    /*
     * Metodo utilizado para inicializar todos los componentes que poseerá la
	 * escena de los créditos
	 */
    virtual bool init();  

    /*
     * Esta sera la funcion a llamar cada vez que sea necesaria la creacion de
     * la escena de los creditos. Este método se apoya en el método init.
     */
    static cocos2d::CCScene* scene();
    
    /*
     * El llamado que se ejecutara cada vez que el boton "Menu Principal" es
     * presionado. Causa que se muestre la pantalla principal del juego
     */
    void mainMenuCallback(CCObject* pSender);
    void setMenuBackMenu();
    
    // implement the "static node()" method manually
    CREATE_FUNC(CreditsScene);
};

#endif // __CREDITS_SCENE_H__
