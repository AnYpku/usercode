#include "CalcCrossSection.h"

void AuxCrossSection(TConfiguration::AnalysisParameters &anPars, int channel, int vgamma)
{
  CalcCrossSection cs(anPars.channel, anPars.vgamma, anPars.blind[channel][vgamma], anPars.configfile);
  cs.Calc();
}//end of AuxCrossSection
