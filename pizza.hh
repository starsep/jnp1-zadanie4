#include <array>
#include <iostream>
#include <tuple>


template<typename... Kinds>
struct Pizzeria {
private:
	template<size_t... pieces>
	struct Pizza {
	public:
		// zeby pamietac skad pochodzi
		using myPizzeria = std::tuple<Kinds...>;

		template<typename CountKind>
		static constexpr size_t count() {
			return ((std::is_same<CountKind, Kinds>::value * pieces) + ...);
		}

		static constexpr std::array<size_t, sizeof...(Kinds)> as_array() {
			return {{ count<Kinds>()... }};
		}

		using sliced_type = Pizza<2 * pieces...>;
	};

public:
	template<typename Kind>
	struct make_pizza {
		// Sprawdzic czy wystepuje
		using type = Pizza<8 * std::is_same<Kind, Kinds>::value ...>;
	};

	template<typename Pizza1, typename Pizza2>
	friend
	struct best_mix;
};

template<size_t N>
struct Int {
	static const size_t value = N;
};

template <typename Int1, typename Int2>
struct SumInt {
	static constexpr size_t value = Int1::value + Int2::value;
};

/////////////////////////////////////////////////////////
template <bool is, typename Kind, typename AnotherType, typename... Kinds>
struct pizza_in_menu {
	static constexpr bool value = ;
};

template <bool is, typename Kind, typename... Kinds>
struct pizza_in_menu {
	static constexpr bool value = is;
};

template <typename... Kinds>
struct pizza_in_menu<true, Kinds...> {
	static constexpr bool value = true;
};

template <typename Kind, typename AnotherKind, typename... Kinds>
struct pizza_in_menu<false, Kind, AnotherKind, Kinds...> {
	static constexpr bool value = pizza_in_menu<std::is_same<Kind, AnotherKind>::value, Kind, Kinds...>::value;
};
///////////////////////////////////////////////////////////

struct NIL {
	using head = NIL;
	using tail = NIL;
};

template <typename Kind, typename... Kinds>
struct check_pizza_in_menu : pizza_in_menu<false, Kind, Kinds...> { };

template<typename H, typename T = NIL>
struct List {
	using head = H;
	using tail = T;
};

template <typename List>
struct Length {
	using tail = typename List::tail;
	static constexpr size_t length = Length<tail>::value;
	static constexpr size_t value = length + 1;
};

template <>
struct Length<NIL> {
	static constexpr size_t value = 0;
};



template <typename List1, typename List2>
struct ListSum {
	using type = List<List1, List2>;
};

template <>
struct ListSum<NIL, NIL> {
	using type = NIL;
};


template<typename Pizza1, typename Pizza2>
struct best_mix {
	// musi sprawdzic czy pizze sa z tej samej pizzeri
	static_assert(std::is_same<typename Pizza1::myPizzeria, typename Pizza2::myPizzeria>::value,
				  "nie sa z tej samej pizzeri");
	using type = Pizza1; //add_pizza<Pizza1, Pizza2, Pizza1::as_array().size()>;
};
