#include "TTemplatesRandConeSyst.h"

#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraph2D.h"
#include "TH2D.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLegend.h"
#include "TPaletteAxis.h"

#include <iostream>
#include <iomanip>

TTemplatesRandConeSyst::TTemplatesRandConeSyst()
{
}

TTemplatesRandConeSyst::TTemplatesRandConeSyst(TemplatesRandConePars pars, TemplatesSidebandVariationPars variationPars)
{
  SetPars(pars);
  _pars.fOutForSave->Close();
  _pars.strFileOutName.ReplaceAll(".root","_Syst.root");
  _pars.fOutForSave = new TFile(_pars.strFileOutName,"recreate");
  _variationPars=variationPars;
}

TTemplatesRandConeSyst::~TTemplatesRandConeSyst()
{
  
}

void TTemplatesRandConeSyst::SidebandVariation()
{

  //  for (int ikin=0; ikin<=_pars.nKinBins; ikin++){
  for (int ikin=1; ikin<=1; ikin++){
    SidebandVariationOneKinBin(ikin);
  }
  //  for (int ikin=0; ikin<=_pars.nKinBins; ikin++){
  for (int ikin=1; ikin<=1; ikin++){
    PlotOneKinBin(ikin);
  }

}// end of SidebandVariation()

void TTemplatesRandConeSyst::SidebandVariationOneKinBin(int ikin)
{
  for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){
    if (_variationPars.nPointsLower[ieta]>_nPointsMax || _variationPars.nPointsUpper[ieta]>_nPointsMax) {
      std::cout<<"ERROR in TTemplatesRandConeSyst::SidebandVariationOneKinBin, ikin="<<StrLabelKin(ikin)<<std::endl;
      std::cout<<"_variationPars.nPointsLower["<<StrLabelEta(ieta)<<"]="<<_variationPars.nPointsLower[ieta]<<" > _nPointsMax="<<_nPointsMax<<" or ";
      std::cout<<"_variationPars.nPointsUpper["<<StrLabelEta(ieta)<<"]="<<_variationPars.nPointsUpper[ieta]<<" > _nPointsMax="<<_nPointsMax<<" or ";
      std::cout<<"no sideband variation will be done"<<std::endl;
      return;
    }
  }//end of loop over ieta


  for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){

   //vary sideband for specific kin bin,
   //for _BARREL or _ENDCAP
   VarySidebandKinEtaBin(ikin,ieta);


   //prepare graphs for plotting
   PrepareGraphsKinEtaBin(ikin,ieta);

   //printout (would have to check log file)
   PrintOutKinEtaBin(ikin,ieta);

  }//end of loop over ieta

}// end of SidebandVariationOneKinBin(int ikin)

void TTemplatesRandConeSyst::PlotOneKinBin(int ikin)
{
  _pars.fOutForSave->cd();
  for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){

    TString canvName="canv_SidebandVariation_TrueVal_";
    canvName+=StrLabelKin(ikin);
    canvName+=StrLabelEta(ieta);
    _grTrueVal[ikin][ieta]->SetTitle(TString("sbVar: trueVal, ")+StrLabelKin(ikin)+StrLabelEta(ieta)); 
    TCanvas* c = new TCanvas(canvName,canvName,900,600);
 //   c->Divide(1,2);
 //   c->cd(1);
    _grTrueVal[ikin][ieta]->Draw("COLZ");
 //   TPaletteAxis *palette = (TPaletteAxis*)_grTrueVal[ikin][ieta]->GetListOfFunctions()->FindObject("palette");
 //   c->cd(2);
 //   palette->Draw();

    TString canvNameH=canvName+TString("H");
    TCanvas* c1 = new TCanvas(canvNameH,canvNameH,900,600);
    _histTrueVal[ikin][ieta]->Draw("COLZ");

    canvName+=".png";
    c->SaveAs(canvName);
    canvNameH+=".png";
    c1->SaveAs(canvNameH);

  } // end of loop over ieta

}// end of PlotOneKinBin(int ikin)

void TTemplatesRandConeSyst::VarySidebandKinEtaBin(int ikin, int ieta)
{
    float unitUpper=(_variationPars.upperSidebandCutTo[ieta]-_variationPars.upperSidebandCutFrom[ieta])/_variationPars.nPointsUpper[ieta];
    float unitLower=(_variationPars.lowerSidebandCutTo[ieta]-_variationPars.lowerSidebandCutFrom[ieta])/_variationPars.nPointsLower[ieta];


    for (int isL=0; isL<_variationPars.nPointsLower[ieta]; isL++){
      _sidebandLowerVal[isL]=_variationPars.lowerSidebandCutFrom[ieta]+unitLower*isL;
      _sidebandLowerErr[isL]=0.01*_sidebandLowerVal[isL];

    }//end of loop over isL, lower sideband cut
    for (int isU=0; isU<_variationPars.nPointsUpper[ieta]; isU++){
      _sidebandUpperVal[isU]=_variationPars.upperSidebandCutFrom[ieta]+unitUpper*isU;
      _sidebandUpperErr[isU]=0.01*_sidebandUpperVal[isU];

    }//end of loop over isU, upper sideband cut

    for (int isL=0; isL<_variationPars.nPointsLower[ieta]; isL++){

      for (int isU=0; isU<_variationPars.nPointsUpper[ieta]; isU++){
        if (_sidebandLowerVal[isL]>=_sidebandUpperVal[isU] || 
            (_sidebandUpperVal[isU]-_sidebandLowerVal[isL])<0.005*(_sidebandUpperVal[isU]+_sidebandLowerVal[isL])){
          _yieldsTrueVal[isL][isU]=-1;
          _yieldsTrueErr[isL][isU]=0.001;
          _yieldsFakeVal[isL][isU]=-1;
          _yieldsFakeErr[isL][isU]=0.001;
          continue;
        }
        _pars.sideband[ikin][ieta]=_sidebandLowerVal[isL];
        _pars.sidebandUp[ikin][ieta]=_sidebandUpperVal[isU];
        // the longest part: performs fits
        // implemented in TTemplatesRandCone
        std::cout<<"SIDEBAND VARIATION"<<std::endl;
        std::cout<<StrLabelKin(ikin)<<StrLabelEta(ieta)<<", ";
        std::cout<<"isL="<<isL<<", isU="<<isU;
        std::cout<<", sideband: ";
        std::cout<<std::setprecision(4)<<_sidebandLowerVal[isL]<<"-"<<_sidebandUpperVal[isU]<<"; "<<std::endl;
        ComputeBackgroundOne(ikin,ieta,0);
        _yieldsTrueVal[isL][isU]=_nTrueYieldsVal[ikin][ieta];
        _yieldsTrueErr[isL][isU]=_nTrueYieldsErr[ikin][ieta];
        _yieldsFakeVal[isL][isU]=_nFakeYieldsVal[ikin][ieta];
        _yieldsFakeErr[isL][isU]=_nFakeYieldsErr[ikin][ieta];
      }//end of loop over isU, upper sideband cut

    }//end of loop over isL, lower sideband cut

}// end of VarySidebandKinEtaBin(int ikin, int ieta)

void TTemplatesRandConeSyst::PrepareGraphsKinEtaBin(int ikin, int ieta)
{
  _pars.fOutForSave->cd();
  int nPL=_variationPars.nPointsLower[ieta];
  int nPU=_variationPars.nPointsUpper[ieta];
  int nP=0;
  float sbL[nPL*nPU];
  float sbU[nPL*nPU];
  float yTrueVal[nPL*nPU];
  std::cout<<std::endl;
  std::cout<<"Prepare Graphs for "<<StrLabelKin(ikin)<<StrLabelEta(ieta)<<std::endl;
  TString histName=TString("histTrueVal")+StrLabelKin(ikin)+StrLabelEta(ieta);
  _histTrueVal[ikin][ieta]=new TH2D(histName,histName,nPU,_sidebandLowerVal,nPL,_sidebandUpperVal);
  for (int isL=0; isL<nPL; isL++){ 
    for (int isU=0; isU<nPU; isU++){ 
      if (_yieldsTrueVal[isL][isU]<0){
        _histTrueVal[ikin][ieta]->SetBinContent(isL+1,isU+1,0);
        _histTrueVal[ikin][ieta]->SetBinError(isL+1,isU+1,0.001);
        continue;
      }
      _histTrueVal[ikin][ieta]->SetBinContent(isL+1,isU+1,_yieldsTrueVal[isL][isU]);
      _histTrueVal[ikin][ieta]->SetBinError(isL+1,isU+1,_yieldsTrueErr[isL][isU]);
      sbL[nP]=_sidebandLowerVal[isL];
      sbU[nP]=_sidebandUpperVal[isU];
      yTrueVal[nP]=_yieldsTrueVal[isL][isU];
      std::cout<<"isL="<<isL<<", isU="<<isU<<", nP="<<nP;
      std::cout<<std::setprecision(4)<<", sbL="<<sbL[nP]<<", sbU="<<sbU[nP];
      std::cout<<std::setprecision(0)<<", yTrueVal="<<yTrueVal[nP]<<std::endl;
      nP++;
    }//end of loop over isU
  }//end of loop over isL

  std::cout<<"_histTrueVal:"<<std::endl;
  for (int isL=1; isL<=_histTrueVal[ikin][ieta]->GetNbinsX(); isL++){ 
    for (int isU=1; isU<=_histTrueVal[ikin][ieta]->GetNbinsY(); isU++){ 
      std::cout<<"isL="<<isL<<", isU="<<isU;
      std::cout<<std::setprecision(0)<<", ="<< _histTrueVal[ikin][ieta]->GetBinContent(isL,isU)<<"+-";
      std::cout<< _histTrueVal[ikin][ieta]->GetBinError(isL,isU)<<std::endl;
    }//end of loop over isU
  }//end of loop over isL

  _grTrueVal[ikin][ieta]=new TGraph2D(nP,sbL,sbU,yTrueVal);
  _grTrueVal[ikin][ieta]->GetXaxis()->SetTitle("lower sb cut");
  _grTrueVal[ikin][ieta]->GetYaxis()->SetTitle("upper sb cut");


}// end of PrepareGraphsKinEtaBin(int ikin, int ieta)

void TTemplatesRandConeSyst::CheckMinAndMax(float val, float err, float& min, float& max)
{
  if (val+err>max) max=val+err;
  if (val-err<min) min=val-err;
}// end of CheckMinAndMax(float val, float err, float& min, float& max)

void TTemplatesRandConeSyst::PrintOutKinEtaBin(int ikin, int ieta)
{

    std::cout<<std::endl;
    std::cout<<"PRINT TRUE AND FAKE YIELDS FOR GIVEN BIN"<<std::endl;
    std::cout<<"ikin="<<StrLabelKin(ikin);
    std::cout<<", ieta="<<StrLabelEta(ieta)<<std::endl;
    for (int isL=0; isL<_variationPars.nPointsLower[ieta]; isL++){
      for (int isU=isL+1; isU<_variationPars.nPointsUpper[ieta]+1; isU++){
      std::cout<<std::setprecision(3)<<std::endl;
      std::cout<<"isL="<<isL<<", isU="<<isU<<", sideband="<<_sidebandLowerVal[isL]<<"-"<<_sidebandUpperVal[isU]<<std::endl;
      std::cout<<"true yield : ";
      std::cout<<std::setprecision(0)<<" "<<_yieldsTrueVal[isL][isU]<<"+-"<<_yieldsTrueErr[isL][isU]<<", ";
      std::cout<<"fake yield : ";
      std::cout<<std::setprecision(0)<<" "<<_yieldsFakeVal[isL][isU]<<"+-"<<_yieldsFakeErr[isL][isU];
      std::cout<<std::endl;
      }//end of loop over isU
    }//end of loop over isL

}// end of PrintOutKinEtaBin



void TTemplatesRandConeSyst::SetPlottingStyles(TGraphErrors* gr, TLine* line)
{
   gr->SetMarkerColor(1);
   gr->SetLineColor(1);
   line->SetLineColor(1);
   gr->SetFillColor(0);
   gr->SetLineWidth(2);
   line->SetLineStyle(7);
}//end of SetPlottingStyles(TGraphErrors* gr[3], TLine* line[3])
