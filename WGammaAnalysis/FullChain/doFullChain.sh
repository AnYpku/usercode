# For template method without sideband variation

source doMergeMCfiles.sh


MergeMUONandELECTRON(){
  # $1 WGamma/ZGamma
  # $2 PreliminaryFroTemplateMethodSelected/FsrSelected etc
  # $3 name
  hadd -f ../WGammaOutput/ChannelsMERGED_$1/$2/$3 ../WGammaOutput/ELECTRON_$1/$2/$3 ../WGammaOutput/MUON_$1/$2/$3
}

MergeChannels(){

  # VeryPreliminary
  MergeMUONandELECTRON ZGamma VeryPreliminarySelected selected_ZGamma_UNblind_DATA.root
  MergeMUONandELECTRON ZGamma VeryPreliminarySelected selected_ZGammaSIGMC.root
  MergeMUONandELECTRON ZGamma VeryPreliminarySelected selected_ZGammaBKGMC_DYjets_to_ll.root

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
  MergeMUONandELECTRON ZGamma PreliminaryForTemplateMethodSelected selected_ZGammaBKGMC_DYjets_to_ll.root
  MergeMUONandELECTRON WGamma PreliminaryForTemplateMethodSelected selected_WGammaSIGMC.root
}

DoFullChainPart1(){

# $1 - MUON or ELECTRON
# $2 - WGamma or ZGamma
# $3 - logBase
# FullChain:

#  
  
#  root -l -b -q runApplySF.C
  echo WILL DO: 'DoFullChainPart1()' $1 $2
# root -l -b -q AuxFchPreliminarySelection.C+\(\"$1\",\"$2\"\) #> $3$1_$2_PreliminarySelection.log 2>&1&
# root -l -b -q AuxFchExtraSelection.C+\(\"$1\",\"$2\"\) #> $3$1_$2_ExtraSelection.log 2>&1&
# root -l  AuxFchPrepareYields.C+\(\"$1\",\"$2\"\) #> $3$1_$2_PrepareYiedls.log 2>&1&
  echo DONE: 'DoFullChainPart1()' $1 $2
}
#end of DoFullChainPart2

DoFullChainPart2(){
  echo WILL DO: 'DoFullChainPart2()' $1 $2
#  root -l -b -q AuxFchTemplateFitsData_CHISO.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
#  root -l -b -q AuxFchTemplateFitsMCclosure_CHISO.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
#  root -l -b -q AuxFchTemplateFitsData_SIHIH.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
#  root -l -b -q AuxFchTemplateFitsMCclosure_SIHIH.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
#  root -l -b -q AuxFchSubtractBackgroundData.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
#  root -l -b -q AuxFchSubtractBackgroundMCclosure.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&

#  root -l -b -q  AuxFchSystRandomizeTempl_CHISO.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
# # # [DO NOT NEED] root -l -b -q  AuxFchSystRandomizeTempl_SIHIH.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
# # # [DO NOT NEED] root -l -b -q  AuxFchSystRandomizeTempl_CHISO_MCclosure.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
# # # [DO NOT NEED] root -l -b -q  AuxFchSystRandomizeTempl_SIHIH_MCclosure.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&

#  root -l -b -q  AuxFchSystZgNorm_CHISO.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
# # # [DO NOT NEED]  root -l -b -q  AuxFchSystZgNorm_SIHIH.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&

# Have to do the next 6 steps again because there are 
# certain files
# got open and didn't get closed
# this has to be fixed
# or have to rerun these steps again#

#  root -l -b -q   AuxFchTemplateFitsData_CHISO.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
###  root -l -b -q  AuxFchTemplateFitsMCclosure_CHISO.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
#  root -l -b -q   AuxFchTemplateFitsData_SIHIH.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
###  root -l -b -q  AuxFchTemplateFitsMCclosure_SIHIH.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
#  root -l -b -q AuxFchSubtractBackgroundData.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
#  root -l -b -q AuxFchSubtractBackgroundMCclosure.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&

# root -l -b -q  AuxFchCombineCHISOandSIHIH.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&

# root -l -b -q  AuxFchCalcAccXEff.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
  echo DONE: 'DoFullChainPart2()' $1 $2
}
#end of DoFullChainPart2

DoFullChainPart3(){
  echo WILL DO: 'DoFullChainPart3()' $1 $2
  root -l  AuxFchCrossSection.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
  echo DONE: 'DoFullChainPart3()' $1 $2
}
#end of DoFullChainPart3

###########################
# Full Chain starts here:
###########################

# DoFullChainPart1 include selection and PrepareYields MC based only:

#DoFullChainPart1 ELECTRON ZGamma logs/log20150320/log_
#DoFullChainPart1 ELECTRON WGamma logs/log20150320/log_
#DoFullChainPart1 MUON ZGamma logs/log20150320/log_
#DoFullChainPart1 MUON WGamma logs/log20150320/log_

#MergeChannels

#MergeMCintoData MUON 
#MergeMCintoData ELECTRON
#MergeMCintoData ChannelsMERGED 

# DD e->gamma bkg estimation for W_GAMMA ELECTRON only: #

#root -l -b -q AuxFchBkgEtoGamma.C+
#root -l -b -q AuxFchBkgEtoGamma_NoWMtCut.C+
#root -l -b -q AuxFchBkgEtoGamma_MCclosure.C+
#root -l -b -q AuxFchBkgEtoGamma_NoWMtCut_MCclosure.C+

#DoFullChainPart2 ELECTRON ZGamma logs/log20150320/log_
#DoFullChainPart2 ELECTRON WGamma logs/log20150320/log_
#DoFullChainPart2 MUON ZGamma logs/log20150320/log_
#DoFullChainPart2 MUON WGamma logs/log20150320/log_

#root -l -b -q runCombineTheoryCS.C

#DoFullChainPart3 ELECTRON ZGamma logs/log20150320/log_
#DoFullChainPart3 ELECTRON WGamma logs/log20150320/log_
DoFullChainPart3 MUON ZGamma logs/log20150320/log_
#DoFullChainPart3 MUON WGamma logs/log20150320/log_

#root -l -b -q runReplotMigrationMatrix.C
#root -l runCompareCS.C
#mv compareCS*WGamma* ../WGammaOutput/ChannelsMERGED_WGamma/Plots/CrossSection/
#mv compareCS*ZGamma* ../WGammaOutput/ChannelsMERGED_ZGamma/Plots/CrossSection/
