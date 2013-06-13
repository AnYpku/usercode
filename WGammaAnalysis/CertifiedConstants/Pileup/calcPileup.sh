cd /home/hep/eavdeeva2/releases/CMSSW_5_0_1/src/RecoLuminosity/LumiDB/scripts

cmsenv

pileupCalc.py -i /home/hep/eavdeeva2/WGamma/UserCode/eavdeeva/WGammaAnalysis/CertifiedConstants/JSONfiles/Total_8TeV_2012_JSON.txt --inputLumiJSON /home/hep/eavdeeva2/WGamma/UserCode/eavdeeva/WGammaAnalysis/CertifiedConstants/Pileup/pileup_latest.txt --calcMode observed --minBiasXsec 69400 --maxPileupBin 50 --numPileupBins 50 /home/hep/eavdeeva2/WGamma/UserCode/eavdeeva/WGammaAnalysis/CertifiedConstants/Pileup/pileup_Total_8TeV_2012_JSON.root

cd /home/hep/eavdeeva2/WGamma/UserCode/eavdeeva/WGammaAnalysis/CertifiedConstants/Pileup


