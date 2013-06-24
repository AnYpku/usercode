#ifndef TMathTools_h
#define TMathTools_h

#include <string>

class TMathTools
{
  public:
    TMathTools();
    virtual ~TMathTools();
    float DeltaR (float eta1, float phi1, float eta2, float phi2);
    float ErrOfTwoIndependent(string type, float x1, float x2, float er1, float er2);
};






#endif