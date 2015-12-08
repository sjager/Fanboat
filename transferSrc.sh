#!/bin/bash

robotics="robotics"

networkName=$(nm-tool | grep \*)

#if echo "$networkName" | grep -q "$robotics"; then
    echo "copying src folder..."
    scp -r "src" odroid@odroid2:Fanboat/
    echo
    echo "you done it!"
    echo
#else
#    echo
#    echo "Wrong network, bubba. Connect to robotics pls";
#    echo
#fi

