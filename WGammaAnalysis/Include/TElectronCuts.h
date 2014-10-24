#ifndef TElectronCuts_h
#define TElectronCuts_h
#include "TEventTree.h"
#include <vector>

class TElectronCuts
  {
     public:
       TElectronCuts ();
       virtual ~TElectronCuts();
       bool HasMoreElectrons(int nEle, int iele, vector <float> *elePt, vector <float> *eleEta);
       bool EleID2012(TEventTree::InputTreeLeaves& leaf, int iele);
       float GetEffectiveArea(float SCEta);

     private:
  };

#endif //#ifndef TElectronCuts_h
