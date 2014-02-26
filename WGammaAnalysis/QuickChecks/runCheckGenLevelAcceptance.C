{

  TFile f("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/job_summer12_Wg_munu.root");
  f.cd("ggNtuplizer");
  TTree* tr = (TTree*)gDirectory->Get("EventTree");

  Int_t           nMC;
  Int_t           mcPID[50];   //[nMC]
  Float_t           mcPt[50];   //[nMC]
  Float_t           mcEt[50];   //[nMC]
  Float_t           mcEta[50];   //[nMC]

  TBranch        *b_nMC;   //!
  TBranch        *b_mcPID;   //!
  TBranch        *b_mcPt;   //!
  TBranch        *b_mcEt;   //!
  TBranch        *b_mcEta;   //!

  tr->SetBranchAddress("nMC", &nMC, &b_nMC);
  tr->SetBranchAddress("mcPID", mcPID, &b_mcPID);
  tr->SetBranchAddress("mcPt", mcPt, &b_mcPt);
  tr->SetBranchAddress("mcEt", mcEt, &b_mcEt);
  tr->SetBranchAddress("mcEta", mcEta, &b_mcEta);

  int nPhotons = 0;
  int nPhotonsAcc = 0;
  int nMuons = 0;
  int nMuonsAcc = 0;
  int nPhoMuAcc = 0;

  for (int entry=0; entry<tr->GetEntries(); entry++)
    {
      b_nMC->GetEntry(entry); 
      b_mcPID->GetEntry(entry);
      b_mcPt->GetEntry(entry);
      b_mcEt->GetEntry(entry);
      b_mcEta->GetEntry(entry);

      if (nMC==0){
        std::cout<<"no particles were generated in the event "<<enrty<<std::endl; 
        continue;
      }
      bool passed = 0;
      for (int iMC=0; iMC<nMC; iMC++)
        {

          if (mcPID[iMC]==22)
            {
              nPhotons++;
              if (mcEt[iMC]>8.2 && (mcEta[iMC]>-3 && mcEta[iMC]<3))
                { 
                  nPhotonsAcc++;
                  passed=1;
                }
            }     
          if ((mcPID[iMC]==13) || (mcPID[iMC]==-13))
            {
              nMuons++;
              if (mcPt[iMC]>5 && (mcEta[iMC]>-2.7 && mcEta[iMC]<2.7)) 
                {
                   nMuonsAcc++;
                   if (passed)
                     nPhoMuAcc++;
                }
            }    
        }
    }  
  std::cout<<"nMuons="<<nMuons<<", nMuonsAcc="<<nMuonsAcc<<", eff="<<1.0*nMuonsAcc/nMuons<<std::endl;
  std::cout<<"nPhotons="<<nPhotons<<", nPhotonsAcc="<<nPhotonsAcc<<", eff="<<1.0*nPhotonsAcc/nPhotons<<std::endl;
  std::cout<<"nEvents="<<tr->GetEntries()<<", nPhoMuAcc="<<nPhoMuAcc<<", eff="<<1.0*nPhoMuAcc/(tr->GetEntries())<<std::endl;

}
