cd /home/hep/eavdeeva2/releases/CMSSW_5_0_1/src/RecoLuminosity/LumiDB/scripts

cmsenv

pileupCalc.py -i /home/hep/eavdeeva2/WGamma/UserCode/eavdeeva/WGammaAnalysis/CertifiedConstants/JSONfiles/Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON_v2.txt --inputLumiJSON /home/hep/eavdeeva2/WGamma/UserCode/eavdeeva/WGammaAnalysis/CertifiedConstants/Pileup/pileup_latest.txt --calcMode observed --minBiasXsec 69400 --maxPileupBin 50 --numPileupBins 50 /home/hep/eavdeeva2/WGamma/UserCode/eavdeeva/WGammaAnalysis/CertifiedConstants/Pileup/pileup_Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON_v2.root

cd /home/hep/eavdeeva2/WGamma/UserCode/eavdeeva/WGammaAnalysis/CertifiedConstants/Pileup


