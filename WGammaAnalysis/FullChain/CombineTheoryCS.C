#include "TFile.h"
#include "TH1F.h"
#include "TString.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLatex.h"

#include "../Configuration/TConfiguration.h"

#include <iostream>

void CombineTheoryCS(int vgamma)
{

  TFile* fCS[2]; //MUON, ELECTRON
  TH1F* hTheory1D[2];
  TH1F* hTheoryTot[2];

  TConfiguration config;
  TFile* fOut=new TFile("fOut.root","recreate");
 
  for (int ich=config.MUON; ich<=config.ELECTRON; ich++){
    fCS[ich]=new TFile(config.GetAccXEffFileName(ich, vgamma));
    hTheory1D[ich]=(TH1F*)fCS[ich]->Get(config.GetTheoryCSname(config.ONEDI));
    hTheoryTot[ich]=(TH1F*)fCS[ich]->Get(config.GetTheoryCSname(config.TOTAL));
  }//end of loop over ich

  TH1F* hTheoryCombine1D = (TH1F*)hTheory1D[0]->Clone();
  hTheoryCombine1D->Add(hTheory1D[1]);
  TH1F* hTheoryCombineTot = (TH1F*)hTheoryTot[0]->Clone();
  hTheoryCombineTot->Add(hTheoryTot[1]);

  for (int ib=1; ib<=hTheoryCombineTot->GetNbinsX(); ib++){
    float cont = hTheoryCombineTot->GetBinContent(ib);
    float err = hTheoryCombineTot->GetBinError(ib);
    hTheoryCombineTot->SetBinContent(ib,0.5*cont);
    hTheoryCombineTot->SetBinError(ib,0.5*err);
    std::cout<<"ib="<<ib<<", cs0="<< hTheoryTot[0]->GetBinContent(ib)<<"+-"<<hTheoryTot[0]->GetBinError(ib)<<", cs1="<<hTheoryTot[1]->GetBinContent(ib)<<"+-"<<hTheoryTot[1]->GetBinError(ib)<<", csComb="<<0.5*cont<<"+-"<<0.5*err<<std::endl;
  }//end of for (int ib=1...

  for (int ib=1; ib<=hTheoryCombine1D->GetNbinsX(); ib++){
    float cont = hTheoryCombine1D->GetBinContent(ib);
    float err = hTheoryCombine1D->GetBinError(ib);
    hTheoryCombine1D->SetBinContent(ib,0.5*cont);
    hTheoryCombine1D->SetBinError(ib,0.5*err);
    std::cout<<"ib="<<ib<<", cs0="<< hTheory1D[0]->GetBinContent(ib)<<"+-"<<hTheory1D[0]->GetBinError(ib)<<", cs1="<<hTheory1D[1]->GetBinContent(ib)<<"+-"<<hTheory1D[1]->GetBinError(ib)<<", csComb="<<0.5*cont<<"+-"<<0.5*err<<std::endl;
  }//end of for (int ib=1...
  
  TFile* fComb = new TFile(config.GetAccXEffFileName(2, vgamma),"recreate");
  hTheoryCombine1D->Write();
  hTheoryCombineTot->Write();

}//end of CombineTheoryCS
