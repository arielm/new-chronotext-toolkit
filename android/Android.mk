CHR_SRC = $(CINDER_PATH)/blocks/new-chronotext-toolkit/src
UTF8_CPP_SRC = $(CINDER_PATH)/include/utf8-cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(CHR_SRC)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(UTF8_CPP_SRC)

LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/android/cinder/CinderDelegate.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/android/cinder/CinderSketchComplex.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/cinder/CinderApp.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/InputSource.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/FontManager.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/FontMatrix.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/XFont.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/XFontSequence.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/os/Handler.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/os/Looper.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/os/MessageQueue.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/path/ASPC.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/path/FollowablePath.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/path/SplinePath.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/text/TextHelper.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/text/WordWrapper.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/PVRHelper.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/Sprite.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/Texture.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/TextureHelper.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/TextureManager.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/time/Clock.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/time/MasterClock.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/time/TickingClock.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/utils/glUtils.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/utils/MathUtils.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/utils/Utils.cpp

ifdef CHR_USE_TEXTURE_ATLAS
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/TextureAtlas.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/TextureAtlasAnimation.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/TextureAtlasManager.cpp
endif

ifdef CHR_USE_INCUBATOR_UI
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/Button.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/Container.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/HorizontalLayout.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/HyperTextBox.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/Icon.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/Shape.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/TextBox.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/Touchable.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/TouchableLink.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/TouchGroup.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/VerticalLayout.cpp
endif
