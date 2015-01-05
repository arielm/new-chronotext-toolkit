LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(CINDER_PATH)/include/utf8-cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(CINDER_PATH)/src/libtess2
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(CINDER_PATH)/blocks/Freetype/include

CHR_SRC = $(CHR_BLOCK)/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(CHR_SRC)

LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/Context.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/Log.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/InputSource.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/system/SystemManagerBase.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/android/cinder/CinderDelegate.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/android/cinder/CinderSketch.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/android/cinder/JNI.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/android/system/SystemManager.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/android/system/MemoryManager.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/system/DisplayHelper.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/system/FileHelper.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/os/Handler.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/os/SuperHandler.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/os/Task.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/os/TaskManager.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/quad/QuadMatrix.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/PVRHelper.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/Texture.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/TextureHelper.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/TextureManager.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/time/Clock.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/time/FrameClock.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/utils/GLUtils.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/utils/MathUtils.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/utils/Utils.cpp

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

ifdef CHR_USE_XFONT
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/xf/FontManager.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/xf/Font.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/xf/FontSequence.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/font/xf/TextHelper.cpp

    CHR_USE_PATH := 1
endif

ifdef CHR_USE_PATH
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/path/Hairline.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/path/FollowablePath.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/path/FXGDocument.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/path/ShapeMesh.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/path/ShapeTesselator.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/path/SplinePath.cpp
endif

ifdef CHR_USE_ATLAS
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/atlas/Sprite.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/atlas/TextureAtlas.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/atlas/TextureAtlasAnimation.cpp
endif

ifdef CHR_USE_FMOD_SOUND
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/sound/Effect.cpp
    LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/sound/SoundManager.cpp
endif
