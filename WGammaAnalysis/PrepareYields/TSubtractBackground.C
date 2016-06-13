#include "../Configuration/TConfiguration.h"

#include "TSubtractBackground.h"

#include <iostream>
#include <iomanip>

TSubtractBackground::TSubtractBackground()
{
  _nDDsources=0;
}

TSubtractBackground::~TSubtractBackground()
{
  
}

void TSubtractBackground::Increase_nDDsources(){
  _nDDsources++;
}

void TSubtractBackground::SetYieldsDataDrivenEtoGamma(TString name, TString label, int color, TString fileName, TString strYieldsName1D[3], TString strYieldsNameTot[3])
{
  SetYieldsDataDriven(_sourceDDEtoGamma, name, label, color, fileName,  strYieldsName1D, strYieldsNameTot);
}

void TSubtractBackground::SetYieldsDataDrivenTrue(TString name, TString label, int color, TString fileName, TString strYieldsName1D[3], TString strYieldsNameTot[3])
{
  SetYieldsDataDriven(_sourceDDTrue[_nDDsources], name, label, color, fileName,  strYieldsName1D, strYieldsNameTot);
}

void TSubtractBackground::SetYieldsDataDrivenFake(TString name, TString label, int color, TString fileName, TString strYieldsName1D[3], TString strYieldsNameTot[3])
{
  SetYieldsDataDriven(_sourceDDFake[_nDDsources], name, label, color, fileName,  strYieldsName1D, strYieldsNameTot);
}

void TSubtractBackground::SetYieldsDataDriven(YieldsSource& source, TString name, TString label, int color, TString fileName, TString strYieldsName1D[3], TString strYieldsNameTot[3])
{

  _pyPars.fOut->cd(); 
  source.sourceType=DATA_DRIVEN;
  source.tr=0;
  source.name=name;
  source.label=label;
  source.color=color;
  TFile* f = new TFile(fileName);
  std::cout<<"TSubtractBackground::SetYieldsDataDriven: fileName: "<<fileName<<std::endl;
  if (!f->IsOpen()){
    std::cout<<"ERROR in TSubtractBackground::SetYieldsDataDriven: file "<<fileName<<" can't be open"<<std::endl;
    return;
  }

  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    source.hist[ieta]=(TH1F*)f->Get(strYieldsName1D[ieta]);
    for(int ib=1; ib<=source.hist[ieta]->GetNbinsX(); ib++){
      float cont=source.hist[ieta]->GetBinContent(ib);
      float err=source.hist[ieta]->GetBinError(ib);
//      source.hist[ieta]->SetBinContent(ib,cont*_pyPars.blindFraction);
//      source.hist[ieta]->SetBinError(ib,err*_pyPars.blindFraction);
    }
    source.hist[ieta]->SetFillColor(source.color);

    int linecolor=1;//black
    if (_nDDsources==1) linecolor=4;//blue
    if (_nDDsources==2) linecolor=2;//red
    if (_nDDsources==3) linecolor=3;//green
    source.hist[ieta]->SetLineColor(linecolor);

    TH1F* hTot = (TH1F*)f->Get(strYieldsNameTot[ieta]);
//    source.yieldTotVal[ieta]=hTot->GetBinContent(1)*_pyPars.blindFraction;
//    source.yieldTotErr[ieta]=hTot->GetBinError(1)*_pyPars.blindFraction;

  }// end of loop over ieta

}//end of SetYieldsDataDriven

void TSubtractBackground::SubtractBackground()
{
  std::cout<<"#####################"<<std::endl;
  std::cout<<"Subtract Background"<<std::endl;
  int is_data=-1;
  for (int isDD=0; isDD<_nDDsources; isDD++){
    for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
      _sourceBkgSubtrData[isDD].hist[ieta] = (TH1F*)_sourceDDTrue[isDD].hist[ieta]->Clone(_pyPars.strYieldsName1D_BkgSubtrData[ieta]);
      _sourceBkgSubtrData[isDD].yieldTotVal[ieta]=_sourceDDTrue[isDD].yieldTotVal[ieta];
      _sourceBkgSubtrData[isDD].yieldTotErr[ieta]=_sourceDDTrue[isDD].yieldTotErr[ieta]*_sourceDDTrue[isDD].yieldTotErr[ieta];
      _sourceBkgSubtrData[isDD].hist[ieta]->SetTitle(_pyPars.strYieldsName1D_BkgSubtrData[ieta]);
      _sourceBkgSubtrData[isDD].hist[ieta]->SetName(_pyPars.strYieldsName1D_BkgSubtrData[ieta]);
      int nBins = _sourceBkgSubtrData[isDD].hist[ieta]->GetNbinsX();
      for (int i=0; i<_sources.size(); i++){
        if (_sources[i].sourceType==DATA) is_data=i;;
        if (_sources[i].sourceType!=BKGMC_TRUE) continue;
        _sourceBkgSubtrData[isDD].hist[ieta]->Add(_sources[i].histBlind[ieta],-1);
        _sourceBkgSubtrData[isDD].yieldTotVal[ieta]-=_sources[i].yieldTotVal[ieta];
        _sourceBkgSubtrData[isDD].yieldTotErr[ieta]+=_sources[i].yieldTotErr[ieta]*_sources[i].yieldTotErr[ieta];
      }//end of loop over i
      if (_pyPars.doEtoGammaSubtr){ // for W_GAMMA ELECTRON only
        _sourceBkgSubtrData[isDD].hist[ieta]->Add(_sourceDDEtoGamma.hist[ieta],-1);
        _sourceBkgSubtrData[isDD].yieldTotVal[ieta]-=_sourceDDEtoGamma.yieldTotVal[ieta];
        _sourceBkgSubtrData[isDD].yieldTotErr[ieta]+=_sourceDDEtoGamma.yieldTotErr[ieta]*_sourceDDEtoGamma.yieldTotErr[ieta];
      }// end of if (_pyPars.doEtoGammaSubtr)
      _sourceBkgSubtrData[isDD].yieldTotErr[ieta]=sqrt(_sourceBkgSubtrData[isDD].yieldTotErr[ieta]);

//      // if yield<0, set yield=0
      /*
      std::cout<<std::endl;     
      std::cout<<"Error replaced by stat only. ieta="<<ieta<<std::endl;
      for (int ib=1; ib<=_sourceBkgSubtrData[isDD].hist[ieta]->GetNbinsX(); ib++){
        if (_sourceBkgSubtrData[isDD].hist[ieta]->GetBinContent(ib)<0)
          _sourceBkgSubtrData[isDD].hist[ieta]->SetBinContent(ib,0);
        if (is_data<0) continue;
        float err = _sources[is_data].histBlind[ieta]->GetBinError(ib);
        std::cout<<"ib="<<ib<<", nData = "<<_sources[is_data].histBlind[ieta]->GetBinContent(ib)<<"+-"<<_sources[is_data].histBlind[ieta]->GetBinError(ib)<<"; sqrt("<<_sources[is_data].histBlind[ieta]->GetBinContent(ib)<<")="<<sqrt(_sources[is_data].histBlind[ieta]->GetBinContent(ib))<<"; ";
        std::cout<<"nBkgSubtr = "<<_sourceBkgSubtrData[isDD].hist[ieta]->GetBinContent(ib)<<"+-"<<_sourceBkgSubtrData[isDD].hist[ieta]->GetBinError(ib)<<"; ";
        _sourceBkgSubtrData[isDD].hist[ieta]->SetBinError(ib,err);
        std::cout<<"nBkgSubtr becomes = "<<_sourceBkgSubtrData[isDD].hist[ieta]->GetBinContent(ib)<<"+-"<<_sourceBkgSubtrData[isDD].hist[ieta]->GetBinError(ib)<<"; ";
        std::cout<<std::endl;
      }//end of loop over ib   
      std::cout<<std::endl;   
      */
    }//end of loop over ieta
  }//end of loop over isDD

  std::cout<<"#####################"<<std::endl;
}// end of SubtractBackground

void TSubtractBackground::CompareFakeDDvsMC(int ieta)
{
  CompareDDvsMC(ieta, "Fake", BKGMC_FAKE, _sourceDDFake, _canvFakeDDvsMC[ieta]);
}// end of CompareFakeDDvsMC

void TSubtractBackground::CompareTrueDDvsMC(int ieta)
{
  CompareDDvsMC(ieta, "True", BKGMC_TRUE, _sourceDDTrue, _canvTrueDDvsMC[ieta]);
}// end of CompareTrueDDvsMC

void TSubtractBackground::CompareDDvsMC(int ieta, TString strDD, int bkgType, YieldsSource sourceDD[_nHistsMax], TCanvas* canv)
{

  _pyPars.fOut->cd(); 
  TString canvName=strDD+TString("DDvsMC_")+_pyPars.strPlotsBaseName;
  canvName+=StrLabelEta(ieta);
  canv= new TCanvas(canvName,canvName,1200,800);
  TLegend* legend = new TLegend(0.65,0.60,0.90,0.90);
  legend->SetFillColor(0);
  THStack* mcHists = new THStack("mcHistsTot",strDD+TString(" DD vs MC"));

  TH1F* hSum;
  bool hSumStarted=0;

  int isSign=-1;

  for (int is=0; is<_sources.size(); is++){
    if (_sources[is].sourceType==SIGMC) isSign=is;
    if (_sources[is].sourceType!=bkgType) continue;
    std::cout<<TString("Compare")+strDD+TString("DDvsMC: processing isource ")<<_sources[is].name<<std::endl;
    mcHists->Add(_sources[is].histBlind[ieta]);
    if (!hSumStarted){
      hSum=(TH1F*)_sources[is].histBlind[ieta]->Clone("hSum_WholeMC");
      hSumStarted=1;
    }
    else hSum->Add(_sources[is].histBlind[ieta]);
    legend->AddEntry(_sources[is].histBlind[ieta],_sources[is].label);
  }//end of loop over is

  if (isSign!=-1 && bkgType==BKGMC_TRUE) {
    mcHists->Add(_sources[isSign].histBlind[ieta]);  
    legend->AddEntry(_sources[isSign].histBlind[ieta],_sources[isSign].label);  
    if (!hSumStarted){
      hSum=(TH1F*)_sources[isSign].histBlind[ieta]->Clone("hSum_WholeMC");
      hSumStarted=1;
    }
    else hSum->Add(_sources[isSign].histBlind[ieta]);
  }

  TH1F* hists[_nDDsources];
  for (int ih=0; ih<_nDDsources; ih++){
    legend->AddEntry(sourceDD[ih].hist[ieta],sourceDD[ih].label,"l");
    sourceDD[ih].hist[ieta]->SetTitle("");
    hists[ih]=sourceDD[ih].hist[ieta];
  }

  CompareStackVsHist(strDD+TString(" DD vs MC"), _nDDsources, hists, hSum, legend, canv, 1, mcHists);

  canv->Write();

}// end of CompareDDvsMC

void TSubtractBackground::CompareDATAvsDDsum(int ieta)
{
  _pyPars.fOut->cd(); 
  TString canvName=TString("DATAvsDDsum")+_pyPars.strPlotsBaseName;
  canvName+=StrLabelEta(ieta);
  _canvDATAvsDDsum[ieta]= new TCanvas(canvName,canvName,1200,800);
  TLegend* legend = new TLegend(0.60,0.65,0.90,0.90);
  legend->SetFillColor(0);
  THStack* mcHists = new THStack("mcHistsTot",TString("DATAvsDDsum"));
  TH1F* hSum;

  mcHists->Add(_sourceDDFake[0].hist[ieta]);
  legend->AddEntry(_sourceDDFake[0].hist[ieta],_sourceDDFake[0].label,"f");
  hSum=(TH1F*)_sourceDDFake[0].hist[ieta]->Clone("hSum_WholeMC");

  mcHists->Add(_sourceDDTrue[0].hist[ieta]);
  legend->AddEntry(_sourceDDTrue[0].hist[ieta],_sourceDDTrue[0].label,"f");
  hSum->Add(_sourceDDTrue[0].hist[ieta]);

  int isData=-1;

  for (int is=0; is<_sources.size(); is++)
    if (_sources[is].sourceType==DATA) isData=is;

  if (isData==-1) return;
  legend->AddEntry(_sources[isData].hist[ieta],_sources[isData].label,"l");
  
  _sources[isData].hist[ieta]->SetTitle("");

  _sourceDDFake[0].hist[ieta]->SetLineColor(_sourceDDFake[0].color);
  _sourceDDTrue[0].hist[ieta]->SetLineColor(_sourceDDTrue[0].color);
  CompareStackVsHist(TString("DATA vs DD fake + true"), _sources[isData].hist[ieta], hSum, legend, _canvDATAvsDDsum[ieta], 1, mcHists);
  _sourceDDFake[0].hist[ieta]->SetLineColor(1);
  _sourceDDTrue[0].hist[ieta]->SetLineColor(1);

  _canvDATAvsDDsum[ieta]->Write();
}// end of CompareDATAvsDDsum

void TSubtractBackground::CompareDATAvsBKGplusSIGMC(int ieta)
{
  _pyPars.fOut->cd(); 
  TString canvName=TString("DATAvsBkgPlusSigMC")+_pyPars.strPlotsBaseName;
  canvName+=StrLabelEta(ieta);
  _canvDATAvsBKGplusSIGMC[ieta]= new TCanvas(canvName,canvName,1200,800);
  TLegend* legend = new TLegend(0.60,0.65,0.90,0.90);
  legend->SetFillColor(0);
  // prepare sum of real-photon bkg MC
  TH1F* hSumBkgMC;
  bool hSumStartedMC=0;
  int isSign=-1;
  for (int is=0; is<_sources.size(); is++){
    if (_sources[is].sourceType==SIGMC) isSign=is;
    if (_sources[is].sourceType!=BKGMC_TRUE) continue;
    std::cout<<TString("Compare DATAvsBkgPlusSigMC: processing isource ")<<_sources[is].name<<std::endl;
    if (!hSumStartedMC){
      hSumBkgMC=(TH1F*)_sources[is].histBlind[ieta]->Clone("hSum_WholeMC");
      hSumStartedMC=1;
    }
    else hSumBkgMC->Add(_sources[is].histBlind[ieta]);
    //legend->AddEntry(_sources[is].histBlind[ieta],_sources[is].label);
  }//end of loop over is

  // form THStack out of real bkg MC + fake DD + signal MC 

  THStack* mcHists = new THStack("mcHistsTot",TString("DATAvsBkgPlusSigMC"));
  TH1F* hSum;
  bool hSumStarted=0;

  // add real gamma MC background
  if (hSumStartedMC){
    hSumBkgMC->SetLineColor(_sourceDDTrue[0].color);
    hSumBkgMC->SetFillColor(_sourceDDTrue[0].color);
    mcHists->Add(hSumBkgMC);
    legend->AddEntry(hSumBkgMC,"real #gamma bkg MC","f");
    hSum=(TH1F*)hSumBkgMC->Clone("hSum_WholeBkgPlusMC");  
    hSumStarted=1;  
  }// end of if (hSumStartedMC)


  if (_pyPars.doEtoGammaSubtr){//W_GAMMA ELECTRON
  // add e->gamma DD background
    mcHists->Add(_sourceDDEtoGamma.hist[ieta]);
    legend->AddEntry(_sourceDDEtoGamma.hist[ieta],_sourceDDEtoGamma.label,"f");
    if (!hSumStarted){
      hSum=(TH1F*)_sourceDDEtoGamma.hist[ieta]->Clone("hSum_WholeBkgPlusMC");
      hSumStarted=1;
    }
    else hSum->Add(_sourceDDEtoGamma.hist[ieta]);
    hSumStarted=1;
  }//end of if (_pyPars.doEtoGammaSubtr)

  // add fake gamma DD background
  mcHists->Add(_sourceDDFake[0].hist[ieta]);
  legend->AddEntry(_sourceDDFake[0].hist[ieta],_sourceDDFake[0].label,"f");
  if (!hSumStarted){
    hSum=(TH1F*)_sourceDDFake[0].hist[ieta]->Clone("hSum_WholeBkgPlusMC");
    hSumStarted=1;
  }
  else hSum->Add(_sourceDDFake[0].hist[ieta]);
  hSumStarted=1;

  // add signal MC
  if (isSign!=-1) {
    mcHists->Add(_sources[isSign].histBlind[ieta]);  
    legend->AddEntry(_sources[isSign].histBlind[ieta],_sources[isSign].label);  
    if (!hSumStarted){
      hSum=(TH1F*)_sources[isSign].histBlind[ieta]->Clone("hSum_WholeMC");
      hSumStarted=1;
    }
    else hSum->Add(_sources[isSign].histBlind[ieta]);
  }

  int isData=-1;

  for (int is=0; is<_sources.size(); is++)
    if (_sources[is].sourceType==DATA) isData=is;

  if (isData==-1) return;
  legend->AddEntry(_sources[isData].hist[ieta],_sources[isData].label,"l");
  
  _sources[isData].hist[ieta]->SetTitle("");

  _sourceDDFake[0].hist[ieta]->SetLineColor(_sourceDDFake[0].color);

  CompareStackVsHist(TString("DATA vs (DD fake + MC real)"), _sources[isData].hist[ieta], hSum, legend, _canvDATAvsBKGplusSIGMC[ieta], 1, mcHists);

  _sourceDDFake[0].hist[ieta]->SetLineColor(1);

  _canvDATAvsBKGplusSIGMC[ieta]->Write();


}// end of CompareDATAvsBKGplusSIGMC

void TSubtractBackground::CompareBkgSubtrDATAvsSIGMC(int ieta)
{

  _pyPars.fOut->cd(); 
  TString canvName=TString("BkgSubtrDATAvsSIGMC_")+_pyPars.strPlotsBaseName;
  canvName+=StrLabelEta(ieta);
  _canvDATAvsBKGplusSIGMC[ieta]= new TCanvas(canvName,canvName,1200,800);
  TLegend* legend = new TLegend(0.60,0.75,0.90,0.90);
  legend->SetFillColor(0);
  THStack* mcHists = new THStack("mcHistsTot",TString("BkgSubtrDATAvsSIGMC"));

  TH1F* hSum;
  bool hSumStarted=0;

  int isSign=-1;

  for (int is=0; is<_sources.size(); is++){
    if (_sources[is].sourceType==SIGMC); 
    else continue;
    std::cout<<TString("Compare BkgSubtrDATA vs SIGMC: processing isource ")<<_sources[is].name<<std::endl;
    hSum=(TH1F*)_sources[is].histBlind[ieta]->Clone("hSum_WholeMC");
    hSumStarted=1;
    _sources[is].histBlind[ieta]->SetLineWidth(2);
    legend->AddEntry(_sources[is].histBlind[ieta],_sources[is].label,"l");
  }//end of loop over is

//  if (isSign!=-1 && bkgType==BKGMC_TRUE) {
//    mcHists->Add(_sources[isSign].histBlind[ieta]);  
//    legend->AddEntry(_sources[isSign].histBlind[ieta],_sources[isSign].label);  
//    if (!hSumStarted){
//      hSum=(TH1F*)_sources[isSign].histBlind[ieta]->Clone("hSum_WholeMC");
//      hSumStarted=1;
//    }
//    else hSum->Add(_sources[isSign].histBlind[ieta]);
//  }

  TH1F* hists[_nDDsources];
  for (int ih=0; ih<_nDDsources; ih++){
    TString label = _sourceDDTrue[ih].label;
    label.ReplaceAll("DD_true","DATA - BKG");
    legend->AddEntry(_sourceBkgSubtrData[ih].hist[ieta],label,"l");
    _sourceBkgSubtrData[ih].hist[ieta]->SetTitle("");
    hists[ih]=_sourceBkgSubtrData[ih].hist[ieta];
  }

  CompareStackVsHist(TString("BkgSubtrDATA vs SIGMC"), _nDDsources, hists, hSum, legend, _canvDATAvsBKGplusSIGMC[ieta]);

  _canvDATAvsBKGplusSIGMC[ieta]->Write();
}// end of CompareBkgSubtrDATAvsSIGMC

void TSubtractBackground::PlotPrintSave()
{

  std::cout<<"_nDDsources="<<_nDDsources<<std::endl;

  //Plot
  if (_nDDsources>1)  {
    for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
      CompareFakeDDvsMC(ieta);
      CompareTrueDDvsMC(ieta);
      CompareBkgSubtrDATAvsSIGMC(ieta);
    }
  }
  else {
    for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
      CompareTotalDATAvsMC(ieta);
      CompareFakeDDvsMC(ieta);
      CompareTrueDDvsMC(ieta);
      CompareDATAvsBKGplusSIGMC(ieta);
      CompareBkgSubtrDATAvsSIGMC(ieta);
      if (ieta!=_COMMON) CompareDATAvsDDsum(ieta);
    }
  }

 _pyPars.fOut->cd();

  //Print Yields and Write to output file
  std::cout<<std::endl;
  std::cout<<"Print Yields:"<<std::endl;
  std::cout<<std::endl;

  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    std::cout<<StrLabelEta(ieta)<<std::endl;
    for (int is=0; is<_sources.size(); is++){
      PrintYieldsOne(_sources[is].name+TString(": "), _sources[is].yieldTotVal[ieta], _sources[is].yieldTotErr[ieta], _sources[is].hist[ieta]);
      _sources[is].hist[ieta]->Write();
    }//end of loop over is
    for (int is=0; is<_nDDsources; is++){
      PrintYieldsOne(_sourceDDFake[is].name+TString(": "), _sourceDDFake[is].yieldTotVal[ieta], _sourceDDFake[is].yieldTotErr[ieta], _sourceDDFake[is].hist[ieta]);
      PrintYieldsOne(_sourceDDTrue[is].name+TString(": "), _sourceDDTrue[is].yieldTotVal[ieta], _sourceDDTrue[is].yieldTotErr[ieta], _sourceDDTrue[is].hist[ieta]);
      _sourceDDFake[is].hist[ieta]->Write();
      _sourceDDTrue[is].hist[ieta]->Write();
      if (_pyPars.doEtoGammaSubtr) {
        PrintYieldsOne(_sourceDDEtoGamma.name+TString(": "), _sourceDDEtoGamma.yieldTotVal[ieta], _sourceDDEtoGamma.yieldTotErr[ieta], _sourceDDEtoGamma.hist[ieta]);
	_sourceDDEtoGamma.hist[ieta]->Write();
      }
      PrintYieldsOne(_sourceBkgSubtrData[is].name+TString(": "), _sourceBkgSubtrData[is].yieldTotVal[ieta], _sourceBkgSubtrData[is].yieldTotErr[ieta], _sourceBkgSubtrData[is].hist[ieta]);
    }//end of loop over is   
    std::cout<<std::endl; 
  }//end of loop over ieta
  std::cout<<std::endl;

  int isSign=-1;
  for (int is=0; is<_sources.size(); is++){
    if (_sources[is].sourceType==SIGMC) isSign=is;
  }

  // Print Latex
  for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){
    std::cout<<StrLabelEta(ieta)<<std::endl;
    // real-g (if Wg) _sources[is]. (SUM)
    TH1F* hRealGammaBkg;
    bool isRealStarted=0;
    for (int is=0; is<_sources.size(); is++){
      if (_sources[is].sourceType!=BKGMC_TRUE) continue;
      if (_sources[is].name!="Zg" && _sources[is].name!="Wg_to_taunu" && _sources[is].name!="WWg") continue;
      if (!isRealStarted) {hRealGammaBkg=(TH1F*)_sources[is].hist[ieta]->Clone("hRealGammaBkg"+ieta); isRealStarted=1;}
      else hRealGammaBkg->Add(_sources[is].hist[ieta]);
    }// end of loop over is
    TH1F* hTotalCHISO; TH1F* hTotalSIHIH; 
    hTotalCHISO = (TH1F*)_sourceDDFake[0].hist[ieta]->Clone("hTotalCHISO"+ieta);
    if (_nDDsources>1) hTotalSIHIH = (TH1F*)_sourceDDFake[1].hist[ieta]->Clone("hTotalCHISO"+ieta);
    if (isSign!=-1) {hTotalCHISO->Add(_sources[isSign].hist[ieta]);  if (_nDDsources>1) hTotalSIHIH->Add(_sources[isSign].hist[ieta]);}
    if (_pyPars.doEtoGammaSubtr){hTotalCHISO->Add(_sourceDDEtoGamma.hist[ieta]);  if (_nDDsources>1) hTotalSIHIH->Add(_sourceDDEtoGamma.hist[ieta]);}
    if (isRealStarted) {hTotalCHISO->Add(hRealGammaBkg); if (_nDDsources>1) hTotalSIHIH->Add(hRealGammaBkg);}
    std::cout<<"\\begin{table}[h]"<<std::endl;
    std::cout<<"  \\scriptsize"<<std::endl;
    std::cout<<"  \\begin{center}"<<std::endl;
    std::cout<<"  \\caption{Data, signal and background yields. ";
    std::cout<<_sources[isSign].name<<" "<<StrLabelEta(ieta)<<"}"<<std::endl;
    std::cout<<"  \\begin{tabular}{|c|c|c|c|";
    if (_nDDsources>1) std::cout<<"c|c|";
    if (_pyPars.doEtoGammaSubtr) std::cout<<"c|";
    if (isRealStarted) std::cout<<"c|";
    std::cout<<"}"<<std::endl;
    std::cout<<"    bin & DD fake &";
    if (_nDDsources>1) std::cout<<" DD fake &"; // DD fake SIHIH
    if (_pyPars.doEtoGammaSubtr) std::cout<<" DD &"; // DD e->gamma
    if (isRealStarted) std::cout<<" MC real-\\gamma & ";// MC real-gamma bkg
    std::cout<<" SIGMC & bkg+sig & ";
    if (_nDDsources>1) std::cout<<" bkg+sig &";// bkg+sig SIHIH  
    std::cout<<" data \\\\ "<<std::endl;

    std::cout<<"    lims & CHISO &"; 
    if (_nDDsources>1) std::cout<<" SIHIH &";// DD fake SIHIH  
    if (_pyPars.doEtoGammaSubtr) std::cout<<"e\\rightarrow\\gamma &"; // DD e->gamma
    if (isRealStarted) std::cout<<" bkg & ";// MC real-gamma bkg  
    std::cout<<"("<<_sources[isSign].name<<") & CHISO & ";
    if (_nDDsources>1) std::cout<<" SIHIH &";// bkg+sig SIHIH  
    std::cout<<"\\\\ \\hline"<<std::endl;
    for (int ib=1; ib<= _sourceDDFake[0].hist[ieta]->GetNbinsX(); ib++){
      // bin
      std::cout<<_sourceDDFake[0].hist[ieta]->GetBinLowEdge(ib)<<"-"<<_sourceDDFake[0].hist[ieta]->GetBinLowEdge(ib)+_sourceDDFake[0].hist[ieta]->GetBinWidth(ib)<<" & ";
      // DD-fake-CHISO _sourceDDFake[is].yieldTotVal[ieta], _sourceDDFake[is].yieldTotErr[ieta], _sourceDDFake[is].hist[ieta]
      std::cout<<_sourceDDFake[0].hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<_sourceDDFake[0].hist[ieta]->GetBinError(ib)<<" & ";
      // DD-fake-SIHIH _sourceDDFake[is].yieldTotVal[ieta], _sourceDDFake[is].yieldTotErr[ieta], _sourceDDFake[is].hist[ieta]
      if (_nDDsources>1)
        {std::cout<<_sourceDDFake[1].hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<_sourceDDFake[1].hist[ieta]->GetBinError(ib)<<" & ";}
      // e->g ( if (_pyPars.doEtoGammaSubtr) ) _sourceDDEtoGamma.
      if (_pyPars.doEtoGammaSubtr)
        {std::cout<<_sourceDDEtoGamma.hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<_sourceDDEtoGamma.hist[ieta]->GetBinError(ib)<<" & ";}
      // real-g (if Wg) _sources[is]. (SUM)
      //      std::cout<<"isRealStarted="<<isRealStarted<<std::endl;
      if (isRealStarted)
        {std::cout<<hRealGammaBkg->GetBinContent(ib)<<"$\\pm$"<<hRealGammaBkg->GetBinError(ib)<<" & ";}
      // signal MC _sources[is].
      if (isSign!=-1)
        {std::cout<<_sources[isSign].hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<_sources[isSign].hist[ieta]->GetBinError(ib)<<" & ";}
      // sum CHISO SUM
      {std::cout<<hTotalCHISO->GetBinContent(ib)<<"$\\pm$"<<hTotalCHISO->GetBinError(ib)<<" & ";}
      // sum SIHIH SUM
      if (_nDDsources>1)
        {std::cout<<hTotalSIHIH->GetBinContent(ib)<<"$\\pm$"<<hTotalSIHIH->GetBinError(ib)<<" & ";}
      // data _sources[is].
      for (int is=0; is<_sources.size(); is++){
        if (_sources[is].sourceType!=DATA) continue;
        {std::cout<<_sources[is].hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<_sources[is].hist[ieta]->GetBinError(ib)<<" \\\\ \\hline ";}
      }// end of loop over is
      std::cout<<std::endl;
    }// end of loop over ib
    std::cout<<"  \\end{tabular}"<<std::endl;
    std::cout<<"  \\label{tab:systInPercentyields_";
    std::cout<<_sources[isSign].name<<"_"<<StrLabelEta(ieta);
    std::cout<<"}"<<std::endl;
    std::cout<<"  \\end{center}"<<std::endl;
    std::cout<<"\\end{table}"<<std::endl;
    std::cout<<std::endl; 
  }//end of loop over ieta
  std::cout<<std::endl;

  //Save Yields




  TH1F* hBkgSubtrDataTot[3];
  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    _sourceBkgSubtrData[0].hist[ieta]->Write();
      //let's say "0" corresponds to result of CHISO fits

    TString hTotName =  _pyPars.strYieldsNameTot_BkgSubtrData[ieta];
    float lowEdge =  _sourceBkgSubtrData[0].hist[ieta]->GetBinLowEdge(1);
    int nBins = _sourceBkgSubtrData[0].hist[ieta]->GetNbinsX();
    float upEdge = _sourceBkgSubtrData[0].hist[ieta]->GetBinLowEdge(nBins)+ _sourceBkgSubtrData[0].hist[ieta]->GetBinWidth(nBins);
    hBkgSubtrDataTot[ieta] = new TH1F(hTotName, hTotName, 1, lowEdge, upEdge);
    float totVal=0;
    float totErr=0;

    // write of yields total
    // very first bin is underflow (10-15 GeV)
    // need to start with 2nd bin (15-20 GeV)
    for (int ib=2; ib<=nBins; ib++){
      totVal+= _sourceBkgSubtrData[0].hist[ieta]->GetBinContent(ib);
      totErr+= _sourceBkgSubtrData[0].hist[ieta]->GetBinError(ib)*_sourceBkgSubtrData[0].hist[ieta]->GetBinError(ib);
    }// end of loop over ib
    totErr = sqrt(totErr);
    hBkgSubtrDataTot[ieta]->SetBinContent(1,totVal);
    hBkgSubtrDataTot[ieta]->SetBinError(1,totErr);
    hBkgSubtrDataTot[ieta]->Write();

  }// end of loop over ieta

  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    _sources[isSign].hist[ieta]->Write(_sources[isSign].strYieldsName1D[ieta]);
    _sources[isSign].histBlind[ieta]->Write(_sources[isSign].strYieldsName1D[ieta]);
  }
  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    _sigMCGenYields[ieta]->Print();
    _sigMCGenYields[ieta]->Write(_pyPars.strYieldsName1D_SignalMCGenBins[ieta]);
  }

  //Write syst error due to CHISO vs SIHIH fits
  TConfiguration conf;
  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    TString strSyst=conf.GetSystCHISOvsSIHIHname(conf.ONEDI, ieta);
    TH1F* hSystCHISOvsSIHIH1D = (TH1F*)_sourceBkgSubtrData[0].hist[ieta]->Clone(strSyst);
    hSystCHISOvsSIHIH1D->SetTitle(strSyst);

    TString strSystTot=conf.GetSystCHISOvsSIHIHname(conf.TOTAL, ieta);
    TH1F* hSystCHISOvsSIHIHTot = new TH1F(strSystTot,strSystTot,1,15.0,500.0);
    float spreadTot=0;
    std::cout<<"hSyst (CHISO-SIHIH); ieta="<<StrLabelEta(ieta)<<std::endl;
    for (int ib=1; ib<=hSystCHISOvsSIHIH1D->GetNbinsX(); ib++){
      hSystCHISOvsSIHIH1D->SetBinContent(ib,0);
      float spread;
      if (_nDDsources==1) spread=0;
      else spread=fabs(_sourceBkgSubtrData[0].hist[ieta]->GetBinContent(ib)-_sourceBkgSubtrData[1].hist[ieta]->GetBinContent(ib));
      hSystCHISOvsSIHIH1D->SetBinError(ib,spread);
      if (ib>1) spreadTot+=spread*spread; 
      std::cout<<"ib="<<ib<<", val="<<_sourceBkgSubtrData[0].hist[ieta]->GetBinContent(ib)<<"+-"<<_sourceBkgSubtrData[0].hist[ieta]->GetBinError(ib)<<", syst="<<hSystCHISOvsSIHIH1D->GetBinError(ib)<<std::endl;
    }//end of loop over ib
    spreadTot=sqrt(spreadTot);
    hSystCHISOvsSIHIH1D->Write();
    hSystCHISOvsSIHIHTot->SetBinContent(1,0);
    hSystCHISOvsSIHIHTot->SetBinError(1,spreadTot);
    hSystCHISOvsSIHIHTot->Write();
    delete hSystCHISOvsSIHIHTot;
  }//end of loop over ieta

  //Write syst error due to Zg and Wg->taunu stat and normalization
  int isZg=-1; int isWgtaunu=-1;
  for (int is=0; is<_sources.size(); is++){
    if(_sources[is].name=="Zg") isZg=is;
    if(_sources[is].name=="Wg_to_taunu") isWgtaunu=is;
  }//end of loop over is

  for (int ieta=_BARREL; ieta<=_COMMON; ieta++){
    TString strSyst=conf.GetSystBkgSubtrZgWgtaunu(conf.ONEDI, ieta);

    TH1F* hSystBkgSubtr1D = (TH1F*)_sourceBkgSubtrData[0].hist[ieta]->Clone(strSyst);
    hSystBkgSubtr1D->SetTitle(strSyst);

    TString strSystTot=conf.GetSystBkgSubtrZgWgtaunu(conf.TOTAL, ieta);
    TH1F* hSystBkgSubtrTot = new TH1F(strSystTot,strSystTot,1,15.0,500.0);
    float errTot=0;
    std::cout<<"hSyst BkgSubtr Zg and Wgtaunu; ieta="<<StrLabelEta(ieta)<<std::endl;
    for (int ib=1; ib<=hSystBkgSubtr1D->GetNbinsX(); ib++){
      float Err1 = 0;
      if (isZg>-1) Err1=2*0.046*_sources[isZg].hist[ieta]->GetBinContent(ib);
      float Err2 = 0;
      if (isWgtaunu>-1) Err2=2*0.2*_sources[isWgtaunu].hist[ieta]->GetBinContent(ib);     
      float Err3 = 0;
      if (isZg>-1) Err3=_sources[isZg].hist[ieta]->GetBinError(ib); 
      float Err4 = 0;
      if (isWgtaunu>-1) Err4=_sources[isWgtaunu].hist[ieta]->GetBinError(ib);
      float Err=sqrt(Err1*Err1+Err2*Err2+Err3*Err3+Err4*Err4);
      std::cout<<"ieta="<<ieta<<", ib="<<ib<<", Err1="<<Err1<<", Err2="<<Err2<<", Err3="<<Err3<<", Err4="<<Err4<<", Err="<<Err<<std::endl;
      hSystBkgSubtr1D->SetBinError(ib,Err);
      errTot+=Err*Err;
    }//end of loop over ib
    errTot=sqrt(errTot);
    hSystBkgSubtr1D->Write();
    hSystBkgSubtrTot->SetBinContent(1,0);
    hSystBkgSubtrTot->SetBinError(1,errTot);
    hSystBkgSubtrTot->Write();
    delete hSystBkgSubtrTot;
  }//end of loop over ieta

  std::cout<<"everything written to "<<_pyPars.fOut->GetName()<<std::endl;
}// end of PlotPrintSave()
