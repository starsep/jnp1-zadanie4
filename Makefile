CXXFLAGS = -std=c++1z -O2 -Wall -Wunused -Wshadow -pedantic
COMPILER := $(shell ./choose_clang.sh)

pizza: pizza.cc pizza.hh
	 ${COMPILER} ${CXXFLAGS} pizza.cc -o pizza

clean:
	rm -f pizza