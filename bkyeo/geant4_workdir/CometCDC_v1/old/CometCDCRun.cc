//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: CometCDCRun.cc 66536 2012-12-19 14:32:36Z ihrivnac $
//
/// \file CometCDCRun.cc
/// \brief Implementation of the CometCDCRun class

#include "CometCDCRun.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CometCDCRun::CometCDCRun()
: G4Run()
{
  fw = new TFile ("data.root", "recreate");
  hedep = new TH1F ("hedep", "edep", 200, 0, 200);
  ntgen = new TNtuple ("ntgen", "gen", "x:y:z:px:py:pz:time:idhep:isthep");
  ntrec = new TNtuple ("ntrec", "rec", "evtid:det:sec1:sec2:x:y:z:px:py:pz:t");
  
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CometCDCRun::~CometCDCRun()
{
  //if (h1) h1->Delete();
  //if (fw) fw->Delete();
} 

void CometCDCRun::writeFiles () const {
  fw->cd();
  hedep->Write();
  ntgen->Write();
  ntrec->Write();
  fw->Close();
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CometCDCRun::Merge(const G4Run* run)
{
  //const CometCDCRun* localRun = static_cast<const CometCDCRun*>(run);
  G4Run::Merge(run); 
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



