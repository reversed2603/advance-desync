#pragma once
/*
#include "..\..\dependencies\imgui\imgui.h"
#include "..\..\dependencies\imgui\imgui_impl_dx9.h"
#include "..\..\dependencies\imgui\imgui_impl_win32.h"
#include "PostProcessing/PostProcessing.h"
#include <map>
#include <d3d9.h>
#include <vector>
#include <string>
#include <tchar.h>
#include <optional>
#include <iostream>
#include <algorithm>
#include <functional>

#include <d3dx9core.h>
#pragma comment(lib, "d3dx9.lib")
*/
#include "../../nem.hpp"

#include "Deps/Hash.h"

#define CALL_ONCE_START  static bool bInitialized = false; if ( bInitialized ) return;
#define CALL_ONCE_END    bInitialized = true;

#define ADD_ELEMENT(elem) g_NL->m_cVecElemets.push_back( elem )
#define ADD_ELEMENT_ADDITIONAL(elem) g_NL->m_cAdditionalElemets.push_back( elem )
#define ADD_ANIMATION(elem) g_NL->m_cVecAnimations.push_back( elem )

#define RGB_VEC(r, g, b, a) ImVec4( r / 255.f, g / 255.f, b / 255.f, a / 255.f )

#define CHILD_ELEMENT_POS(index) ImVec2( 13.f, 69.f + ( index * 41.f ) )

#include "Classes/C_BindComboItemProperty.h"
#include "Classes/C_Var.h"
#include "Classes/C_Element.h"
#include "Classes/C_Animation.h"

enum E_Icons
{
    IC_AIMBOT,
    IC_ANTIAIM,
    IC_LEGITBOT,
    IC_PLAYERS,
    IC_WEAPONS,
    IC_GRENADES,
    IC_BOMB,
    IC_WORLD,
    IC_VIEW,
    IC_MAIN,
    IC_INVENTORY,
    IC_SCRIPTS,
    IC_CONFIGS,
    IC_COLOR_PICKER,
    IC_MAX,
};


class C_Nl
{
public:
    int m_iIndexMinDamage = -1;
    int m_iIndexMinDamageOv = -1;
    int m_iIndexHitchance = -1;
    int m_iIndexStaticPoints = -1;
    int m_iIndexHeadScale = -1;
    int m_iIndexBodyScale = -1;
    int m_iIndexHitboxes = -1;
    int m_iIndexMultipoints = -1;
    int m_iIndexAutoStopType = -1;
    int m_iIndexAutoStopPred = -1;

    int m_iConfigsElementIndex = 212;
    char m_chConfigName[256]{ 0x00 };

public:
    void Draw( LPDIRECT3DDEVICE9 pDevice, bool bState );
    void InitializeFonts( );
    void AfterMenu( );
    void ChangeWeaponGroupRefs( int iWeaponGroup );
    int m_iSelectedWeaponGroup = 0;

public:
    void InitializeTextures( );
    void InitializeCreateCallbacks( );
    void InitializeAnimationCallbacks( );
    void InitializeStyle( );
    void InitializeElements( );
    void ProcessAnimations( );
    void ProcessElement( C_Element* pElement );

    LPDIRECT3DDEVICE9 m_pDevice = nullptr;

public:
    bool CheckMouseIntersectionLocal( ImVec2 sStart, ImVec2 sSize );
    void ClearElementAnimations( C_Element* pElement, uint32_t u32CallbackType );
    bool IsAnimationPresent( C_Element* pElement, uint32_t u32CallbackType );
    void ResetAnimations( );
    void MoveWindows( );

public: // Text
    static void TextCreateCallback( C_Element* pElement );

public: // Window
    static void WindowBeginCreateCallback( C_Element* pElement );
    static void WindowEndCreateCallback( C_Element* pElement );

public: // RectFilled
    static void RectFilledCreateCallback( C_Element* pElement );

public: // Image
    static void ImageCreateCallback( C_Element* pElement );

public: // Tab
    static void TabCreateCallback( C_Element* pElement );
    static void OnTabChangeCallback( C_Element* pElement, int iPrevTab, int iCurrentTab );

public: // Child
    static void ChildBeginCreateCallback( C_Element* pElement );
    static void ChildEndCreateCallback( C_Element* pElement );

public: // Dynamic text
    static void DrawDropTextCreateCallback( C_Element* pElement );

public: // Checkbox
    static void CheckboxCreateCallback( C_Element* pElement );
    static void OnMouseCallbackTextColorChangeCheckbox( C_Element* pElement, bool bMouseIntersect );
    static void OnMouseClickIntersectCheckboxCallback( C_Element* pElement );
    static void OnMouseCheckboxCallbackBoxColorChange( C_Element* pElement, bool bMouseIntersect );

public: // Slider
    static void SliderCreateCallback( C_Element* pElement );
    static void OnMouseCallbackTextColorChangeSlider( C_Element* pElement, bool bMouseIntersect );

public: // Combo
    static void ComboCreateCallback( C_Element* pElement );
    static void ComboMouseIntersectCallback( C_Element* pElement, bool bIntersected );
    static void ComboFullMouseIntersectCallback( C_Element* pElement, bool bIntersected );
    static void ComboClickCallback( C_Element* pElement, bool bIntersected );

public: // Bind combo
    static void BindComboCreateCallback( C_Element* pElement );
    static void BindComboMouseIntersectCallback( C_Element* pElement, bool bIntersected );
    static void BindComboFullMouseIntersectCallback( C_Element* pElement, bool bIntersected );
    static void BindComboClickCallback( C_Element* pElement, bool bIntersected );

public: // Multicombo
    static void MultiComboCreateCallback( C_Element* pElement );
    static void MultiComboMouseIntersectCallback( C_Element* pElement, bool bIntersected );
    static void MultiComboFullMouseIntersectCallback( C_Element* pElement, bool bIntersected );
    static void MultiComboClickCallback( C_Element* pElement, bool bIntersected );

public: // Keybind value
    static void KeybindValueCreateCallback( C_Element* pElement );
    static void KeybindValueMouseIntersectCallback( C_Element* pElement, bool bIntersected );
    static void KeybindValueFullMouseIntersectCallback( C_Element* pElement, bool bIntersected );
    static void KeybindValueClickCallback( C_Element* pElement, bool bIntersected );

public: // Button
    static void ButtonCreateCallback( C_Element* pElement );
    static void ButtonMouseIntersectCallback( C_Element* pElement, bool bIntersected );
    static void ButtonFullMouseIntersectCallback( C_Element* pElement, bool bIntersected );
    static void ButtonClickCallback( C_Element* pElement, bool bIntersected );

public: // Color Picker
    static void ColorPickerCreateCallback( C_Element* pElement );

public: // Text Input
    static void TextInputCreateCallback( C_Element* pElement );

public: // Skins block
    static void SkinsBlockCreateCallback( C_Element* pElement );

public:
    static void LuaBlockCreateCallback( C_Element* pElement );

public: // ESP Preview
    static void PreviewCreateCallback( C_Element* pElement );
    void Preview( std::vector < std::string >& vecStrings, ImVec2 sPos, ImVec2 sSize, bool bOnlyInit = false );
    void InitializePreview( std::vector < std::string >& vecStrings, ImVec2 sPos, ImVec2 sSize, bool bOnlyInit = false );
    void BuildPreviewOrigins( std::vector < ImVec2 >& cVecOut, std::vector < int >& vecElements, ImVec2 sSize );
    void ApplyOrigins( std::vector < ImVec2 >& cVecOrigins, ImVec2 sPos, bool bAnimation, int iIngnoreIndex );
    int GetIndexByPos( ImVec2 sMousePos );
    static int DynamicTextMouseIntersectCheck( C_Element* pElement );
    
    ImVec2 m_sSize = ImVec2( 0, 0 );
    ImVec2 m_sPos = ImVec2( 0, 0 );

    std::vector < int > m_cVecPreviewElements = { };
    std::vector < int > m_cVecPreviewElementsToRender = { };
    bool m_bUpdatedOrigins = false;

public:
    std::vector < std::string > m_vecPreviw = { "Armor Bar", "Health Bar", "Name", "Money", "Health", "Taser", "Box", "Hit", "Has Bomb", "Pin", "Blind" };

public:
    IDirect3DTexture9* m_pIcons[IC_MAX];

public:
    ImFont* pTabGroupsNameFont = nullptr;
    ImFont* pTabNameFont = nullptr;
    ImFont* pMenuFont = nullptr;
    ImFont* pLogoFont = nullptr;
    ImFont* pInputFont = nullptr;

public: // Other
    std::map < uint32_t, std::function < void( C_Element* ) > > m_cMapCreateCallbacks = { };
    std::map < uint32_t, std::function < void( C_Animation*, float ) > > m_cMapAnimationCallbacks = { };

    std::vector < C_Element > m_cVecElemets = { };
    std::vector < C_Element > m_cAdditionalElemets = { };
    std::vector < C_Animation > m_cVecAnimations = { };

    std::vector < C_Element* > m_pTabs = { };

    int m_iSelectedTab = 0;
    bool m_bTabFullySwitched = true;
    bool m_bSkipElements = false;

    int m_iMenuPosX = 295;
    int m_iMenuPosY = 84;
    int m_bSelectingShit = false;

    int m_iCurrentTabRendering = -1;
    int m_iCurrentTabSetup = -1;

};

inline C_Nl* g_NL = new C_Nl;

enum E_KeybindType : int
{
    E_TOGGLE,
    E_HOLD,
};

class C_KeybindKeyInfo
{
public:
    E_KeybindType m_eKeyType = E_TOGGLE;
    int m_iKeyValue = 0;
    bool m_bShowInKeyBinds = false;

    // // //

    bool m_bNewValueCheckbox = false;
    int m_iNewValueSlider = 0;
    int m_iNewValueCombo = 0;
    int m_iNewValueMultiCombo = 0;

    // // //

    C_KeybindKeyInfo( )
    {
        m_eKeyType = E_TOGGLE;
        m_iKeyValue = 0;
        m_bShowInKeyBinds = false;
        m_bNewValueCheckbox = false;
        m_iNewValueSlider = 0;
        m_iNewValueCombo = 0;
        m_iNewValueMultiCombo = 0;
    }
};

class C_KeybindInfo
{
public:
    E_ElementType m_eKeybindElementType = ET_NONE;
    std::vector < C_KeybindKeyInfo > m_cVecKeys = { C_KeybindKeyInfo( ) };

    bool m_bOverriding = false;

    // // //
    bool m_bBackupCheckbox = false;
    int m_iBackupSlider = 0;
    int m_iBackupCombo = 0;
    int m_iBackupMultiCombo = 0;

    C_KeybindInfo( E_ElementType eType )
    {
        m_eKeybindElementType = eType;
        m_cVecKeys = { C_KeybindKeyInfo( ) };
    }

    C_KeybindInfo( )
    {
        m_eKeybindElementType = ET_NONE;
        m_cVecKeys = { C_KeybindKeyInfo( ) };
    }
};

#include "..\..\..\dependencies\json.hpp"

class C_Keybinds
{
public:
    std::vector < std::string > m_cVecKeybindsToShow = { };
    std::vector < std::string > m_cVecKeybindsStates = { };

public:
    void GetAllBinds( nlohmann::json& jsonIn );
    void LoadAllBinds( nlohmann::json& jsonIn );

public:
    void ApplyDefValues( );
    void SetAllKeybinds( std::vector<C_Element>& cVecElements, bool bReset );
    void InitializeKeybindVars( );
    void InitializeElements( );
    void DrawKeybindWindow( );
    void EnableKeybindSelection( C_Element* pElement, E_ElementType eElementType, uint32_t u32Hash );
    void CheckKeybind( );

public:
    bool m_bWindowState = false;
    float m_flWindowAlpha = 0.f;
    int m_iKeybindWindowPosX = 0;
    int m_iKeybindWindowPosY = 0;

public:
    C_Element m_cBindCombo;
    C_Element m_cBindValue;
    C_Element m_cBindMode;
    C_Element m_cBindShowInBinds;

    // // // 
    C_Element m_cBindNewValueCheckbox;
    C_Element m_cBindNewValueSlider;
    C_Element m_cBindNewValueCombo;
    C_Element m_cBindNewValueMultiCombo;

public:
    std::map < uint32_t, C_KeybindInfo > m_cMapKeybinds = { };
    std::vector < std::string > m_cVecTempStrings = { "Empty bind", "New bind" };
    int m_iChoosenString = 0;
    int m_iLastChoosenString = 0;
    int m_iLastSize = 0;
    int m_iKeyValue = 0;
    int m_iKeybindMode = 0;
    bool m_bShowInBinds = false;

    // // //
    bool m_bNewValueCheckbox = false;
    int m_iNewValueSlider = 0;
    int m_iNewValueCombo = 0;
    int m_iNewValueMultiCombo = 0;

    // // //

    int m_iLastPressedKey = 0;

    C_KeybindInfo* m_pCurrentKeybind = nullptr;

public:
    C_Element m_StartWindow = { };
};

inline C_Keybinds* g_Keybinds = new C_Keybinds;

class C_ColorPicker
{
public:
    void DrawColorpickerWindow( );
    void EnableColorpicker( float* pValue );

    bool m_bWindowState = false;
    float m_flWindowAlpha = 0.f;
    int m_iKeybindWindowPosX = 0;
    int m_iKeybindWindowPosY = 0;

    float* m_pValue = nullptr;
};

inline C_ColorPicker* g_ColorPicker = new C_ColorPicker;
