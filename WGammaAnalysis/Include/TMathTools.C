#include "TMathTools.h"
  //this class

#include "TMath.h"
#include "TString.h"
#include "TH1F.h"
 //root class

#include <string>
#include <iostream>
  //standard C++

TMathTools::TMathTools()
{
}


TMathTools::~TMathTools()
{
}

float TMathTools::DeltaR (float phi1, float eta1, float phi2, float eta2)
{
  float deta=fabs(eta1-eta2);
  float dphi=fabs(phi1-phi2);
  while (dphi>=TMath::Pi()) dphi-=2*TMath::Pi();
  while (dphi<-TMath::Pi()) dphi+=2*TMath::Pi();
  float dR=sqrt(deta*deta+dphi*dphi);
  return dR;
}

float TMathTools::ErrOfTwoIndependent(string type, float x1, float x2, float er1, float er2)
{
  if (type=="x1/x2"){
    if (x2==0){
      std::cout<<"ERROR detected in TMathTools::ErrOfTwoIndependent: x2=0; value -1 will be returned"<<std::endl;
      return -1;
    }
    return (sqrt(x1*x1*er2*er2+x2*x2*er1*er1)/(x2*x2));
  }
  else if (type=="x1/(x1+x2)"){
    if ((x1+x2)==0){
      std::cout<<"ERROR detected in TMathTools::ErrOfTwoIndependent: x1+x2=0; value -1 will be returned"<<std::endl;
      return -1;
    }
    return (sqrt(x1*x1*er2*er2+x2*x2*er1*er1)/((x1+x2)*(x1+x2)));
  }
  
  std::cout<<"unknown expression type "<<type<<"; value -1 will be returned"<<std::endl;
  return -1;
}

float TMathTools::ErrOfThreeIndependent(string type, float x1, float x2, float x3, float er1, float er2, float er3)
{
  if (type=="x1/(x2*x3)"){
    if (x2*x3==0){
      std::cout<<"ERROR detected in TMathTools::ErrOfThreeIndependent: x2*x3=0; value -1 will be returned"<<std::endl;
      return -1;
    }
    float er = x2*x2*x3*x3*er1*er1 + x3*x3*x1*x1*er2*er2 + x1*x1*x2*x2*er3*er3;
    er = sqrt(er);
    er = er/(x2*x2*x3*x3);
    return er;
  }
  if (type=="(x1*x2)/x3"){
    if (x3==0){
      std::cout<<"ERROR detected in TMathTools::ErrOfThreeIndependent: x3=0; value -1 will be returned"<<std::endl;
      return -1;
    }
    float er = x2*x2*x3*x3*er1*er1 + x3*x3*x1*x1*er2*er2 + x1*x1*x2*x2*er3*er3;
    er = sqrt(er);
    er = er/(x3*x3);
    return er;
  }
  std::cout<<"unknown expression type "<<type<<"; value -1 will be returned"<<std::endl;
  return -1;
}

TH1F* TMathTools::ComputeHistTotal(TString hName, TH1F* h1D)
{
  float lowEdge =  h1D->GetBinLowEdge(1);
  int nBins = h1D->GetNbinsX();
  float upEdge = h1D->GetBinLowEdge(nBins)+ h1D->GetBinWidth(nBins);
  TH1F* hTot = new TH1F(hName, hName, 1, lowEdge, upEdge);
  float totVal=0;
  float totErr=0;
  for (int ib=1; ib<=nBins; ib++){
    totVal+= h1D->GetBinContent(ib);
    totErr+= h1D->GetBinError(ib)*h1D->GetBinError(ib);
  }// end of loop over ib
  totErr = sqrt(totErr);
  hTot->SetBinContent(1,totVal);
  hTot->SetBinError(1,totErr);
  return hTot;
}
