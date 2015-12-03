#!/bin/sh
exec ~accek/clang-3.7.0/bin/clang++ -isystem ~accek/clang-3.7.0/include/c++/v1 -std=c++1z -O2 -Wall pizza.cc -o pizza
