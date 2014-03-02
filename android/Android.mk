CHR_SRC = $(CINDER_PATH)/blocks/new-chronotext-toolkit/src
UTF8_CPP_SRC = $(CINDER_PATH)/include/utf8-cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(CHR_SRC)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(UTF8_CPP_SRC)

LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/android/cinder/CinderDelegate.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/android/cinder/CinderSketchComplex.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/cinder/CinderApp.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/FileSystem.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/InputSource.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/FontMatrix.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/xf/FontManager.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/xf/Font.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/xf/FontSequence.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/xf/TextHelper.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/os/Handler.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/os/SuperHandler.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/path/FollowablePath.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/path/SplinePath.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/PVRHelper.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/Sprite.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/Texture.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/TextureHelper.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/TextureManager.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/time/Clock.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/time/MasterClock.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/time/TickingClock.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/utils/GLUtils.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/utils/MathUtils.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/utils/Utils.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/atlas/TextureAtlas.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/atlas/TextureAtlasAnimation.cpp

ifdef CHR_USE_ZFONT
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/zf/ActualFont.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/zf/FontManager.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/zf/FontSequence.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/zf/FontTexture.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/zf/LangHelper.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/zf/LayoutCache.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/zf/LineLayout.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/zf/scrptrun.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/zf/TextItemizer.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/zf/VirtualFont.cpp
endif

ifdef CHR_USE_INCUBATOR_UI
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/Button.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/Container.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/Fill.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/HorizontalLayout.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/HyperTextBox.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/Icon.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/Shape.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/TextBox.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/Touchable.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/TouchableLink.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/TouchGroup.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/VerticalLayout.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/incubator/ui/WordWrapper.cpp
endif
