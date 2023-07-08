#pragma once

struct C_BindComboItemProperty
{
public:
    bool m_bEraseOnZero = false;
    float m_flTextAlpha = 1.f;
    float m_flCircleScale = 0.f;
    float m_flColorScale = 0.f;
    int m_iOffsetY = 0;
    int m_iSizeY = 0;

    C_BindComboItemProperty( )
    {
        m_bEraseOnZero = false;
        m_flCircleScale = 0.f;
        m_flTextAlpha = 1.f;
        m_flColorScale = 0.f;
        m_iOffsetY = 0;
        m_iSizeY = 0;
    }
};
