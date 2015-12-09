#include <array>
#include <iostream>
#include <tuple>
#include <climits>

template <typename... Kinds>
struct Pizzeria {
private:

    //tylko interfejs
    template <typename Kind, typename... otherKinds>
    struct pizza_in_menu;

    //pusta lista
    template <typename Kind>
    struct pizza_in_menu<Kind> : std::false_type {};

    //głowa jest ok
    template <typename Kind, typename... otherKinds>
    struct pizza_in_menu<Kind, Kind, otherKinds...> : std::true_type {};

    //głowa nie jest ok
    template <typename Kind, typename AnotherKind, typename... otherKinds>
    struct pizza_in_menu<Kind, AnotherKind, otherKinds...> : pizza_in_menu<Kind, otherKinds...> {};

    //////////////////////////////////////////////////////////

    template <typename Kind, typename... otherKinds>
    struct count_same_pizzas;

    template <typename Kind>
    struct count_same_pizzas<Kind> {
        static constexpr size_t value = 0;
    };

    template <typename Kind, typename... otherKinds>
    struct count_same_pizzas<Kind, Kind, otherKinds...> {
        static constexpr size_t value = 1 + count_same_pizzas<Kind, otherKinds...>::value;
    };

    template <typename Kind, typename AnotherKind, typename... otherKinds>
    struct count_same_pizzas<Kind, AnotherKind, otherKinds...> {
        static constexpr size_t value = count_same_pizzas<Kind, otherKinds...>::value;
    };

    template <typename Kind, typename... otherKinds>
    struct unique_pizza {
        static constexpr bool value = count_same_pizzas<Kind, otherKinds...>::value <= 1;
    };

    template <typename... otherKinds>
    struct unique_pizzas;

    template <typename Kind>
    struct unique_pizzas<Kind> {
        static constexpr bool value = true;
    };

    template <typename Kind, typename... otherKinds>
    struct unique_pizzas<Kind, Kind, otherKinds...> {
        static constexpr bool value = false;
    };

    template <typename Kind, typename AnotherKind, typename... otherKinds>
    struct unique_pizzas<Kind, AnotherKind, otherKinds...> {
        static constexpr bool value = unique_pizza<Kind, otherKinds...>::value && unique_pizza<AnotherKind, otherKinds...>::value;
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

    template<typename Kind, typename = void>
    struct has_yumminess : std::false_type {};

    template<typename Kind>
    struct has_yumminess<Kind, typename void_t<package<size_t, Kind::yumminess(0)>>::type> {
        static constexpr bool value = Kind::yumminess(0) == 0;// && std::is_arithmetic<decltype(Kind::yumminess(0))>::value;
    };

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

    template<typename Kind>
    static constexpr bool czyJest() {
        return ((std::is_same<Kind, Kinds>::value) || ... );
    }
    
    template <typename Kind>
    static constexpr int bestNumber(size_t piece) {
		static_assert(has_yumminess<Kind>::value, "Brak metody yumminess");
        size_t result = 0;
       	long double max = 0;
        for (size_t i = 0; i <= piece; ++i) {
            const long double curr = Kind::yumminess(i);
            if (curr > max) {
                max = curr;
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

    template <typename Kind>
    static constexpr int magia(size_t piece) {
        return Kind::yumminess(piece);
    }
    
    template<typename Kind>
    struct make_pizza {
        // Sprawdzic czy wystepuje      
        //static_assert(has_yumminess<Kind>::value, "no yumminess :(");
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
	//static_assert(has_yumminess<typename Pizza1::kind>::value, "no yumminess :(");
	//static_assert(has_yumminess<typename Pizza2::kind>::value, "no yumminess :(");
	using type = typename Pizza1::myPizzeria:: template helpBestMix<Pizza1,Pizza2>::type;
};


/* 
ASERCJE:
czy pizze z tej samej pizzeri w miksie
czy pizza wystepuje w menu przy tworzeniu
czy yumminess od zera to zero
czy przy tworzeniu pizzeri wszystkie typy rozne
*/
