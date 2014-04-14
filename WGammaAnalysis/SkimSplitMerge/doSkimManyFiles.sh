#!/bin/bash

inputDirName=/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/
skimmedDirName=/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/MuPhoSkimmed/
rootFileDirName=ggNtuplizer
treeName=EventTree

echo ${inputDirName}
echo ${skimmedDirName}

nameRoot=.root
nameMuPhoSkim=_MuPhoSkim

fileName1=job_summer12_ZZ_2e2tau
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}

fileName1=job_summer12_ZZ_2l2nu
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}

fileName1=job_summer12_ZZ_2mu2tau
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}

fileName1=job_summer12_ZZ_2q2nu
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}

fileName1=job_summer12_ZZ_4mu
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}

fileName1=job_summer12_ZZ_4e
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}

fileName1=job_summer12_ZZ_4tau
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}

fileName1=job_summer12_ZZZ
fileNameFull1=${inputDirName}${fileName1}${nameRoot}
fileNameSkimmed1=${fileName1}${nameMuPhoSkim}${nameRoot}
root -l -b -q auxSkim.C+\(\"${fileNameFull1}\"\)
mv ${fileNameSkimmed1} ${skimmedDirName}
