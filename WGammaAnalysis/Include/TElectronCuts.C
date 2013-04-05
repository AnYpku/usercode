#include "TElectronCuts.h" //class of this package

TElectronCuts::TElectronCuts(float elePt)
{
  elePt_=elePt;
}

TElectronCuts::~TElectronCuts()
{
}

bool TElectronCuts::Passed()
{
  return true;
}

bool TElectronCuts::MoreElectronsVeto()
{
  return true;
}
