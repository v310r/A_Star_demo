#pragma once


template <typename T, glm::length_t L>
std::ostream& operator<<(std::ostream& os, const glm::vec<L, T, glm::defaultp>& vec)
{
	os << "(";
	for (glm::length_t i = 0; i < L; ++i)
	{
		os << vec[i];
		if (i < L - 1) os << ", ";
	}
	os << ")";
	return os;
}


template <typename T, glm::length_t L>
struct fmt::formatter<glm::vec<L, T, glm::defaultp>>
{
	// Parse format specifications (optional, for custom formatting)
	constexpr auto parse(fmt::format_parse_context& ctx)
	{
		return ctx.begin();
	}

	// Format the vector
	template <typename FormatContext>
	auto format(const glm::vec<L, T, glm::defaultp>& vec, FormatContext& ctx) const
	{
		fmt::format_to(ctx.out(), "(");
		for (glm::length_t i = 0; i < L; ++i)
		{
			fmt::format_to(ctx.out(), "{}", vec[i]);
			if (i < L - 1)
			{
				fmt::format_to(ctx.out(), ", ");
			}
		}
		return fmt::format_to(ctx.out(), ")");
	}
};

// Convert vector to string for logging
std::string VectorToString(const std::vector<glm::ivec2>& vec);
