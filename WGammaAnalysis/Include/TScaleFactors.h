#ifndef TScaleFactors_h
#define TScaleFactors_h

#include "TFile.h"
#include "TH1D.h"
#include "TString.h"
 //ROOT

class TScaleFactors
{
  public:
    TScaleFactors();
    virtual ~TScaleFactors();

    float SF_MediumPhoID(float pt, float eta);
    float SF_MediumEleID(float pt, float eta);
    float SF_MuonIso(float pt, float eta);
    float SF_MuonID(float pt, float eta);

    float SF_Err_MediumPhoID(float pt, float eta);
    float SF_Err_MuonIso(float pt, float eta);
    float SF_Err_MuonID(float pt, float eta);

};
#endif //TScaleFactors_h
