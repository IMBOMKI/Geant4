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
// $Id: CometCDCSteppingAction.cc 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file CometCDCSteppingAction.cc
/// \brief Implementation of the CometCDCSteppingAction class

#include "CometCDCSteppingAction.hh"
#include "CometCDCEventAction.hh"
#include "CometCDCDetectorConstruction.hh"
#include "CometCDCDataType.hh"
#include "CometCDCChamberDescription.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4StepPoint.hh"
#include "CometCDCDataType.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//CometCDCSteppingAction::CometCDCSteppingAction(CometCDCEventAction* eventAction, const CometCDCChamberDescription * _chamberDesc)
CometCDCSteppingAction::CometCDCSteppingAction(const CometCDCChamberDescription * _chamberDesc, CometCDCDataType * _data)
: G4UserSteppingAction(),
  fChamberVolume(0),
  fHodoscopeVolume(0),
  fChamberDesc(_chamberDesc),
  fData (_data)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CometCDCSteppingAction::~CometCDCSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CometCDCSteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fChamberVolume) { 
    const CometCDCDetectorConstruction* detectorConstruction
      = static_cast<const CometCDCDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fChamberVolume = detectorConstruction->getChamberVolume();   
  }
  if (!fHodoscopeVolume) { 
    const CometCDCDetectorConstruction* detectorConstruction
      = static_cast<const CometCDCDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fHodoscopeVolume = detectorConstruction->getHodoscopeVolume();   
  }

  assert (fChamberDesc);

  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
      
  // check if we are in scoring volume
  G4StepPoint * prestep = step->GetPreStepPoint();
  const G4ThreeVector & pos = prestep->GetPosition();
  DetectorType det = UNDEFINED_DETECTOR;
  if (volume == fChamberVolume) {
    G4double rho = pos.perp();
    if (rho < fChamberDesc->getEndPlateRadius(1) ||
        rho > fChamberDesc->getEndPlateRadius(NLAYERS-2)) return;
    det = CHAMBER; 
  }
  else if (volume == fHodoscopeVolume) det = HODOSCOPE;
  else return; 
  std::pair<int,int> sec = getSection (det, pos);

  int idhep = 0;
  bool isPrimary = false;
  G4Track * track = step->GetTrack();
  if (!track) {
    G4cerr << "Error: Cannot find G4Track for step " << G4endl;
  }
  else {
    idhep = track->GetParticleDefinition()->GetPDGEncoding();
    if (track->GetParentID() == 0) isPrimary = true;
  }

    
  fData->addStepPoint(det, sec.first, sec.second, prestep->GetPosition(), prestep->GetMomentum(), prestep->GetGlobalTime(), step->GetTotalEnergyDeposit(), idhep, isPrimary); 

  G4LogicalVolume* volumePost
    = step->GetPostStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
  //if (volumePost != volume) {
  //  G4StepPoint * poststep = step->GetPostStepPoint();
  //  fData->addStepPoint(det, sec.first, sec.second, poststep->GetPosition(), poststep->GetMomentum(), poststep->GetGlobalTime(), 0);
  //}
    
}


std::pair<int,int> CometCDCSteppingAction::getSection (DetectorType det, G4ThreeVector pos) {
  if (det == CHAMBER) {
    std::pair<int,int> slrange;
    slrange.first = -1;
    slrange.second = -2;
    G4double rho = pos.perp();
    const std::vector<G4int> & sl = fChamberDesc->senseLayers;
    const std::vector<G4double> & slr = fChamberDesc->senseLayerRadius;

    if (rho < *(slr.begin())) {
      slrange.first  = *(sl.begin());
      slrange.second = *(sl.begin());
    }
    else if (rho >= *(slr.rbegin())) {
      slrange.first  = *(sl.rbegin());
      slrange.second = *(sl.rbegin());
    }
    else {
      for (unsigned int i = 1 ; i < sl.size() ; ++i) {
        if (slr[i-1] <= rho && rho < slr[i]) {
          slrange.first  = sl[i-1];
          slrange.second = sl[i];
          break;
        }
      }
    }
    if (slrange.first <0 || slrange.second < slrange.first) {
      G4cerr << "Warning: cannot find corresponding sense layer at CometCDCSteppingAction::UserSteppingAction." << G4endl;
      return std::pair<int,int>(-1,-1);
    }

    G4double cdist2=1.e99;
    int clay=-1, cwir=-1;
    for (int lay = slrange.first ; lay <= slrange.second ; ++lay) {
      if (fChamberDesc->getWireType(lay) != SENSE_WIRE) continue;
      for (int wir = 0 ; wir < fChamberDesc->getNumberOfWires(lay) ; ++wir) {
        G4double phi = pos.phi();
        G4double phim2pi = phi-CLHEP::twopi;
        G4double phip2pi = phi+CLHEP::twopi;
        const WireDataType & thisWire = fChamberDesc->getWire(lay, wir);
        if ( (thisWire.phi0 < phi && phi < thisWire.phi1) ||
             (thisWire.phi0 < phim2pi && phim2pi < thisWire.phi1) ||
             (thisWire.phi0 < phip2pi && phip2pi < thisWire.phi1) ) {

          G4ThreeVector x1_m_x0 = thisWire.StartPosition - pos;
          G4ThreeVector x2_m_x1 = thisWire.StartPosition - thisWire.EndPosition;
          G4double t = -1. * x1_m_x0.dot(x2_m_x1) / x2_m_x1.mag2();
          G4double dx2 = x1_m_x0.x() + t * x2_m_x1.x();
          G4double dy2 = x1_m_x0.y() + t * x2_m_x1.y();
          G4double dz2 = x1_m_x0.z() + t * x2_m_x1.z();
          G4double dist2 = dx2*dx2 + dy2*dy2 + dz2*dz2; // see Wolfram MathWorld "point-line distance --  3-dimensional
          if (dist2 < cdist2) {
            cdist2 = dist2;
            clay = lay;
            cwir = wir;
          }
        }
      }
    }

    int sec1 = clay;
    int sec2 = cwir;
    return std::pair<int,int>(sec1,sec2);
  }
  else if (det == HODOSCOPE) {
    int sec1, sec2;
    if (pos.z() <0) sec1 = 0;
    else sec1 = 1;
    G4double phi = pos.phi();
    while (phi<0) {
      phi += CLHEP::twopi;
    }
    while (phi>= CLHEP::twopi) {
      phi -= CLHEP::twopi;
    }
    sec2 = (int) (phi / CLHEP::twopi * fChamberDesc->numberOfHodoscopeSection);
    return std::pair<int,int>(sec1,sec2);
  }
  else {
    return std::pair<int,int>(-1,-1);
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

