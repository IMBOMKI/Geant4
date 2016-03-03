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
// $Id: CometCDCActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file CometCDCActionInitialization.cc
/// \brief Implementation of the CometCDCActionInitialization class

#include "CometCDCActionInitialization.hh"
#include "CometCDCPrimaryGeneratorAction.hh"
#include "CometCDCRunAction.hh"
#include "CometCDCEventAction.hh"
#include "CometCDCSteppingAction.hh"
#include "CometCDCDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CometCDCActionInitialization::CometCDCActionInitialization(const CometCDCChamberDescription * _chamberDesc, CometCDCDataType * _data)
 : G4VUserActionInitialization(),
  fChamberDesc (_chamberDesc),
  fData(_data)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CometCDCActionInitialization::~CometCDCActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CometCDCActionInitialization::BuildForMaster() const
{
  SetUserAction(new CometCDCRunAction(fData));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CometCDCActionInitialization::Build() const
{
  SetUserAction(new CometCDCPrimaryGeneratorAction(fChamberDesc, fData));
  SetUserAction(new CometCDCRunAction (fData));
  CometCDCEventAction * eventAction = new CometCDCEventAction(fData);
  SetUserAction(eventAction);
  //SetUserAction(new CometCDCSteppingAction(eventAction, fChamberDesc));
  SetUserAction(new CometCDCSteppingAction(fChamberDesc, fData));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
