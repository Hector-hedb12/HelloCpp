LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

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
                   ../../Classes/PopupLayer.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,extensions)