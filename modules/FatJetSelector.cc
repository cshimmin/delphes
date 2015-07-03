#include "modules/FatJetSelector.h"

#include "classes/DelphesClasses.h"

#include "TObjArray.h"

#include <iostream>

FatJetSelector::FatJetSelector() :
  fItInputJetArray(0)
{
}

FatJetSelector::~FatJetSelector() {
}

void FatJetSelector::Init() {
  fInputJetArray = ImportArray(GetString("InputJetArray", "FastJetFinder/jets"));
  fInputPhotonArray = ImportArray(GetString("InputPhotonArray", "Calorimeter/photons"));

  fItInputJetArray = fInputJetArray->MakeIterator();

  fOutputConstituentArray = ExportArray(GetString("OutputConstituentArray", "constituents"));
  fOutputJetArray = ExportArray(GetString("OutputJetArray", "fatjet"));
}

void FatJetSelector::Finish() {
  if(fItInputJetArray) delete fItInputJetArray;
}

void FatJetSelector::Process() {
  fItInputJetArray->Reset();

  if (fInputJetArray->IsEmpty()) {
    return;
  }

  if (fInputPhotonArray->IsEmpty()) {
    return;
  }

  // Get the leading photon as the reference object.
  TLorentzVector vph;
  TIter it(fInputPhotonArray);
  Candidate *photon;
  while ((photon = static_cast<Candidate *>(it.Next()))) {
    if (photon->Momentum.Pt() > vph.Pt()) {
      vph = photon->Momentum;
    }
  }

  // the best jet is the one which has pT closest to the reference photon
  Candidate *best_jet = 0;
  double min_ptdiff = 1e9;

  Candidate *injet;
  while ((injet = static_cast<Candidate*>(fItInputJetArray->Next()))) {
    // only consider jets more than 0.5 away from the photon in dR
    if (vph.DeltaR(injet->Momentum) < 0.5) {
      continue;
    }
    double ptdiff = TMath::Abs(injet->Momentum.Pt() - vph.Pt());
    if (ptdiff < min_ptdiff) {
      min_ptdiff = ptdiff;
      best_jet = injet;
    }
  }
  if (! best_jet) { 
    std::cout << " -- no suitable jet --" << std::endl;
    return;
  }

  // output the winning jet object
  fOutputJetArray->Add(best_jet);

  // get the tower constituents from the best jet object
  TObjArray *constituents = best_jet->GetCandidates();
  TIter constIt(constituents);

  // and add them to the output constituent array
  Candidate *constituent;
  while ((constituent = static_cast<Candidate *>(constIt.Next()))) {
    fOutputConstituentArray->Add(constituent);
  }
}
