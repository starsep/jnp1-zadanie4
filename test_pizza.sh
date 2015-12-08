#!/bin/bash

# lista rzeczy, ktore maja sie skompilowac i uruchomic poprawnie
OKLIST="100 111 116 119 122 123 124 134"

# lista rzeczy, ktore maja sie skompilowac bez zadnych ostrzezen
WERRORLIST="100 119 122 123 124 134"

MINTEST=100
MAXTEST=134

# limit czasu kompilacji (s)
COMPILETL=20
# limit czasu wykonania (s)
RUNTL=0.1

ACCEK="/home/staff/iinf/accek"


make_test() {
  ntest=$1
  cmd="`./choose_clang.sh` -std=c++1z -O2 pizza_sokolowski.cc -o pizza -DTEST_NUM=$ntest"

  echo -n "$ntest: "

  if [[ $OKLIST =~ $ntest ]]
  then
    cmd="$cmd -Wall"

    if [[ $WERRORLIST =~ $ntest ]]
    then
      cmd="$cmd -W -Wextra -Wunused -Wshadow -Wuninitialized -Werror"
    fi

    timeout $COMPILETL $cmd > /dev/null 2> /dev/null
    errcode=$?

    if [[ $errcode -eq 124 ]]
    then
      echo "CE (limit czasu kompilacji)"
      return
    elif [[ $errcode -ne 0 ]]
    then
      echo "CE"
      return
    fi

    timeout $RUNTL ./pizza > /dev/null 2> /dev/null
    errcode=$?
    if [[ $errcode -eq 124 ]]
    then
      echo "TL"
    elif [[ $errcode -ne 0 ]]
    then
      echo "RE"
    else
      echo "OK (program wykonal sie poprawnie)"
    fi


  else
    cmd="$cmd -w"

    timeout $COMPILETL $cmd > /dev/null 2> /dev/null
    errcode=$?

    if [[ $errcode -eq 124 ]]
    then
      echo "CE (limit czasu kompilacji)"
    elif [[ $errcode -eq 0 ]]
    then
      echo "WA (program sie skompilowal)"
    else
      echo "OK (blad kompilacji)"
    fi
  fi

}


for t in `seq $MINTEST $MAXTEST`
do
  make_test $t
done

