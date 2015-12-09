

#include "pizza.hh"

#include <cstddef>
#include <limits>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <vector>

struct supreme {
	static constexpr int yumminess(size_t slices) {
		return -125LL*(slices - 6)*slices*slices/243;
	}
};

struct napoli {
	int tab[12345678901ULL];
	static constexpr uint64_t yumminess(size_t slices) {
		return slices;
	}
};

struct margherita {
	static constexpr double AlmostPi = 3.14159265358979323846;

	static constexpr double almost_sin(double x) {
		// Wzor Taylora do 9-tej potegi, wykonany mod 2 * Pi; dokladnosc tutaj to
		// okolo 0.000004
		if (x < 0) { return -almost_sin(-x); }
		if (x > AlmostPi + 0.01) { return almost_sin(x - AlmostPi * 2); }

		return x * (1
					- x * x / (2 * 3) * (1
										 - x * x / (4 * 5) * (1
															  - x * x / (6 * 7) * (1
																				   - x * x / (8 * 9)))));
	}

	static constexpr double yumminess(size_t slices) {
		return almost_sin(slices);
	}
};

struct AType {
private:
	int value_;

public:
	constexpr AType() : value_(0) {}
	constexpr AType(int x) : value_(x) {}

	constexpr bool operator<(const AType& other) const { return value_ < other.value_; }
	constexpr bool operator>(const AType& other) const { return value_ > other.value_; }
	constexpr bool operator<=(const AType& other) const { return value_ <= other.value_; }
	constexpr bool operator>=(const AType& other) const { return value_ >= other.value_; }
	constexpr bool operator==(const AType& other) const { return value_ == other.value_; }
	constexpr bool operator!=(const AType& other) const { return value_ != other.value_; }
};

struct pepperoni {
	static constexpr AType yumminess(size_t slices) {
		return AType((slices * slices * slices) % 37);
	}
};


template<unsigned id>
struct P {
	static constexpr long yumminess(size_t slices) {
		return 1L * id * slices * (10 - slices);
	}
};

template<>
struct P<222> {
	static constexpr long yumminess(size_t slices) {
		return 222222222L * slices * (250000 - slices);
	}
};

// Przepraszam milosnikow hawajskiej.
struct hawaiian {
	static constexpr long yumminess(size_t slices) {
		return -1L * slices * slices;
	}
};

struct italiano {
private:
	template<typename T> static constexpr T myabs(T x) {
		return (x >= 0) ? x : (-x);
	}

public:
	static constexpr size_t yumminess(size_t slices) {
		return 7 - myabs(int64_t(slices) % 14 - 7);
	}
};

struct primavera {
	static constexpr __uint128_t yumminess(size_t slices) {
		return slices == 0 ? 0 : (slices == 1 ? 1 : yumminess(slices - 1) * slices);
	}
};



struct bad_pizza_101 {
	static constexpr int yumminess(size_t slices) {
		return slices * 5 + 3;
	}
};

struct bad_pizza_102 {
	static constexpr int yumminess = 0;
};

struct bad_pizza_103 {
	static int yumminess(size_t slices) {
		return slices;
	}
};

struct bad_pizza_104 {
	static const int yumminess = 0;
};

struct bad_pizza_105 {
	static constexpr int yumminess(size_t a, size_t b) {
		return a * b;
	}
};

struct bad_pizza_106 {
	template<typename T> static constexpr int yumminess(size_t a, T x = T(0)) {
		return a + x;
	}
};

struct bad_pizza_107 {};


template<typename T>
constexpr const T make_const(const T& x) {
	return x;
}


int main() {
#if TEST_NUM == 100
	using Dominion = Pizzeria<supreme, napoli>;

    using Supreme = Dominion::make_pizza<supreme>::type;
    static_assert(Supreme::count<supreme>() == 8,
            "Where's my supreme!?");
    static_assert(Supreme::count<napoli>() == 0,
            "Napoli!?");
    assert((Supreme::as_array() == std::array<size_t, 2>{{8, 0}}));

    using SuperSupreme = Supreme::sliced_type;
    static_assert(SuperSupreme::count<supreme>() == 16,
            "SuperSupreme does not look supreme!");

    using SuperExtraSupreme = SuperSupreme::sliced_type;
    static_assert(SuperExtraSupreme::count<supreme>() == 32,
            "SuperExtraSupreme is not supreme enough!");

    using SupremeMix = best_mix<Supreme, SuperSupreme>::type;
    static_assert(SupremeMix::count<supreme>() == 4,
            "http://www.wolframalpha.com/input/?i=1000*%28%28x%2F18%29%5E2-3%28x%2F18%29%5E3%29");
    static_assert(Supreme::count<napoli>() == 0,
            "How come I got napoli from supreme!?");

    using Napoli = Dominion::make_pizza<napoli>::type;
    static_assert(Napoli::count<napoli>() == 8,
            "Where's my napoli!?");
    static_assert(Napoli::count<supreme>() == 0,
            "Supreme!?");

    using BestMix = best_mix<Supreme, Napoli>::type;
    static_assert(BestMix::count<supreme>() == 4,
            "Buy yourself a supreme calculator!");
    static_assert(BestMix::count<napoli>() == 8,
            "There should be 8 napoli slices here!");

    using AnotherSupreme = Dominion::make_pizza<supreme>::type;
    static_assert(std::is_same<Supreme, AnotherSupreme>::value,
            "Pizzas don't have names, so how come can you say they're different!?");
    static_assert(!std::is_same<Supreme, SuperSupreme>::value,
            "Sure, no one can see the difference between Supreme and SuperSupreme!");

    using MixedMixes = best_mix<SupremeMix, BestMix>::type;
    static_assert(std::is_same<BestMix, MixedMixes>::value,
            "Something is definitely mixed here...");

    using Margherita = Pizzeria<margherita>::make_pizza<margherita>::type;
    using BigMargherita = Margherita::sliced_type::sliced_type::sliced_type
                                    ::sliced_type::sliced_type;
    static_assert(BigMargherita::count<margherita>() == 256,
            "It really should be 2^8");

    using BestMargherita = best_mix<BigMargherita, BigMargherita>::type;
    static_assert(make_const(BestMargherita::as_array())[0] == 366,
            "Everyone knows the best margherita has 366 slices!");

    using Pepperoni = Pizzeria<pepperoni>::make_pizza<pepperoni>::type;
    using GoodPepperoni = best_mix<Pepperoni, Pepperoni>::type;
    static_assert(GoodPepperoni::count<pepperoni>() == 11, "Should be eleven");

    using LargePizzeria = Pizzeria<
      P<0>,  P<1>,  P<2>,  P<3>,  P<4>,  P<5>,  P<6>,  P<7>,  P<8>,  P<9>, 
      P<10>,  P<11>,  P<12>,  P<13>,  P<14>,  P<15>,  P<16>,  P<17>,  P<18>,  P<19>, 
      P<20>,  P<21>,  P<22>,  P<23>,  P<24>,  P<25>,  P<26>,  P<27>,  P<28>,  P<29>, 
      P<30>,  P<31>,  P<32>,  P<33>,  P<34>,  P<35>,  P<36>,  P<37>,  P<38>,  P<39>, 
      P<40>,  P<41>,  P<42>,  P<43>,  P<44>,  P<45>,  P<46>,  P<47>,  P<48>,  P<49>, 
      P<50>,  P<51>,  P<52>,  P<53>,  P<54>,  P<55>,  P<56>,  P<57>,  P<58>,  P<59>, 
      P<60>,  P<61>,  P<62>,  P<63>,  P<64>,  P<65>,  P<66>,  P<67>,  P<68>,  P<69>, 
      P<70>,  P<71>,  P<72>,  P<73>,  P<74>,  P<75>,  P<76>,  P<77>,  P<78>,  P<79>, 
      P<80>,  P<81>,  P<82>,  P<83>,  P<84>,  P<85>,  P<86>,  P<87>,  P<88>,  P<89>, 
      P<90>,  P<91>,  P<92>,  P<93>,  P<94>,  P<95>,  P<96>,  P<97>,  P<98>,  P<99>, 
      P<100>,  P<101>,  P<102>,  P<103>,  P<104>,  P<105>,  P<106>,  P<107>,  P<108>,  P<109>, 
      P<110>,  P<111>,  P<112>,  P<113>,  P<114>,  P<115>,  P<116>,  P<117>,  P<118>,  P<119>, 
      P<120>,  P<121>,  P<122>,  P<123>,  P<124>,  P<125>,  P<126>,  P<127>,  P<128>,  P<129>, 
      P<130>,  P<131>,  P<132>,  P<133>,  P<134>,  P<135>,  P<136>,  P<137>,  P<138>,  P<139>, 
      P<140>,  P<141>,  P<142>,  P<143>,  P<144>,  P<145>,  P<146>,  P<147>,  P<148>,  P<149>, 
      P<150>,  P<151>,  P<152>,  P<153>,  P<154>,  P<155>,  P<156>,  P<157>,  P<158>,  P<159>, 
      P<160>,  P<161>,  P<162>,  P<163>,  P<164>,  P<165>,  P<166>,  P<167>,  P<168>,  P<169>, 
      P<170>,  P<171>,  P<172>,  P<173>,  P<174>,  P<175>,  P<176>,  P<177>,  P<178>,  P<179>, 
      P<180>,  P<181>,  P<182>,  P<183>,  P<184>,  P<185>,  P<186>,  P<187>,  P<188>,  P<189>, 
      P<190>,  P<191>,  P<192>,  P<193>,  P<194>,  P<195>,  P<196>,  P<197>,  P<198>,  P<199>, 
      P<200>,  P<201>,  P<202>,  P<203>,  P<204>,  P<205>,  P<206>,  P<207>,  P<208>,  P<209>, 
      P<210>,  P<211>,  P<212>,  P<213>,  P<214>,  P<215>,  P<216>,  P<217>,  P<218>,  P<219>, 
      P<220>,  P<221>,  P<222>,  P<223>,  P<224>,  P<225>,  P<226>,  P<227>,  P<228>,  P<229>, 
      P<230>,  P<231>,  P<232>,  P<233>,  P<234>,  P<235>,  P<236>,  P<237>,  P<238>,  P<239>, 
      P<240>,  P<241>,  P<242>,  P<243>,  P<244>,  P<245>,  P<246>,  P<247>,  P<248>,  P<249> 
    >;
    using SomePizza = LargePizzeria::make_pizza<P<222>>::type;
    using SomeHugePizza = SomePizza::sliced_type::sliced_type::sliced_type::sliced_type
                                   ::sliced_type::sliced_type::sliced_type::sliced_type
                                   ::sliced_type::sliced_type::sliced_type::sliced_type
                                   ::sliced_type;
    using SomeHPizza = best_mix<SomeHugePizza, SomeHugePizza>::type;
    using SomeEnermousPizza = SomeHPizza::sliced_type::sliced_type::sliced_type::sliced_type
                                        ::sliced_type::sliced_type::sliced_type::sliced_type
                                        ::sliced_type::sliced_type::sliced_type::sliced_type
                                        ::sliced_type::sliced_type::sliced_type::sliced_type
                                        ::sliced_type::sliced_type::sliced_type::sliced_type
                                        ::sliced_type::sliced_type::sliced_type::sliced_type
                                        ::sliced_type::sliced_type::sliced_type::sliced_type
                                        ::sliced_type::sliced_type::sliced_type::sliced_type
                                        ::sliced_type::sliced_type::sliced_type::sliced_type
                                        ::sliced_type::sliced_type::sliced_type::sliced_type
                                        ::sliced_type::sliced_type::sliced_type::sliced_type
                                        ::sliced_type::sliced_type::sliced_type;

    static_assert(SomePizza::count<P<222>>() == 8, "Ha!");
    static_assert(SomePizza::count<P<69>>() == 0, "Haha!");
    static_assert(SomeHugePizza::count<P<222>>() == 65536, "Hahaha!");
    static_assert(SomeHPizza::count<P<222>>() == 125000, "Hahahaha!");
    static_assert(SomeEnermousPizza::count<P<222>>() == 17592186044416000000UL,
        "Hahahahaha!");
#endif

#if (TEST_NUM >= 101) && (TEST_NUM <= 107)
	#define BAD_PIZZA_IN(x) bad_pizza_ ## x
#define BAD_PIZZA(x) BAD_PIZZA_IN(x)

    using PizzaType   = BAD_PIZZA(TEST_NUM);
    using BadPizzeria = Pizzeria<PizzaType>;
    using BadPizza    = BadPizzeria::make_pizza<PizzaType>::type;
    using X = best_mix<BadPizza, BadPizza>::type;

#endif

#if TEST_NUM == 108
	using IndecisivePizzeria = Pizzeria<supreme, supreme>;
    using IndecisiveSupreme = IndecisivePizzeria::make_pizza<supreme>::type;
#endif

#if TEST_NUM == 109
	using DeGrasso = Pizzeria<supreme>;
    using SupremeNapoli = DeGrasso::make_pizza<napoli>::type;
#endif

#if TEST_NUM == 110
	using Dominion = Pizzeria<supreme, napoli>;
    using Supreme = Dominion::make_pizza<supreme>::type;
    using DeGrasso = Pizzeria<supreme>;
    using DeSupreme = DeGrasso::make_pizza<supreme>::type;
    using IllegalMix = best_mix<Supreme, DeSupreme>::type;
#endif


#if (TEST_NUM >= 111) && (TEST_NUM <= 114)
	using ClosedPizzeria = Pizzeria<>;
#if TEST_NUM == 112
    using NonexistentPizza = ClosedPizzeria::make_pizza<supreme>::type;
#elif TEST_NUM == 113
    using EmptyPizza = ClosedPizzeria::make_pizza<>::type;
#elif TEST_NUM == 114
    using OverflowPizza = ClosedPizzeria::make_pizza<supreme, supreme>::type;
#endif
#endif

#if TEST_NUM == 115
	using Dominion = Pizzeria<supreme, napoli>;
    using ImpossibleOrder = Dominion::make_pizza<supreme, napoli>::type;
#endif

#if (TEST_NUM >= 116) && (TEST_NUM <= 118)
	using DoubledPizzeria = Pizzeria<double>;
    using DoublePizza = DoubledPizzeria::make_pizza<double>::type;
#if TEST_NUM == 117
    using BadMixPizza = best_mix<DoublePizza, DoublePizza>::type;
#elif TEST_NUM == 118
    using FloatPizza = DoubledPizzeria::make_pizza<float>::type;
#endif
#endif

#if TEST_NUM == 119
	using Dominion1 = Pizzeria<supreme, napoli>;
    using Dominion2 = Pizzeria<supreme, napoli>;
    using Supreme1 = Dominion1::make_pizza<supreme>::type;
    using Supreme2 = Dominion2::make_pizza<supreme>::type;
    using SupremeMix = best_mix<Supreme1, Supreme2>::type;
    static_assert(SupremeMix::count<supreme>() == 4, "WAT");
    static_assert(SupremeMix::count<napoli>() == 0, "WAT x2");
#endif

#if TEST_NUM == 120
	using Dominion1 = Pizzeria<double, supreme, napoli>;
    using Dominion2 = Pizzeria<double, napoli, supreme>;
    using Supreme1 = Dominion1::make_pizza<supreme>::type;
    using Supreme2 = Dominion2::make_pizza<supreme>::type;
    using SupremeMix = best_mix<Supreme1, Supreme2>::type;
#endif

#if TEST_NUM == 121
	using Dominion1 = Pizzeria<int, uint64_t, double, float, supreme, short>;
    using Dominion2 = Pizzeria<int, uint64_t, float, double, supreme, short>;
    using Supreme1 = Dominion1::make_pizza<supreme>::type;
    using Supreme2 = Dominion2::make_pizza<supreme>::type;
    using SupremeMix = best_mix<Supreme1, Supreme2>::type;
#endif

#if TEST_NUM == 122
	using Recursive = Pizzeria<Pizzeria<Pizzeria<Pizzeria<Pizzeria<Pizzeria<
                               Pizzeria<Pizzeria<Pizzeria<Pizzeria<>>>>>>>>>>;
    using RecursivePizza =
        Recursive::make_pizza<Pizzeria<Pizzeria<Pizzeria<Pizzeria<Pizzeria<
                                       Pizzeria<Pizzeria<Pizzeria<Pizzeria<>>>>>>>>>>::type;

    using SlicedRecursive = RecursivePizza::sliced_type::sliced_type::sliced_type;
    static_assert(SlicedRecursive::count<Pizzeria<>>() == 0, "Not enough recursive");
    static_assert(SlicedRecursive::count<Pizzeria<Pizzeria<Pizzeria<Pizzeria<
        Pizzeria<Pizzeria<Pizzeria<Pizzeria<>>>>>>>>>() == 0,
        "Still not enough recursive");
    static_assert(SlicedRecursive::count<Pizzeria<Pizzeria<Pizzeria<Pizzeria<
        Pizzeria<Pizzeria<Pizzeria<Pizzeria<Pizzeria<>>>>>>>>>>() == 64,
        "It should be that recursive!");
    static_assert(SlicedRecursive::count<Pizzeria<Pizzeria<Pizzeria<Pizzeria<
        Pizzeria<Pizzeria<Pizzeria<Pizzeria<Pizzeria<Pizzeria<>>>>>>>>>>>() == 0,
        "Too much recursion!");
#endif

#if (TEST_NUM >= 123) && (TEST_NUM <= 124)
	using MyPizzeria = Pizzeria<hawaiian, italiano, primavera>;
    using Hawaiian  = MyPizzeria::make_pizza<hawaiian> ::type::sliced_type::sliced_type;
    using Italiano  = MyPizzeria::make_pizza<italiano> ::type::sliced_type::sliced_type;
    using Primavera = MyPizzeria::make_pizza<primavera>::type::sliced_type::sliced_type;

    using MixHIP = best_mix<best_mix<Hawaiian, Italiano>::type, Primavera>::type;
    using MixHPI = best_mix<best_mix<Hawaiian, Primavera>::type, Italiano>::type;
    using MixIHP = best_mix<best_mix<Italiano, Hawaiian>::type, Primavera>::type;
    using MixIPH = best_mix<best_mix<Italiano, Primavera>::type, Hawaiian>::type;
    using MixPHI = best_mix<best_mix<Primavera, Hawaiian>::type, Italiano>::type;
    using MixPIH = best_mix<best_mix<Primavera, Italiano>::type, Hawaiian>::type;

#if TEST_NUM == 123
#define TEST_MIX(mix) \
    static_assert(make_const(mix::as_array())[0] == 0, "Hawaiian mix"); \
    static_assert(make_const(mix::as_array())[1] % 14 == 7, "Italiano mix"); \
    static_assert(make_const(mix::as_array())[2] == 32, "Primavera mix");

    TEST_MIX(MixHIP);
    TEST_MIX(MixHPI);
    TEST_MIX(MixIHP);
    TEST_MIX(MixIPH);
    TEST_MIX(MixPHI);
    TEST_MIX(MixPIH);

#else
    // W sumie ponizsze asserty moga sie wywalic (w niektorych pizzach teoretycznie
    // mogloby byc 7 kawalkow Italiano, w innych 21), ale jest to dosc nieprawdopodobne.
    static_assert(std::is_same<MixHIP, MixHPI>::value, "Test 1");
    static_assert(std::is_same<MixHIP, MixIHP>::value, "Test 2");
    static_assert(std::is_same<MixHIP, MixIPH>::value, "Test 3");
    static_assert(std::is_same<MixHIP, MixPHI>::value, "Test 4");
    static_assert(std::is_same<MixHIP, MixPIH>::value, "Test 5");
#endif

#endif


#if TEST_NUM == 125
	vector<int> V;
#endif

#if TEST_NUM == 126
	pizza<> P;
#endif

#if TEST_NUM == 127
	pizza<supreme> P;
#endif

#if TEST_NUM == 128
	pizza<supreme, size_t> P;
#endif

#if TEST_NUM == 129
	pizza<supreme, 1> P;
#endif

#if TEST_NUM == 130
	Pizza<> P;
#endif

#if TEST_NUM == 131
	Pizza<supreme> P;
#endif

#if TEST_NUM == 132
	Pizza<supreme, size_t> P;
#endif

#if TEST_NUM == 133
	Pizza<supreme, 1> P;
#endif

}


