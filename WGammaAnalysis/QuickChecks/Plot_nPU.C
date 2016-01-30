#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

void Plot_nPU()
{
  TFile* fZg = new TFile("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Zg.root");
  TFile* fZj = new TFile("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_DYToLL.root");
  TFile* fData = new TFile("../CertifiedConstants/Pileup/Pileup_Observed_69300.root");

  TH1D* h_PU_Data = (TH1D*)fData->Get("pileup");
  fZg->cd("ggNtuplizer");
  TH1D* h_PU_Zg = (TH1D*)gDirectory->Get("hPU");
  fZj->cd("ggNtuplizer");
  TH1D* h_PU_Zj = (TH1D*)gDirectory->Get("hPU");

  h_PU_Data->SetLineColor(1);
  h_PU_Zg->SetLineColor(2);
  h_PU_Zj->SetLineColor(3);

  h_PU_Data->DrawNormalized();
  h_PU_Zg->DrawNormalized("same");
  h_PU_Zj->DrawNormalized("same");

}// end of Plot_nPU
