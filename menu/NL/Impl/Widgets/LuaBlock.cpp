#include "..\..\..\..\nem.hpp"

#include <filesystem>

C_Element& C_Element::LuaBlock( )
{
	m_eType = ET_LUA_BLOCK;

	m_cMapVars[ _h( "CreateCallback" ) ] = C_Var( _h( "LuaBlockCreateCallback" ) );

	return *this;
}

static int current_lua {};
static std::vector< std::string > luas {};

void load( )
{
	if ( !luas.empty( ) )
		nem::g_lua->load( luas.at( current_lua ) );
}

void refresh( )
{
	current_lua = 0;

	luas.clear( );

	for ( auto& file : std::filesystem::recursive_directory_iterator { std::filesystem::path{ "off-white/luas" } } )
		luas.emplace_back ( file.path ( ).filename ( ).string ( ) );
}

void unload( )
{
	if ( !luas.empty( ) )
		nem::g_lua->unload( luas.at( current_lua ) );
}

void unload_all( )
{
	nem::g_lua->unload_all( );
}

int page = 0;

void prev( )
{
	page = std::clamp( page - 1, 0, ( int ) ( luas.size( ) / 15 ) );
}

void next( )
{
	page = std::clamp( page + 1, 0, ( int ) ( luas.size( ) / 15 ) );
}

void C_Nl::LuaBlockCreateCallback( C_Element* pElement )
{
	/*static const auto once = [&] ( ) {
		for ( auto& file : std::filesystem::recursive_directory_iterator { std::filesystem::path{  "off-white/luas" } } )
			luas.emplace_back( file.path( ).filename( ).string( ) );

		return true;
	}( );*/

	//ImGui::PushFont( g_NL->pMenuFont );

	//auto sMainButtonsPos = CHILD_ELEMENT_POS( 0 );
	//
	//sMainButtonsPos.x -= 15.f;
	//sMainButtonsPos.y -= 10.f;

	//static C_Element cButtonRefresh = C_Element( ).Button( " Refresh ", sMainButtonsPos, ImVec2( 90.f, 30.f ), refresh );
	//
	//sMainButtonsPos.x += 100.f;
	//
	//static C_Element cButtonUnloadAll = C_Element( ).Button( "Unload All", sMainButtonsPos, ImVec2( 100.f, 30.f ), unload_all );
	//
	//sMainButtonsPos.x += 110.f;

	//static C_Element cButtonPrev = C_Element( ).Button( "Prev", sMainButtonsPos, ImVec2( 60.f, 30.f ), prev );

	//sMainButtonsPos.x += 70.f;

	//static C_Element cButtonNext = C_Element( ).Button( "Next", sMainButtonsPos, ImVec2( 60.f, 30.f ), next );

	//ButtonCreateCallback( &cButtonRefresh );
	//ButtonCreateCallback( &cButtonUnloadAll );
	//ButtonCreateCallback( &cButtonPrev );
	//ButtonCreateCallback( &cButtonNext );

	//static std::vector < C_Element > cButtons = { };

	//int iPos = 0;

	//for ( int i = page * 15; i < std::min( ( page * 15 ) + 15, ( int ) luas.size( ) ); i++, iPos++ ) {

	//	if ( cButtons.size( ) <= i )
	//		cButtons.push_back( C_Element( ).Button( " ", ImVec2( ), ImVec2( ), nullptr ) );

	//	auto& cButtonElem = cButtons[ i ];

	//	current_lua = i;
	//	bool bIsLoaded = nem::g_lua->is_loaded( luas[ i ] );

	//	auto sTextPos = CHILD_ELEMENT_POS( ( iPos + 1 ) );
	//	ImGui::SetCursorPos( sTextPos );

	//	ImGui::PushStyleColor( ImGuiCol_Text, bIsLoaded ? RGB_VEC( 242.f, 242.f, 242.f, 255.f ) : RGB_VEC( 153.f, 176.f, 189.f, 255.f ) );
	//	ImGui::Text( luas[ i ].c_str( ) );
	//	ImGui::PopStyleColor( );

	//	sTextPos.x += 230.f;
	//	sTextPos.y -= 5.f;

	//	if ( !bIsLoaded )
	//		sTextPos.x += 10.f;

	//	cButtonElem.GetVar( _h( "Origin" ) ).GetVec2( ) = sTextPos;
	//	cButtonElem.GetVar( _h( "Size" ) ).GetVec2( ) = bIsLoaded ? ImVec2( 90.f, 30.f ) : ImVec2( 70.f, 30.f );
	//	cButtonElem.GetVar( _h( "Text" ) ).GetString( ) = bIsLoaded ? "Unload" : "Load";
	//	cButtonElem.GetVar( _h( "UnId" ) ).GetInt( ) = i;

	//	cButtonElem.GetVar( _h( "ClickCallback" ) ).SetRef( bIsLoaded ? unload : load );

	//	ButtonCreateCallback( &cButtonElem );
	//}

	//ImGui::PopFont( );


	static const auto once = [&] ( ) {
		for ( auto& file : std::filesystem::recursive_directory_iterator { std::filesystem::path{  "off-white/luas" } } )
			luas.emplace_back( file.path( ).filename( ).string( ) );

		return true;
	}( );

	ImGui::SetCursorPosX( 19.f );
	ImGui::SetCursorPosY( 69.f );

	ImGui::PushFont( g_NL->pMenuFont );
	ImGui::PushStyleColor( ImGuiCol_Border, RGB_VEC( 24.f, 24.f, 24.f, 255.f ) );
	ImGui::PushStyleColor( ImGuiCol_ChildBg, RGB_VEC( 13.f, 13.f, 13.f, 165.f ) );
	ImGui::PushStyleColor( ImGuiCol_Header, ImVec4( 45.f / 255.f, 45.f / 255.f, 45.f / 255.f, 80.f / 255.f ) );
	ImGui::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 4.f );


	if (ImGui::BeginListBox( "##lua", ImVec2( 315.f, 200.f ) ) )
	{
		for ( int i = 0; i < luas.size( ); i++ ) {
			auto loaded = luas[ i ] + " loaded";
			if ( ImGui::Selectable( nem::g_lua->is_loaded( luas[ i ] ) ? loaded.c_str( ) : luas[ i ].data( ), i == current_lua ) ) {
				current_lua = i;
			}
		}

		ImGui::EndListBox( );
	}

	ImGui::PopFont( );
	ImGui::PopStyleColor( 3 );
	ImGui::PopStyleVar( );

	static C_Element cButtonLoad = C_Element( ).Button( " Load ", CHILD_ELEMENT_POS( 5 ), ImVec2( 70.f, 30.f ), load );
	static C_Element cButtonRefresh = C_Element( ).Button( " Refresh ", CHILD_ELEMENT_POS( 6 ), ImVec2( 90.f, 30.f ), refresh );
	static C_Element cButtonUnload = C_Element( ).Button( " Unload ", CHILD_ELEMENT_POS( 7 ), ImVec2( 90.f, 30.f ), unload );
	static C_Element cButtonUnloadAll = C_Element( ).Button( " Unload All ", CHILD_ELEMENT_POS( 8 ), ImVec2( 110.f, 30.f ), unload );

	ButtonCreateCallback( &cButtonLoad );
	ButtonCreateCallback( &cButtonRefresh );
	ButtonCreateCallback( &cButtonUnload );
	ButtonCreateCallback( &cButtonUnloadAll );

	/*ImGui::SetCursorPosX ( 15 );
	if ( ImGui::Button ( "load", ImVec2 ( 54, 25 ) ) && !luas.empty ( ) )
		nem::g_lua->load ( luas.at ( current_lua ) );

	ImGui::Spacing ( );
	ImGui::SetCursorPosX ( 15 );
	if ( ImGui::Button ( "refresh", ImVec2 ( 57, 25 ) ) ) {
		current_lua = 0;

		luas.clear ( );

		for ( auto& file : std::filesystem::recursive_directory_iterator { std::filesystem::path{ "off-white/luas" } } )
			luas.emplace_back ( file.path ( ).filename ( ).string ( ) );
	}
	ImGui::Spacing ( );
	ImGui::SetCursorPosX ( 15 );
	if ( ImGui::Button ( "unload", ImVec2 ( 57, 25 ) ) && !luas.empty ( ) )
		nem::g_lua->unload ( luas.at ( current_lua ) );
	ImGui::Spacing ( );
	ImGui::SetCursorPosX ( 15 );

	if ( ImGui::Button ( "unload all", ImVec2 ( 65, 25 ) ) && !luas.empty ( ) )
		nem::g_lua->unload_all ( );*/

}