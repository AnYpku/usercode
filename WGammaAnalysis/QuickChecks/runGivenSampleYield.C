{
  TFile f("../WGammaOutput/MUON/FullySelected/selectedBKGMC_Zg.root");
  TTree* tr = (TTree*)f.Get("selectedEvents");
  int min = tr->GetMinimum("inputFileNumber");
  int max = tr->GetMaximum("inputFileNumber");
  TH1F* hist = new TH1F("hist","hist",1,-1,100);
  TCut cutTrueGamma="phoGenPID==22";
  TCut cutFakeGamma="phoGenPID==-1000";
  TCut cutMomIsLepton="phoGenMomPID==13 || phoGenMomPID==-13 || phoGenMomPID==11 || phoGenMomPID==-11";
  TCut cutMomIsGaugeBoson="phoGenMomPID==21 || phoGenMomPID==23 || phoGenMomPID==24 || phoGenMomPID==-24";
  TCut cutMomIsQuark="phoGenMomPID>-6 && phoGenMomPID<6 && phoGenMomPID!=0";

  TCut cutTrueRoberta=cutTrueGamma&&(cutMomIsLepton||cutMomIsGaugeBoson||cutMomIsQuark);

  TCut cutWeight="weight";
  for (int i=min; i<=max;i++){
    TString str = "(inputFileNumber==";
    str+=i;
    str+=")";
    TCut cutSample(str);
    std::cout<<"inputfileN="<<i;
    tr->Draw("inputFileNumber>>hist",cutSample*cutWeight,"goff");
    float y = hist->GetSumOfWeights();
    std::cout<<", yield="<<y;
    tr->Draw("inputFileNumber>>hist",(cutSample&&cutTrueGamma)*cutWeight,"goff");
    float y = hist->GetSumOfWeights();
    std::cout<<", true="<<y;
    tr->Draw("inputFileNumber>>hist",(cutSample&&cutFakeGamma)*cutWeight,"goff");
    float y = hist->GetSumOfWeights();
    std::cout<<", fake="<<y;
    tr->Draw("inputFileNumber>>hist",(cutSample&&cutTrueRoberta)*cutWeight,"goff");
    float y = hist->GetSumOfWeights();
    std::cout<<", true Roberta="<<y;
    std::cout<<std::endl;
  }
}
