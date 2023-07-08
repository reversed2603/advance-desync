#include "../nem.hpp"

const char* wtf[ ] = {
	"slot 1",
	"slot 2",
	"slot 3",
	"slot 4",
	"slot 5"
};

namespace sdk {
	void c_cfg::save( std::ptrdiff_t idx ) const {
		const auto path = std::filesystem::path{ SDK_CFG_ID_OBJECT } /= wtf [ idx ];

		g_Keybinds->ApplyDefValues( );

		/* create directory if its not exists */
		std::filesystem::create_directory( SDK_CFG_ID_OBJECT );
		nlohmann::json json{};

		auto& object = json[ ENCSTR( SDK_CFG_ID_OBJECT ) ];

		for ( const auto& var : m_vars )
			var->save( object );

		g_Keybinds->GetAllBinds( json );

		auto str = json.dump( );

		/* encrypt our cfg so the user wouldn't know tf stored in it */
		for ( auto& chr : str )
			chr ^= k_byte_xor;

		if ( std::ofstream file{ path, std::ios::out | std::ios::trunc } )
			file << str;
	}

	void c_cfg::load( std::ptrdiff_t idx ) {
		const auto path = std::filesystem::path{ SDK_CFG_ID_OBJECT } /= wtf[ idx ];

		std::filesystem::create_directory( SDK_CFG_ID_OBJECT );

		std::string str{};
		if ( std::ifstream file{ path, std::ios::in } )
			file >> str;
		else
			return;

		if ( str.empty( ) )
			return;

		for ( auto& chr : str )
			chr ^= k_byte_xor;

		const auto json = nlohmann::json::parse( str );
		if ( !json.is_object( ) )
			return;

		const auto object = json.find( ( SDK_CFG_ID_OBJECT ) );
		if ( object == json.end( ) )
			return;

		const auto& value = object.value( );

		for ( auto& var : m_vars )
			var->load( value );

		g_Keybinds->LoadAllBinds( const_cast < nlohmann::json& > ( json ) );

		for ( auto& cElem : g_NL->m_cVecElemets )
			if ( cElem.GetType( ) == ET_SLIDER )
				cElem.ResetSlider( );
	}
}