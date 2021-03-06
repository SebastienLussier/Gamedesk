/**
 *  @file       EngineEnums.cpp
 *  @brief      Enum strings for the Engine module.
 *  @author     gdprop tool.
 *
 *  WARNING!
 *  This file has been autogenerated, all change made will be lost!
 */
#include "Engine.h"
#include "EngineEnums.h"

#if GD_CFG_USE_PROPERTIES == GD_ENABLED

#include ".\Graphic\Renderer.h"
#include ".\Graphic\Texture\TextureStage.h"
#include ".\Graphic\Buffer\IndexBuffer.h"
#include ".\Graphic\Buffer\VertexBuffer.h"
#include ".\Sound\Sound.h"
#include ".\Sound\SoundSubsystem.h"
#include ".\Input\Keyboard.h"
#include ".\Input\Mouse.h"
#include ".\World\TestEntities\TestProperties.h"


namespace Gamedesk {

	
namespace EnumInfo
{
    namespace Renderer
    {
        const char* PixelCompareFunc::mStrings[] =
        {
            "CmpAlways",
            "CmpEqual",
            "CmpGreater",
            "CmpGreaterEqual",
            "CmpLess",
            "CmpLessEqual",
            "CmpNever",
            "CmpNotEqual"
        };
        const UInt32 PixelCompareFunc::mValues[] =
        {
            Gamedesk::Renderer::CmpAlways,
            Gamedesk::Renderer::CmpEqual,
            Gamedesk::Renderer::CmpGreater,
            Gamedesk::Renderer::CmpGreaterEqual,
            Gamedesk::Renderer::CmpLess,
            Gamedesk::Renderer::CmpLessEqual,
            Gamedesk::Renderer::CmpNever,
            Gamedesk::Renderer::CmpNotEqual
        };
        const UInt32 PixelCompareFunc::mSize = 8;
    }

    namespace Renderer
    {
        const char* PixelBlendingFactor::mStrings[] =
        {
            "BlendDstAlpha",
            "BlendDstColor",
            "BlendInvDstAlpha",
            "BlendInvDstColor",
            "BlendInvSrcAlpha",
            "BlendInvSrcColor",
            "BlendOne",
            "BlendSrcAlpha",
            "BlendSrcAlphaSaturate",
            "BlendSrcColor",
            "BlendZero"
        };
        const UInt32 PixelBlendingFactor::mValues[] =
        {
            Gamedesk::Renderer::BlendDstAlpha,
            Gamedesk::Renderer::BlendDstColor,
            Gamedesk::Renderer::BlendInvDstAlpha,
            Gamedesk::Renderer::BlendInvDstColor,
            Gamedesk::Renderer::BlendInvSrcAlpha,
            Gamedesk::Renderer::BlendInvSrcColor,
            Gamedesk::Renderer::BlendOne,
            Gamedesk::Renderer::BlendSrcAlpha,
            Gamedesk::Renderer::BlendSrcAlphaSaturate,
            Gamedesk::Renderer::BlendSrcColor,
            Gamedesk::Renderer::BlendZero
        };
        const UInt32 PixelBlendingFactor::mSize = 11;
    }

    namespace Renderer
    {
        const char* LightType::mStrings[] =
        {
            "LightDirectional",
            "LightPoint",
            "LightSpot"
        };
        const UInt32 LightType::mValues[] =
        {
            Gamedesk::Renderer::LightDirectional,
            Gamedesk::Renderer::LightPoint,
            Gamedesk::Renderer::LightSpot
        };
        const UInt32 LightType::mSize = 3;
    }

    namespace TextureStage
    {
        const char* TextureMode::mStrings[] =
        {
            "Tex_Add",
            "Tex_Blend",
            "Tex_Decal",
            "Tex_Modulate",
            "Tex_Replace"
        };
        const UInt32 TextureMode::mValues[] =
        {
            Gamedesk::TextureStage::Tex_Add,
            Gamedesk::TextureStage::Tex_Blend,
            Gamedesk::TextureStage::Tex_Decal,
            Gamedesk::TextureStage::Tex_Modulate,
            Gamedesk::TextureStage::Tex_Replace
        };
        const UInt32 TextureMode::mSize = 5;
    }

    namespace TextureStage
    {
        const char* TextureCoordGenMode::mStrings[] =
        {
            "Gen_Disabled",
            "Gen_NormalMap",
            "Gen_ReflectionMap",
            "Gen_SphereMap"
        };
        const UInt32 TextureCoordGenMode::mValues[] =
        {
            Gamedesk::TextureStage::Gen_Disabled,
            Gamedesk::TextureStage::Gen_NormalMap,
            Gamedesk::TextureStage::Gen_ReflectionMap,
            Gamedesk::TextureStage::Gen_SphereMap
        };
        const UInt32 TextureCoordGenMode::mSize = 4;
    }

    namespace IndexBuffer
    {
        const char* Usage::mStrings[] =
        {
            "Usage_Dynamic",
            "Usage_Static",
            "Usage_Stream"
        };
        const UInt32 Usage::mValues[] =
        {
            Gamedesk::IndexBuffer::Usage_Dynamic,
            Gamedesk::IndexBuffer::Usage_Static,
            Gamedesk::IndexBuffer::Usage_Stream
        };
        const UInt32 Usage::mSize = 3;
    }

    namespace IndexBuffer
    {
        const char* LockType::mStrings[] =
        {
            "Lock_Full",
            "Lock_Read",
            "Lock_Write"
        };
        const UInt32 LockType::mValues[] =
        {
            Gamedesk::IndexBuffer::Lock_Full,
            Gamedesk::IndexBuffer::Lock_Read,
            Gamedesk::IndexBuffer::Lock_Write
        };
        const UInt32 LockType::mSize = 3;
    }

    namespace VertexBuffer
    {
        const char* Usage::mStrings[] =
        {
            "Usage_Dynamic",
            "Usage_Static",
            "Usage_Stream"
        };
        const UInt32 Usage::mValues[] =
        {
            Gamedesk::VertexBuffer::Usage_Dynamic,
            Gamedesk::VertexBuffer::Usage_Static,
            Gamedesk::VertexBuffer::Usage_Stream
        };
        const UInt32 Usage::mSize = 3;
    }

    namespace VertexBuffer
    {
        const char* LockType::mStrings[] =
        {
            "Lock_Full",
            "Lock_Read",
            "Lock_Write"
        };
        const UInt32 LockType::mValues[] =
        {
            Gamedesk::VertexBuffer::Lock_Full,
            Gamedesk::VertexBuffer::Lock_Read,
            Gamedesk::VertexBuffer::Lock_Write
        };
        const UInt32 LockType::mSize = 3;
    }

    namespace Sound
    {
        const char* eSoundMode::mStrings[] =
        {
            "kSoundModeLoop",
            "kSoundModeOneShot"
        };
        const UInt32 eSoundMode::mValues[] =
        {
            Gamedesk::Sound::kSoundModeLoop,
            Gamedesk::Sound::kSoundModeOneShot
        };
        const UInt32 eSoundMode::mSize = 2;
    }

    namespace Sound
    {
        const char* eSoundConstant::mStrings[] =
        {
            "kStereoPan"
        };
        const UInt32 eSoundConstant::mValues[] =
        {
            Gamedesk::Sound::kStereoPan
        };
        const UInt32 eSoundConstant::mSize = 1;
    }

    namespace SoundSubsystem
    {
        const char* eSpeakerMode::mStrings[] =
        {
            "kSpeakerModeDolbyDigital",
            "kSpeakerModeHeadphone",
            "kSpeakerModeMono",
            "kSpeakerModeQuad",
            "kSpeakerModeStereo",
            "kSpeakerModeSurround"
        };
        const UInt32 eSpeakerMode::mValues[] =
        {
            Gamedesk::SoundSubsystem::kSpeakerModeDolbyDigital,
            Gamedesk::SoundSubsystem::kSpeakerModeHeadphone,
            Gamedesk::SoundSubsystem::kSpeakerModeMono,
            Gamedesk::SoundSubsystem::kSpeakerModeQuad,
            Gamedesk::SoundSubsystem::kSpeakerModeStereo,
            Gamedesk::SoundSubsystem::kSpeakerModeSurround
        };
        const UInt32 eSpeakerMode::mSize = 6;
    }

    namespace SoundSubsystem
    {
        const char* eOutputType::mStrings[] =
        {
            "kOutputTypeA3D",
            "kOutputTypeAlsa",
            "kOutputTypeAutoDetect",
            "kOutputTypeDirectSound",
            "kOutputTypeESD",
            "kOutputTypeNoSound",
            "kOutputTypeOSS",
            "kOutputTypeWinMM"
        };
        const UInt32 eOutputType::mValues[] =
        {
            Gamedesk::SoundSubsystem::kOutputTypeA3D,
            Gamedesk::SoundSubsystem::kOutputTypeAlsa,
            Gamedesk::SoundSubsystem::kOutputTypeAutoDetect,
            Gamedesk::SoundSubsystem::kOutputTypeDirectSound,
            Gamedesk::SoundSubsystem::kOutputTypeESD,
            Gamedesk::SoundSubsystem::kOutputTypeNoSound,
            Gamedesk::SoundSubsystem::kOutputTypeOSS,
            Gamedesk::SoundSubsystem::kOutputTypeWinMM
        };
        const UInt32 eOutputType::mSize = 8;
    }

    namespace SoundSubsystem
    {
        const char* eMixerType::mStrings[] =
        {
            "kMixerTypeAutoDetect",
            "kMixerTypeBlendMode",
            "kMixerTypeMMXP5",
            "kMixerTypeMMXP6",
            "kMixerTypeQualityAutoDetect",
            "kMixerTypeQualityFPU",
            "kMixerTypeQualityMMXP5",
            "kMixerTypeQualityMMXP6"
        };
        const UInt32 eMixerType::mValues[] =
        {
            Gamedesk::SoundSubsystem::kMixerTypeAutoDetect,
            Gamedesk::SoundSubsystem::kMixerTypeBlendMode,
            Gamedesk::SoundSubsystem::kMixerTypeMMXP5,
            Gamedesk::SoundSubsystem::kMixerTypeMMXP6,
            Gamedesk::SoundSubsystem::kMixerTypeQualityAutoDetect,
            Gamedesk::SoundSubsystem::kMixerTypeQualityFPU,
            Gamedesk::SoundSubsystem::kMixerTypeQualityMMXP5,
            Gamedesk::SoundSubsystem::kMixerTypeQualityMMXP6
        };
        const UInt32 eMixerType::mSize = 8;
    }

    namespace Keyboard
    {
        const char* Key::mStrings[] =
        {
            "Key_0",
            "Key_1",
            "Key_2",
            "Key_3",
            "Key_4",
            "Key_5",
            "Key_6",
            "Key_7",
            "Key_8",
            "Key_9",
            "Key_A",
            "Key_Apostrophe",
            "Key_Apps",
            "Key_B",
            "Key_Backslash",
            "Key_Backspace",
            "Key_C",
            "Key_CapsLock",
            "Key_Comma",
            "Key_D",
            "Key_Delete",
            "Key_DownArrow",
            "Key_E",
            "Key_End",
            "Key_Enter",
            "Key_Equals",
            "Key_Escape",
            "Key_F",
            "Key_F1",
            "Key_F10",
            "Key_F11",
            "Key_F12",
            "Key_F13",
            "Key_F14",
            "Key_F15",
            "Key_F2",
            "Key_F3",
            "Key_F4",
            "Key_F5",
            "Key_F6",
            "Key_F7",
            "Key_F8",
            "Key_F9",
            "Key_G",
            "Key_Grave",
            "Key_H",
            "Key_Home",
            "Key_I",
            "Key_Insert",
            "Key_J",
            "Key_K",
            "Key_L",
            "Key_LeftAlt",
            "Key_LeftArrow",
            "Key_LeftBracket",
            "Key_LeftCtrl",
            "Key_LeftShift",
            "Key_LeftWin",
            "Key_M",
            "Key_MaxValue",
            "Key_Minus",
            "Key_N",
            "Key_None",
            "Key_NumAdd",
            "Key_NumDivide",
            "Key_NumKeys",
            "Key_NumLock",
            "Key_NumMultiply",
            "Key_NumPad0",
            "Key_NumPad1",
            "Key_NumPad2",
            "Key_NumPad3",
            "Key_NumPad4",
            "Key_NumPad5",
            "Key_NumPad6",
            "Key_NumPad7",
            "Key_NumPad8",
            "Key_NumPad9",
            "Key_NumPadEnter",
            "Key_NumPoint",
            "Key_NumSubtract",
            "Key_O",
            "Key_P",
            "Key_PageDown",
            "Key_PageUp",
            "Key_Pause",
            "Key_Period",
            "Key_PrintScreen",
            "Key_Q",
            "Key_R",
            "Key_RightAlt",
            "Key_RightArrow",
            "Key_RightBracket",
            "Key_RightCtrl",
            "Key_RightShift",
            "Key_RightWin",
            "Key_S",
            "Key_ScrollLock",
            "Key_Semicolon",
            "Key_Slash",
            "Key_Spacebar",
            "Key_T",
            "Key_Tab",
            "Key_U",
            "Key_UpArrow",
            "Key_V",
            "Key_W",
            "Key_X",
            "Key_Y",
            "Key_Z"
        };
        const UInt32 Key::mValues[] =
        {
            Gamedesk::Keyboard::Key_0,
            Gamedesk::Keyboard::Key_1,
            Gamedesk::Keyboard::Key_2,
            Gamedesk::Keyboard::Key_3,
            Gamedesk::Keyboard::Key_4,
            Gamedesk::Keyboard::Key_5,
            Gamedesk::Keyboard::Key_6,
            Gamedesk::Keyboard::Key_7,
            Gamedesk::Keyboard::Key_8,
            Gamedesk::Keyboard::Key_9,
            Gamedesk::Keyboard::Key_A,
            Gamedesk::Keyboard::Key_Apostrophe,
            Gamedesk::Keyboard::Key_Apps,
            Gamedesk::Keyboard::Key_B,
            Gamedesk::Keyboard::Key_Backslash,
            Gamedesk::Keyboard::Key_Backspace,
            Gamedesk::Keyboard::Key_C,
            Gamedesk::Keyboard::Key_CapsLock,
            Gamedesk::Keyboard::Key_Comma,
            Gamedesk::Keyboard::Key_D,
            Gamedesk::Keyboard::Key_Delete,
            Gamedesk::Keyboard::Key_DownArrow,
            Gamedesk::Keyboard::Key_E,
            Gamedesk::Keyboard::Key_End,
            Gamedesk::Keyboard::Key_Enter,
            Gamedesk::Keyboard::Key_Equals,
            Gamedesk::Keyboard::Key_Escape,
            Gamedesk::Keyboard::Key_F,
            Gamedesk::Keyboard::Key_F1,
            Gamedesk::Keyboard::Key_F10,
            Gamedesk::Keyboard::Key_F11,
            Gamedesk::Keyboard::Key_F12,
            Gamedesk::Keyboard::Key_F13,
            Gamedesk::Keyboard::Key_F14,
            Gamedesk::Keyboard::Key_F15,
            Gamedesk::Keyboard::Key_F2,
            Gamedesk::Keyboard::Key_F3,
            Gamedesk::Keyboard::Key_F4,
            Gamedesk::Keyboard::Key_F5,
            Gamedesk::Keyboard::Key_F6,
            Gamedesk::Keyboard::Key_F7,
            Gamedesk::Keyboard::Key_F8,
            Gamedesk::Keyboard::Key_F9,
            Gamedesk::Keyboard::Key_G,
            Gamedesk::Keyboard::Key_Grave,
            Gamedesk::Keyboard::Key_H,
            Gamedesk::Keyboard::Key_Home,
            Gamedesk::Keyboard::Key_I,
            Gamedesk::Keyboard::Key_Insert,
            Gamedesk::Keyboard::Key_J,
            Gamedesk::Keyboard::Key_K,
            Gamedesk::Keyboard::Key_L,
            Gamedesk::Keyboard::Key_LeftAlt,
            Gamedesk::Keyboard::Key_LeftArrow,
            Gamedesk::Keyboard::Key_LeftBracket,
            Gamedesk::Keyboard::Key_LeftCtrl,
            Gamedesk::Keyboard::Key_LeftShift,
            Gamedesk::Keyboard::Key_LeftWin,
            Gamedesk::Keyboard::Key_M,
            Gamedesk::Keyboard::Key_MaxValue,
            Gamedesk::Keyboard::Key_Minus,
            Gamedesk::Keyboard::Key_N,
            Gamedesk::Keyboard::Key_None,
            Gamedesk::Keyboard::Key_NumAdd,
            Gamedesk::Keyboard::Key_NumDivide,
            Gamedesk::Keyboard::Key_NumKeys,
            Gamedesk::Keyboard::Key_NumLock,
            Gamedesk::Keyboard::Key_NumMultiply,
            Gamedesk::Keyboard::Key_NumPad0,
            Gamedesk::Keyboard::Key_NumPad1,
            Gamedesk::Keyboard::Key_NumPad2,
            Gamedesk::Keyboard::Key_NumPad3,
            Gamedesk::Keyboard::Key_NumPad4,
            Gamedesk::Keyboard::Key_NumPad5,
            Gamedesk::Keyboard::Key_NumPad6,
            Gamedesk::Keyboard::Key_NumPad7,
            Gamedesk::Keyboard::Key_NumPad8,
            Gamedesk::Keyboard::Key_NumPad9,
            Gamedesk::Keyboard::Key_NumPadEnter,
            Gamedesk::Keyboard::Key_NumPoint,
            Gamedesk::Keyboard::Key_NumSubtract,
            Gamedesk::Keyboard::Key_O,
            Gamedesk::Keyboard::Key_P,
            Gamedesk::Keyboard::Key_PageDown,
            Gamedesk::Keyboard::Key_PageUp,
            Gamedesk::Keyboard::Key_Pause,
            Gamedesk::Keyboard::Key_Period,
            Gamedesk::Keyboard::Key_PrintScreen,
            Gamedesk::Keyboard::Key_Q,
            Gamedesk::Keyboard::Key_R,
            Gamedesk::Keyboard::Key_RightAlt,
            Gamedesk::Keyboard::Key_RightArrow,
            Gamedesk::Keyboard::Key_RightBracket,
            Gamedesk::Keyboard::Key_RightCtrl,
            Gamedesk::Keyboard::Key_RightShift,
            Gamedesk::Keyboard::Key_RightWin,
            Gamedesk::Keyboard::Key_S,
            Gamedesk::Keyboard::Key_ScrollLock,
            Gamedesk::Keyboard::Key_Semicolon,
            Gamedesk::Keyboard::Key_Slash,
            Gamedesk::Keyboard::Key_Spacebar,
            Gamedesk::Keyboard::Key_T,
            Gamedesk::Keyboard::Key_Tab,
            Gamedesk::Keyboard::Key_U,
            Gamedesk::Keyboard::Key_UpArrow,
            Gamedesk::Keyboard::Key_V,
            Gamedesk::Keyboard::Key_W,
            Gamedesk::Keyboard::Key_X,
            Gamedesk::Keyboard::Key_Y,
            Gamedesk::Keyboard::Key_Z
        };
        const UInt32 Key::mSize = 110;
    }

    namespace Keyboard
    {
        const char* KeyState::mStrings[] =
        {
            "KeyState_MaxValue",
            "Key_Down",
            "Key_NumStates",
            "Key_Pressed",
            "Key_Released",
            "Key_Up"
        };
        const UInt32 KeyState::mValues[] =
        {
            Gamedesk::Keyboard::KeyState_MaxValue,
            Gamedesk::Keyboard::Key_Down,
            Gamedesk::Keyboard::Key_NumStates,
            Gamedesk::Keyboard::Key_Pressed,
            Gamedesk::Keyboard::Key_Released,
            Gamedesk::Keyboard::Key_Up
        };
        const UInt32 KeyState::mSize = 6;
    }

    namespace Mouse
    {
        const char* Button::mStrings[] =
        {
            "Button_0",
            "Button_1",
            "Button_2",
            "Button_3",
            "Button_4",
            "Button_5",
            "Button_6",
            "Button_7",
            "Button_Left",
            "Button_MaxValue",
            "Button_Middle",
            "Button_None",
            "Button_NumButtons",
            "Button_Right"
        };
        const UInt32 Button::mValues[] =
        {
            Gamedesk::Mouse::Button_0,
            Gamedesk::Mouse::Button_1,
            Gamedesk::Mouse::Button_2,
            Gamedesk::Mouse::Button_3,
            Gamedesk::Mouse::Button_4,
            Gamedesk::Mouse::Button_5,
            Gamedesk::Mouse::Button_6,
            Gamedesk::Mouse::Button_7,
            Gamedesk::Mouse::Button_Left,
            Gamedesk::Mouse::Button_MaxValue,
            Gamedesk::Mouse::Button_Middle,
            Gamedesk::Mouse::Button_None,
            Gamedesk::Mouse::Button_NumButtons,
            Gamedesk::Mouse::Button_Right
        };
        const UInt32 Button::mSize = 14;
    }

    namespace Mouse
    {
        const char* Axis::mStrings[] =
        {
            "Axis_MaxValue",
            "Axis_NumAxis",
            "Axis_X",
            "Axis_Y",
            "Axis_Z"
        };
        const UInt32 Axis::mValues[] =
        {
            Gamedesk::Mouse::Axis_MaxValue,
            Gamedesk::Mouse::Axis_NumAxis,
            Gamedesk::Mouse::Axis_X,
            Gamedesk::Mouse::Axis_Y,
            Gamedesk::Mouse::Axis_Z
        };
        const UInt32 Axis::mSize = 5;
    }

    namespace Mouse
    {
        const char* ButtonState::mStrings[] =
        {
            "ButtonState_MaxValue",
            "Button_Down",
            "Button_NumStates",
            "Button_Pressed",
            "Button_Released",
            "Button_Up"
        };
        const UInt32 ButtonState::mValues[] =
        {
            Gamedesk::Mouse::ButtonState_MaxValue,
            Gamedesk::Mouse::Button_Down,
            Gamedesk::Mouse::Button_NumStates,
            Gamedesk::Mouse::Button_Pressed,
            Gamedesk::Mouse::Button_Released,
            Gamedesk::Mouse::Button_Up
        };
        const UInt32 ButtonState::mSize = 6;
    }

    namespace TestProperties
    {
        const char* MyEnum::mStrings[] =
        {
            "First",
            "MAX",
            "Second",
            "Third"
        };
        const UInt32 MyEnum::mValues[] =
        {
            Gamedesk::TestProperties::First,
            Gamedesk::TestProperties::MAX,
            Gamedesk::TestProperties::Second,
            Gamedesk::TestProperties::Third
        };
        const UInt32 MyEnum::mSize = 4;
    }

}


} // namespace Gamedesk
	

#endif
