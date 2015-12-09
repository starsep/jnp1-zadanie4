#include <array>
#include <iostream>
#include <tuple>
#include <climits>

template <typename... Kinds>
struct Pizzeria {
private:
    //interfejs pizza_in_menu
    template <typename Kind, typename... otherKinds>
    struct pizza_in_menu;

    //specjalizacja pizza_in_menu: pusta lista
    template <typename Kind>
    struct pizza_in_menu<Kind> : std::false_type {};

    //specjalizacja pizza_in_menu: głowa tego samego typu
    template <typename Kind, typename... otherKinds>
    struct pizza_in_menu<Kind, Kind, otherKinds...> : std::true_type {};

    //specjalizacja pizza_in_menu: głowa innego typu
    template <typename Kind, typename AnotherKind, typename... otherKinds>
    struct pizza_in_menu<Kind, AnotherKind, otherKinds...> : pizza_in_menu<Kind, otherKinds...> {};

	//interfejs count_same_pizzas
    template <typename Kind, typename... otherKinds>
    struct count_same_pizzas;

	//specjalizacja pizza_in_menu: pusta lista
    template <typename Kind>
    struct count_same_pizzas<Kind> {
        static constexpr size_t value = 0;
    };

	//specjalizacja pizza_in_menu: głowa tego samego typu
    template <typename Kind, typename... otherKinds>
    struct count_same_pizzas<Kind, Kind, otherKinds...> {
        static constexpr size_t value = 1 + count_same_pizzas<Kind, otherKinds...>::value;
    };

	//specjalizacja pizza_in_menu: głowa innego typu
    template <typename Kind, typename AnotherKind, typename... otherKinds>
    struct count_same_pizzas<Kind, AnotherKind, otherKinds...> {
        static constexpr size_t value = count_same_pizzas<Kind, otherKinds...>::value;
    };

    template <typename Kind, typename... otherKinds>
    struct unique_pizza {
        static constexpr bool value = count_same_pizzas<Kind, otherKinds...>::value <= 1;
    };

	//interfejs unique_pizzas
    template <typename... otherKinds>
    struct unique_pizzas;

	//specjalizacja unique_pizzas: pusta lista
    template <typename Kind>
    struct unique_pizzas<Kind> : std::true_type { };

	//specjalizacja unique_pizzas: głowa tego samego typu
    template <typename Kind, typename... otherKinds>
    struct unique_pizzas<Kind, Kind, otherKinds...> : std::false_type { };

	//specjalizacja unique_pizzas: głowa innego typu
    template <typename Kind, typename AnotherKind, typename... otherKinds>
    struct unique_pizzas<Kind, AnotherKind, otherKinds...> {
        static constexpr bool value = unique_pizza<Kind, otherKinds...>::value && unique_pizza<AnotherKind, otherKinds...>::value;
    };

	//struktura opakowująca wartość w typ
    template <typename T, T x>
    struct package {
        static constexpr T value = x;
    };

    template<typename>
    struct void_t {
        using type = void;
    };

    template<typename Kind, typename = void>
    struct has_yumminess : std::false_type {};

    template<typename Kind>
    struct has_yumminess<Kind, typename void_t<package<decltype(Kind::yumminess(0)), Kind::yumminess(0)>>::type>
		: std::true_type { };

	template<size_t... pieces>
    struct Pizza {
	public:
		// zeby pamietac skad pochodzi
		using myPizzeria = Pizzeria<Kinds...>;

        template<typename CountKind>
        static constexpr size_t count() {           
            return ((std::is_same<CountKind,Kinds>::value * pieces) + ...);
        }               

        static constexpr std::array<size_t, sizeof...(Kinds)> as_array() {
            return {{count<Kinds>()...}};
        }
        using sliced_type = Pizza<2*pieces...>;
    };
    
    template <typename Kind>
    static constexpr int bestNumber(size_t piece) {
		static_assert(has_yumminess<Kind>::value, "Brak metody yumminess");
		using YumminessType = decltype(Kind::yumminess(0));
		static_assert(Kind::yumminess(0) == YumminessType(0), "yumminess(0) nie jest zerowe");
		//static_assert(std::is_arithmetic<YumminessType>::value, "Typ zwracany przez yumminess nie jest arytmetyczny");
        size_t result = 0;
       	YumminessType max(0);
        for (size_t i = 0; i <= piece; ++i) {
            const YumminessType current = Kind::yumminess(i);
            if (current > max) {
                max = current;
                result = i;
            }
        }       
        return result;
    }

    template<typename Kind1, typename Kind2> 
    struct helpBestMix {
        using type = Pizza<bestNumber<Kinds>
                (Kind1::template count<Kinds>() + Kind2:: template count<Kinds>()) ...>;
    };

public:
	static_assert(unique_pizzas<Kinds...>::value, "W pizzeri powtarzaja sie typy pizz");

    template<typename Kind>
    struct make_pizza {
        // Sprawdzic czy wystepuje
        static_assert(pizza_in_menu<Kind, Kinds...>::value, "Tej pizzy nie ma w menu tej pizzeri");
        using type = Pizza<8 * std::is_same<Kind,Kinds>::value ...>;
    };

    template<typename Pizza1, typename Pizza2>
    friend struct best_mix;
};


template<typename Pizza1, typename Pizza2>
struct best_mix {
    // musi sprawdzic czy pizze sa z tej samej pizzeri
    static_assert(std::is_same<typename Pizza1::myPizzeria, typename Pizza2::myPizzeria>::value, "Pizze z roznych pizzeri");
	using type = typename Pizza1::myPizzeria:: template helpBestMix<Pizza1,Pizza2>::type;
};
