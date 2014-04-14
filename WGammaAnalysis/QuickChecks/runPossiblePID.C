{
  TFile f("../WGammaOutput/MUON/FullySelected/selectedSIGMC.root");
  TTree* tr = (TTree*)f.Get("selectedEvents");
  int nMC;
  vector <int> *mcPID;
  vector <int> *mcMomPID;
  vector <int> *mcGMomPID;
  int min = tr->GetMinimum("phoGenMomPID");
  int max = tr->GetMaximum("phoGenMomPID");
  std::cout<<"number of photon moms of certain type:"<<std::endl;
  int PIDs[35]={-20213,-999,-323,-313,-213,-24,-15,-13,-6,-5,-4,-3,-2,-1,1,2,3,4,5,6,11,13,15,21,23,24,111,113,221,223,313,323,331,333};
  for (int i=0; i<35; i++){
    TString cutMomStr="phoGenMomPID==";
    cutMomStr+=PIDs[i];
    TCut cutMom(cutMomStr);
    TCut cutGamma="phoGenPID==22";
    if (tr->GetEntries(cutGamma&&cutMom)>0)
      std::cout<<PIDs[i]<<": "<<tr->GetEntries(cutGamma&&cutMom)<<std::endl;
  }
}
