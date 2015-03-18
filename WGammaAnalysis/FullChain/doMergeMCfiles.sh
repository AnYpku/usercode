#!/bin/bash

MergeMCintoDataOne(){

  # $1 - ${namedir}, $2 - namevgamma
  affixselected=selected_
  affixdata=_UNblind_DATA_MCclosure
  affixsigmc=SIGMC
  affixbkgmcbase=BKGMC*
  affixroot=.root

  namefiledatafull=$1${affixselected}$2${affixdata}${affixroot}

  #merge all MC files to make them look like data
  #make sure there are no debugMode, noPuReweight, etc files
  #otherwise the result will not be correct
  hadd -f ${namefiledatafull} $1${affixselected}$2${affixsigmc}${affixroot} $1${affixselected}$2${affixbkgmcbase}${affixroot}

}
# end of MergeMCintoDataOne()

MergeMCintoData(){

  # $1 - MUON or ELECTRON, $2 - WGamma or ZGamma
  MergeMCintoDataOne ../WGammaOutput/$1_$2/PreliminaryForTemplateMethodSelected/ $2
  MergeMCintoDataOne ../WGammaOutput/$1_$2/FullySelected/ $2
  MergeMCintoDataOne ../WGammaOutput/$1_$2/FsrSelected/ $2
}
# end of MergeMCintoData
