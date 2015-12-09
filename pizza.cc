#include "pizza.hh"

#include <limits>
#include <iostream>
#include <algorithm>
#include <cassert>

struct supreme {
    static constexpr int yumminess(size_t slices) {
        return -125LL*(slices - 6)*slices*slices/243;
    }
};

struct napoli {
    static constexpr int yumminess(size_t slices) {
        return slices;
    }
};

int main() {
	using Dominion = Pizzeria<supreme, napoli>;

    using Supreme = Dominion::make_pizza<supreme>::type;

    static_assert(Supreme::count<supreme>() == 8, "Where's my supreme!?");
    static_assert(Supreme::count<napoli>() == 0, "Napoli!?");
    assert((Supreme::as_array() == std::array<size_t, 2>{{8, 0}}));
    
    using SuperSupreme = Supreme::sliced_type;
    static_assert(SuperSupreme::count<supreme>() == 16,
            "SuperSupreme does not look supreme!");

    using SuperExtraSupreme = SuperSupreme::sliced_type;
    static_assert(SuperExtraSupreme::count<supreme>() == 32,
            "SuperExtraSupreme is not supreme enough!");



	using notDominion = Pizzeria<napoli>;
	using notDominionPizza = notDominion::make_pizza<napoli>::type;
	static_assert(!std::is_same<Supreme::myPizzeria, notDominionPizza::myPizzeria>::value, "nie te same pizzerie"); 
    
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

    // Following code fragments should not compile, with clear and
    // understandable compile errors.

    //using IndecisivePizzeria = Pizzeria<supreme, supreme>;
    //using IndecisiveSupreme = IndecisivePizzeria::make_pizza<supreme>::type;

    //using DeGrasso = Pizzeria<supreme>;
    //using SupremeNapoli = DeGrasso::make_pizza<napoli>::type;

    //using DeSupreme = DeGrasso::make_pizza<supreme>::type;
    //using IllegalMix = best_mix<Supreme, DeSupreme>::type;
}
