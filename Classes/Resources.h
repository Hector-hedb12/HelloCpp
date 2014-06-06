#ifndef _RESOURCES_H_
#define _RESOURCES_H_

/*
 * Archivo de encabezado que posee la mayoria de las constantes y recursos
 * utilizados a lo largo del proyecto
 */

#include "cocos2d.h"
#include <string>

USING_NS_CC;

#define FONT_SIZE ( isScreenSizeHD() ? 18 : 16)

static const int PLAY    = 1;
static const int CREDITS = 2;
static const int EXIT    = 3;
static const int MENU_OPTIONS_COUNT = 3;

static bool isScreenSizeHD() {
    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    CCLOG("size: %f ", screenSize.height);

    if (screenSize.height > 320)
    	return true;

    return false;
}

static const int NUM_OF_PLAYER = 2;
static const int NUM_OF_USERS = 1;
static const int NUM_OF_MACHINES = 1;
static const int VELOCITY_SPRITE_MOVEMENT = 100.0;
static const std::string PATH_PLAYER_SPRITE[] = {"sprite/player.png","sprite/cpu.png"};
static const int UP = 0;
static const int RIGHT = 1;
static const int DOWN = 2;
static const int LEFT = 3;
static const int NUM_FASES = 3;
static const int NUM_SUB_FASES = 4;
static const int ZOMBIES_TO_WIN = 25;
static const float EPSILON_DISTANCE = 5;
static const std::string NAME_PLAYER_SPRITE_ANIMATION[][4] = {
		{"player_up","player_right","player_down","player_left"},
		{"cpu_up","cpu_right","cpu_down","cpu_left"}
};

static const std::string NAME_ZOMBIE_SPRITE_ANIMATION[4] = {"zombie_up","zombie_right","zombie_down","zombie_left"};

// ############################# STRINGS #############################

static const std::string MAIN_MENU_STRING = "Menú Principal";

const std::string MENU_OPTIONS[MENU_OPTIONS_COUNT] =
{
	"Jugar",
	"Créditos",
	"Salir"
};

#endif /* RESOURCES_H_ */
