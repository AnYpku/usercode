#####################
## WGammaAnalysis ###
#####################
#####################

##############################
## The sequence itself:


##############################
1) SkimSplitMerge

- it is necessary to run the splitter of the WGamma sample
- it is good to run skimmers of all MC samples for which it works except Wg_to_numu

performes skimming 
performes splitting of WGamma to munu, enu, taunu channels
potentially can easily do merging if needed

to run splitter, one should go to

$ cd SkimSplitMerge

in file runSplitVGamma.C, type nameVGammaInput (original WGamma ggNtuple)
nameWGammaEle/Muo/Tau - names of three output files including intended dir where they are wanted to be saved
nameDir and nameTree don't need to be changed

then run
$ root -l runSplitVGamma.C

do to skimming of data and MC files SkimLeptonPhoton can be used
skimming can be done for muon, electron analysis or for both at the same time 
(outputs will be save din different files for muon and electron)
in runSkimLepPho.C fileToSkim is file to be skimmed, the last TString variable in the SkimLeptonPhoton constructor is output directory

to skim many files, doSkimManyFiles.sh can be used 
but it may not be up to date

##############################
2) FullChain

FullChain made to run full analysis
it has struct with analysis parameters (FullChainParameters anPars) and ideally all the parameters to be changed for analysis, main checking and debugging purposes should be there

FullChain has method 
SetDefaultFullChainParameters(FullChainParameters& anPars)
which sets the parameters to the default values and it can be use to run the whole analysis
Hovewer in practical purposes it's frequently needed to run only some parts of analysis or run Selection in debugMode or look at the WMt rather than phoPt yields etc.

The FullChain runs the analysis in the following order:
2.1) with class Selection (in Selection directory)
     Preliminary Selection (can be skipped with anPars.noPreSelection=1)
     Extra Selection (can be skipped with anPars.noExtraSelection)
2.2) with class TTemplates (in DDBkgTemplateMethod directory)
     Data Driven Background Estimation (jets to gamma only) (can be skipped with anPars.noDDBkgComputation)
2.3) with class PrepareYields
     Prepare Yields (anPars.noPrepareYields)
2.4) with class CalcAccAndEff (AcceptanceAndEfficiency directory)
     Calculation of efficiency and acceptance (can be skipped with anPars.noCalcAccAndEff)
2.5) with class CalcCrossSection (CrossSection directory)
     Calculation of CrossSection
     Unfolding constants are computed on the fly in CalcCrossSection

##############################
2.1) Selection
- performs event selection cuts
- selection is performed in two steps:
  Selection::LoopOverTree() - does VeryPreliminary selection, only kinematic cuts are applied (phoEta, muPt, muEta, dR(pho,mu))
    it stores the preselected trees in WGammaOutput/MUON/VeryPreliminarySelected/
    one root file for each MC source (ttjets to 1l and to 2l are combined to be 1 source; WWg, WW, WZ are also combined to  1 source)
    and three root files for data: UNblind; blindPRESCALE; blindDECRACC (now blindDECRACC is blind combined method)
  Selection::ExtraSelection() - with TTree::CopyTree() does Preliminary for template method selection, Preliminary for WMt cut optimization and Fully selection 
    for each preselected tree prepares 3 trees and store them to:
    WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/
    WGammaOutput/MUON/PreliminaryForMetCutSelected/
    WGammaOutput/MUON/FullySelected/
    all the output dir names are listed in Configuration/TConfiguration.h and can be changed if necessary
- selection needs results of SkimSplitMerge (WGamma splitter), it will not separate by different lepton channels 

##############################
2.2) DDBkgTemplateMethod
- class TTemplates estimates background using data driven template method
- is capable to to estimate background not only for analysis binning but for any binning (phoEta, WMt etc). Hovewer it only can be used for DDBkgEstimation and PrepareYields, for cross section calculation one would need to run it for analysis binning so that new outputs will overwrite the outputs for e.g. WMt) 
  that's why it has so many arguments
  they are all necessary
  int channel - channel (MUON or ELECTRON)
  int blind - UNBLINB, BLIND_PRESCALE or BLIND_DECRACC
  int phoWP - WP_TIGHT, WP_MEDIUM or WP_LOOSE (it's important because the procedure has to apply sigmaIEtaIEta and phoSCRChIsoCorr cuts)
  TString varFit - "phoSCRChIsoCorr"; "phoSigmaIEtaIEta" is possible but fit doesn't work
  TString varSideband - "phoSigmaIEtaIEta"
  TString varKin - kinematic variable in bins of which we want to build the spectrum. Usually "phoEt" but also might be "WMt", "lePhoDeltaR" etc 
  int nKinBins - how many bins kinematic variable has
  float* kinBinLims, 
  int* nBinsLeftB, float* maxVarFitB, int* nBinsLeftE, float* maxVarFitE - number of fit bins before cut and upper limit for barrel and endcap for different kinematic bins 
  int nBinsLeftBTot, float maxVarFitBTot, int nBinsLeftETot, float maxVarFitETot  - the same for total kinematic range 
  bool isMetCutOptimization=0 - should be set to 1 only for varKin="WMt"

  there is a little mess in the code: TTemplates uses range for total yield kinBinLims[0]-kinBinLims[nKinBins-1]
  however in the TConfiguration there are numbers _phoPtMin and _phoPtMax and some procedures may get them from TConfiguration

##############################
2.3) PrepareYields
- estimates weighted selected yields and subtracts background
- is capable to estimate background not only for analysis binning but for any binning (phoEta, WMt etc). (see the note for DDBkgTemplateMethod)
- needs results of Selection and DDBkgTemplateMethod
- now nothing matches

##############################
2.4) AcceptanceAndEfficiency
- calculates acceptance and efficiency
- needs results of SkimSplitMerge (WGamma splitter)

##############################
2.5) CrossSection
- calculates cross section
- needs results of PrepareYields, AcceptanceAndEfficiency
- performes Unfolding on the fly (prepare constants and apply them; it's fast because for Unfolding the selected tree is used)

##############################
2.5.1) Unfolding
- performes unfolding using RooUnfold using method kBayes
  there is some mess with errors estimation here
- needs results of SkimSplitMerge (WGamma splitter) and Selection

##############################
2.5.1.1) RooUnfold
- RooUnfold which I got from GIT but as far as I know, it's not up to date and I can't even store it properly as a part of the package
  but it's necessary to do something with that

##############################
## Auxilary:

##############################
a) CertifiedConstants

lists constants officially provided by CMS
containes 
- file with photon energy correction constants
pathes to files must be listed in the /Configuration
shouldn't be hardcoded anywhere else
- JSONfiles/ list of JSON files (which are not used anywhere in the package so far but are used for calculating luminocities and pileup distributions in data using CMSSW)
- Pileup/ pileup JSON file, script with line for calculation pileup distribution in data using CMSSW, root files with pileup distributions

2013 June 11

##############################
b) Configuration

has three classes TConfiguration, TInputSample, TAllInputSamples
and text configuration file config.txt

-- config.txt is location dependednt file (up to date for UNL tier 3 as it is)
the syntax rules are written in the file

--TConfiguration.h/.C - class TConfiguration. Contains locations and names of directories and files in the package. Also contains some constants (photon Pt binning information and unhidden blinding constants). Access to all names is possible through "getters".

--TInputSample.h/.C - class TInputSample. A class for input sample structure (input file names, proper cross sections, colors for plots etc)

--TAllInputSamples.h/.C - class TAllInputSamples. Has a vector of TInputSample as a field. Can read information from config.txt. Has method Print() which prints information from the configuration files about all samples (data, sigMC, different bkg MCs). After significant modifications done in config.txt or in any class in Configuration directory, it's recommended to do Print() to make sure that reading from config.txt still properly works. Right now one can test it with
$ root -l runTest.C

2013 June 11

##############################
c) Include

-- rootlogon.C - load all classes in the package. Does not load root macros which are not classes (e.g. "aux" files).  

-- PhosphorCorrectorFunctor.hh/.cc - class PhosphorCorrectionFunctor. Calculated corrected photon Et and En for data and MC. With minor modifications, was copied from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/

-- TEventTree.h/.C - class TEventTree. The whole structure of input tree, data and MC. Variables, branches, SetBranchAddress(es), GetEntr(ies) etc. Constants of maximum possible nMu, nPho, nHLT etc in the input tree are listed here. If some tree has larger number - it has to be modified here.

-- TPhotonCuts, TElectronCuts, TMuonCuts - lists proper cuts and has cut functions. Contains kinematic, ID, isolation cuts, second lepton veto. TElectronCuts is empty now, before actual work on electrons started.

-- TMetTools - class for MET corrections (MET smearing for MC now)

-- TFullCuts - class for full cuts. Function TFullCuts::Cut is used for both Selection and AcceptanceAndEfficiency

-- TPhotonScaleFactor

-- TPuReweight

##############################
d) QuickChecks

many scripts for quick checks which have many file names and other things hardcoded
they are not compiled in Include/rootlogon.C so most of them are not up to date but many can be easily modified and can be useful

##############################
e) WGammaOutput

the output root files and plots should be stored here
More precise pathes should be listed in /Configuration

##############################
f) SearchForOverlapInMC

weak attempts to find overlaps in WGamma/Wjets and ZGamma/DYjets samples
not part of FullChain

##############################
g)remove_dot_so_d_tilda_files.sh

this file simply goes into directories and removes files
*.so *.d *~
now the names of the directories where to go are just hardcoded
into the file
Improvement may be achieved here if to do loop over directory names
in the current directory

