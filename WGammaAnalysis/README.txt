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

##############################
2) Selection
- performs event selection cuts
- selection is performed in three steps:
  WGammaSelection::LoopOverTree() - does VeryPreliminary selection
  ExtraSelection.C - does Preliminary (also applies WMt cut - but no such cut introduced)
                     and  Fully (applies phoSigmaIEtaIEta and phoChIso cuts)
- needs results of SkimSplitMerge (WGamma splitter)

##############################
3) DDBkgTemplateMethod
- estimates background using data driven template method
- needs results of Selection

##############################
4) PrepareYields
- estimates weighted selected yields and subtracts background
- needs results of Selection and DDBkgTemplateMethod
- now nothing matches

##############################
5) AcceptanceAndEfficiency
- calculates acceptance and efficiency
- must apply phoSigmaIEtaIEta and phoChIso cuts (TFullCuts::ExtraCut)
- needs results of SkimSplitMerge (WGamma splitter)
- is commented in Include/rootlogon.C

##############################
6) Unfolding
- performes unfolding using RooUnfold
- needs results of SkimSplitMerge (WGamma splitter) and Selection
- the plan is to do unfolding on events which are in acceptance but before the selection cuts applied

##############################
7) CrossSection
- calculates cross section
- needs results of PrepareYields, AcceptanceAndEfficiency, Unfolding
- is commented in Include/rootlogon.C now



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

--TConfiguration.h/.C - class TConfiguration. Contains locations and names of directories and files in the package. Also contains some constants (photon Pt binning information). Access to all names is possible through "getters".

--TInputSample.h/.C - class TInputSample. A class for input sample structure (input file names, proper cross sections, colors for plots etc)

--TAllInputSamples.h/.C - class TAllInputSamples. Has a vector of TInputSample as a field. Can read information from config.txt. Has method Print() which prints information from the configuration files about all samples (data, sigMC, different bkg MCs). After significant modifications done in config.txt or in any class in Configuration directory, it's recommended to do Print() to make sure that reading from config.txt still properly works. Right now one can test it with
$ root -l runTest.C

2013 June 11

##############################
c) Include

-- rootlogon.C - load all classes in the package. Does not load root macros which are not classes (e.g. "aux" files). Is recommended to use for "run" files in the directories. ("run" files are those which call main functions). Now, if one runs some class - all other classes in the package have to be compilable.

-- PhosphorCorrectorFunctor.hh/.cc - class PhosphorCorrectionFunctor. Calculated corrected photon Et and En for data and MC. With minor modifications, was copied from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/

-- TEventTree.h/.C - class TEventTree. The whole structure of input tree, data and MC. Variables, branches, SetBranchAddress(es), GetEntr(ies) etc. Constants of maximum possible nMu, nPho, nHLT etc in the input tree are listed here. If some tree has larger number - it has to be modified here.

-- TPhotonCuts, TElectronCuts, TMuonCuts - lists proper cuts and has cut functions. Contains kinematic, ID, isolation cuts, second lepton veto. TElectronCuts is empty now, before actual work on electrons started.

-- TMetTools - class for MET corrections (MET smearing for MC now)

-- TFullCuts - class for full cuts. Function TFullCuts::Cut is used for both Selection and AcceptanceAndEfficiency

2013 June 11

##############################
d) QuickChecks

##############################
e) WGammaOutput

the output root files and plots should be stored here
More precise pathes should be listed in /Configuration

##############################
f)remove_dot_so_d_tilda_files.sh

this file simply goes into directories and removes files
*.so *.d *~
now the names of the directories where to go are just hardcoded
into the file
Improvement may be achieved here if to do loop over directory names
in the current directory

##############################
8) SplitWGammaMC

empty directory; I was trying to rename this directory to SkimSplitMerge
So, I created directory SkimSplitMerge
but was not able to remove directory SplitWGammaMC from the CVS
now it still stays in the CVS but remains empty

##############################
and more...
