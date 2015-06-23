#include "modules/FatJet.h"

#include "classes/DelphesClasses.h"

#include "TObjArray.h"

FatJet::FatJet() :
  fItInputArray(0)
{
}

FatJet::~FatJet() {
}

void FatJet::Init() {
  fInputArray = ImportArray(GetString("InputArray", "FastJetFinder/jets"));
  fItInputArray = fInputArray->MakeIterator();
  fOutputArray = ExportArray(GetString("OutputArray", "constituents"));
}

void FatJet::Finish() {
  if(fItInputArray) delete fItInputArray;
}

void FatJet::Process() {
  Candidate *injet, *constituent;
  fItInputArray->Reset();
  injet = static_cast<Candidate*>(fItInputArray->Next());
  if (!injet) {
    return;
  }

  TObjArray *constituents = injet->GetCandidates();
  TIterator *constIt = constituents->MakeIterator();

  while ((constituent = static_cast<Candidate *>(constIt->Next()))) {
    fOutputArray->Add(constituent);
  }

  delete constIt;
}
