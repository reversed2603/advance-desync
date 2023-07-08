#include "nem.hpp"
C_Config* g_ConfigBackup = new C_Config;

std::string C_Config::GetSave( )
{
    *g_ConfigBackup = *this;

    g_Keybinds->ApplyDefValues( );

    nlohmann::json json;

    auto& object_cfg = json[ "main" ];

    unsigned char* pData = ( unsigned char* ) this;

    json[ "data_size" ] = sizeof( *this );

    for ( int i = 0; i < sizeof( *this ); ++i )
        object_cfg.emplace_back( pData[ i ] );

    g_Keybinds->GetAllBinds( json );

    *this = *g_ConfigBackup;

    return json.dump( );
}

void C_Config::Load( std::string& cData )
{
    nlohmann::json json = nlohmann::json::parse( cData );

    auto& object_cfg = json[ "main" ];
    unsigned int data_size = json[ "data_size" ].get<unsigned int >( );

    unsigned char* pData = ( unsigned char* ) this;

    for ( int i = 0; i < data_size; ++i )
        pData[ i ] = object_cfg.at( i ).get<unsigned char >( );

    g_Keybinds->LoadAllBinds( json );

    for ( auto& cElem : g_NL->m_cVecElemets )
        if ( cElem.GetType( ) == ET_SLIDER )
            cElem.ResetSlider( );
}