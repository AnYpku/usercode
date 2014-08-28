#ifndef TAllInputSamples_h
#define TAllInputSamples_h

#include "TInputSample.h"
#include <vector>
#include <string>

class TAllInputSamples
{
  public:
    TAllInputSamples();
    TAllInputSamples(int channel, int vgamma, string configFile);
    virtual ~TAllInputSamples();

    void ReadConfig(int channel, int vgamma, string configFile);
    bool ReadSampleGeneralInfo(int channel, int vgamma, string line);
    void ReadFileSpecificInfo(string line);

    int ChannelNumber (string channelStr);
    int SampleNumber  (string sampleStr);
    int VGammaNumber  (string vgammaStr);

    void CalcLumiWeights();

    void Print();
    

    int nSources_;
    vector <TInputSample> allInputs_;

};

#endif //#ifndef TAllInputSamples_h
