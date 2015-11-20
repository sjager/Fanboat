#!/bin/bash

robotics="robotics"

networkName=$(nm-tool | grep \*)

#if echo "$networkName" | grep -q "$robotics"; then
    echo "copying lab3 folder..."
    scp -r "lab3" odroid@odroid2:Fanboat/src/
    echo
    echo "you done it!"
    echo
#else
    #echo
    #echo "Wrong network, bubba. Connect to robotics pls";
    #echo
#fi

