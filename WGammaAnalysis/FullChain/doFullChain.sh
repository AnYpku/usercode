# For template method without sideband variation

#source doMergeMCfiles.sh


MergeMUONandELECTRON(){
  # $1 WGamma/ZGamma
  # $2 PreliminaryFroTemplateMethodSelected/FsrSelected etc
  # $3 name
  hadd -f ../WGammaOutput/ChannelsMERGED_$1/$2/$3 ../WGammaOutput/MUON_$1/$2/$3 ../WGammaOutput/ELECTRON_$1/$2/$3
}

MergeForTemplates(){

  # FsrExcluded (to get fake-photon template)
  MergeMUONandELECTRON ZGamma FsrExcludedSelected selected_ZGamma_UNblind_DATA.root
  MergeMUONandELECTRON ZGamma FsrExcludedSelected selected_ZGammaSIGMC.root
  MergeMUONandELECTRON ZGamma FsrExcludedSelected selected_ZGammaBKGMC_DYjets_to_ll.root

  # Fsr (to (possibly) get real-photon templates)
  MergeMUONandELECTRON ZGamma FsrSelected selected_ZGamma_UNblind_DATA.root
  MergeMUONandELECTRON ZGamma FsrSelected selected_ZGammaSIGMC.root
  MergeMUONandELECTRON ZGamma FsrSelected selected_ZGammaBKGMC_DYjets_to_ll.root

  # Fully (to prepare certai plots)
  MergeMUONandELECTRON ZGamma FullySelected selected_ZGamma_UNblind_DATA.root
  MergeMUONandELECTRON ZGamma FullySelected selected_ZGammaSIGMC.root
  MergeMUONandELECTRON ZGamma FullySelected selected_ZGammaBKGMC_DYjets_to_ll.root

  # No SIHIH and CHISO cuts (to get real-photon templates if CHISO fits)
  MergeMUONandELECTRON ZGamma PreliminaryForTemplateMethodSelected selected_ZGamma_UNblind_DATA.root
  MergeMUONandELECTRON WGamma PreliminaryForTemplateMethodSelected selected_WGamma_UNblind_DATA.root
  MergeMUONandELECTRON ZGamma PreliminaryForTemplateMethodSelected selected_ZGammaSIGMC.root
  MergeMUONandELECTRON WGamma PreliminaryForTemplateMethodSelected selected_WGammaSIGMC.root
}

DoFullChainPart1(){

# $1 - MUON or ELECTRON
# $2 - WGamma or ZGamma
# $3 - logBase
# FullChain:

#  root -l -b -q AuxFchPreliminarySelection.C+\(\"$1\",\"$2\"\) #> $3$1_$2_PreliminarySelection.log 2>&1&
  echo WILL DO: 'DoFullChainPart1()' $1 $2
  root -l -b -q AuxFchExtraSelection.C+\(\"$1\",\"$2\"\) #> $3$1_$2_ExtraSelection.log 2>&1&
  root -l -b -q AuxFchPrepareYields.C+\(\"$1\",\"$2\"\) #> $3$1_$2_PrepareYiedls.log 2>&1&
#  MergeMCintoData $1 $2 > $3MergeMC.log 2>&1&
  echo DONE: 'DoFullChainPart1()' $1 $2
}
#end of DoFullChainPart2

DoFullChainPart2(){
  echo WILL DO: 'DoFullChainPart2()' $1 $2
  root -l   AuxFchTemplateFitsData.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
  root -l   AuxFchSubtractBackgroundData.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
  echo DONE: 'DoFullChainPart2()' $1 $2
}
#end of DoFullChainPart2

#DoFullChainPart1 ELECTRON ZGamma logs/log20150320/log_
#DoFullChainPart1 ELECTRON WGamma logs/log20150320/log_
#DoFullChainPart1 MUON ZGamma logs/log20150320/log_
#DoFullChainPart1 MUON WGamma logs/log20150320/log_

#MergeForTemplates

DoFullChainPart2 ELECTRON ZGamma logs/log20150320/log_
DoFullChainPart2 ELECTRON WGamma logs/log20150320/log_
DoFullChainPart2 MUON ZGamma logs/log20150320/log_
DoFullChainPart2 MUON WGamma logs/log20150320/log_

