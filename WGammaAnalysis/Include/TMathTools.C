#include "TMathTools.h"
  //this class

#include "TMath.h"
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

float TMathTools::DeltaR (float eta1, float phi1, float eta2, float phi2)
{
  float dphi;
  for (dphi=fabs(phi1-phi2); fabs(dphi)>=2*TMath::Pi(); dphi=dphi-2*TMath::Pi()) ;
  float dR=sqrt((eta1-eta2)*(eta1-eta2)+dphi*dphi);
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
  std::cout<<"unknown expression type "<<type<<"; value -1 will be returned"<<std::endl;
  return -1;
}
