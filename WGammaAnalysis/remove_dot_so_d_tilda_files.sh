#!/bin/sh

rm *~ .nfs00*
affixtilda=/*~
affixso=/*.so
affixd=/*.d
affixnsf00=/.nsf00*

for d in *
  do
    if [ "$d" = "RooUnfold" ] 
    then
      echo "no removals into $d" 
    else 
      echo "will try to do removals into $d"
      rm ${d}${affixtilda} ${d}${affixso} ${d}${affixd} ${d}${affixnsf00}
    fi
  done 
