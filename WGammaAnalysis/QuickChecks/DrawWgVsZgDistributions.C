#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TH1F.h"

void DrawWgVsZgDistributions()
{
  TFile* fWg=new TFile("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Wg.root");
  TFile* fWj=new TFile("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Wjets.root");
  TFile* fZg=new TFile("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_Zg.root");
  TFile* fZj=new TFile("/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMCsummer12New/job_summer12_DYToLL.root");

  fWg->cd("ggNtuplizer");
  TTree* trWg = (TTree*)gDirectory->Get("EventTree");  

  fWj->cd("ggNtuplizer");
  TTree* trWj = (TTree*)gDirectory->Get("EventTree");  

  fZg->cd("ggNtuplizer");
  TTree* trZg = (TTree*)gDirectory->Get("EventTree");  

  fZj->cd("ggNtuplizer");
  TTree* trZj = (TTree*)gDirectory->Get("EventTree"); 

  

  TCut cut = "phoSCEta[0]>-1.4442 && phoSCEta[0]<1.4442 && phoEt[0]>15 && phoEt[0]<20 && mcDecayType[0]==3";
  TCut cutW="nMu>=1";
  TCut cutZ="nMu>=2";
  TCut cutTrue="phoGenIndex[0]!=-1";

  TH1F* hWgRandCone = new TH1F("hWgRandCone","hWgRandCone",7,-1,6); 
  trWg->Draw("phoRandConeChIso[0]>>hWgRandCone",cut && cutW && cutTrue,"goff");

  TH1F* hWgSCR = new TH1F("hWgSCR","hWgSCR",7,-1,6);
  trWg->Draw("phoSCRChIso[0]>>hWgSCR",cut && cutW && cutTrue,"goff");

  TH1F* hWjRandCone = new TH1F("hWjRandCone","hWjRandCone",7,-1,6);
  trWj->Draw("phoRandConeChIso[0]>>hWjRandCone",cut && cutW,"goff");

  TH1F* hZgRandCone = new TH1F("hZgRandCone","hZgRandCone",7,-1,6); 
  trZg->Draw("phoRandConeChIso[0]>>hZgRandCone",cut && cutZ && cutTrue,"goff");

  TH1F* hZgSCR = new TH1F("hZgSCR","hZgSCR",7,-1,6);
  trZg->Draw("phoRandConeChIso[0]>>hZgSCR",cut && cutZ && cutTrue,"goff");

  TH1F* hZjRandCone = new TH1F("hZjRandCone","hZjRandCone",7,-1,6);  
  trZg->Draw("phoRandConeChIso[0]>>hZjRandCone",cut && cutZ,"goff");

  TCanvas* cW = new TCanvas("cW","cW");
  hWgRandCone->SetLineColor(2); 
  hWgRandCone->DrawNormalized();  
  hWgSCR->SetLineColor(4);
  hWgSCR->DrawNormalized("same"); 
  hWjRandCone->DrawNormalized("same"); 

  TCanvas* cZ = new TCanvas("cZ","cZ");
  hZgRandCone->SetLineColor(2); 
  hZgRandCone->DrawNormalized(); 
  hZgSCR->SetLineColor(4); 
  hZgSCR->DrawNormalized("same"); 
  hZjRandCone->DrawNormalized("same"); 
}
