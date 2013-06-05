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

calculates Acceptance times Efficiency

##############################
2) CertifiedConstants

lists constants officially provided by CMS
containes 
- list of JSON files (which are not used anywhere in the package so far)
- file with photon energy correction constants
pathes to files must be listed in the /Configuration
shouldn't be hardcoded anywhere else

##############################
3) Configuration

##############################
4) Include

##############################
5) QuickChecks

##############################
6) Selection

does selection and makes selection plots

##############################
7) SkimSplitMerge

does skimming and splitting of WGamma to munu, enu, taunu channels
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
more precise pathes should be listed in /Configuration

##############################
10)remove_dot_so_d_tilda_files.sh

this file simply goes into directories and removes files
*.so *.d *~
now the names of the directories where to go are just hardcoded
into the file
Improvement may be achieved here if to do loop over directory names
in the current directory
