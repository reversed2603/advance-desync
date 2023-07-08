#pragma once

namespace nem {
	class c_menu {
	private:
		IDirect3DTexture9* m_logo{}, *m_avatar{};
	public:
		void init( IDirect3DDevice9* const device );
		void draw_multicombo ( const char* name, std::vector<int>& variable, char* labels[ ], int count );

		void on_paint( );
	};

	inline const auto g_menu = std::make_unique< c_menu >( );
}