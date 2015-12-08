#!/bin/bash

robotics="robotics"
alex="alex"

networkName=$(nm-tool | grep \*)

if echo "$USER" | grep -q "$alex"; then

    echo "copying waypoint folder..."
    scp -r "waypoint" odroid@odroid2:Fanboat/src/
    echo
    echo "you done it!"
    echo
    
else

    if echo "$networkName" | grep -q "$robotics"; then
        echo "copying waypoint folder..."
        scp -r "waypoint" odroid@odroid2:Fanboat/src/
        echo
        echo "you done it!"
        echo
    else
        echo
        echo "Wrong network, bubba. Connect to robotics pls";
        echo
    fi

fi
