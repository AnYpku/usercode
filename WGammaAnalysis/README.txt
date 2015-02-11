#####################
## WGammaAnalysis ###
#####################
#####################

based on ggNtuples
is to compute total and differential cross sections of W_GAMMA
(while it's not intended to analize Z_GAMMA, the package has Z_GAMMA built in, it's used for different cross-checks)

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

to do skimming of data and MC files SkimLeptonPhoton can be used
skimming can be done for muon, electron analysis or for both at the same time;
for Z_GAMMA (lep+lep+pho) or for W_GAMMA (lep+pho) or for both at the same time 
(outputs will be save din different files for muon and electron)
in runSkimLepPho.C fileToSkim is file to be skimmed, the last TString variable in the SkimLeptonPhoton constructor is output directory

to skim many files, doSkimManyFiles.sh can be used 
but it may not be up to date

##############################
2) FullChain

FullChain made to run full analysis
has to have splitted and skimmed ggNtuples listed into the configuration file 
it has struct with analysis parameters (FullChainParameters anPars) and ideally all the parameters to be changed for analysis, main checking and debugging purposes should be there

FullChain has method 
SetDefaultFullChainParameters(FullChainParameters& anPars, TString varKin)
which sets the parameters to the default values and it can be used to run the whole analysis
varKin = "phoEt", photon transverse momentum for the analysis purposes, 
however other kinematic variables can be used to plot and look at the yields
in practical purposes it's frequently needed to run only some parts of analysis or run Selection in debugMode or look at the WMt rather than phoPt yields etc.

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
  Selection::LoopOverInputFiles() - does VeryPreliminary selection, only kinematic cuts are applied (phoEta, muPt, muEta, dR(pho,mu) and phoEt - 
    !!! needs to be excluded from here for the unfolding purposes !!! )
    it stores the preselected trees in WGammaOutput/MUON/VeryPreliminarySelected/
    one root file for each MC source (ttjets to 1l and to 2l are combined to be 1 source; WWg, WW, WZ are also combined to  1 source)
    and three root files for data: UNblind and blindPRESCALE
  Selection::ExtraSelection() - with TTree::CopyTree() does Preliminary for template method selection and Fully selection 
    for each preselected tree prepares 3 trees and store them to:
    WGammaOutput/MUON/PreliminaryForTemplateMethodSelected/
    WGammaOutput/MUON/FullySelected/
    all the output dir names are listed in Configuration/TConfiguration.h and can be changed if necessary
- selection needs results of SkimSplitMerge (WGamma splitter), it will not separate by different lepton channels 
* MUON stand for muon channel, for ELECTRON channel the output dir is WGammaOutput/ELECTRON; 
right now only muon channel is being actively worked on in this framework

##############################
2.2) DDBkgTemplateMethod
- class TTemplates estimates background using data driven template method
- is capable to to estimate background not only for analysis binning but for any binning (phoSCEta, WMt etc). However it only can be used for DDBkgEstimation and PrepareYields) 
  the class is developed to be independent from the rest of the framework and is initiated from FullChain with AuxTemplates.C

##############################
2.3) PrepareYields
- estimates weighted selected yields and subtracts background (only jets->g and true-gamma from MC at the moment)
- is capable to estimate background not only for analysis binning but for any binning (phoEta, WMt etc). (see the note for DDBkgTemplateMethod)
- needs results of Selection and DDBkgTemplateMethod
- if anPars.noDDBkgComputation=1, will only plot data vs MC plots for barrel, endcap and sum

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
- RooUnfold which I got from GIT but as far as I know, it's not up to date; now made to look as a part of this package

##############################
## Auxilary:

##############################
a) CertifiedConstants

lists constants officially provided by CMS
containes 
- JSONfiles/ list of JSON files (which are not used anywhere in the package so far but are used for calculating luminocities and pileup distributions in data using CMSSW)
- Pileup/ pileup JSON file, script with line for calculation pileup distribution in data using CMSSW, root files with pileup distributions

pathes to files must be listed in the /Configuration
shouldn't be hardcoded anywhere else

##############################
b) Configuration

has three classes TConfiguration, TInputSample, TAllInputSamples
and text configuration file config.txt

-- config.txt is location dependednt file (up to date for UNL tier 3 as it is)
the syntax rules are written in the file

--TConfiguration.h/.C - class TConfiguration. Contains locations and names of directories and files in the package. Also contains some constants (photon Pt binning information and unhidden blinding constants).

--TInputSample.h/.C - class TInputSample. A class for input sample structure (input file names, proper cross sections, colors for plots etc)

--TAllInputSamples.h/.C - class TAllInputSamples. Has a vector of TInputSample as a field. Can read information from config.txt. Has method Print() which prints information from the configuration files about all samples (data, sigMC, different bkg MCs). After significant modifications done in config.txt or in any class in Configuration directory, it's recommended to do Print() to make sure that reading from config.txt still properly works. Right now one can test it with
$ root -l runTest.C


##############################
c) Include

-- rootlogon.C - load all classes in the package. Does not load root macros which are not classes (e.g. "aux" files).  

-- TEventTree.h/.C - class TEventTree. The whole structure of input tree, data and MC. Variables, branches, SetBranchAddress(es), GetEntr(ies) etc. Constants of maximum possible nMu, nPho, nHLT etc in the input tree are listed here. If some tree has larger number - it has to be modified here.

-- TPhotonCuts, TElectronCuts, TMuonCuts - lists proper cuts and has cut functions. Contains kinematic, ID, isolation cuts, second lepton veto. TElectronCuts is not up to date since the electron channel is being analyzed with different frameworks.

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

##############################
h)PHOSPHOR_CORRECTION
-- PhosphorCorrectorFunctor.hh/.cc - class PhosphorCorrectionFunctor. Calculated corrected photon Et and En for data and MC. With minor modifications, was cloned from https://github.com/cmorgoth/PHOSPHOR_CORRECTION/
right now is switched off
