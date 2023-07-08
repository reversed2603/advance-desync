#include "..\Neverlose.h"

#include "..\Icons\Aimbot.h"
#include "..\Icons\AntiAim.h"
#include "..\Icons\Legitbot.h"
#include "..\Icons\Players.h"
#include "..\Icons\Weapons.h"
#include "..\Icons\Grenades.h"
#include "..\Icons\Bomb.h"
#include "..\Icons\World.h"
#include "..\Icons\View.h"
#include "..\Icons\Main.h"
#include "..\Icons\Inventory.h"
#include "..\Icons\Scripts.h"
#include "..\Icons\Configs.h"
#include "..\Icons\ColorPicker.h"

#include "..\Fonts\MuseoSans700.h"
#include "..\Fonts\MuseoSans900.h"
#include "../../../dependencies/imgui/imgui_freetype.h"
void C_Nl::InitializeTextures( )
{
    CALL_ONCE_START;
    {
        D3DXCreateTextureFromFileInMemory( m_pDevice, chRawAimbotIcon, sizeof( chRawAimbotIcon ), &m_pIcons[IC_AIMBOT] );
        D3DXCreateTextureFromFileInMemory( m_pDevice, chAntiAimTab, sizeof( chAntiAimTab ), &m_pIcons[IC_ANTIAIM] );
        D3DXCreateTextureFromFileInMemory( m_pDevice, chLegitBot, sizeof( chLegitBot ), &m_pIcons[IC_LEGITBOT] );
        D3DXCreateTextureFromFileInMemory( m_pDevice, chPlayers, sizeof( chPlayers ), &m_pIcons[IC_PLAYERS] );
        D3DXCreateTextureFromFileInMemory( m_pDevice, chWeapons, sizeof( chWeapons ), &m_pIcons[IC_WEAPONS] );
        D3DXCreateTextureFromFileInMemory( m_pDevice, chGrenades, sizeof( chGrenades ), &m_pIcons[IC_GRENADES] );
        D3DXCreateTextureFromFileInMemory( m_pDevice, chBomb, sizeof( chBomb ), &m_pIcons[IC_BOMB] );
        D3DXCreateTextureFromFileInMemory( m_pDevice, chWorld, sizeof( chWorld ), &m_pIcons[IC_WORLD] );
        D3DXCreateTextureFromFileInMemory( m_pDevice, chView, sizeof( chView ), &m_pIcons[IC_VIEW] );
        D3DXCreateTextureFromFileInMemory( m_pDevice, chMain, sizeof( chMain ), &m_pIcons[IC_MAIN] );
        D3DXCreateTextureFromFileInMemory( m_pDevice, chInventory, sizeof( chInventory ), &m_pIcons[IC_INVENTORY] );
        D3DXCreateTextureFromFileInMemory( m_pDevice, chScripts, sizeof( chScripts ), &m_pIcons[IC_SCRIPTS] );
        D3DXCreateTextureFromFileInMemory( m_pDevice, chConfigs, sizeof( chConfigs ), &m_pIcons[IC_CONFIGS] );
        D3DXCreateTextureFromFileInMemory( m_pDevice, chColorPicker, sizeof( chColorPicker ), &m_pIcons[IC_COLOR_PICKER] );
    }
    CALL_ONCE_END;
}

void C_Nl::InitializeFonts( )
{
    CALL_ONCE_START;
    {
        ImFontConfig cfg;
        cfg.PixelSnapH = 0;
        cfg.OversampleH = 5;
        cfg.OversampleV = 5;
        cfg.RasterizerMultiply = 1.2f;
        cfg.RasterizerFlags = ImGuiFreeType::RasterizerFlags::MonoHinting;

        static const ImWchar ranges[] =
        {
            0x0020, 0x00FF, // Basic Latin + Latin Supplement
            0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
            0x2DE0, 0x2DFF, // Cyrillic Extended-A
            0xA640, 0xA69F, // Cyrillic Extended-B
            0xE000, 0xE226, // icons
            0,
        };

        cfg.GlyphRanges = ranges;

        ImGuiIO& sIo = ImGui::GetIO( );

        pTabGroupsNameFont = sIo.Fonts->AddFontFromMemoryTTF( chMuseoSans700, sizeof( chMuseoSans700 ), 20.f, &cfg );
        pTabNameFont = sIo.Fonts->AddFontFromMemoryTTF( chMuseoSans700, sizeof( chMuseoSans700 ), 20.f, &cfg );
        pMenuFont = sIo.Fonts->AddFontFromMemoryTTF( chMuseoSans700, sizeof( chMuseoSans700 ), 21.f, &cfg );
        pInputFont = sIo.Fonts->AddFontFromMemoryTTF( chMuseoSans700, sizeof( chMuseoSans700 ), 15.f, &cfg );
        pLogoFont = sIo.Fonts->AddFontFromMemoryTTF( chMuseoSans900, sizeof( chMuseoSans900 ), 40.f, &cfg );

        ImGuiFreeType::BuildFontAtlas( sIo.Fonts );

    }
    CALL_ONCE_END;
}

void C_Nl::InitializeStyle( )
{
    CALL_ONCE_START;
    {
        auto& sStyle = ImGui::GetStyle( );

        sStyle.WindowRounding = 4.f;
        sStyle.Colors[ImGuiCol_Text] = RGB_VEC( 242.f, 242.f, 242.f, 255.f );
    }
    CALL_ONCE_END;
}

void C_Nl::InitializeCreateCallbacks( )
{
    CALL_ONCE_START;
    {
        m_cMapCreateCallbacks[_h( "TextCreateCallback" )] = TextCreateCallback;
        m_cMapCreateCallbacks[_h( "WindowBeginCreateCallback" )] = WindowBeginCreateCallback;
        m_cMapCreateCallbacks[_h( "WindowEndCreateCallback" )] = WindowEndCreateCallback;
        m_cMapCreateCallbacks[_h( "RectFilledCreateCallback" )] = RectFilledCreateCallback;
        m_cMapCreateCallbacks[_h( "ImageCreateCallback" )] = ImageCreateCallback;
        m_cMapCreateCallbacks[_h( "TabCreateCallback" )] = TabCreateCallback;
        m_cMapCreateCallbacks[_h( "ChildBeginCreateCallback" )] = ChildBeginCreateCallback;
        m_cMapCreateCallbacks[_h( "ChildEndCreateCallback" )] = ChildEndCreateCallback;
        m_cMapCreateCallbacks[_h( "PreviewCreateCallback" )] = PreviewCreateCallback;
        m_cMapCreateCallbacks[_h( "DrawDropTextCreateCallback" )] = DrawDropTextCreateCallback;
        m_cMapCreateCallbacks[_h( "CheckboxCreateCallback" )] = CheckboxCreateCallback;
        m_cMapCreateCallbacks[_h( "AlphaControlCallback" )] = [ & ]( C_Element* pElement ) -> void { ImGui::GetStyle( ).Alpha = pElement->GetVar( _h( "Alpha" ) ).GetFloat( ); };
        m_cMapCreateCallbacks[_h( "SliderCreateCallback" )] = SliderCreateCallback;
        m_cMapCreateCallbacks[_h( "ComboCreateCallback" )] = ComboCreateCallback;
        m_cMapCreateCallbacks[_h( "BindComboCreateCallback" )] = BindComboCreateCallback;
        m_cMapCreateCallbacks[_h( "MultiComboCreateCallback" )] = MultiComboCreateCallback;
        m_cMapCreateCallbacks[_h( "KeybindValueCreateCallback" )] = KeybindValueCreateCallback;
        m_cMapCreateCallbacks[_h( "ColorPickerCreateCallback" )] = ColorPickerCreateCallback;
        m_cMapCreateCallbacks[_h( "ButtonCreateCallback" )] = ButtonCreateCallback;
        m_cMapCreateCallbacks[_h( "TextInputCreateCallback" )] = TextInputCreateCallback;
        m_cMapCreateCallbacks[_h( "SkinsBlockCreateCallback" )] = SkinsBlockCreateCallback;
        m_cMapCreateCallbacks[_h( "LuaBlockCreateCallback" )] = LuaBlockCreateCallback;
    }
    CALL_ONCE_END;
}
