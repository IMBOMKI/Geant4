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
// $Id: CometCDCSensitiveDetector.cc 75215 2013-10-29 16:07:06Z gcosmo $
//
/// \file CometCDCSensitiveDetector.cc
/// \brief Implementation of the CometCDCSensitiveDetector class

#include "CometCDCSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CometCDCSensitiveDetector::CometCDCSensitiveDetector(
                            const G4String& name, 
                            const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHitsCollection(0)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CometCDCSensitiveDetector::~CometCDCSensitiveDetector() 
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CometCDCSensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection 
    = new CometCDCHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  // Create hits
  // fNofCells for cells + one more for total sums 
  //for (G4int i=0; i<fNofCells+1; i++ ) {
  //  fHitsCollection->insert(new CometCDCHit());
  //}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool CometCDCSensitiveDetector::ProcessHits(G4Step* step, 
                                     G4TouchableHistory*)
{  
  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();

  if (edep == 0.) return false;

  G4StepPoint * sp = step->GetPreStepPoint();

  //process for stepPoint here
  
  
  // Get hit accounting data for this cell
  CometCDCHit* hit = (*fHitsCollection)[layerNumber];
  if ( ! hit ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hit " << layerNumber; 
    G4Exception("CometCDCSensitiveDetector::ProcessHits()",
      "MyCode0004", FatalException, msg);
  }         

  // Get hit for total accounting
  CometCDCHit* hitTotal 
    = (*fHitsCollection)[fHitsCollection->entries()-1];
  
  // Add values
  hit->Add(edep, stepLength);
  hitTotal->Add(edep, stepLength); 
      
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CometCDCSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
  /*if ( verboseLevel>1 ) { 
     G4int nofHits = fHitsCollection->entries();
     G4cout << "\n-------->Hits Collection: in this event they are " << nofHits 
            << " hits in the tracker chambers: " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
