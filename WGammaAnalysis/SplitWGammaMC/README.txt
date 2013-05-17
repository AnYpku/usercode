####################
## SplitWGammaMC  ##
####################

to run WGamma Monte Carlo sample splitter (e, mu, tau channels)
  $ cd SplitWGammaMC
  $ root -l runSplitWGammaMC.C

_________________________________________________________

in /SplitWGammaMC:

   runSplitWGammaMC.C calls SplitWGammaMC::LoopOverInputTree();
   run it with line
   $ root -l runSplitWGammaMC.C
   splits the  job_summer12_Wg.root ntuple produced by Ming
   to Wg_enu.root, Wg_munu.root, Wg_taunu.root
   SplitWGammaMC : TOutputTree,
   TOuputTree : TInputTree
   TInputTree - automatucally generated class with the structure of the tree in the job_summer12_Wg.root
   TOutputTree - class which sets branches to the output tree, inherites all branch variables from TInputTree
   SplitWGammaMC - loop over input tree entries, over iMC in each entry and searches for iMC for which mcPID[iMC]==24
      if the mcPID[iMC]==24 (W) and:
        mcDecayType[iMC]==2:   W->enu
        mcDecayType[iMC]==3:   W->munu
        mcDecayType[iMC]==4:   W->taunu 
      then the whole event is filled to W_enu, W_munu or W_taunu tree respectively 
_________________________________________________________
