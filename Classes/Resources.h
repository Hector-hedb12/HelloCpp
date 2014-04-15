#ifndef RESOURCES_H_
#define RESOURCES_H_

#include <string>
#define FONT_SIZE ( isScreenSizeHD() ? 18 : 10)

static const int PLAY    = 0;
static const int CREDITS = 1;
static const int EXIT    = 2;
static const int MENU_OPTIONS_COUNT = 3;

static bool isScreenSizeHD() {
    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    CCLOG("size: %f ", screenSize.height);

    if (screenSize.height > 320)
    	return true;

    return false;
}

static const int NUM_OF_PLAYER = 2;
static const int VELOCITY_SPRITE_MOVEMENT = 23.0;
static const std::string PATH_PLAYER_SPRITE[] = {"sprite/player_d.png","sprite/player_d.png"};
static const int UP = 0;
static const int RIGHT = 1;
static const int DOWN = 2;
static const int LEFT = 3;
static const std::string NAME_PLAYER_SPRITE_ANIMATION[][4] = {
		{"player_up","player_right","player_down","player_left"},
		{"player_up","player_right","player_down","player_left"}
};

// ############################# STRINGS #############################

static const std::string MAIN_MENU_STRING = "Back Menu";

const std::string MENU_OPTIONS[MENU_OPTIONS_COUNT] =
{
	"Play",
	"Credits",
	"Exit"
};

#endif /* RESOURCES_H_ */
