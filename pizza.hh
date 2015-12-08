#include <array>
#include <iostream>
#include <tuple>

/////////////////////////////////////////////////////////
/*
 * pizza_in_menu check
 */

//tylko interfejs
template <typename Kind, typename... Kinds>
struct pizza_in_menu;

//pusta lista
template <typename Kind>
struct pizza_in_menu<Kind> : std::false_type {};

//głowa jest ok
template <typename Kind, typename... Kinds>
struct pizza_in_menu<Kind, Kind, Kinds...> : std::true_type {};

//głowa nie jest ok
template <typename Kind, typename AnotherKind, typename... Kinds>
struct pizza_in_menu<Kind, AnotherKind, Kinds...> : pizza_in_menu<Kind, Kinds...> {};

///////////////////////////////////////////////////////////

template <typename T, T x>
struct package {
	static constexpr T value = x;
};

template<typename>
struct void_t {
	using type = void;
};


///////////////////////////////////////////////////////////

/*
 * pizza has yumminess check (and yumminess(0) = 0) (using SFINAE)
 */
template<typename Kind, typename = void>
struct has_yumminess : std::false_type { };

template<typename Kind>
struct has_yumminess<Kind, typename void_t<package<size_t, Kind::yumminess(0)>>::type> : std::conditional<Kind::yumminess(0) == 0, std::true_type, std::false_type> {};

//////////////////////////////////////////////////////////


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
		static_assert(pizza_in_menu<Kind, Kinds...>::value, "Nie ma w menu");
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

struct NIL {
	using head = NIL;
	using tail = NIL;
};

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
