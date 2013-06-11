####################
## WGammaAnalysis ##
####################

1) AcceptanceAndEfficiency
2) CertifiedConstants
3) Configuration
4) Include
5) QuickChecks
6) Selection
7) SkimSplitMerge
8) SplitWGammaMC
9) WGammaOutput
10)remove_dot_so_d_tilda_files.sh

##############################
1) AcceptanceAndEfficiency

has class TAcceptanceAndEfficiency
which calculates Acceptance times Efficiency

to run it do
$ root -l runAccAndEff.C

now, it calculates 
1. skim efficiency if any (it actually is calculated during skimming procedure, this script only extracts this values from historgam hskim; if there is no histogram hskim found in the file - skim efficiency must be 1)
2. acceptance within skim efficiency. Acceptance cuts are muon and photon kinematic cuts applied on generator level values of reconstructed muon and photon. So, the acceptance is only calculated if at least 1 reconstructed muon and 1 reconstructed photon exists. If no reconstructed muon-photon pair - acceptance fails, even for not skimmed file.
3. efficiency. Applies exactly all the same cuts as for Selection procedure. Uses function Include/TFullCuts::Cut(). This cut includes kinematic cuts for reconstructed values of muon and photon.

Improvement plans:
- add photon Pt binning
- store acc*eff values to .root file
- make plot (in this or separate class)

2013 June 7

##############################
2) CertifiedConstants

lists constants officially provided by CMS
containes 
- file with photon energy correction constants
pathes to files must be listed in the /Configuration
shouldn't be hardcoded anywhere else
- JSONfiles/ list of JSON files (which are not used anywhere in the package so far but are used for calculating luminocities and pileup distributions in data using CMSSW)
- Pileup/ pileup JSON file, script with line for calculation pileup distribution in data using CMSSW, root files with pileup distributions

2013 June 11

##############################
3) Configuration

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
4) Include

-- rootlogon.C - load all classes in the package. Does not load root macros which are not classes (e.g. "aux" files). Is recommended to use for "run" files in the directories. ("run" files are those which call main functions). Now, if one runs some class - all other classes in the package have to be compilable.

-- PhosphorCorrectorFunctor.hh/.cc - class PhosphorCorrectionFunctor. Calculated corrected photon Et and En for data and MC. With minor modifications, was copied from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CPena/src/PHOSPHOR_Corr_v2/

-- TEventTree.h/.C - class TEventTree. The whole structure of input tree, data and MC. Variables, branches, SetBranchAddress(es), GetEntr(ies) etc. Constants of maximum possible nMu, nPho, nHLT etc in the input tree are listed here. If some tree has larger number - it has to be modified here.

-- TPhotonCuts, TElectronCuts, TMuonCuts - lists proper cuts and has cut functions. Contains kinematic, ID, isolation cuts, second lepton veto. TElectronCuts is empty now, before actual work on electrons started.

-- TMetTools - class for MET corrections (MET smearing for MC now)

-- TFullCuts - class for full cuts. Function TFullCuts::Cut is used for both Selection and AcceptanceAndEfficiency

2013 June 11

##############################
5) QuickChecks

##############################
6) Selection

does selection and makes selection plots

##############################
7) SkimSplitMerge

does skimming (has problems with some files: ttjets, WWg, qcd_170to300)
does splitting of WGamma to munu, enu, taunu channels
potentially can easily do merging

##############################
8) SplitWGammaMC

empty directory; I was trying to rename this directory to SkimSplitMerge
So, I created directory SkimSplitMerge
but was not able to remove directory SplitWGammaMC from the CVS
now it still stays in the CVS but remains empty

##############################
9) WGammaOutput

the output root files and plots should be stored here
More precise pathes should be listed in /Configuration

##############################
10)remove_dot_so_d_tilda_files.sh

this file simply goes into directories and removes files
*.so *.d *~
now the names of the directories where to go are just hardcoded
into the file
Improvement may be achieved here if to do loop over directory names
in the current directory
