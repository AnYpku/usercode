source doMergeMCfiles.sh

DoFullChain(){
# $1 - MUON or ELECTRON
# $2 - WGamma or ZGamma
#  root -l -b -q AuxFchPreliminarySelection.C+\(\"$1\",\"$2\"\)
#  root -l -b -q AuxFchExtraSelection.C+\(\"$1\",\"$2\"\)
  root -l -b -q AuxFchPrepareYields.C+\(\"$1\",\"$2\"\)
  MergeMCintoData $1 $2
}
#end of DoFullChain

DoFullChain MUON ZGamma
