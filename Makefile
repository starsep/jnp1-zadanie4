CXXFLAGS = -std=c++1z -O2 -Wall
pizza: pizza.cc
	exec ~accek/clang-3.7.0/bin/clang++ -isystem ~accek/clang-3.7.0/include/c++/v1 ${CXXFLAGS} pizza.cc -o pizza
