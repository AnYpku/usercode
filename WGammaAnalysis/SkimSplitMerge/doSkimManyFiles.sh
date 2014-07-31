#!/bin/bash

inputDirName=/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/
skimmedDirName=/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/MuPhoSkim/
rootFileDirName=ggNtuplizer
treeName=EventTree

echo ${inputDirName}
echo ${skimmedDirName}

nameRoot=.root
nameMuPhoSkim=_MuPhoSkim

fileName1=job_summer12_DYToLL
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}

fileName1=job_summer12_ttg
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}

fileName1=job_summer12_ttjets_1l
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}

fileName1=job_summer12_ttjets_2l
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}

fileName1=job_summer12_Wg_taunu
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}

fileName1=job_summer12_WW_2l2nu
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}

fileName1=job_summer12_WWg
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}

fileName1=job_summer12_WZ_2l2q
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}

fileName1=job_summer12_WZ_3lnu
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}
