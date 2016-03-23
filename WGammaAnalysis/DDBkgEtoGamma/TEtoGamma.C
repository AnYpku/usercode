#include "TEtoGamma.h"
  //this class

#include "../Configuration/TConfiguration.h"
#include "../Include/TPhotonCuts.h"
#include "../Include/TFullCuts.h"
#include "../Include/TMathTools.h"

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TCut.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TVectorD.h"
#include "TLine.h"
#include "TLatex.h"

#include "RooRealVar.h"
#include "RooArgList.h"
#include "RooArgSet.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooBreitWigner.h"
#include "RooAddPdf.h"
#include "RooFormulaVar.h"
#include "RooCBShape.h"
#include "RooFFTConvPdf.h"
#include "RooExponential.h"
#include "RooPlot.h"
using namespace RooFit;

TEtoGamma::TEtoGamma(TConfiguration::AnalysisParameters &anPars, bool isMCclosure)
{

  _isMCclosure=isMCclosure;
  _cutAdd=anPars.cutAdd;
  _varKin=anPars.varKin;
  TString strFout = _conf.GetDDBkgEtoGammaFileName(anPars.varKin);
  if (isMCclosure) strFout.ReplaceAll(".root","_MCclosure.root");
  _fOut=new TFile(strFout,"recreate");
  _nKinBins=anPars.nKinBins;
  std::cout<<"_fOut="<<strFout<<std::endl;
  std::cout<<"_varKin="<<_varKin<<std::endl;
  std::cout<<"_nKinBins="<<_nKinBins<<", _kinBinLims: ";
  for (int ik=0; ik<=_nKinBins; ik++){
    _kinBinLims[ik]=anPars.kinBinLims[ik];
    std::cout<<_kinBinLims[ik];
    if (ik<_nKinBins) std::cout<<", ";
  }// end of loop over ik
  std::cout<<std::endl;



}// end of   TEtoGamma::TEtoGamma(TConfiguration::AnalysisParameters &anPars) 

TEtoGamma::~TEtoGamma()
{
}

void TEtoGamma::ComputePlotSave()
{

  SetYields(_ZJETS_EtoGAMMA_ENR);
  SetYields(_ZJETS_NOM);   
  SetYields(_SIGMC_NOM);  

  SetYields(_SIGMC_ENR);
  SetYields(_WGAMMATAUNU_ENR);
  SetYields(_ZGAMMA_ENR);
  SetYields(_TTJETS_ENR);
  SetYields(_WJETS_ENR);

  SetYields(_DATA_EtoGAMMA_ENR); 
 
  for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){
    // data driven e->gamma estimate as to be used for background subtraction:
    TString strName=_conf.GetYieldsDDBkgEtoGamma(_conf.ONEDI, ieta);
    _yieldDDEtoGamma[ieta] = new TH1F(strName,strName,_nKinBins,_kinBinLims);
    _yieldDDEtoGamma[ieta]->Sumw2();
  }//end of loop over ieta
  ComputeBkg();
  WriteToFile();
}// end of TEtoGamma::EstimateBkg()

TTree* TEtoGamma::GetTree(TString strFileName){
  TFile* f = new TFile(strFileName);
  TTree* tr=(TTree*)f->Get("selectedEvents");
}// end of GetTree

TString TEtoGamma::FName(int inum)
{
  TString str;
  if (inum==_DATA_EtoGAMMA_ENR) {
        str=_conf.GetSelectedName(_conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_CUT,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.DATA);
        if (_isMCclosure) str.ReplaceAll(".root","_MCclosure.root");
    std::cout<<"file _DATA_EtoGAMMA_ENR: "<<str<<std::endl;
  }
  if (inum==_ZJETS_EtoGAMMA_ENR) {
    str=_conf.GetSelectedName(_conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_CUT,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"DYjets_to_ll");
    std::cout<<"file _ZJETS_EtoGAMMA_ENR: "<<str<<std::endl;
  }
  if (inum==_SIGMC_ENR) {
    str=_conf.GetSelectedName(_conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_CUT,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.SIGMC);
    std::cout<<"file _SIGMC_ENR: "<<str<<std::endl;
  }
  if (inum==_WGAMMATAUNU_ENR) {
    str=_conf.GetSelectedName(_conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_CUT,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"Wg_to_taunu");
    std::cout<<"file __WGAMMATAUNU_ENR: "<<str<<std::endl;
  }
  if (inum==_ZGAMMA_ENR) {
    str=_conf.GetSelectedName(_conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_CUT,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"Zg");
    std::cout<<"file _ZGAMMA_ENR: "<<str<<std::endl;
  }
  if (inum==_TTJETS_ENR) {
    str=_conf.GetSelectedName(_conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_CUT,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"ttbarjets");
    std::cout<<"file _TTJETS_ENR: "<<str<<std::endl;
  }
  if (inum==_WJETS_ENR) {
    str=_conf.GetSelectedName(_conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_CUT,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"Wjets_to_lnu");
    std::cout<<"file _WJETS_ENR: "<<str<<std::endl;
  }
  if (inum==_ZJETS_NOM) {
    str=_conf.GetSelectedName(_conf.FULLY,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.BKGMC,"DYjets_to_ll");
    std::cout<<"file _ZJETS_NOM: "<<str<<std::endl;
  }
  if (inum==_SIGMC_NOM) {
    str=_conf.GetSelectedName(_conf.FULLY,_conf.ELECTRON,_conf.W_GAMMA,_conf.UNBLIND,_conf.SIGMC);
    std::cout<<"file _SIGMC_NOM: "<<str<<std::endl;
  }
  return str;
}//end of TEtoGamma::FName

TString TEtoGamma::HName(int inum, int ieta)
{
  TString name;
  if (inum==_DATA_EtoGAMMA_ENR) name="h_Data_EtoGamma_Enr_";
  if (inum==_ZJETS_EtoGAMMA_ENR) name="h_ZJets_EtoGamma_Enr_";
  if (inum==_ZJETS_NOM)  name="h_ZJets_Nom_";
  if (inum==_SIGMC_NOM)  name="h_SIGMC_Nom_";
  if (inum==_SIGMC_ENR)  name="h_SIGMC_Enr_";
  if (inum==_WGAMMATAUNU_ENR) name="h_Wgtaunu_Enr_";
  if (inum==_ZGAMMA_ENR) name="h_Zg_Enr_";
  if (inum==_TTJETS_ENR) name="h_ttjets_Enr_";
  if (inum==_WJETS_ENR) name="h_Wjets_Enr_";
  return name+_conf.StrEtaBin(ieta);
}//end of TEtoGamma::HName

void TEtoGamma::SetYields(int inum)
{
  _fOut->cd();
  TPhotonCuts photon;
  TFullCuts fullcut;
  _yield[inum].num=inum;
  TString fName=FName(inum);
  _yield[inum].tr = GetTree(fName);
  TCut cutW="weight";
  //  TCut cutWMt=fullcut.RangeMetRelatedCut(2012,_conf.ELECTRON);
  for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){
    TString hName=HName(inum,ieta);
    _yield[inum].hist[ieta] = new TH1F(hName,hName,_nKinBins,_kinBinLims);
    _yield[inum].hist[ieta]->Sumw2();
    TCut cutEta;
    if (ieta==_conf.BARREL) cutEta=photon.RangeBarrel();
    if (ieta==_conf.ENDCAP) cutEta=photon.RangeEndcap();
    TCut cut = cutEta && _cutAdd;// && cutWMt;
    if (inum==_ZJETS_NOM || inum==_ZJETS_EtoGAMMA_ENR) {TCut cutMatch("pho_genEle_dRMin<0.4"); cut=cut && cutMatch;}
    _yield[inum].tr->Draw(_varKin+TString(">>")+_yield[inum].hist[ieta]->GetName(),cut*cutW,"goff");

    if (inum!=_DATA_EtoGAMMA_ENR) continue;
    for (int ikin=0; ikin<_nKinBins; ikin++){
      for (int ietaFine=0; ietaFine<4; ietaFine++){
        TString strCutKin;
        strCutKin=_varKin+">";
        strCutKin+=_kinBinLims[ikin];
        strCutKin+=" && "+_varKin+"<";
        strCutKin+=_kinBinLims[ikin+1]; 
        TCut cutKin(strCutKin);
        std::cout<<"ikin="<<ikin<<", cutKin="<<cutKin.GetTitle()<<std::endl; 
        TCut cutEtaFine = CutEtaFine(ieta,ietaFine);
        TCut cutWithKin = cutEta && cutEtaFine && _cutAdd && cutKin;
        TString hNameZmass="hZmass_"+hName+"_pt";
        hNameZmass+=(int)_kinBinLims[ikin];
        hNameZmass+="to";
        hNameZmass+=(int)_kinBinLims[ikin+1]; 
        hNameZmass+="_ieta";  
        hNameZmass+=ietaFine;      
        int nB = _ZmassNbins[0];
        _hZmassData[ieta][ietaFine][ikin] = new TH1F(hNameZmass,hNameZmass,nB,_ZmassMin,_ZmassMax);    
        _yield[inum].tr->Draw("Mpholep1>>"+hNameZmass,cutWithKin*cutW,"goff");

	        FitMeg(ikin, ieta, ietaFine, _hZmassData[ieta][ietaFine][ikin],"sa_"+hNameZmass);


      }//end of loop over ietaFine

      float val=0; float err=0;
      for (int ietaFine=0; ietaFine<4; ietaFine++){
        val += _NsigVal[ietaFine];
        err += _NsigErr[ietaFine]*_NsigErr[ietaFine];
      }//end of ietaFine 
      err=sqrt(err);
      _yield[_DATA_EtoGAMMA_ENR].hist[ieta]->SetBinContent(ikin+1,val);
      _yield[_DATA_EtoGAMMA_ENR].hist[ieta]->SetBinError(ikin+1,err);
    }//end of loop over ikin
  }// end of loop over ieta
    
}// end of TEtoGamma::SetYields

TCut TEtoGamma::CutEtaFine(int ieta, int ietaFine){
  float etaMin=0.0; float etaMax=2.5;
  if (ieta==0){//BARREL
    if (ietaFine==0) {etaMin=0.00; etaMax=0.10;}
    if (ietaFine==1) {etaMin=0.10; etaMax=0.50;}
    if (ietaFine==2) {etaMin=0.50; etaMax=1.00;}
    if (ietaFine==3) {etaMin=1.00; etaMax=1.45;} 
  }//end of  if (ieta==0)
  if (ieta==1){//ENDCAP
    if (ietaFine==0) {etaMin=1.56; etaMax=2.10;}
    if (ietaFine==1) {etaMin=2.10; etaMax=2.20;}
    if (ietaFine==2) {etaMin=2.20; etaMax=2.40;}
    if (ietaFine==3) {etaMin=2.40; etaMax=2.51;} 
  }//end of  if (ieta==1)
  TString strCut;
  strCut="(phoSCEta>=";
  strCut+=etaMin;
  strCut+=" && ";
  strCut+="phoSCEta<"; 
  strCut+=etaMax;
  strCut+=") || (phoSCEta<="; 
  strCut+=-etaMin;
  strCut+=" && ";
  strCut+="phoSCEta>"; 
  strCut+=-etaMax;
  strCut+=")";
  TCut cut(strCut);
  std::cout<<"ieta="<<ieta<<", ietaFine="<<ietaFine<<", cut="<<cut.GetTitle()<<std::endl;
  return cut;
}//end of  CutEtaFine

void TEtoGamma::ComputeBkg()
{
  _fOut->cd();
  for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){
    TString strName=_conf.GetYieldsDDBkgEtoGamma(_conf.ONEDI, ieta);
    _yieldDDEtoGamma[ieta] = (TH1F*)_yield[_ZJETS_NOM].hist[ieta]->Clone(strName);
    _yieldDDEtoGamma[ieta]->Multiply(_yield[_DATA_EtoGAMMA_ENR].hist[ieta]);
    _yieldDDEtoGamma[ieta]->Divide(_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]);
    _yieldDDEtoGamma[ieta]->Print();

    // underflow bin: take MC value
    _yieldDDEtoGamma[ieta]->SetBinContent(1,_yield[_ZJETS_NOM].hist[ieta]->GetBinContent(1));
    _yieldDDEtoGamma[ieta]->SetBinError(1,_yield[_ZJETS_NOM].hist[ieta]->GetBinError(1));
   
  std::cout<<"==============================="<<std::endl;
  std::cout<<"||||========== Print Latex"<<std::endl;

  std::cout<<"||||========== Table with err in %"<<std::endl;


    std::cout<<"\\begin{table}[h]"<<std::endl;
    std::cout<<"  \\scriptsize"<<std::endl;
    std::cout<<"  \\begin{center}"<<std::endl;
    std::cout<<"  \\caption{$e\\rightarrow\\gamma$ background }";
    std::cout<<"  \\begin{tabular}{|c|c|c|c|c|c|c|}"<<std::endl;
    std::cout<<" bin  & DYjets    & Data                      & DYjets & scale & $e\\rightarrow\\gamma$ & sigmc\\\\ "<<std::endl;
    std::cout<<" lims & nom. sel. & $e\\rightarrow\\gamma$ enr. & $e\\rightarrow\\gamma$ enr. & & yield & (W\\gamma\\rightarrow e\\nu\\gamma)\\\\ \\hline"<<std::endl;

    for (int ib=1; ib<=_yield[_ZJETS_NOM].hist[ieta]->GetNbinsX(); ib++){
      std::cout<<_yield[_ZJETS_NOM].hist[ieta]->GetBinLowEdge(ib)<<"-"<<_yield[_ZJETS_NOM].hist[ieta]->GetBinLowEdge(ib)+_yield[_ZJETS_NOM].hist[ieta]->GetBinWidth(ib)<<" & ";
      std::cout<<(int)_yield[_ZJETS_NOM].hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)_yield[_ZJETS_NOM].hist[ieta]->GetBinError(ib)<<" & ";
      std::cout<<(int)_yield[_DATA_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)_yield[_DATA_EtoGAMMA_ENR].hist[ieta]->GetBinError(ib)<<" & ";
      std::cout<<(int)_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->GetBinError(ib)<<" & ";
      float scale = _yield[_DATA_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib)/_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib);
      TMathTools math;
      float scaleErr = math.ErrOfTwoIndependent("x1/x2", _yield[_DATA_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib), _yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->GetBinContent(ib),_yield[_DATA_EtoGAMMA_ENR].hist[ieta]->GetBinError(ib), _yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->GetBinError(ib) );
      std::cout<<std::setprecision(3)<<scale<<"$\\pm$"<<scaleErr<<"& ";
      std::cout<<(int)_yieldDDEtoGamma[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)_yieldDDEtoGamma[ieta]->GetBinError(ib)<<"& ";
      std::cout<<(int)_yield[_SIGMC_NOM].hist[ieta]->GetBinContent(ib)<<"$\\pm$"<<(int)_yield[_SIGMC_NOM].hist[ieta]->GetBinError(ib);
      std::cout<<" \\\\ \\hline"<<std::endl;
    }//end of loop over ib

    std::cout<<"  \\end{tabular}"<<std::endl;
    std::cout<<"  \\label{tab:EtoGAMMA_"<<ieta<<"}"<<std::endl;
    std::cout<<"  \\end{center}"<<std::endl;
    std::cout<<"\\end{table}"<<std::endl;

  std::cout<<"|||| end of Print Latex"<<std::endl;
  std::cout<<"==============================="<<std::endl;

  }// end of loop over ieta

}// end of TEtoGamma::ComputeBkg()

void TEtoGamma::WriteToFile()
{
  _fOut->cd();
  TH1F* histTot[3];
    for (int ieta=_conf.BARREL; ieta<=_conf.ENDCAP; ieta++){
      _yieldDDEtoGamma[ieta]->Write();

      // TOTAL
      TString strNameTot=_conf.GetYieldsDDBkgEtoGamma(_conf.TOTAL, ieta);
      histTot[ieta] = new TH1F(strNameTot,strNameTot,1,_kinBinLims[0],_kinBinLims[_nKinBins]);
      histTot[ieta]->SetBinContent(1,0.0); histTot[ieta]->SetBinError(1,0.0);
      float cont=0.0; float err=0.0;      
      for (int ib=1; ib<=_yieldDDEtoGamma[ieta]->GetNbinsX(); ib++){     
        cont+=_yieldDDEtoGamma[ieta]->GetBinContent(ib);
        err+=_yieldDDEtoGamma[ieta]->GetBinError(ib)*_yieldDDEtoGamma[ieta]->GetBinError(ib);
      }
      err = sqrt(err);
      std::cout<<"ieta="<<ieta<<", total = "<<(int)cont<<"+-"<<(int)err<<std::endl;
      histTot[ieta]->SetBinContent(1,cont); histTot[ieta]->SetBinError(1,err);
      histTot[ieta]->Write();
    }//end of loop over ieta

    // _conf.ETA_COMMON
    TString str =_conf.GetYieldsDDBkgEtoGamma(_conf.TOTAL, _conf.COMMON);
    histTot[_conf.COMMON] = (TH1F*)histTot[_conf.BARREL]->Clone(str);
    histTot[_conf.COMMON]->Add(histTot[_conf.ENDCAP]);
    histTot[_conf.COMMON]->SetTitle(str);
    histTot[_conf.COMMON]->Write();
    str =_conf.GetYieldsDDBkgEtoGamma(_conf.ONEDI, _conf.COMMON);
    _yieldDDEtoGamma[_conf.COMMON] = (TH1F*)_yieldDDEtoGamma[_conf.BARREL]->Clone(str);
    _yieldDDEtoGamma[_conf.COMMON]->Add(_yieldDDEtoGamma[_conf.ENDCAP]);
    _yieldDDEtoGamma[_conf.COMMON]->SetTitle(str);
    _yieldDDEtoGamma[_conf.COMMON]->Write(str);
}// end of WriteToFile()

void  TEtoGamma::FitMeg(int ikin, int ieta, int ietaFine, TH1F* Z_mass, TString saveas)
{
  
  double hmin = _ZmassMin;//Z_mass->GetXaxis()->GetXmin();
  double hmax = _ZmassMax;//Z_mass->GetXaxis()->GetXmax();
  double nMax = Z_mass->Integral();
  
  // Declare observable x
  RooRealVar xtmp("xtmp","xtmp",_ZmassMin,_ZmassMax) ;
  RooRealVar x("x","x",hmin,hmax) ;
  RooDataHist dh("dh","dh",x,Import(*Z_mass)) ;
  RooDataHist x1("x1","x1",xtmp,Import(*Z_mass)) ;
  //Construct the signal P.D.F., a gaussian function
  RooRealVar mean_bw("mean_bw","mean of bw",92,75.,100.);
  //  RooRealVar mean_bw("mean_bw","mean of bw",91.2);
  //  mean_bw.setConstant(kTRUE);
  RooRealVar mean_cb("mean_cb","mean of cb",0,-5.0,10);
  RooRealVar sigma_bw("sigma_bw","width of bw",2.5,0,5.);
  //  RooRealVar sigma_bw("sigma_bw","width of bw",2.5);
  //  sigma_bw.setConstant(kTRUE);
  RooRealVar sigma_cb("sigma_cb","width of cb",2.0, 0.00001, 8.0);//this range worked for overall pt range
  RooRealVar n("n","", 6.7489,1.0,20.0);
  RooRealVar alpha("alpha","", 1.0,-50.,50);
  RooBreitWigner bwPDF("BreitWigner","BreitWigner",x,mean_bw,sigma_bw);
  RooCBShape cbPDF("cball", "crystal ball", x, mean_cb, sigma_cb, alpha, n);
  std::cout<<"I will declare RooFFTConvPdf"<<std::endl;
  RooFFTConvPdf SigPDF("bwxCryBall", "FFT Conv CryBall and BW", x, bwPDF, cbPDF);
  std::cout<<"I declared RooFFTConvPdf"<<std::endl;  
  //Now define the background P.D.F, a simple exponential
  RooRealVar tau("tau","exponential function parameter",0,-10.,10.);//this range worked for overall pt range
  RooExponential exp("exponential","Background PDF",x,tau);
  RooFormulaVar eff("eff","0.5*(TMath::Erf((x-1)/0.5)+1)",x) ;
  RooAddPdf BkgPDF("BkgPDF","exp. * Err.fn",exp,eff) ;
  //RooRealVar a0("a0","a0",0.,-5.,5.);
  //RooRealVar a1("a1","a1",0.,-1.,1.);
  //RooRealVar a2("a2","a2",0.,-1.,1.);
  //Now construct the total PDF. We need to define the number of signal and background events in the model
  RooRealVar Nsig("Nsig","Number of signal events",0.5*nMax,0.,nMax);
  RooRealVar Nbkg("Nbkg","Number of background events",0.2*nMax,0.,nMax);
  RooAddPdf PDFtot("PDFtot","PDFtot",RooArgList(SigPDF,BkgPDF),RooArgList(Nsig,Nbkg));
  //RooAddPdf PDFtot("PDFtot","PDFtot",RooArgList(bwPDF,BkgPDF),RooArgList(Nsig,Nbkg));
  std::cout<<"I will fit PDFtot  "<<std::endl;  
    PDFtot.fitTo(dh);//ML fit is default
  std::cout<<"I fitted PDFtot  "<<std::endl; 
  
  // Print values of mean and sigma (that now reflect fitted values and errors, unless you fixed them)
  mean_bw.Print();
  // sigma.Print();
  //Now plot the data and the fitted PDF
  
  RooPlot* frame = x.frame() ;
  //  x1.plotOn(frame);//MarkerColor(2),MarkerSize(0.9),MarkerStyle(21)); //this will show histogram data points on canvas
  dh.plotOn(frame);//MarkerColor(2),MarkerSize(0.9),MarkerStyle(21)); //this will show histogram data points on canvas
  //dh.statOn(frame); //this will display hist stat on canvas
  PDFtot.plotOn(frame,LineColor(4));//this will show fit overlay on canvas
  PDFtot.paramOn(frame,Layout(0.70,0.98,0.8)); //this will display the fit parameters on canvas
  //One can also plot the single components of the total PDF, like the background component
  PDFtot.plotOn(frame,Components(BkgPDF),LineStyle(kDashed),LineColor(kRed));
  //Actually plot the result
  TCanvas* c1 = new TCanvas(saveas,saveas);
  c1->cd();
  //c1->SetLogy();
  frame->Draw();

  // Prepare and plot THStack
  
  THStack* mcHists = new THStack("mcHistsTot","DATA vs MC and FIT");
  
  _yield[_SIGMC_ENR].hist[ieta]->SetFillColor(634);_yield[_SIGMC_ENR].hist[ieta]->SetLineColor(634);
  mcHists->Add(_yield[_SIGMC_ENR].hist[ieta]);
  _yield[_WGAMMATAUNU_ENR].hist[ieta]->SetFillColor(401);_yield[_WGAMMATAUNU_ENR].hist[ieta]->SetLineColor(401);
  mcHists->Add(_yield[_WGAMMATAUNU_ENR].hist[ieta]);
  _yield[_ZGAMMA_ENR].hist[ieta]->SetFillColor(879);_yield[_ZGAMMA_ENR].hist[ieta]->SetLineColor(879);
  mcHists->Add(_yield[_ZGAMMA_ENR].hist[ieta]);
  _yield[_TTJETS_ENR].hist[ieta]->SetFillColor(631);_yield[_TTJETS_ENR].hist[ieta]->SetLineColor(631);
  mcHists->Add(_yield[_TTJETS_ENR].hist[ieta]);
  _yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->SetFillColor(418);_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]->SetLineColor(418);
  mcHists->Add(_yield[_ZJETS_EtoGAMMA_ENR].hist[ieta]);
  _yield[_WJETS_ENR].hist[ieta]->SetFillColor(433);_yield[_WJETS_ENR].hist[ieta]->SetLineColor(433);
  mcHists->Add(_yield[_WJETS_ENR].hist[ieta]);

  //  mcHists->Draw("same");
  //  frame->Draw("same");
  
  c1->Draw();
  //double chi2 = RooChi2Var("chi2","chi2",PDFtot,dh);
  double chi2 = frame->chiSquare('PDFtot');
  cout<<"chi2="<<chi2<<endl;
  if (_isMCclosure) saveas+="_MCclosure";
  c1->SaveAs("../WGammaOutput/ELECTRON_WGamma/Plots/EtoGammaFits/"+saveas+".png");
  cout<<"Nsig.getVal()="<<Nsig.getVal()<<endl;
  _NsigVal[ietaFine]=Nsig.getVal();
  _NsigErr[ietaFine]=Nsig.getError();
  //  _yield[_DATA_EtoGAMMA_ENR].hist[ieta]->SetBinContent(ikin+1,Nsig.getVal());
  //  _yield[_DATA_EtoGAMMA_ENR].hist[ieta]->SetBinError(ikin+1,Nsig.getError());
  
  return;
  
}//end of FitMeg
