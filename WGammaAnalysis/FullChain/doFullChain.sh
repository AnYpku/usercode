source doMergeMCfiles.sh

MergeForTemplates(){
  hadd -f ../WGammaOutput/MERGED/selected_WGamma_ForTemplates.root ../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGamma_UNblind_DATA.root ../WGammaOutput/ELECTRON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGamma_UNblind_DATA.root

  hadd -f ../WGammaOutput/MERGED/selected_WGamma_ForTemplates_MCclosure.root ../WGammaOutput/MUON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGamma_UNblind_DATA_MCclosure.root ../WGammaOutput/ELECTRON_WGamma/PreliminaryForTemplateMethodSelected/selected_WGamma_UNblind_DATA_MCclosure.root

  hadd -f ../WGammaOutput/MERGED/selected_ZGamma_ForTemplates_MCclosure.root ../WGammaOutput/MUON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGamma_UNblind_DATA_MCclosure.root ../WGammaOutput/ELECTRON_ZGamma/PreliminaryForTemplateMethodSelected/selected_ZGamma_UNblind_DATA_MCclosure.root
}

DoFullChain(){

# $1 - MUON or ELECTRON
# $2 - WGamma or ZGamma
# $3 - logBase
# FullChain:

#  root -l -b -q AuxFchPreliminarySelection.C+\(\"$1\",\"$2\"\) #> $3$1_$2_PreliminarySelection.log 2>&1&
#  root -l -b -q AuxFchExtraSelection.C+\(\"$1\",\"$2\"\) #> $3$1_$2_ExtraSelection.log 2>&1&
#  root -l  AuxFchPrepareYields.C+\(\"$1\",\"$2\"\) #> $3$1_$2_PrepareYiedls.log 2>&1&
#  MergeMCintoData $1 $2 > $3MergeMC.log 2>&1&
#  MergeForTemplates
#  root -l -b -q AuxFchMCclosureSidebandVariation.C+\(\"$1\",\"$2\"\) #> $3MCclosureSidebandVariation.log 2>&1&
#  root -l -b -q AuxFchTemplatesPickSideband.C+\(\"$1\",\"$2\"\) #> $3PickSideband.log 2>&1&
#  root -l -b -q AuxFchMCclosureFits.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
#  root -l -b -q AuxFchMCclosureCompareYields.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
#  root -l -b -q AuxFchTemplateFitsData.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
#  root -l -b -q AuxFchSubtractBackgroundData.C+\(\"$1\",\"$2\"\) #> $3$1_$2MCclosure.log 2>&1&
#  root -l -b -q AuxFchSidebandVariationData.C+\(\"$1\",\"$2\"\) #> $3MCclosureSidebandVariation.log 2>&1&
   root -l -b -q AuxFchComputeSystDueToSbVar.C+\(\"$1\",\"$2\"\)
}
#end of DoFullChain

echo Will DO ELECTRON ZGamma
DoFullChain ELECTRON ZGamma logs/log20150320/log_
echo DONE ELECTRON ZGamma

echo Will DO ELECTRON WGamma
DoFullChain ELECTRON WGamma logs/log20150320/log_
echo DONE ELECTRON WGamma

echo Will DO MUON ZGamma
DoFullChain MUON ZGamma logs/log20150320/log_
echo DONE MUON ZGamma

echo Will DO MUON WGamma
DoFullChain MUON WGamma logs/log20150320/log_
echo DONE MUON WGamma
