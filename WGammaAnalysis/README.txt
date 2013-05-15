####################
## WGammaAnalysis ##
####################

to run WGamma Monte Carlo sample splitter (e, mu, tau channels)
  $ cd SplitWGammaMC
  $ root -l runSplitWGammaMC.C

to run Selection:
  $ cd Selection
  $ root -l runSelection.C

output root files availiable in the WGammaOutput

_________________________________________________________

in /SplitWGammaMC:

   independent script, splits Wg to Wg_munug, Wg_enug and Wg_taunug
   $ root -l runSplitWGammaMC.C
   more detailed README aviliable in the /SplitWGammaMC
_________________________________________________________


in /Selection:

   runSelection.C calls WGammaSelection::LoopOverFiles()
   run it with line
   $ root -l runSelection.C
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
      
