#ifndef FatJetSelector_h
#define FatJetSelector_h

#include "classes/DelphesModule.h"

class TObjArray;
class TIterator;

class FatJetSelector: public DelphesModule
{
public:

  FatJetSelector();
  ~FatJetSelector();

  void Init();
  void Process();
  void Finish();
 
private:
  const TObjArray *fInputJetArray; //!
  const TObjArray *fInputPhotonArray; //!
  TIterator *fItInputJetArray; //!
  TObjArray *fOutputConstituentArray; //!
  TObjArray *fOutputJetArray; //!

  ClassDef(FatJetSelector, 1)
};

#endif
