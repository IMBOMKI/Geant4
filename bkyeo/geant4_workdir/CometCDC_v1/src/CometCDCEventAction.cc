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
// $Id: CometCDCEventAction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file CometCDCEventAction.cc
/// \brief Implementation of the CometCDCEventAction class

#include "CometCDCEventAction.hh"
#include "CometCDCDataType.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "TH1.h"
#include "TNtuple.h"
//#include "G4EventManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CometCDCEventAction::CometCDCEventAction(CometCDCDataType * _data)
: G4UserEventAction(),
  fData(_data)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CometCDCEventAction::~CometCDCEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CometCDCEventAction::BeginOfEventAction(const G4Event*)
{    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CometCDCEventAction::EndOfEventAction(const G4Event* event )
{   

  G4int evtid = event->GetEventID();

  fData->fillGenParticleNtuple(evtid);
  fData->reconstructHitmapData(evtid, CHAMBER);
  fData->reconstructHitmapData(evtid, HODOSCOPE);
  std::pair<int,int> nsp = fData->fillStepPointsNtuple(evtid);
  fData->reconstructStereoHit(); //TODO
  fData->fillDataTree(evtid);
  fData->clear();

  if (
      (evtid<10) ||
      (evtid<100 && evtid%10 == 0)||
      (evtid<1000 && evtid%100 == 0)||
      (evtid<10000 && evtid%1000 == 0)||
      (evtid<100000 && evtid%10000 == 0)||
      (evtid%100000 == 0) )
    G4cout << "Event " << evtid <<  ", " << nsp.first << ", " << nsp.second  << " points saved" << G4endl;
  
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
