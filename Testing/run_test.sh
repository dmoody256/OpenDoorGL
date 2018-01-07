#! /bin/sh

export LD_LIBRARY_PATH=../lib

if [ -z "$1" ]
  then
    ./${1}
  else
    $VGLRUN ./${1}
fi

exit
