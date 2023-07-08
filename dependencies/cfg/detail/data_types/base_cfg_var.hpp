

#pragma once

namespace sdk::detail {
	struct base_cfg_var_t {
	protected:
		std::size_t m_hash{};
	public:
		__forceinline constexpr base_cfg_var_t( ) = default;

		__forceinline constexpr base_cfg_var_t( const std::size_t hash )
			: m_hash{ hash } {}

		virtual void save( nlohmann::json& object ) const = 0;

		virtual void load( const nlohmann::json& object ) = 0;

		__forceinline std::size_t hash( ) const { return m_hash; }
	};
}