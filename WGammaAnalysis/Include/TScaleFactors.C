#include "TScaleFactors.h"
  //this class
  //this package
#include "TFile.h"
#include "TH1D.h"
#include "TString.h"
 //ROOT

TScaleFactors::TScaleFactors()
{

}

TScaleFactors::~TScaleFactors()
{

}

// For Medium Electron ID:
// https://twiki.cern.ch/twiki/bin/view/Main/EGammaScaleFactors2012#2012_8_TeV_Jan22_Re_recoed_data
float TScaleFactors::SF_MediumEleID(float pt, float eta) {
  if ( pt <= 40){
    if (fabs(eta) <= 0.8)                       { return 1.002;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442)  { return 0.980;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0)  { return 0.967;}
    if (fabs(eta) > 2.0 )    { return 1.021;}
  }
  if ( pt > 40 && pt <= 50){
    if (fabs(eta) <= 0.8)                       { return 1.005;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442)  { return 0.988;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0)  { return 0.992;}
    if (fabs(eta) > 2.0 )    { return 1.019;}
  }
  if ( pt > 50 ) {
    if (fabs(eta) <= 0.8)                       { return 1.004;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442)  { return 0.988;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0)  { return 1.000;}
    if (fabs(eta) > 2.0 )    { return 1.022;}
  }
  return 1.0;
}// end of SF_MediumEleID

//https://twiki.cern.ch/twiki/bin/view/Main/EGammaScaleFactors2012#2012_8_TeV_Jan22_Re_recoed_data
float TScaleFactors::SF_TightEleID(float pt, float eta) {
  if ( pt <= 40){
    if (fabs(eta) <= 0.8)                       { return 0.978;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442)  { return 0.958;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0)  { return 0.909;}
    if (fabs(eta) > 2.0 )    { return 0.987;}
  }
  if ( pt > 40 && pt <= 50){
    if (fabs(eta) <= 0.8)                       { return 0.981;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442)  { return 0.969;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0)  { return 0.942;}
    if (fabs(eta) > 2.0 )    { return 0.991;}
  }
  if ( pt > 50 ) {
    if (fabs(eta) <= 0.8)                       { return 0.982;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442)  { return 0.969;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0)  { return 0.957;}
    if (fabs(eta) > 2.0 )    { return 0.999;}
  }
  return 1.0;
}// end of SF_TightEleID

float TScaleFactors::SF_Err_TightEleID(float pt, float eta) {
  if ( pt <= 40){
    if (fabs(eta) <= 0.8)                       { return 0.001;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442)  { return 0.002;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0)  { return 0.003;}
    if (fabs(eta) > 2.0 )    { return 0.004;}
  }
  if ( pt > 40 && pt <= 50){
    if (fabs(eta) <= 0.8)                       { return 0.001;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442)  { return 0.001;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0)  { return 0.002;}
    if (fabs(eta) > 2.0 )    { return 0.003;}
  }
  if ( pt > 50 ) {
    if (fabs(eta) <= 0.8)                       { return 0.002;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442)  { return 0.002;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0)  { return 0.004;}
    if (fabs(eta) > 2.0 )    { return 0.005;}
  }
  return 0.0;
}// end of SF_Err_TightEleID

float TScaleFactors::SF_PixelSeedVeto(float pt, float eta){
  if(fabs(eta) <= 1.4442){
    if(pt > 15 && pt <= 20) {return 0.996;}
    if(pt > 20 && pt <= 25) {return 0.994;}
    if(pt > 25 && pt <= 30) {return 0.996;}
    if(pt > 30 && pt <= 40) {return 0.999;}
    if(pt > 40 && pt <= 50) {return 1.009;}
    if(pt > 50 && pt <= 70) {return 0.993;}
    if(pt > 70 )            {return 1.047;}
  } else if(fabs(eta) > 1.566 && fabs(eta) <= 2.5){
    if(pt > 15 && pt <= 20) {return 0.960;}
    if(pt > 20 && pt <= 25) {return 0.977;}
    if(pt > 25 && pt <= 30) {return 0.951;}
    if(pt > 30 && pt <= 40) {return 1.029;}
    if(pt > 40 && pt <= 50) {return 0.971;}
    if(pt > 50 && pt <= 70) {return 0.965;}
    if(pt > 70 )           {return 1.145;}
  } 
  return 1.0;
}// end of SF_PixelSeedVeto(float pt, float eta)

float TScaleFactors::SF_Err_PixelSeedVeto(float pt, float eta){
  if(fabs(eta) <= 1.4442){
    if(pt > 15 && pt <= 20) {return 0.020;}
    if(pt > 20 && pt <= 25) {return 0.024;}
    if(pt > 25 && pt <= 30) {return 0.030;}
    if(pt > 30 && pt <= 40) {return 0.033;}
    if(pt > 40 && pt <= 50) {return 0.073;}
    if(pt > 50 && pt <= 70) {return 0.128;}
    if(pt > 70 )            {return 0.111;}
  } else if(fabs(eta) > 1.566 && fabs(eta) <= 2.5){
    if(pt > 15 && pt <= 20) {return 0.041;}
    if(pt > 20 && pt <= 25) {return 0.051;}
    if(pt > 25 && pt <= 30) {return 0.062;}
    if(pt > 30 && pt <= 40) {return 0.081;}
    if(pt > 40 && pt <= 50) {return 0.150;}
    if(pt > 50 && pt <= 70) {return 0.294;}
    if(pt > 70 )           {return 0.371;}
  } 
  return 0.0;
}// end of SF_Err_PixelSeedVeto(float pt, float eta)


// For Medium Photon ID:
// https://indico.cern.ch/event/305105/contribution/3/material/slides/0.pdf 
// (slide 5)
float TScaleFactors::SF_MediumPhoID(float pt, float eta){
  if( pt > 15 && pt <= 20){
    if (fabs(eta) <= 0.8)                     { return 0.9462;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442){ return 0.9919;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0){ return 1.0013;}
    if (fabs(eta) > 2.0 && fabs(eta) <= 2.5)  { return 1.0169;}
  }
  if( pt > 20 && pt <= 30 ){
    if (fabs(eta) <= 0.8)                     { return 0.9639;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442){ return 0.9730;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0){ return 0.9835;}
    if (fabs(eta) > 2.0 && fabs(eta) <= 2.5)  { return 1.0046;}
  }
  if( pt > 30 && pt <= 40 ){
    if (fabs(eta) <= 0.8)                     { return 0.9764;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442){ return 0.9777;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0){ return 0.9919;}
    if (fabs(eta) > 2.0 && fabs(eta) <= 2.5)  { return 1.0043;}
  }
  if( pt > 40 && pt <= 50 ){
    if (fabs(eta) <= 0.8)                     { return 0.9804;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442){ return 0.9840;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0){ return 0.9959;}
    if (fabs(eta) > 2.0 && fabs(eta) <= 2.5)  { return 1.0066;}
  }
  if( pt > 50 ) {
    if (fabs(eta) <= 0.8)                     { return 0.9787;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442){ return 0.9822;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0){ return 0.9973;}
    if (fabs(eta) > 2.0 && fabs(eta) <= 2.5)  { return 1.0075;}
  }
  return 1.0;
}// end of SF_MediumPhoID

float TScaleFactors::SF_Err_MediumPhoID(float pt, float eta){
  if( pt > 15 && pt <= 20){
    if (fabs(eta) <= 0.8)                     { return 0.0205;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442){ return 0.0205;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0){ return 0.0209;}
    if (fabs(eta) > 2.0 && fabs(eta) <= 2.5)  { return 0.0206;}
  }
  if( pt > 20 && pt <= 30 ){
    if (fabs(eta) <= 0.8)                     { return 0.0101;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442){ return 0.0101;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0){ return 0.0102;}
    if (fabs(eta) > 2.0 && fabs(eta) <= 2.5)  { return 0.0102;}
  }
  if( pt > 30 && pt <= 40 ){
    if (fabs(eta) <= 0.8)                     { return 0.0100;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442){ return 0.0100;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0){ return 0.0100;}
    if (fabs(eta) > 2.0 && fabs(eta) <= 2.5)  { return 0.0101;}
  }
  if( pt > 40 && pt <= 50 ){
    if (fabs(eta) <= 0.8)                     { return 0.0100;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442){ return 0.0100;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0){ return 0.0100;}
    if (fabs(eta) > 2.0 && fabs(eta) <= 2.5)  { return 0.0100;}
  }
  if( pt > 50 ) {
    if (fabs(eta) <= 0.8)                     { return 0.0100;}
    if (fabs(eta) > 0.8 && fabs(eta) <= 1.4442){ return 0.0101;}
    if (fabs(eta) > 1.566 && fabs(eta) <= 2.0){ return 0.0101;}
    if (fabs(eta) > 2.0 && fabs(eta) <= 2.5)  { return 0.0101;}
  }
  return 0.0;
}// end of SF_MediumPhoID

float TScaleFactors::SF_MuonIso(float pt, float eta){
  if(pt > 10 && pt < 20){
    if(fabs(eta) < 0.9)                    { return 0.958554;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.966286;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.982435;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 1.0762;}
  }
  if(pt > 20 && pt < 25){
    if(fabs(eta) < 0.9)                    { return 0.988193;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.989546;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.994891;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 1.06033;}
  }
  if(pt > 25 && pt < 30){
    if(fabs(eta) < 0.9)                    { return 0.999495;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 1.00217;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 1.00249;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 1.04687;}
  }
  if(pt > 30 && pt < 35){
    if(fabs(eta) < 0.9)                    { return 0.998766;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 1.0023;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 1.00291;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 1.03181;}
  }
  if(pt > 35 && pt < 40){
    if(fabs(eta) < 0.9)                    { return 0.998527;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 1.0012;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 1.00192;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 1.02255;}
  }
  if(pt > 40 && pt < 45){
    if(fabs(eta) < 0.9)                    { return 0.998499;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 1.00023;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 1.00006;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 1.01055;}
  }
  if(pt > 45 && pt < 50){
    if(fabs(eta) < 0.9)                    { return 1.000;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 1.000;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 1.000;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 1.000;}
  }
  if(pt > 50 && pt < 60){
    if(fabs(eta) < 0.9)                    { return 0.999247;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.999899;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 1.00023;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 1.00687;}
  }
  if(pt > 60 && pt < 90){
    if(fabs(eta) < 0.9)                    { return 1.00047;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 1.00096;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 1.00024;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 1.0045;}
  }
  if(pt > 90 && pt < 140){
    if(fabs(eta) < 0.9)                    { return 1.00075;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 1.0012;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.999889;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 0.999248;}
  }
  if(pt > 140){
    if(fabs(eta) < 0.9)                    { return 1.0009;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 1.00404;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.997354;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 1.00521;}
  }
  return 1.0;
}// end of SF_MuonIso

float TScaleFactors::SF_Err_MuonIso(float pt, float eta){
  if(pt > 10 && pt < 20){
    if(fabs(eta) < 0.9)                    { return 0.003;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.003;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.002;}
  }
  if(pt > 20 && pt < 25){
    if(fabs(eta) < 0.9)                    { return 0.001;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.002;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.001;}
  }
  if(pt > 25 && pt < 30){
    if(fabs(eta) < 0.9)                    { return 0.001;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.001;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.001;}
  }
  if(pt > 30 && pt < 35){
    if(fabs(eta) < 0.9)                    { return 0.000;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.001;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.000;}
  }
  if(pt > 35 && pt < 40){
    if(fabs(eta) < 0.9)                    { return 0.000;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.001;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.000;}
  }
  if(pt > 40 && pt < 45){
    if(fabs(eta) < 0.9)                    { return 0.000;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.000;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.000;}
  }
  if(pt > 45 && pt < 50){
    if(fabs(eta) < 0.9)                    { return 0.000;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.000;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.000;}
  }
  if(pt > 50 && pt < 60){
    if(fabs(eta) < 0.9)                    { return 0.000;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.000;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.000;}
  }
  if(pt > 60 && pt < 90){
    if(fabs(eta) < 0.9)                    { return 0.000;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.001;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.000;}
  }
  if(pt > 90 && pt < 140){
    if(fabs(eta) < 0.9)                    { return 0.001;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.001;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.001;}
  }
  if(pt > 140){
    if(fabs(eta) < 0.9)                    { return 0.002;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.005;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.002;}
  }
  return 0.0;
}// end of SF_Err_MuonIso

float TScaleFactors::SF_MuonID(float pt, float eta){
  if(pt > 10 && pt < 20){
    if(fabs(eta) < 0.9)                    { return 0.970275;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 1.00173;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 1.01802;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 1.00504;}
  }
  if(pt > 20 && pt < 25){
    if(fabs(eta) < 0.9)                    { return 0.988865;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.993947;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 1.00035;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 0.998089;}
  }
  if(pt > 25 && pt < 30){
    if(fabs(eta) < 0.9)                    { return 0.992338;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.994722;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.998486;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 0.996183;}
  }
  if(pt > 30 && pt < 35){
    if(fabs(eta) < 0.9)                    { return 0.993283;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.993391;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.996558;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 1.00055;}
  }
  if(pt > 35 && pt < 40){
    if(fabs(eta) < 0.9)                    { return 0.993662;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.992285;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.996926;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 0.992563;}
  }
  if(pt > 40 && pt < 45){
    if(fabs(eta) < 0.9)                    { return 0.992392;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.99187;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.996962;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 0.995144;}
  }
  if(pt > 45 && pt < 50){
    if(fabs(eta) < 0.9)                    { return 0.992392;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.99187;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.996962;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 0.995144;}
  }
  if(pt > 50 && pt < 60){
    if(fabs(eta) < 0.9)                    { return 0.99119;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.99501;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.995183;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 0.99359;}
  }
  if(pt > 60 && pt < 90){
    if(fabs(eta) < 0.9)                    { return 0.989417;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.990406;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.992486;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 0.989484;}
  }
  if(pt > 90 && pt < 140){
    if(fabs(eta) < 0.9)                    { return 1.00375;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 1.00903;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 1.02313;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 1.06017;}
  }
  if(pt > 140){
    if(fabs(eta) < 0.9)                    { return 1.00375;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 1.00903;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 1.02313;}
    if(fabs(eta) > 2.1 && fabs(eta) < 2.4) { return 1.06017;}
  }
  return 1.0;
}//end of SF_MuonID

float TScaleFactors::SF_Err_MuonID(float pt, float eta){
  if(pt > 10 && pt < 20){
    if(fabs(eta) < 0.9)                    { return 0.005;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.007;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.004;}
  }
  if(pt > 20 && pt < 25){
    if(fabs(eta) < 0.9)                    { return 0.002;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.003;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.001;}
  }
  if(pt > 25 && pt < 30){
    if(fabs(eta) < 0.9)                    { return 0.001;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.001;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.001;}
  }
  if(pt > 30 && pt < 35){
    if(fabs(eta) < 0.9)                    { return 0.001;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.001;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.001;}
  }
  if(pt > 35 && pt < 40){
    if(fabs(eta) < 0.9)                    { return 0.000;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.001;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.001;}
  }
  if(pt > 40 && pt < 50){
    if(fabs(eta) < 0.9)                    { return 0.000;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.000;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.000;}
  }
  if(pt > 50 && pt < 60){
    if(fabs(eta) < 0.9)                    { return 0.001;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.001;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.001;}
  }
  if(pt > 60 && pt < 90){
    if(fabs(eta) < 0.9)                    { return 0.001;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.002;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.002;}
  }
  if(pt > 90 && pt < 140){
    if(fabs(eta) < 0.9)                    { return 0.003;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.006;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.005;}
  }
  if(pt > 140){
    if(fabs(eta) < 0.9)                    { return 0.017;}
    if(fabs(eta) > 0.9 && fabs(eta) < 1.2) { return 0.035;}
    if(fabs(eta) > 1.2 && fabs(eta) < 2.1) { return 0.030;}
  }
  return 0.0;
}//end of SF_Err_MuonID
