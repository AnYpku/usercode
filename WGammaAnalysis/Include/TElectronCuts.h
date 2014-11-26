#ifndef TElectronCuts_h
#define TElectronCuts_h
#include "TEventTree.h"
#include "../Configuration/TConfiguration.h"
#include <vector>

class TElectronCuts
  {
     public:
       TElectronCuts ();
       virtual ~TElectronCuts();
       enum {ELE_VETO=0, ELE_LOOSE=1, ELE_MEDIUM=2, ELE_TIGHT=3};
       bool HasMoreElectrons(TEventTree::InputTreeLeaves& leaf, int iele);
       bool PassedKinematics(float pt, float eta);
       bool EleID2012(TEventTree::InputTreeLeaves& leaf, int iele, int wp);
       float GetCorrectedIsolation(TEventTree::InputTreeLeaves& leaf, int iele);
       float GetEffectiveArea(float eta);
       bool IsBarrel(float eta);
       bool IsEndcap(float eta);

     private:
       TConfiguration _config;
  };

  float _elePtCut=30;

  float _dEtaIn_Cut[2][4]={0.007,0.007,0.004,0.004, //BARREL
                           0.010,0.009,0.007,0.005};//ENDCAP
    //eledEtaAtVtx

  float _dPhiIn_Cut[2][4]={0.8,0.15,0.06,0.03, //BARREL
                           0.7,0.10,0.03,0.02};//ENDCAP
    //eledPhiAtVtx

  float _sigmaIEtaIEta_Cut[2]={0.01,0.03}; //all WPs; B,E
    //eleSigmaIEtaIEta

  float _HoverE_Cut[2][4]={0.15,0.12,0.12,0.12, //BARREL
                           1000,0.10,0.10,0.10};//ENDCAP
    //eleHoverE12

  float _d0_vtx_Cut[4]={0.04,0.02,0.02,0.02};
    //eleD0Vtx, same for BARREL and ENDCAP
  float _dZ_vtx_Cut[4]={0.2,0.2,0.1,0.1};
    //eleDzVtx, same for BARREL and ENDCAP



  float _PFisoOvPT03_Cut[4]={0.15,0.15,0.15,0.10};
    //BARREL and ENDCAP for pt>20

  float _PFisoOvPT03_E_PTless20_Cut[4]={0.15,0.10,0.10,0.07};
    //BARREL and ENDCAP for pt<20

  float _1ovE_1ovp_Cut=0.05; //except VETO
    // 1/eleEcalEn-1/elePt, same for BARREL and ENDCAP
  float _vertex_fit_probability_Cut=10e-6; //except VETO
    //eleConvVtxFit, same for BARREL and ENDCAP
  int   _missing_hits_Cut[4]={10000,1,1,0};
    //eleMissHits, same for BARREL and ENDCAP, no cut for VETO



#endif //#ifndef TElectronCuts_h
