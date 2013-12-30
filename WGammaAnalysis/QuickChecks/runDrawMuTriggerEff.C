{
  gStyle->SetCanvasColor(kWhite); 
  TFile fMC("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Wg_munu.root");
  fMC.cd("ggNtuplizer");
  TTree* trMC = (TTree*) gDirectory->Get("EventTree");
  trMC->SetLineColor(2);//red
  trMC->SetLineWidth(2);
  trMC->Draw("muTrg>0:muPt","muPt>20 && muPt<30","profile");
}
