#! /bin/sh

export LD_LIBRARY_PATH=../lib

if [ -z "$1" ]
  then
    echo "No Args"
  else
    ./${1}
fi
