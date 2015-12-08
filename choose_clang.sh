#!/bin/bash
if [ `whoami` = 'starsep' ]; then
	echo '/usr/bin/clang++-3.7 -stdlib=libstdc++';
else
	echo 'exec ~accek/clang-3.7.0/bin/clang++ -isystem ~accek/clang-3.7.0/include/c++/v1'
fi
