#!/bin/bash

TREASURE="Here lies your part"

a=`expr index "$TREASURE" real`; b=${TREASURE:$a}
c=`expr match "$b" '\(.........\)'`; d=${c:4:5}; 
two=${d: -1}; one=${b:((((($a*2)+42)/2)-3)/4)-1:1}

echo $TREASURE
exit 0
