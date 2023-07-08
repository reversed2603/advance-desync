#include "..\Neverlose.h"

void C_ColorPicker::EnableColorpicker( float* pValue )
{
    if ( m_bWindowState )
        return;

    m_pValue = pValue;

    m_iKeybindWindowPosX = ImGui::GetMousePos( ).x;
    m_iKeybindWindowPosY = ImGui::GetMousePos( ).y;

    m_bWindowState = true;

    ADD_ANIMATION( C_Animation( ).ColorPickerWindowAlphaAnimate( AT_HIGH_TO_LOW, 0.1f, 1.f, m_flWindowAlpha ) );
}

void C_ColorPicker::DrawColorpickerWindow( )
{
    if ( !m_bWindowState )
        return;

    g_NL->m_bSelectingShit = false;

    auto flBackupAlpha = ImGui::GetStyle( ).Alpha;
    ImGui::GetStyle( ).Alpha = m_flWindowAlpha;

    ImGui::PushStyleColor( ImGuiCol_WindowBg, RGB_VEC( 13.f, 13.f, 13.f, 255.f * ImGui::GetStyle( ).Alpha ) );

    {
        ImGui::PushStyleColor( ImGuiCol_Border, RGB_VEC( 30.f, 30.f, 32.f, 70.f * ImGui::GetStyle( ).Alpha ) );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 4.f );

        ImGui::SetNextWindowSize( ImVec2( 287.f, 241.f ) );
        ImGui::SetNextWindowPos( ImVec2( m_iKeybindWindowPosX - 3.f, m_iKeybindWindowPosY - 3.f ) );
        ImGui::SetNextWindowBgAlpha( 0.f );

        ImGui::Begin( "ColorPickerWindow_1", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
        ImGui::End( );

        ImGui::PopStyleVar( );
        ImGui::PopStyleColor( );
    }

    {
        ImGui::PushStyleColor( ImGuiCol_Border, RGB_VEC( 26.f, 26.f, 26.f, 255.f * ImGui::GetStyle( ).Alpha ) );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 2.f );

        ImGui::SetNextWindowSize( ImVec2( 283.f, 237.f ) );
        ImGui::SetNextWindowPos( ImVec2( m_iKeybindWindowPosX - 1.f, m_iKeybindWindowPosY - 1.f ) );
        ImGui::SetNextWindowBgAlpha( 0.f );

        ImGui::Begin( "ColorPickerWindow_2", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
        ImGui::End( );

        ImGui::PopStyleVar( );
        ImGui::PopStyleColor( );
    }

    ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.f );

    ImGui::SetNextWindowSize( ImVec2( 281.f, 235.f ) );
    ImGui::SetNextWindowPos( ImVec2( m_iKeybindWindowPosX, m_iKeybindWindowPosY ) );
    ImGui::Begin( "ColorPickerWindow_Main", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
    {
        if ( ImGui::GetIO( ).MouseClicked[0] && !g_NL->CheckMouseIntersectionLocal( ImVec2( 0.f, 0.f ), ImGui::GetWindowSize( ) ) )
            ADD_ANIMATION( C_Animation( ).ColorPickerWindowAlphaAnimate( AT_HIGH_TO_LOW, 0.1f, 0.f, m_flWindowAlpha, [ ]( C_Animation* ) -> void { g_ColorPicker->m_bWindowState = false; g_NL->m_bSelectingShit = false; } ) );

        ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 5.f );
        ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + 7.f );

        ImGui::PushStyleColor( ImGuiCol_FrameBg, RGB_VEC( 13.f, 13.f, 13.f, 255.f ) );
        ImGui::PushStyleColor( ImGuiCol_Border, RGB_VEC( 13.f, 13.f, 13.f, 200.f ) );
        ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.5f );
        ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.f );

        ImGui::PushItemWidth( 270.f );
        ImGui::ColorPicker4( "Color", m_pValue, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoTooltip );
        ImGui::PopItemWidth( );

        ImGui::PopStyleVar( 2 );
        ImGui::PopStyleColor( 2 );
    }
    ImGui::End( );

    ImGui::PopStyleColor( );
    ImGui::PopStyleVar( );

    ImGui::GetStyle( ).Alpha = flBackupAlpha;

    g_NL->m_bSelectingShit = true;
}
