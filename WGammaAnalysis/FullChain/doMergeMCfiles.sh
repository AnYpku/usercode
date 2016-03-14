#!/bin/bash

MergeMCintoDataOneWg(){

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
  hadd -f ${namefiledatafull} $1${affixselected}$2${affixbkgmcbase}${affixroot} $1${affixselected}$2${affixsigmc}${affixroot} 

}
# end of MergeMCintoDataOne()

MergeMCintoDataOneZg(){

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
  hadd -f ${namefiledatafull} $1${affixselected}$2${affixbkgmcbase}${affixroot} $1${affixselected}$2${affixsigmc}${affixroot} 

}
# end of MergeMCintoDataOne()

MergeMCintoData(){

  # $1 - MUON or ELECTRON, $2 - WGamma or ZGamma
  MergeMCintoDataOneWg ../WGammaOutput/$1_WGamma/PreliminaryForTemplateMethodSelected/ WGamma
  MergeMCintoDataOneWg ../WGammaOutput/$1_WGamma/FullySelected/ WGamma
  MergeMCintoDataOneWg ../WGammaOutput/$1_WGamma/PreliminaryForEtoGammaWithPSVcutSelected/ WGamma
  MergeMCintoDataOneWg ../WGammaOutput/$1_WGamma/PreliminaryForEtoGammaNoPSVcutSelected/ WGamma

  MergeMCintoDataOneZg ../WGammaOutput/$1_ZGamma/PreliminaryForTemplateMethodSelected/ ZGamma
  MergeMCintoDataOneZg ../WGammaOutput/$1_ZGamma/FullySelected/ ZGamma
  MergeMCintoDataOneZg ../WGammaOutput/$1_ZGamma/FsrSelected/ ZGamma
  MergeMCintoDataOneZg ../WGammaOutput/$1_ZGamma/FsrExcludedSelected/ ZGamma

}
# end of MergeMCintoData
