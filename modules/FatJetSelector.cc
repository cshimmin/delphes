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
  Candidate *injet, *constituent;
  fItInputJetArray->Reset();

  // get the leading photon
  Candidate *photon;

  if (fInputJetArray->IsEmpty()) {
    std::cout << "no jets." << std::endl;
    return;
  }

  if (fInputPhotonArray->IsEmpty()) {
    std::cout << "no photons." << std::endl;
    return;
  }
  
  TIter it(fInputPhotonArray);
  TLorentzVector vph;
  while ((photon = static_cast<Candidate *>(it.Next()))) {
    if (photon->Momentum.Pt() > vph.Pt()) {
      vph = photon->Momentum;
    }
  }
  std::cout << "max ph pt = " << vph.Pt() << " GeV" << std::endl;

  double min_ptdiff = 1e9;
  Candidate *best_jet = 0;
  double best_dphi = 0;
  while ((injet = static_cast<Candidate*>(fItInputJetArray->Next()))) {
    if (vph.DeltaR(injet->Momentum) < 0.5) {
      continue;
    }
    double dphi = TMath::Abs(TMath::Pi() - TMath::Abs(TMath::Pi() - TMath::Abs(vph.Phi() - injet->Momentum.Phi())));
    double ptdiff = TMath::Abs(injet->Momentum.Pt() - vph.Pt());
    if (ptdiff < min_ptdiff) {
      min_ptdiff = ptdiff;
      best_jet = injet;
      best_dphi = dphi;
    }
  }
  if (! best_jet) { 
    std::cout << " -- no suitable jet --" << std::endl;
    return;
  }
  std::cout << "best jet pt = " << best_jet->Momentum.Pt() << " GeV" << std::endl;
  std::cout << "best jet dphi = " << best_dphi << std::endl;
  std::cout << "best jet dR = " << best_jet->Momentum.DeltaR(vph) << std::endl;
  std::cout << std::endl;

  TObjArray *constituents = best_jet->GetCandidates();
  TIter constIt(constituents);

  fOutputJetArray->Add(best_jet);

  while ((constituent = static_cast<Candidate *>(constIt.Next()))) {
    fOutputConstituentArray->Add(constituent);
  }
}
