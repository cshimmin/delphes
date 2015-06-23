#include "classes/DelphesModule.h"

class TObjArray;
class TIterator;

class FatJet: public DelphesModule
{
public:

  FatJet();
  ~FatJet();

  void Init();
  void Process();
  void Finish();
 
private:
  const TObjArray *fInputArray; //!
  TIterator *fItInputArray; //!
  TObjArray *fOutputArray; //!
};
