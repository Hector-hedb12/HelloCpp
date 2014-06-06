LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#NDK_TOOLCHAIN_VERSION := 4.8
# APP_STL := stlport_shared  --> does not seem to contain C++11 features
#APP_STL := gnustl_shared
# Enable c++11 extentions in source code
#APP_CPPFLAGS += -std=c++11

LOCAL_CFLAGS := -std=gnu++11


LOCAL_MODULE := hellocpp_shared

LOCAL_MODULE_FILENAME := libhellocpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/CCLayerPanZoom.cpp \
                   ../../Classes/CreditsScene.cpp \
                   ../../Classes/MenuScene.cpp \
                   ../../Classes/PlayScene.cpp \
                   ../../Classes/VisibleRect.cpp \
                   ../../Classes/Modelo/map.cpp \
                   ../../Classes/Modelo/mapcard.cpp \
                   ../../Classes/Modelo/position.cpp \
                   ../../Classes/Modelo/state.cpp \
                   ../../Classes/Modelo/tile.cpp \
                   ../../Classes/Modelo/user.cpp \
                   ../../Classes/Modelo/strategy.cpp \
                   ../../Classes/PopupLayer.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,extensions)
$(call import-module,CocosDenshion/android)