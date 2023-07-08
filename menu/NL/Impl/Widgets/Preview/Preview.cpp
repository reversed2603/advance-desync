#include "..\..\..\Neverlose.h"

C_Element& C_Element::Preview( )
{
    m_eType = ET_PREVIEW;

    m_cMapVars[_h( "CreateCallback" )] = C_Var( _h( "PreviewCreateCallback" ) );

    return *this;
}

void C_Nl::PreviewCreateCallback( C_Element* pElement )
{
    g_NL->Preview( g_NL->m_vecPreviw, ImVec2( 10, 49 ), ImVec2( 360, 100 ) );
}

void C_Nl::Preview( std::vector<std::string>& vecStrings, ImVec2 sPos, ImVec2 sSize, bool bOnlyInit )
{
    if ( bOnlyInit )
    {
        InitializePreview( vecStrings, sPos, sSize, true );
        return;
    }

    m_sPos = sPos;
    m_sSize = sSize;

    InitializePreview( vecStrings, sPos, sSize );

    ImGui::SetCursorPos( sPos );
    //ImGui::BeginChild( "###PreviewChild", sSize );
    //ImGui::EndChild( );

    auto sMousePos = ImGui::GetIO( ).MousePos;

    sMousePos.x -= ImGui::GetWindowPos( ).x;
    sMousePos.y -= ImGui::GetWindowPos( ).y;

    static bool bLastState = false;
    bool bCurrentState = ImGui::GetIO( ).MouseDown[0];

    static bool bFix = false;
    static bool bMovingElement = false;
    static int iMovingElementIndex = 0;
    static int iRenderIndex = 0;
    static ImVec2 sLastMovElemOrigin = ImVec2( 0, 0 );
    static int iOffsetMovX = 0;
    static int iOffsetMovY = 0;
    static bool bSetLastPos = false;

    auto InVec = [ ]( std::vector < int >& cVec, int iValue ) -> int
    {
        for ( int i = 0; i < cVec.size( ); ++i )
            if ( iValue == cVec[i] )
                return i;

        return -1;
    };

    if ( bLastState != bCurrentState )
    {
        bLastState = bCurrentState;

        if ( bCurrentState )
        {
            for ( auto& iIndex : m_cVecPreviewElements )
            {
                auto& sElemOrigin = m_cVecElemets[iIndex].GetVar( _h( "Origin" ) ).GetVec2( );
                auto sTextSize = ImGui::CalcTextSize( m_cVecElemets[iIndex].GetVar( _h( "Text" ) ).GetString( ).c_str( ) );

                if ( DynamicTextMouseIntersectCheck( &m_cVecElemets[iIndex] ) )
                {
                    bMovingElement = true;
                    iMovingElementIndex = iIndex;
                    sLastMovElemOrigin = sElemOrigin;

                    iRenderIndex = GetIndexByPos( sMousePos );

                    iOffsetMovX = sElemOrigin.x - sMousePos.x;
                    iOffsetMovY = sElemOrigin.y - sMousePos.y;

                    bSetLastPos = true;
                }
            }
        }
    }

    static int iLastCurr = 0;
    static int iLastSize = 0;

    if ( bCurrentState && bMovingElement )
    {
        int iCurrIndex = GetIndexByPos( sMousePos );

        if ( iLastCurr == -1 && iCurrIndex != -1 && iCurrIndex == iRenderIndex && iLastSize == m_cVecPreviewElementsToRender.size( ) )
        {
            iRenderIndex = -1;
            bFix = true;
        }

        if ( iCurrIndex != -1 && iCurrIndex != iRenderIndex )
        {
            std::vector < ImVec2 > sVecOrigins = { };

            if ( InVec( m_cVecPreviewElementsToRender, iMovingElementIndex ) != -1 )
            {
                if ( iRenderIndex == -1 )
                    goto LABEL_1;

                std::swap( m_cVecPreviewElementsToRender[iCurrIndex], m_cVecPreviewElementsToRender[iRenderIndex] );
            }
            else
            {
                m_cVecPreviewElementsToRender.insert( m_cVecPreviewElementsToRender.begin( ) + iCurrIndex, iMovingElementIndex );
            }

            BuildPreviewOrigins( sVecOrigins, m_cVecPreviewElementsToRender, sSize );
            ApplyOrigins( sVecOrigins, sPos, true, iCurrIndex );

            sLastMovElemOrigin = sVecOrigins[iCurrIndex];

            sLastMovElemOrigin.x += sPos.x;
            sLastMovElemOrigin.y += sPos.y;

            iRenderIndex = iCurrIndex;
        }

    LABEL_1:

        iLastSize = m_cVecPreviewElementsToRender.size( );
        iLastCurr = iCurrIndex;

        auto& sElemOrigin = m_cVecElemets[iMovingElementIndex].GetVar( _h( "Origin" ) ).GetVec2( );

        sElemOrigin.x = sMousePos.x + iOffsetMovX;
        sElemOrigin.y = sMousePos.y + iOffsetMovY;

        if ( sMousePos.y < sPos.y - 50.f )
        {
            auto iIndex = InVec( m_cVecPreviewElementsToRender, iMovingElementIndex );

            if ( iIndex != -1 )
            {
                m_cVecPreviewElementsToRender.erase( m_cVecPreviewElementsToRender.begin( ) + iIndex );
                m_bUpdatedOrigins = false;
                bSetLastPos = false;
            }
        }

    }
    else if ( bMovingElement )
    {
        if ( bFix )
        {
            bFix = false;
            bSetLastPos = false;

            m_bUpdatedOrigins = false;
        }
        else if ( bSetLastPos )
        {
            ADD_ANIMATION( C_Animation( ).OriginMove( &m_cVecElemets[iMovingElementIndex],
                AT_HIGH_TO_LOW, 0.45f, sLastMovElemOrigin ) );

            bSetLastPos = false;
        }

        bMovingElement = false;
    }

    if ( ImGui::GetIO( ).MouseClicked[1] )
    {
        m_bUpdatedOrigins = false;
    }

    if ( !m_bUpdatedOrigins )
    {
        std::vector < ImVec2 > sVecOrigins = { };

        BuildPreviewOrigins( sVecOrigins, m_cVecPreviewElementsToRender, sSize );
        ApplyOrigins( sVecOrigins, sPos, true, -1 );

        m_bUpdatedOrigins = true;
    }
}

void C_Nl::InitializePreview( std::vector<std::string>& vecStrings, ImVec2 sPos, ImVec2 sSize, bool bOnlyInit )
{
    if ( bOnlyInit )
    {
        for ( auto& cString : vecStrings )
        {
            m_cVecPreviewElements.push_back( m_cVecElemets.size( ) );
            ADD_ELEMENT( C_Element( ).DrawDropText( cString.c_str( ),
                ImVec2( 0, 0 ),
                RGB_VEC( 153.f, 176.f, 189.f, 255.f ),
                pTabGroupsNameFont
            ) );
        }

        return;
    }

    CALL_ONCE_START;
    {
        m_cVecPreviewElementsToRender = m_cVecPreviewElements;

        std::vector < ImVec2 > sVecOrigins = { };

        BuildPreviewOrigins( sVecOrigins, m_cVecPreviewElementsToRender, sSize );
        ApplyOrigins( sVecOrigins, sPos, false, -1 );

        m_bUpdatedOrigins = true;
    }
    CALL_ONCE_END;
}

void C_Nl::BuildPreviewOrigins( std::vector<ImVec2>& cVecOut, std::vector<int>& vecElements, ImVec2 sSize )
{
    cVecOut.clear( );

    for ( int i = 0; i < vecElements.size( ); ++i )
    {
        int iCurIndex = vecElements[i];

        bool bFirst = cVecOut.empty( );
        cVecOut.push_back( ImVec2( 10, 5 ) );

        auto& cElem = m_cVecElemets[iCurIndex];

        if ( !bFirst )
        {
            int iPrevIndex = vecElements[i - 1];
            auto sPreTextSize = ImGui::CalcTextSize( m_cVecElemets[iPrevIndex].GetVar( _h( "Text" ) ).GetString( ).c_str( ) );
            auto sCurTextSize = ImGui::CalcTextSize( m_cVecElemets[iCurIndex].GetVar( _h( "Text" ) ).GetString( ).c_str( ) );

            ImVec2 sLastPos = cVecOut[cVecOut.size( ) - 2];

            sLastPos.x += sPreTextSize.x + 12.f;

            if ( ( sLastPos.x + sCurTextSize.x ) >= sSize.x - 12.f )
            {
                sLastPos.x = 10;
                sLastPos.y += 19.f;
            }

            cVecOut.back( ) = sLastPos;
        }
    }
}

void C_Nl::ApplyOrigins( std::vector<ImVec2>& cVecOrigins, ImVec2 sPos, bool bAnimation, int iIngnoreIndex )
{
    for ( int i = 0; i < cVecOrigins.size( ); ++i )
    {
        if ( i == iIngnoreIndex )
            continue;

        auto& sElem = m_cVecElemets[m_cVecPreviewElementsToRender[i]];
        auto& sCurrOrigin = sElem.GetVar( _h( "Origin" ) ).GetVec2( );

        for ( int i = 0; i < m_cVecAnimations.size( ); ++i )
            if ( m_cVecAnimations[i].GetElement( ) == &sElem )
                m_cVecAnimations.erase( m_cVecAnimations.begin( ) + i );

        if ( bAnimation )
        {
            ADD_ANIMATION( C_Animation( ).OriginMove( &sElem, AT_HIGH_TO_LOW, 0.45f, ImVec2( sPos.x + cVecOrigins[i].x, sPos.y + cVecOrigins[i].y ) ) );
        }
        else
        {
            sCurrOrigin.x = sPos.x + cVecOrigins[i].x;
            sCurrOrigin.y = sPos.y + cVecOrigins[i].y;
        }
    }
}

int C_Nl::GetIndexByPos( ImVec2 sMousePos )
{
    sMousePos.x -= m_sPos.x;
    sMousePos.y -= m_sPos.y;

    if ( sMousePos.x < 0 || sMousePos.y < 0 ||
        sMousePos.x > m_sSize.x || sMousePos.y > m_sSize.y )
        return -1;

    if ( m_cVecPreviewElementsToRender.empty( ) )
        return 0;

    std::vector < ImVec2 > sVecOrigins = { };
    BuildPreviewOrigins( sVecOrigins, m_cVecPreviewElementsToRender, m_sSize );

    bool bUpper = true;

    for ( int i = 0; i < sVecOrigins.size( ); ++i )
    {
        auto sStart = sVecOrigins[i];
        auto sEnd = ImGui::CalcTextSize( m_cVecElemets[m_cVecPreviewElementsToRender[i]].GetVar( _h( "Text" ) ).GetString( ).c_str( ) );

        sEnd.x += sStart.x;
        sEnd.y += sStart.y;

        sStart.x -= 12.f;
        sStart.y -= 5;

        if ( bUpper )
            sStart.y -= 100.f;

        if ( sMousePos.x >= sStart.x && sMousePos.x <= sEnd.x &&
            sMousePos.y >= sStart.y && sMousePos.y <= sEnd.y )
            return i;

        if ( i && sVecOrigins[i - 1].y != sVecOrigins[i].y )
        {
            bUpper = false;

            sStart.x = 0;
            sStart.y = sVecOrigins[i - 1].y;

            sEnd.x = m_sSize.x;
            sEnd.y = sStart.y + 12.f;

            sStart.y -= 5;

            if ( sMousePos.x >= sStart.x && sMousePos.x <= sEnd.x &&
                sMousePos.y >= sStart.y && sMousePos.y <= sEnd.y )
                return i;
        }
    }

    return sVecOrigins.size( ) - 1;
}
