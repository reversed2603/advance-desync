#pragma once

enum E_ElementType
{
    ET_ALPHA_CONTLOR,
    ET_NONE,
    ET_TEXT,
    ET_DRAG_DROP_TEXT_CONVERT,
    ET_WINDOW_BEGIN,
    ET_WINDOW_END,
    ET_RECT_FILLED,
    ET_IMAGE,
    ET_TAB,
    ET_CHILD_BEGIN,
    ET_CHILD_END,
    ET_CHECKBOX,
    ET_PREVIEW,
    ET_SLIDER,
    ET_COMBO,
    ET_BIND_COMBO,
    ET_MULTI_COMBO,
    ET_KEYBIND_VALUE,
    ET_COLOR_PICKER,
    ET_BUTTON,
    ET_TEXT_INPUT,
    ET_SKINS_COMBO,
    ET_LUA_BLOCK,
};

class C_Element
{
private:
    E_ElementType m_eType = ET_NONE;
    std::map < uint32_t, C_Var > m_cMapVars = { };

public:
    C_Element& Text( const char* szText, ImVec2 sPos, ImVec4 sColor, ImFont* pFont );
    C_Element& WindowBegin( const char* szText, ImVec2 sPos, ImVec2 sSize, ImVec4 sColor, ImGuiWindowFlags uFlags, bool bZeroBorder = false );
    C_Element& WindowEnd( );
    C_Element& RectFilled( ImVec2 sStart, ImVec2 sEnd, ImVec4 sColor );
    C_Element& Image( IDirect3DTexture9* pTexture, ImVec2 sPos, ImVec2 sSize );
    C_Element& Tab( ImVec2 sPos, IDirect3DTexture9* pIcon, ImVec2 sIconSize, const char* pText, int* pSelectedTab, int iCurrentTab, ImVec2 sTextOffset = ImVec2( 35.f, 1.f ) );
    C_Element& ChildBegin( const char* pText, ImVec2 sPos, ImVec2 sSize, bool bWindowed = false, int iTabIndex = -1 );
    C_Element& ChildEnd( bool bWindowed = false, int iTabIndex = -1 );
    C_Element& Preview( );
    C_Element& DrawDropText( const char* szText, ImVec2 sPos, ImVec4 sColor, ImFont* pFont);
    C_Element& Checkbox( uint32_t u32HashIndex, const char* szText, ImVec2 sPos, bool* pValue );
    C_Element& GlobalAlphaController( )
    {
        m_eType = ET_ALPHA_CONTLOR;

        m_cMapVars[_h( "Alpha" )] = C_Var( 0.f );

        m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "AlphaControlCallback" ) );

        return *this;
    }
    C_Element& Slider( uint32_t u32Hash, const char* szText, ImVec2 sPos, int iMin, int iMax, int* pValue );
    C_Element& Combo( uint32_t u32Hash, const char* szText, ImVec2 sPos, std::vector < std::string > cVecStrings, int* pValue );
    C_Element& BindCombo( const char* szText, ImVec2 sPos, std::vector < std::string >* cVecStrings, int* pValue );
    C_Element& MultiCombo( uint32_t u32Hash, const char* szText, ImVec2 sPos, std::vector < std::string > cVecStrings, int* pResult );
    C_Element& KeybindValue( const char* szText, ImVec2 sPos, int* pResult );
    C_Element& ColorPicker( float flOffsetX, ImVec2 sPos, float* pResult );
    C_Element& Button( const char* szText, ImVec2 sPos, ImVec2 sSize, const void* pCallback );
    C_Element& TextInput( const char* szText, ImVec2 sPos, ImVec2 sSize, char* pOut );
    C_Element& SkinsBlock( );
    C_Element& LuaBlock( );

public:
    void ResetCheckbox( );
    void ResetSlider( bool bZeroOffset = true);

public:
    C_Var& GetVar( uint32_t u32VarHash ) { return m_cMapVars[u32VarHash]; }
    std::map < uint32_t, C_Var >& GetVars( ) { return m_cMapVars; }
    E_ElementType GetType( ) { return m_eType; }
};
