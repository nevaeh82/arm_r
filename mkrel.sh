#!/bin/sh
cd `dirname $0`

#/etc/profile.d/qt46.sh
#echo QT:
#echo $PATH

test -e /tmp/.X4-lock || Xvfb :4 -ac 1>/dev/null 2>/dev/null & 
export DISPLAY=:4

make distclean 
make astra mkrel

exit 0;