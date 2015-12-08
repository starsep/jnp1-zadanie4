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

//////////////////////////////////////////////////////////

template <typename Kind, typename... Kinds>
struct count_same_pizzas;

template <typename Kind>
struct count_same_pizzas<Kind> {
	static constexpr size_t value = 0;
};

template <typename Kind, typename... Kinds>
struct count_same_pizzas<Kind, Kind, Kinds...> {
	static constexpr size_t value = 1 + count_same_pizzas<Kind, Kinds...>::value;
};

template <typename Kind, typename AnotherKind, typename... Kinds>
struct count_same_pizzas<Kind, AnotherKind, Kinds...> {
	static constexpr size_t value = count_same_pizzas<Kind, Kinds...>::value;
};

template <typename Kind, typename... Kinds>
struct unique_pizza {
	static constexpr bool value = count_same_pizzas<Kind, Kinds...>::value <= 1;
};

template <typename... Kinds>
struct unique_pizzas;

template <>
struct unique_pizzas<> {
	static constexpr bool value = true;
};

template <typename Kind>
struct unique_pizzas<Kind> {
	static constexpr bool value = true;
};

template <typename Kind, typename... Kinds>
struct unique_pizzas<Kind, Kind, Kinds...> {
	static constexpr bool value = false;
};

template <typename Kind, typename AnotherKind, typename... Kinds>
struct unique_pizzas<Kind, AnotherKind, Kinds...> {
	static constexpr bool value = unique_pizza<Kind, Kinds...>::value && unique_pizza<AnotherKind, Kinds...>::value;
};


/////////////////////////////////////////////////////////

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
struct has_yumminess<Kind, typename void_t<package<decltype(Kind::yumminess(0)), Kind::yumminess(0)>>::type> {
	static constexpr bool value = Kind::yumminess(0) == 0 && std::is_arithmetic<decltype(Kind::yumminess(0))>::value;
};

//////////////////////////////////////////////////////////


template<typename... Kinds>
struct Pizzeria {
private:
	static_assert(unique_pizzas<Kinds...>::value, "Not unique");
	template<typename Kind, size_t... pieces>
	struct Pizza {
	public:
		// zeby pamietac skad pochodzi
		using myPizzeria = std::tuple<Kinds...>;

		using kind = Kind;

		template<typename CountKind>
		static constexpr size_t count() {
			return ((std::is_same<CountKind, Kinds>::value * pieces) + ...);
		}

		static constexpr std::array<size_t, sizeof...(Kinds)> as_array() {
			return {{ count<Kinds>()... }};
		}

		using sliced_type = Pizza<kind, 2 * pieces...>;
	};

public:
	template<typename Kind>
	struct make_pizza {
		// Sprawdzic czy wystepuje
		//static_assert(has_yumminess<Kind>::value, "no yumminess :(");
		static_assert(pizza_in_menu<Kind, Kinds...>::value, "Nie ma w menu");
		using type = Pizza<Kind, 8 * std::is_same<Kind, Kinds>::value ...>;
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
	static_assert(std::is_same<typename Pizza1::myPizzeria, typename Pizza2::myPizzeria>::value, "nie sa z tej samej pizzeri");
	static_assert(has_yumminess<typename Pizza1::kind>::value, "no yumminess :(");
	static_assert(has_yumminess<typename Pizza2::kind>::value, "no yumminess :(");
	using type = Pizza1; //add_pizza<Pizza1, Pizza2, Pizza1::as_array().size()>;
};
