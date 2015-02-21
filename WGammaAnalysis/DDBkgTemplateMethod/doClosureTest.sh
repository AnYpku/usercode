#!/bin/bash

MergeMCintoData(){

  # $1 - ${namedir}, $2 - namevgamma
  affixselected=selected_
  affixdata=_UNblind_DATA
  affixsigmc=SIGMC
  affixbkgmcbase=BKGMC*
  affixroot=.root
  affixref=_Ref

  namefiledatafull=$1${affixselected}$2${affixdata}${affixroot}
  namefiledataref=$1${affixselected}$2${affixdata}${affixref}${affixroot}

  #move data file for reference (because it will be replaced now)
  mv ${namefiledatafull} ${namefiledataref}
#  echo moved ${namefiledatafull} to ${namefiledataref}

  #merge all MC files to make them look like data
  #make sure there are no debugMode, noPuReweight, etc files
  #otherwise the result will not be correct
  hadd ${namefiledatafull} $1${affixselected}$2${affixsigmc}${affixroot} $1${affixselected}$2${affixbkgmcbase}${affixroot}

}
# end of MergeMCintoData()

MoveDataFileBack(){

  # $1 - ${namedir}, $2 - namevgamma
  affixselected=selected_
  affixdata=_UNblind_DATA
  affixroot=.root
  affixref=_Ref

  namefiledatafull=$1${affixselected}$2${affixdata}${affixroot}
  namefiledataref=$1${affixselected}$2${affixdata}${affixref}${affixroot}

  #move data file back (because it was replaced)
  mv ${namefiledataref} ${namefiledatafull}
#  echo moved ${namefiledataref} to ${namefiledatafull}
}
# end of MoveDataFileBack()

MovePlotNamesToReference(){
  # $1 - ${namedir}, $2 - ${fileextension}
  star=*_
  ref=_Ref
  for f in $1${star}$2
    do
      orig=$f
      orig=${orig/$2/${ref}$2}
      mv $f ${orig}
#      echo moved $f to ${orig}
    done
}
# end of MovePlotNamesToReference() 

MovePlotNamesToReferenceAllExt(){
  # $1 - ${namedir}
  MovePlotNamesToReference $1 .png
  MovePlotNamesToReference $1 .pdf
  MovePlotNamesToReference $1 .root
}
# end of MovePlotNamesToReferenceAllExt()

MovePlotNamesToClosure(){
  # $1 - ${namedir}, $2 - ${fileextension}
  star=*_
  clos=_MCclosure
  for f in $1${star}$2
    do
      orig=$f
      orig=${orig/$2/${clos}$2}
      mv $f ${orig}
#      echo moved $f to ${orig}
    done
}
# end of MovePlotNamesToClosure()

MovePlotNamesToClosureAllExt(){
  # $1 - ${namedir}
  MovePlotNamesToClosure $1 .png
  MovePlotNamesToClosure $1 .pdf
  MovePlotNamesToClosure $1 .root
}
# end of MovePlotNamesToClosureAllExt()

MovePlotNamesReferencesBack(){
  # $1 - ${namedir}, $2 - ${fileextension}
  star=*_Ref
  ref=_Ref
  for f in $1${star}$2
    do
      orig=$f
      orig=${orig/${ref}$2/$2}
      mv $f ${orig}
#      echo moved $f to ${orig}
    done
}
# end of MovePlotNamesReferencesBack()

MovePlotNamesReferencesBackAllExt(){
  # $1 - ${namedir}
  MovePlotNamesReferencesBack $1 .png
  MovePlotNamesReferencesBack $1 .pdf
  MovePlotNamesReferencesBack $1 .root
}
# end of MovePlotNamesReferencesBackAllExt()

##################################
## Merge MC files into data

namevgamma=WGamma

namedir=../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/
MergeMCintoData $namedir $namevgamma

namedir=../WGammaOutput/MUON_WGamma/FullySelected/
MergeMCintoData $namedir $namevgamma

namedir=../WGammaOutput/ELECTRON_WGamma/PreliminaryForTemplateMethodSelected/
MergeMCintoData $namedir $namevgamma

namedir=../WGammaOutput/ELECTRON_WGamma/FullySelected/
MergeMCintoData $namedir $namevgamma

namevgamma=ZGamma

namedir=../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/
MergeMCintoData $namedir $namevgamma

namedir=../WGammaOutput/MUON_ZGamma/FullySelected/
MergeMCintoData $namedir $namevgamma

namedir=../WGammaOutput/MUON_ZGamma/FsrSelected/
MergeMCintoData $namedir $namevgamma

namedir=../WGammaOutput/ELECTRON_ZGamma/PreliminaryForTemplateMethodSelected/
MergeMCintoData $namedir $namevgamma

namedir=../WGammaOutput/ELECTRON_ZGamma/FullySelected/
MergeMCintoData $namedir $namevgamma

namedir=../WGammaOutput/ELECTRON_ZGamma/FsrSelected/
MergeMCintoData $namedir $namevgamma

## end of Merge MC files into data
##################################

MovePlotNamesToReferenceAllExt ../WGammaOutput/MUON_ZGamma/Plots/TemplateFits/
MovePlotNamesToReferenceAllExt ../WGammaOutput/MUON_WGamma/Plots/TemplateFits/
MovePlotNamesToReferenceAllExt ../WGammaOutput/MUON_ZGamma/Plots/PrepareYields/
MovePlotNamesToReferenceAllExt ../WGammaOutput/MUON_WGamma/Plots/PrepareYields/

MovePlotNamesToReferenceAllExt ../WGammaOutput/ELECTRON_ZGamma/Plots/TemplateFits/
MovePlotNamesToReferenceAllExt ../WGammaOutput/ELECTRON_WGamma/Plots/TemplateFits/
MovePlotNamesToReferenceAllExt ../WGammaOutput/ELECTRON_ZGamma/Plots/PrepareYields/
MovePlotNamesToReferenceAllExt ../WGammaOutput/ELECTRON_WGamma/Plots/PrepareYields/

#root -l -b -q runClosureTest.C
root -l runClosureTest.C


MovePlotNamesToClosureAllExt ../WGammaOutput/MUON_ZGamma/Plots/TemplateFits/
MovePlotNamesToClosureAllExt ../WGammaOutput/MUON_WGamma/Plots/TemplateFits/
MovePlotNamesToClosureAllExt ../WGammaOutput/MUON_ZGamma/Plots/PrepareYields/
MovePlotNamesToClosureAllExt ../WGammaOutput/MUON_WGamma/Plots/PrepareYields/

MovePlotNamesToClosureAllExt ../WGammaOutput/ELECTRON_ZGamma/Plots/TemplateFits/
MovePlotNamesToClosureAllExt ../WGammaOutput/ELECTRON_WGamma/Plots/TemplateFits/
MovePlotNamesToClosureAllExt ../WGammaOutput/ELECTRON_ZGamma/Plots/PrepareYields/
MovePlotNamesToClosureAllExt ../WGammaOutput/ELECTRON_WGamma/Plots/PrepareYields/

MovePlotNamesReferencesBackAllExt ../WGammaOutput/MUON_ZGamma/Plots/TemplateFits/
MovePlotNamesReferencesBackAllExt ../WGammaOutput/MUON_WGamma/Plots/TemplateFits/
MovePlotNamesReferencesBackAllExt ../WGammaOutput/MUON_ZGamma/Plots/PrepareYields/
MovePlotNamesReferencesBackAllExt ../WGammaOutput/MUON_WGamma/Plots/PrepareYields/

MovePlotNamesReferencesBackAllExt ../WGammaOutput/ELECTRON_ZGamma/Plots/TemplateFits/
MovePlotNamesReferencesBackAllExt ../WGammaOutput/ELECTRON_WGamma/Plots/TemplateFits/
MovePlotNamesReferencesBackAllExt ../WGammaOutput/ELECTRON_ZGamma/Plots/PrepareYields/
MovePlotNamesReferencesBackAllExt ../WGammaOutput/ELECTRON_WGamma/Plots/PrepareYields/

##################################
## Move data files back

namevgamma=WGamma

namedir=../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/
MoveDataFileBack $namedir $namevgamma

namedir=../WGammaOutput/MUON_WGamma/FullySelected/
MoveDataFileBack $namedir $namevgamma

namedir=../WGammaOutput/ELECTRON_WGamma/PreliminaryForTemplateMethodSelected/
MoveDataFileBack $namedir $namevgamma

namedir=../WGammaOutput/ELECTRON_WGamma/FullySelected/
MoveDataFileBack $namedir $namevgamma

namevgamma=ZGamma

namedir=../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/
MoveDataFileBack $namedir $namevgamma

namedir=../WGammaOutput/MUON_ZGamma/FullySelected/
MoveDataFileBack $namedir $namevgamma

namedir=../WGammaOutput/MUON_ZGamma/FsrSelected/
MoveDataFileBack $namedir $namevgamma

namedir=../WGammaOutput/ELECTRON_ZGamma/PreliminaryForTemplateMethodSelected/
MoveDataFileBack $namedir $namevgamma

namedir=../WGammaOutput/ELECTRON_ZGamma/FullySelected/
MoveDataFileBack $namedir $namevgamma

namedir=../WGammaOutput/ELECTRON_ZGamma/FsrSelected/
MoveDataFileBack $namedir $namevgamma

## end of Move data files back
##################################

