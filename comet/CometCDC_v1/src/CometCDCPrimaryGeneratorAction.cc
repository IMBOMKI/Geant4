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
// $Id: CometCDCPrimaryGeneratorAction.cc 75215 2013-10-29 16:07:06Z gcosmo $
// 
/// \file CometCDCPrimaryGeneratorAction.cc
/// \brief Implementation of the CometCDCPrimaryGeneratorAction class

#include "CometCDCPrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
//#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh"

#include "G4RunManager.hh"
#include "G4ParticleMomentum.hh"
#include "CometCDCChamberDescription.hh"
#include "CometCDCDataType.hh"

#include <assert.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CometCDCPrimaryGeneratorAction::CometCDCPrimaryGeneratorAction(const CometCDCChamberDescription * chamberDesc, CometCDCDataType * _data)
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0),
   fChamberDesc(chamberDesc),
   fData(_data)
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic
  //
  G4ParticleDefinition* particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleMomentum (105.*CLHEP::MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CometCDCPrimaryGeneratorAction::~CometCDCPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CometCDCPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore
  //
  /*
  G4double worldZHalfLength = 0;
  G4LogicalVolume* worlLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Box* worldBox = 0;
  if ( worlLV) worldBox = dynamic_cast< G4Box*>(worlLV->GetSolid()); 
  if ( worldBox ) {
    worldZHalfLength = worldBox->GetZHalfLength();  
  }
  else  {
    G4ExceptionDescription msg;
    msg << "World volume of box not found." << G4endl;
    msg << "Perhaps you have changed geometry." << G4endl;
    msg << "The gun will be place in the center.";
    G4Exception("CometCDCPrimaryGeneratorAction::GeneratePrimaries()",
      "MyCode0002", JustWarning, msg);
  } 
  */
  // Set gun position
  const int & ntargettot = fChamberDesc->numberOfTargets;
  int ntarget = (int)(G4UniformRand()* ntargettot);
  assert (0<= ntarget && ntarget < ntargettot);
  G4double x = G4RandGauss::shoot(0.*CLHEP::mm, 30./2.355*CLHEP::mm);
  G4double y = G4RandGauss::shoot(0.*CLHEP::mm, 30./2.355*CLHEP::mm);
  G4ThreeVector pos (x, y, fChamberDesc->targetPositions[ntarget]);
  fParticleGun->SetParticlePosition(pos);
  G4ParticleMomentum dir = G4RandomDirection();
  fParticleGun->SetParticleMomentumDirection(dir);
  G4ParticleMomentum mom = dir * fParticleGun->GetParticleMomentum();
  G4double time = fParticleGun->GetParticleTime();
  G4int isthep = 1;
  G4int idhep = fParticleGun->GetParticleDefinition()->GetPDGEncoding();

  fParticleGun->GeneratePrimaryVertex(anEvent);

  fData->addMCParticle (idhep, isthep, pos, mom, time);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

