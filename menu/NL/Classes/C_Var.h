#pragma once

enum E_VarType : int
{
    TP_NONE,
    TP_INT,
    TP_UINT32,
    TP_FLOAT,
    TP_DOUBLE,
    TP_STRING,
    TP_VEC2,
    TP_VEC4,
    TP_REF,
};

class C_Var
{
private:
    E_VarType m_eType = TP_NONE;
    std::optional < int > m_oIntValue = { };
    std::optional < uint32_t > m_oUInt32Value = { };
    std::optional < float > m_oFloatValue = { };
    std::optional < double > m_oDoubleValue = { };
    std::optional < std::string > m_oStringValue = { };
    std::optional < ImVec2 > m_oVec2Value = { };
    std::optional < ImVec4 > m_oVec4Value = { };
    std::optional < const void* > m_oRefValue = { };

public:
    C_Var( ) { m_eType = TP_NONE; }
    C_Var( int iValue ) { m_eType = TP_INT; m_oIntValue = iValue; }
    C_Var( uint32_t u32Value ) { m_eType = TP_UINT32; m_oUInt32Value = u32Value; }
    C_Var( float flValue ) { m_eType = TP_FLOAT; m_oFloatValue = flValue; }
    C_Var( double dbValue ) { m_eType = TP_DOUBLE; m_oDoubleValue = dbValue; }
    C_Var( std::string cStr ) { m_eType = TP_STRING; m_oStringValue = cStr; }
    C_Var( ImVec2 sVec2 ) { m_eType = TP_VEC2; m_oVec2Value = sVec2; }
    C_Var( ImVec4 sVec4 ) { m_eType = TP_VEC4; m_oVec4Value = sVec4; }
    C_Var( const void* pPtr ) { m_eType = TP_REF; m_oRefValue = pPtr; }

public:
    E_VarType GetType( ) { return m_eType; }

    int& GetInt( ) { return m_oIntValue.value( ); }
    uint32_t& GetUInt32( ) { return m_oUInt32Value.value( ); }
    float& GetFloat( ) { return m_oFloatValue.value( ); }
    std::string& GetString( ) { return m_oStringValue.value( ); }
    ImVec2& GetVec2( ) { return m_oVec2Value.value( ); }
    ImVec4& GetVec4( ) { return m_oVec4Value.value( ); }

    template <typename T>
    T* GetRef( ) { return ( T* ) m_oRefValue.value( ); }
    void SetRef( const void* pRef ) { m_oRefValue.value( ) = pRef; }
};
