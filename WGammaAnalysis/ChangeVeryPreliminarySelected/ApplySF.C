#include "../Selection/TSelectedEventsTree.h"
#include "../Configuration/TConfiguration.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Include/TScaleFactors.h"

#include "TTree.h"
#include "TFile.h"
#include "TString.h"

#include <iostream>


void ApplySF(float errCoef, int channel, int vgamma, int sample, TString sourceName)
{
  // errCoef: 0, -1, -1
  TConfiguration conf;
  TFile* f = new TFile(conf.GetSelectedName(conf.VERY_PRELIMINARY, channel, vgamma, conf.UNBLIND, sample, sourceName),"UPDATE");
  if (!f->IsOpen()) {
    std::cout<<"file" <<conf.GetSelectedName(conf.VERY_PRELIMINARY, channel, vgamma, conf.UNBLIND, sample, sourceName)<< "isn't open"<<std::endl;
    return;
  }
  TTree* trInput = (TTree*)f->Get("selectedEvents");
  TTree* trOutput = new TTree("selectedEventsNew","selected events new");
  TSelectedEventsTree selEvTree;
  selEvTree.SetAsInputTree(trInput);
  selEvTree.SetAsOutputTree(trOutput);
  long nEntries = trInput->GetEntries();
  float weight_old, phoSF_old, lep1SF_old, lep2SF_old, weight_new, phoSF_new, lep1SF_new, lep2SF_new;

  TScaleFactors sf;

  for (long ientry=0; ientry<nEntries; ientry++){

    trInput->GetEntry(ientry);
    if ((ientry%50000)==0) std::cout<<"ientry="<<ientry<<std::endl;

    weight_old = selEvTree._weight;
    phoSF_old = selEvTree._phoSF;
    lep1SF_old = selEvTree._lepSF[0];
    lep2SF_old = selEvTree._lepSF[1];

//  _phoSF=1.0; _lepSF[0]=1.0; _lepSF[1]=1.0;

    phoSF_new = sf.SF_MediumPhoID(selEvTree._phoEt,selEvTree._phoSCEta)+errCoef*sf.SF_Err_MediumPhoID(selEvTree._phoEt,selEvTree._phoSCEta);
    if (channel==conf.ELECTRON) phoSF_new*=(sf.SF_PixelSeedVeto(selEvTree._phoEt,selEvTree._phoSCEta)+errCoef*sf.SF_Err_PixelSeedVeto(selEvTree._phoEt,selEvTree._phoSCEta));
    if (channel==conf.MUON) lep1SF_new=(sf.SF_MuonIso(selEvTree._lepPt[0],selEvTree._lepEta[0])+errCoef*sf.SF_Err_MuonIso(selEvTree._lepPt[0],selEvTree._lepEta[0]))*(sf.SF_MuonID(selEvTree._lepPt[0],selEvTree._lepEta[0])+errCoef*sf.SF_Err_MuonID(selEvTree._lepPt[0],selEvTree._lepEta[0]));
    if (channel==conf.ELECTRON) lep1SF_new=(sf.SF_TightEleID(selEvTree._lepPt[0],selEvTree._lepSCEta[0])+errCoef*sf.SF_Err_TightEleID(selEvTree._lepPt[0],selEvTree._lepSCEta[0]));
    if (vgamma==conf.Z_GAMMA){// means Z_GAMMA
      if (channel==conf.MUON) lep2SF_new=(sf.SF_MuonIso(selEvTree._lepPt[1],selEvTree._lepEta[1])+errCoef*sf.SF_Err_MuonIso(selEvTree._lepPt[1],selEvTree._lepEta[1]))*(sf.SF_MuonID(selEvTree._lepPt[1],selEvTree._lepEta[1])+errCoef*sf.SF_Err_MuonID(selEvTree._lepPt[1],selEvTree._lepEta[1]));
      if (channel==conf.ELECTRON) lep2SF_new=sf.SF_TightEleID(selEvTree._lepPt[1],selEvTree._lepSCEta[1])+errCoef*sf.SF_Err_TightEleID(selEvTree._lepPt[1],selEvTree._lepSCEta[1]);    
    } 
    else lep2SF_new=lep2SF_old;

    weight_new = weight_old * (phoSF_new * lep1SF_new * lep2SF_new) / (phoSF_old * lep1SF_old * lep2SF_old);

    selEvTree._weight = weight_new;
    selEvTree._phoSF = phoSF_new;
    selEvTree._lepSF[0] = lep1SF_new;
    selEvTree._lepSF[1] = lep2SF_new;

    trOutput->Fill();

  }//end of loop over ientry

  trInput->SetName("selectedEventsOld");
  f->Delete("selectedEventsOld");
  trOutput->SetName("selectedEvents");
  trOutput->Write();
}//end of ChangePUweight

void ApplySF_Files(float errCoef, int channel, int vgamma, std::string configfile)
{
  TConfiguration conf;
  TAllInputSamples INPUT(channel, vgamma, configfile);
  int nSources = INPUT.nSources_; 
  for (int iSource=0; iSource<nSources; iSource++){
       int sample = INPUT.allInputs_[iSource].sample_;
       if (sample==conf.DATA) continue;
       if (sample==conf.SIGMC_UNSKIMMED) continue;
       std::cout<<"channel="<<channel<<"("<<conf.StrChannel(channel)<<")";
       std::cout<<", vgamma="<<vgamma<<"("<<conf.StrVgType(vgamma)<<")"<<std::endl;
       std::cout<<"sample="<<sample<<"("<<conf.StrSample(sample)<<")";
       std::cout<<", sourceName="<<INPUT.allInputs_[iSource].sourceName_<<std::endl;
       ApplySF(errCoef, channel, vgamma, INPUT.allInputs_[iSource].sample_,INPUT.allInputs_[iSource].sourceName_);
  } //loop over iSource ends
}//end of LoopOverVeryPreliminarySelectedFiles
