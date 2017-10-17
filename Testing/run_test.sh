#! /bin/sh

export LD_LIBRARY_PATH=../lib

if [ -z "$1" ]
  then
    echo "No Args"
  else
    $VGLRUN ./${1} &
fi

exit
