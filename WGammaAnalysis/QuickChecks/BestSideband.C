#include "TTree.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h" 
#include "TCanvas.h" 
#include "TGraph.h" 
#include "TCut.h" 
#include <iostream> 

void BestSidebandLeftAndRight(TTree* trData, TTree* trSign, TTree* trBkg1, TTree* trBkg2, TString sidebandVar, TCut cutExceptWeight, TCut cutWeight, TCut cutTruePhoton, int nPointsLeft, float sidebandLeftMin, float sidebandLeftMax, int nPointsRight=1, float sidebandRightMin=100000, float sidebandRightMax=100000)
{

  TH1D* hSign = new TH1D ("hSign","hist signal",1,trSign->GetMinimum(sidebandVar),trSign->GetMaximum(sidebandVar));
  TH1D* hBkg1 = new TH1D ("hBkg1","hist bkg1",1,trBkg1->GetMinimum(sidebandVar),trBkg1->GetMaximum(sidebandVar));
  TH1D* hBkg2 = new TH1D ("hBkg2","hist bkg2",1,trBkg2->GetMinimum(sidebandVar),trBkg2->GetMaximum(sidebandVar));
  TH1D* hData = new TH1D ("hData","hist data  ",1,trData->GetMinimum(sidebandVar),trData->GetMaximum(sidebandVar));
  TString drawToHistSign=sidebandVar;
  drawToHistSign+=">>hSign";
  TString drawToHistBkg1=sidebandVar;
  drawToHistBkg1+=">>hBkg1";
  TString drawToHistBkg2=sidebandVar;
  drawToHistBkg2+=">>hBkg2";
  TString drawToHistData=sidebandVar;
  drawToHistData+=">>hData";
  trSign->Draw(drawToHistSign,cutWeight,"goff");
  trSign->Draw(drawToHistBkg1,cutWeight*cutTruePhoton,"goff");
  trSign->Draw(drawToHistBkg2,cutWeight*cutTruePhoton,"goff");
  trData->Draw(drawToHistData,cutWeight,"goff");

  float sidebandLeft[nPointsLeft];
  float sidebandRight[nPointsRight];
  float quality[nPointsLeft][nPointsRight];

  std::cout<<"left: ";
  for (int i=0; i<nPointsLeft; i++){
   sidebandLeft[i] = sidebandLeftMin+(sidebandLeftMax-sidebandLeftMin)*(i+0.5)/(nPointsLeft);
   std::cout<<sidebandLeft[i]<<" ";
  }
  std::cout<<std::endl;

  std::cout<<"right: ";
  for (int j=0; j<nPointsRight; j++){
    sidebandRight[j] = sidebandRightMin+(sidebandRightMax-sidebandRightMin)*(j+0.5)/(nPointsRight);
   std::cout<<sidebandRight[j]<<" ";
  }
  std::cout<<std::endl;

  TString name="bestSideband_";
  name+=sidebandVar;
  name+="_";
  name+=cutExceptWeight;

  for (int i=0; i<nPointsLeft; i++){
    for (int j=0; j<nPointsRight; j++){

      if (sidebandRight[j]<=sidebandLeft[i]){
        quality[i][j]=0;
        continue;
      }

      TString sidebandStr = sidebandVar;
      sidebandStr+=" > ";
      sidebandStr+=sidebandLeft[i];

      if (nPointsRight>1){
        sidebandStr+=" && ";
        sidebandStr+=sidebandVar;
        sidebandStr+=" < ";
        sidebandStr+=sidebandRight[j];
      }

      std::cout<<"cut="<<sidebandStr<<std::endl;
      TCut sideband(sidebandStr);

      trSign->Draw(drawToHistSign,(sideband && cutExceptWeight)*cutWeight,"goff");
      trBkg1->Draw(drawToHistBkg1,(sideband && cutExceptWeight && cutTruePhoton)*cutWeight,"goff");
      trBkg2->Draw(drawToHistBkg2,(sideband && cutExceptWeight && cutTruePhoton)*cutWeight,"goff");
      trData->Draw(drawToHistData,(sideband && cutExceptWeight)*cutWeight,"goff"); 

      double Ndata=(double)hData->GetBinContent(1);
      double Nsign=(double)hSign->GetBinContent(1);
      double Nbkg1=(double)hBkg1->GetBinContent(1);
      double Nbkg2=(double)hBkg2->GetBinContent(1);
    
      if (Ndata<=0) quality[i][j]=0;
      else quality[i][j]=(Ndata-Nsign-Nbkg1-Nbkg2)/sqrt(Ndata);

 //     std::cout<<"sideband="<<sidebandStr<<", quality="<<quality[i][j]<<std::endl;

    }//end of loop over j
  }//end of loop over i

  delete hData;
  delete hSign;
  delete hBkg1;
  delete hBkg2;
 

  float Q[nPointsLeft];
  for (int j=0; j<nPointsRight; j++){
    name+=j;
    for (int i=0; i<nPointsLeft; i++){
      Q[i]=quality[i][j];
    }    
    TString name2="sidebandRight=";
    name2+=sidebandRight[j];
    TCanvas* cQ = new TCanvas(name,name2);
    TGraph* grQ = new TGraph(nPointsLeft,sidebandLeft,Q);
    grQ->SetMarkerStyle(20);
    grQ->Draw("AP");
  }


//  TCanvas* cQnPointsRight = new TCanvas(name,name);
//  TGraph2D* grQ = new TGraph2D(nPointsLeft*nPointsRight,left,right,Q);

//  hQuality->Draw("COLZ");
//  grQ->Draw("A COLZ");
//  name.ReplaceAll("(","");
//  name.ReplaceAll(")","");
//  name.ReplaceAll("&","");
//  name.ReplaceAll("|","");
//  name.ReplaceAll(" ","");
//  name.ReplaceAll("-","");
//  name.ReplaceAll(".","");
//  name.ReplaceAll(">","");
//  name.ReplaceAll("<","");
//  name+=".png";
//  cQ->SaveAs(name);
}
