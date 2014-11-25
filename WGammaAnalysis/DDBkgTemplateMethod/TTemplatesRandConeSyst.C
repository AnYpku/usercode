#include "TTemplatesRandConeSyst.h"

#include "TGraph.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLegend.h"

#include <iostream>
#include <iomanip>

TTemplatesRandConeSyst::TTemplatesRandConeSyst()
{
}

TTemplatesRandConeSyst::TTemplatesRandConeSyst(TemplatesRandConePars pars)
{
  SetPars(pars);
  _pars.fOutForSave->Close();
  _pars.strFileOutName.ReplaceAll(".root","_Syst.root");
  _pars.fOutForSave = new TFile(_pars.strFileOutName,"recreate");
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
    if (_nPoints[ieta]>_nPointsMax) {
      std::cout<<"ERROR in TTemplatesRandConeSyst::SidebandVariationOneKinBin, ikin="<<StrLabelKin(ikin)<<std::endl;
      std::cout<<"_nPoints["<<StrLabelEta(ieta)<<"]="<<_nPoints[ieta]<<" > _nPointsMax="<<_nPointsMax<<std::endl;
      std::cout<<"no sideband variation will be done"<<std::endl;
      return;
    }
  }//end of loop over ieta

  // set the sidebands used in analysis for reference
  for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++)
    _sidebandRef[ieta]=_pars.sideband[ikin][ieta]; 

  for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){

   //vary sideband for specific kin bin,
   //for _BARREL or _ENDCAP
   VarySidebandKinEtaBin(ikin,ieta);

   //put sideband to the original value
   _pars.sideband[ikin][ieta]=_sidebandRef[ieta];

   //prepare graphs for plotting
   PrepareGraphsKinEtaBin(ikin,ieta);

   //printout (would have to check log file)
   PrintOutKinEtaBin(ikin,ieta);

  }//end of loop over ieta

}// end of SidebandVariationOneKinBin(int ikin)

void TTemplatesRandConeSyst::PlotOneKinBin(int ikin)
{
  for (int ieta=_BARREL; ieta<=_ENDCAP; ieta++){

    SetPlottingStyles(_grTrue[ikin][ieta],_lineRefTrue[ikin][ieta]);
    SetPlottingStyles(_grFake[ikin][ieta],_lineRefFake[ikin][ieta]);

    TString canvName="canv_SidebandVariation_";
    canvName+=StrLabelKin(ikin);
    canvName+=StrLabelEta(ieta);
    TCanvas* c = new TCanvas(canvName,canvName,1200,600);
    c->Divide(2,1);
    c->cd(1);
    _grEmptTrue[ikin][ieta]->Draw("AP");

    _grTrue[ikin][ieta]->Draw("P"); 
    _lineRefTrue[ikin][ieta]->Draw("same"); 

    c->cd(2);
    TLegend* leg = new TLegend(0.70,0.10,0.95,0.30);
    _grEmptFake[ikin][ieta]->Draw("AP");

      _grFake[ikin][ieta]->Draw("P");
      _lineRefFake[ikin][ieta]->Draw("same"); 
      leg->AddEntry(_grFake[ikin][ieta],StrLabelEta(ieta),"lpf");
      leg->Draw("same");

    canvName+=".png";
    c->SaveAs(canvName);
  } // end of loop over ieta
}// end of PlotOneKinBin(int ikin)

void TTemplatesRandConeSyst::VarySidebandKinEtaBin(int ikin, int ieta)
{
    for (int is=0; is<_nPoints[ieta]; is++){
      _sidebandsVal[is]=_minSideband[ieta]+_unit[ieta]*is;
      _sidebandsErr[is]=0.01*_unit[ieta];
      _pars.sideband[ikin][ieta]=_sidebandsVal[is];

      // the longest part: performs fits
      // implemented in TTemplatesRandCone
      std::cout<<StrLabelKin(ikin)<<StrLabelEta(ieta)<<", sideband="<<_sidebandsVal[is]<<std::endl;
      ComputeBackgroundOne(ikin,ieta,0);

      _yieldsTrueVal[is]=_nTrueYieldsVal[ikin][ieta];
      _yieldsTrueErr[is]=_nTrueYieldsErr[ikin][ieta];
      _yieldsFakeVal[is]=_nFakeYieldsVal[ikin][ieta];
      _yieldsFakeErr[is]=_nFakeYieldsErr[ikin][ieta];

    }//end of loop over is
}// end of VarySidebandKinEtaBin(int ikin, int ieta)

void TTemplatesRandConeSyst::PrepareGraphsKinEtaBin(int ikin, int ieta)
{
  float maxTrue=-1;
  float minTrue=1000000;
  float maxFake=-1;
  float minFake=1000000;
  
  float yieldRefTrue=0;
  float yieldRefFake=0;

  for (int is=0; is<_nPoints[ieta]; is++){ 
      CheckMinAndMax(_yieldsTrueVal[is],_yieldsTrueErr[is],minTrue,maxTrue);
      CheckMinAndMax(_yieldsFakeVal[is],_yieldsFakeErr[is],minFake,maxFake);
      if (fabs(_sidebandRef[ieta]-_sidebandsVal[is])<0.0001){
        yieldRefTrue=_yieldsTrueVal[is];
        yieldRefFake=_yieldsFakeVal[is];
      }
  }//end of loop over is

  _grTrue[ikin][ieta]=new TGraphErrors(_nPoints[ieta],_sidebandsVal,_yieldsTrueVal,_sidebandsErr,_yieldsTrueErr);
  _grFake[ikin][ieta]=new TGraphErrors(_nPoints[ieta],_sidebandsVal,_yieldsFakeVal,_sidebandsErr,_yieldsFakeErr);
  _lineRefTrue[ikin][ieta]=new TLine(_sidebandsVal[0],yieldRefTrue,_sidebandsVal[_nPoints[ieta]-1],yieldRefTrue);
  _lineRefFake[ikin][ieta]=new TLine(_sidebandsVal[0],yieldRefFake,_sidebandsVal[_nPoints[ieta]-1],yieldRefFake);


  float emptX[2]={_sidebandsVal[0], _sidebandsVal[_nPoints[ieta]-1]};

  if (minTrue>0) minTrue=0;
  float emptYTrue[2]={0,maxTrue};
  _grEmptTrue[ikin][ieta] = new TGraph(2, emptX, emptYTrue);
  _grEmptTrue[ikin][ieta]->SetTitle(TString("true gamma yields, ")+StrLabelEta(ieta)+TString(" variation, ")+StrLabelKin(ikin));
  _grEmptTrue[ikin][ieta]->SetLineColor(0);
  _grEmptTrue[ikin][ieta]->SetMarkerColor(0);

  if (minFake>0) minFake=0;
  float emptYFake[2]={minFake,maxFake};
  _grEmptFake[ikin][ieta] = new TGraph(2, emptX, emptYFake);
  _grEmptFake[ikin][ieta]->SetTitle(TString("fake gamma yields, ")+StrLabelEta(ieta)+TString(" variation, ")+StrLabelKin(ikin));
  _grEmptFake[ikin][ieta]->SetLineColor(0);
  _grEmptFake[ikin][ieta]->SetMarkerColor(0);

}// end of PrepareGraphsKinEtaBin(int ikin, int ieta)

void TTemplatesRandConeSyst::CheckMinAndMax(float val, float err, float& min, float& max)
{
  if (val+err>max) max=val+err;
  if (val-err<min) min=val-err;
}// end of CheckMinAndMax(float val, float err, float& min, float& max)

void TTemplatesRandConeSyst::PrintOutKinEtaBin(int ikin, int ieta)
{
    std::cout<<std::endl;
    std::cout<<"ikin="<<StrLabelKin(ikin);
    std::cout<<", ieta="<<StrLabelEta(ieta)<<std::endl;
    for (int is=0; is<_nPoints[ieta]; is++){
      std::cout<<std::setprecision(3)<<std::endl;
      std::cout<<"is="<<is<<", sideband="<<_sidebandsVal[is]<<"+-"<<_sidebandsErr[is]<<std::endl;
      std::cout<<"true yield : ";
      std::cout<<std::setprecision(0)<<" "<<_yieldsTrueVal[is]<<"+-"<<_yieldsTrueErr[is]<<", ";
      std::cout<<"fake yield : ";
      std::cout<<std::setprecision(0)<<" "<<_yieldsFakeVal[is]<<"+-"<<_yieldsFakeErr[is];
      std::cout<<std::endl;
    }//end of loop over is
}// end of SetPlottingStyles



void TTemplatesRandConeSyst::SetPlottingStyles(TGraphErrors* gr, TLine* line)
{
   gr->SetMarkerColor(1);
   gr->SetLineColor(1);
   line->SetLineColor(1);
   gr->SetFillColor(0);
   gr->SetLineWidth(2);
   line->SetLineStyle(7);
}//end of SetPlottingStyles(TGraphErrors* gr[3], TLine* line[3])
