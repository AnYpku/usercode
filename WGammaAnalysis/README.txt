####################
## WGammaAnalysis ##
####################

to run Selection:
  cd Selection
  root -l runSelection.C

output root files availiable in the WGammaOutput
_________________________________________________________

runSelection.C calls WGammaSelection::LoopOverFiles() 
in /Selection:
   WGammaSelection: TEventTree, TSelectedEventsTree
   TEventTree - class of the input tree
   TSelectedEventsTree - class of the output tree
   WGammaSelection - loops over input files, over each event in the input trees,
                     over leptons and photons, decides whether the event 
                     passed the selection cuts or not 
                     and if yes - stores the information to the 
                     output tree

   WGammaSelection also depends on:
     Configuration/TConfiguration.h - names of the files, some constants
     Include/TMuonCuts.h - muon cuts
     Include/TElectronCuts.h - electron cuts which were not implemented yet
     Include/TPhotonCuts.h - photon cuts
     Include/PhosphorCorrectorFunctor.hh - photon energy and Et correction;
       the class was taken from 
       http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/
__________________________________________________________

the next steps to be done:
- fix "failing filling brach for memory-resident tree" error
- split TConfiguration to two files: 
    machine dependent (locations of MC and data input files)
    and machine independent (everything else)
- implement Selection for MC
    add MC-specific variables to the TEventTree and TSelectedEventsTree
    add MC-specific corrections
      
