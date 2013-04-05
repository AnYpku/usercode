#ifndef TElectronCuts_h
#define TElectronCuts_h

class TElectronCuts
  {
     public:
       TElectronCuts (float elePt);
       virtual ~TElectronCuts();
       bool Passed();
       bool MoreElectronsVeto();

     private:
       float elePt_;
  };

#endif //#ifndef TElectronCuts_h
