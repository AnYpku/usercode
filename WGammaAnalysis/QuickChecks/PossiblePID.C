

void PossiblePID()
{
  int nInputs=8;
  TString fileNames[nInputs];
  fileNames[0]="../WGammaOutput/MUON/FullySelected/selectedSIGMC.root";
  fileNames[1]="../WGammaOutput/MUON/FullySelected/selectedBKGMC_Wjets_to_lnu.root";
  fileNames[2]="../WGammaOutput/MUON/FullySelected/selectedBKGMC_Wg_to_taunu.root";
  fileNames[3]="../WGammaOutput/MUON/FullySelected/selectedBKGMC_DYjets_to_ll.root";
  fileNames[4]="../WGammaOutput/MUON/FullySelected/selectedBKGMC_Zg.root";
  fileNames[5]="../WGammaOutput/MUON/FullySelected/selectedBKGMC_ttbarjets.root";
  fileNames[6]="../WGammaOutput/MUON/FullySelected/selectedBKGMC_ttbarg.root";
  fileNames[7]="../WGammaOutput/MUON/FullySelected/selectedBKGMC_multibosons.root";

  TTree* tr;
  int nPIDs=23;
  int PIDs[nPIDs]={1,2,3,4,5,6,11,13,15,21,23,24,111,113,213,221,223,313,323,331,333,999,20213};
  TCut cutPID[nPIDs];
  TCut cutPIDAny="0";
  for (int i=0; i<nPIDs; i++){
    TString cutMomStr="phoGenMomPID==";
    cutMomStr+=i;
    cutMomStr+=" || phoGenMomPID==";
    cutMomStr+=-i;
    cutPID[nPIDs]=cutMomStr;
    cutPIDAny=cutPIDAny || cutcutPID[nPIDs];
  }
  
}
