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
// $Id: CometCDCDetectorConstruction.cc 77601 2013-11-26 17:08:44Z gcosmo $
// 
/// \file CometCDCDetectorConstruction.cc
/// \brief Implementation of the CometCDCDetectorConstruction class

#include "CometCDCDetectorConstruction.hh"
//#include "CometCDCSensitiveDetector.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Polycone.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "CometCDCChamberDescription.hh"
#include "CometCDCField.hh"

#include <assert.h>
#include <cmath>
//#include "TString.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* CometCDCDetectorConstruction::fMagFieldMessenger = 0; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CometCDCDetectorConstruction::CometCDCDetectorConstruction(const CometCDCChamberDescription * _chamberDesc)
 : G4VUserDetectorConstruction(),
   fCheckOverlaps(true),
   chamberDesc(_chamberDesc)
{
  //fField = new CometCDCField ();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CometCDCDetectorConstruction::~CometCDCDetectorConstruction()
{ 
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* CometCDCDetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CometCDCDetectorConstruction::DefineMaterials()
{ 
  // Lead material defined using NIST Manager
  G4NistManager* nistManager = G4NistManager::Instance();
  G4Material * Al = nistManager->FindOrBuildMaterial("G4_Al");
  G4Material * Mylar = nistManager->FindOrBuildMaterial("G4_MYLAR");
  nistManager->FindOrBuildMaterial("G4_W");

  G4double density;
  G4int nel;
  G4int natoms;

  density = 0.95 * Mylar->GetDensity() + 0.05 * Al->GetDensity();
  G4Material *AlMylar = new G4Material ("AlMylar", density, nel = 2, kStateSolid);
  AlMylar->AddMaterial (Al, 0.05);
  AlMylar->AddMaterial (Mylar, 0.95);

 
  G4double fractionHe = 90.*CLHEP::perCent;
  G4double fractionIsoButane = 1. - fractionHe;

  G4double densityHe = 1.663e-4 * CLHEP::g/CLHEP::cm3;
  G4double densityIsoButane = 2.489e-3  * CLHEP::g/CLHEP::cm3; 
  density = fractionHe * densityHe + fractionIsoButane * densityIsoButane;
  G4double fractionMassHe = fractionHe * densityHe / density;
  G4double fractionMassIsoButane = fractionIsoButane * densityIsoButane / density;

  G4Material * CDCGas = new G4Material ("CDCGas", density, nel=2, kStateGas);
  G4Material *He = nistManager->FindOrBuildMaterial ("G4_He");
  G4Material *IsoButane = nistManager->FindOrBuildMaterial ("G4_BUTANE");
  assert (He);
  assert (IsoButane);
  CDCGas->AddMaterial (He, fractionMassHe);
  CDCGas->AddMaterial (IsoButane, fractionMassIsoButane);

  //G4Element *Hydrogen = nistManager->FindOrBuildElement ("G4_H"); //
  //G4Element *Oxyzen = nistManager->FindOrBuildElement ("G4_O");
  //G4Element *Carbon = nistManager->FindOrBuildElement ("G4_C");
  G4Element *Hydrogen = new G4Element ("Hydrogen", "H", 1., 1.01*CLHEP::g/CLHEP::mole); 
  G4Element *Carbon   = new G4Element ("Carbon",   "C", 6., 12.01*CLHEP::g/CLHEP::mole);
  G4Element *Oxyzen   = new G4Element ("Oxyzen",   "O", 8., 16.00*CLHEP::g/CLHEP::mole);
  G4Material* CFresin = new G4Material("CFresin", density = 1.1*CLHEP::g/CLHEP::cm3, nel=3);
  assert (Hydrogen);
  assert (Oxyzen);
  assert (Carbon);
  CFresin->AddElement(Carbon, natoms=5);
  CFresin->AddElement(Hydrogen, natoms=5);
  CFresin->AddElement(Oxyzen, natoms=2);
  
  G4Material * CFibers = new G4Material ("CFibers", density = 1.8*CLHEP::g/CLHEP::cm3, nel=1);
  CFibers->AddElement(Carbon, natoms = 1);

  G4double fiberFrac=46.0*CLHEP::perCent;
  G4Material* CarbonFiber = new G4Material("CarbonFiber", 
      density = fiberFrac*CFibers->GetDensity()+(1.0-fiberFrac)*CFresin->GetDensity(),
      nel=2);
  CarbonFiber->AddMaterial(CFibers, fiberFrac );
  CarbonFiber->AddMaterial(CFresin, (1.0-fiberFrac) ); //

  nistManager->FindOrBuildMaterial ("G4_Galactic");   

  nistManager->FindOrBuildMaterial ("G4_POLYSTYRENE");

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* CometCDCDetectorConstruction::DefineVolumes()
{


  //Material
  G4Material *Al = G4Material::GetMaterial ("G4_Al");
  //G4Material *Tungsten = G4Material::GetMaterial ("G4_W");
  G4Material *AlMylar = G4Material::GetMaterial ("AlMylar");
  G4Material *CDCGas = G4Material::GetMaterial ("CDCGas");
  G4Material *vacuume = G4Material::GetMaterial ("G4_Galactic");
  G4Material *carbonFiber = G4Material::GetMaterial ("CarbonFiber");
  G4Material *polyStyrene = G4Material::GetMaterial ("G4_POLYSTYRENE");
  assert (Al);
  assert (AlMylar);
  assert (CDCGas);
  assert (vacuume);
  assert (carbonFiber);
  assert (polyStyrene);

  //World
  assert (chamberDesc);
  G4double worldHalfLength = chamberDesc->worldHalfLength;
  G4VSolid *worldS = new G4Box ("World", worldHalfLength, worldHalfLength, worldHalfLength);
  worldL = new G4LogicalVolume (worldS, vacuume, "World");
  G4VPhysicalVolume * worldP = new G4PVPlacement (0, G4ThreeVector(), worldL, "World", 0, false, 0, fCheckOverlaps);

  //Chamber
  G4double chamberRO = chamberDesc->chamberRO;
  G4double chamberRI = chamberDesc->chamberRI;
  G4double chamberHLO = chamberDesc->chamberHLO;
  G4double chamberHLI = chamberDesc->chamberHLI;
  G4double chamberSectionZ[4] = {-chamberHLO, -chamberHLI, chamberHLI, chamberHLO};
  G4double chamberSectionRI[4] = {chamberRO, chamberRI, chamberRI, chamberRO};
  G4double chamberSectionRO[4] = {chamberRO, chamberRO, chamberRO, chamberRO}; 
  G4VSolid * chamberS = new G4Polycone ("Chamber", 0, CLHEP::twopi, 4, chamberSectionZ, chamberSectionRI, chamberSectionRO); 
  chamberL = new G4LogicalVolume (chamberS, CDCGas, "Chamber");
  new G4PVPlacement (0, G4ThreeVector(), chamberL, "Chamber", worldL, false, 0, fCheckOverlaps); 


  //Chamber walls, end plate
  G4double chamberOWallT = chamberDesc->chamberOWallT;
  G4double chamberIWallT = chamberDesc->chamberIWallT;
  G4double chamberEPlateT = chamberDesc->chamberEPlateT;
  G4double chamberEPlateAngle = atan ((chamberHLO - chamberHLI) / (chamberRO - chamberRI));
  G4double chamberEPlateDeltaR = chamberEPlateT / std::sin (chamberEPlateAngle);
  G4double chamberEPlateDeltaZ = chamberEPlateT / std::cos (chamberEPlateAngle);

  //outer wall
  G4VSolid * chamberOuterWallS = new G4Tubs ("ChamberOuterWall", chamberRO, chamberRO+chamberOWallT,  chamberHLO, 0, CLHEP::twopi);
  G4LogicalVolume * chamberOuterWallL = new G4LogicalVolume (chamberOuterWallS, carbonFiber, "ChamberOuterWall");
  new G4PVPlacement (0, G4ThreeVector(), chamberOuterWallL, "ChamberOuterWall", worldL, false, 0, fCheckOverlaps);

  //inner wall
  G4VSolid * chamberInnerWallS = new G4Tubs ("ChamberInnerWall", chamberRI-chamberIWallT, chamberRI, chamberHLI+chamberEPlateDeltaZ, 0, CLHEP::twopi);
  G4LogicalVolume * chamberInnerWallL = new G4LogicalVolume (chamberInnerWallS, AlMylar, "ChamberInnerWall");
  new G4PVPlacement (0, G4ThreeVector(), chamberInnerWallL, "ChamberInnerWall", worldL, false, 0, fCheckOverlaps);

  //end plate
  G4double chamberEPlateSectionRI[3] = {chamberRO - chamberEPlateDeltaR, chamberRI, chamberRI};
  G4double chamberEPlateSectionRO[3] = {chamberRO, chamberRI + chamberEPlateDeltaR, chamberRI};
  G4double chamberEPlateSectionZ[3] = {chamberHLI - chamberHLO, -chamberEPlateDeltaZ, 0};
  G4VSolid * chamberEndPlateS = new G4Polycone ("ChamberEndPlate", 0, CLHEP::twopi, 3, chamberEPlateSectionZ, chamberEPlateSectionRI, chamberEPlateSectionRO);
  G4LogicalVolume *chamberEndPlateL = new G4LogicalVolume (chamberEndPlateS, Al, "ChamberEndPlate");
  new G4PVPlacement (0, G4ThreeVector(0, 0, -chamberHLI), chamberEndPlateL, "ChamberEndPlate0", worldL, false, 0, fCheckOverlaps);
  G4RotationMatrix *rot = new G4RotationMatrix(0, CLHEP::pi, 0);
  new G4PVPlacement (rot, G4ThreeVector(0, 0, chamberHLI), chamberEndPlateL, "ChamberEndPlate1", worldL, false, 0, fCheckOverlaps);

  // Proton absorber
  G4double protonAbsorberHalfLength = chamberDesc->protonAbsorberHalfLength;
  G4double protonAbsorberThickness  = chamberDesc->protonAbsorberThickness;
  G4double protonAbsorberInnerRadius = chamberDesc->protonAbsorberInnerRadius;
  G4VSolid * protonAbsorberS = new G4Tubs ("ProtonAbsorber", protonAbsorberInnerRadius, protonAbsorberInnerRadius + protonAbsorberThickness, protonAbsorberHalfLength, 0, CLHEP::twopi);
  G4LogicalVolume * protonAbsorberL = new G4LogicalVolume (protonAbsorberS, carbonFiber, "ProtonAbsorber");
  new G4PVPlacement (0, G4ThreeVector(), protonAbsorberL, "ProtonAbsorber", worldL, false, 0, fCheckOverlaps);

  //Hodoscope
  G4double hodoscopeHalfLength = chamberDesc->hodoscopeHalfLength;
  G4double hodoscopeInnerRadius = chamberDesc-> hodoscopeInnerRadius;
  G4double hodoscopeOuterRadius = chamberDesc->hodoscopeOuterRadius;
  G4double hodoscopeZCenter = chamberDesc->hodoscopeZCenter;
  G4VSolid *hodoscopeS = new G4Tubs ("Hodoscope", hodoscopeInnerRadius, hodoscopeOuterRadius, hodoscopeHalfLength, 0, CLHEP::twopi);
  hodoscopeL = new G4LogicalVolume (hodoscopeS, polyStyrene, "Hodoscope");
  new G4PVPlacement (0, G4ThreeVector(0, 0, -1.*hodoscopeZCenter), hodoscopeL, "Hodoscope0", worldL, false, 0, fCheckOverlaps);
  new G4PVPlacement (0, G4ThreeVector(0, 0, hodoscopeZCenter), hodoscopeL, "Hodoscope1", worldL, false, 0, fCheckOverlaps);

  //Stopping target
  G4double targetRadius = chamberDesc->targetRadius;
  G4double targetThickness = chamberDesc->targetThickness;
  G4VSolid *targetS = new G4Tubs ("Target", 0, targetRadius, 0.5*targetThickness, 0, CLHEP::twopi);
  G4LogicalVolume * targetL = new G4LogicalVolume (targetS, Al, "Target");
  G4int ntarget = 0;
  for (std::vector<G4double>::const_iterator it = chamberDesc->targetPositions.begin() ; it != chamberDesc->targetPositions.end() ; ++it) {
    new G4PVPlacement (0, G4ThreeVector(0.,0.,*it),  targetL, "Target", worldL, false, ntarget, fCheckOverlaps);
    ++ntarget;
  }


  //wires
/*  CometCDCChamberDescription wireDesc(chamberRI, chamberRO, chamberHLI, chamberHLO);
  wireDesc.print();

  G4VSolid * wireS[NLAYERS];
  G4LogicalVolume *wireL[NLAYERS];
  for (int i = 0 ; i < NLAYERS ; ++i) {
    G4double hl = (wireDesc.getWire(i,0).StartPosition - wireDesc.getWire(i,0).EndPosition).mag() * 0.5;
    TString name = TString::Format("Wire_layer%02d",i);
    if (wireDesc.getWireType(i) == SENSE_WIRE) {
      wireS[i] = new G4Tubs (name.Data(), 0, 0.5 * 30. * CLHEP::micrometer , hl, 0, CLHEP::twopi);
      wireL[i] = new G4LogicalVolume(wireS[i], Tungsten, name.Data());
    }
    else {
      wireS[i] = new G4Tubs (name.Data(), 0, 0.5 * 30. * CLHEP::micrometer , hl, 0, CLHEP::twopi);
      wireL[i] = new G4LogicalVolume(wireS[i], Al, name.Data());
    }
    for (int j = 0 ; j <wireDesc.getNumberOfWires(i) ; ++j)  {
      WireDataType thisWire = wireDesc.getWire(i,j);
      G4ThreeVector trans = 0.5* (thisWire.StartPosition + thisWire.EndPosition);
      G4RotationMatrix *rotation = new G4RotationMatrix();
      //G4ThreeVector vp = (thisWire.StartPosition - thisWire.EndPosition).unit();
      //G4ThreeVector v  (0,0,1);
      //rotation->setRows(vp.x()*v, vp.y()*v, vp.z()*v);
      new G4PVPlacement (rotation, trans, wireL[i], name.Data(), worldL, false, j, fCheckOverlaps);  
      //new G4PVPlacement (rotation, trans, wireL[i], name.Data(), worldL, false, j);
    }
  }*/


  // Visualization attributes
  worldL->SetVisAttributes (G4VisAttributes::Invisible);

  G4VisAttributes* chamberVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0)); chamberVisAtt->SetVisibility(true);
  chamberL->SetVisAttributes(chamberVisAtt);

  /*G4VisAttributes* chamberWallVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0)); chamberWallVisAtt->SetVisibility(true);
  chamberOuterWallL->SetVisAttributes(chamberWallVisAtt);
  chamberInnerWallL->SetVisAttributes(chamberWallVisAtt);

  G4VisAttributes* chamberEndPlateVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0)); chamberEndPlateVisAtt->SetVisibility(true);
  chamberEndPlateL->SetVisAttributes(chamberEndPlateVisAtt);*/

  G4VisAttributes* targetVisAtt= new G4VisAttributes(G4Colour(1.0,0,0)); targetVisAtt->SetVisibility(true);
  targetL->SetVisAttributes(targetVisAtt);

  G4VisAttributes* hodoscopeVisAtt= new G4VisAttributes(G4Colour(0,0,1.0)); hodoscopeVisAtt->SetVisibility(true);
  hodoscopeL->SetVisAttributes(hodoscopeVisAtt);

  /*G4VisAttributes* wireVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,1.0)); wireVisAtt->SetVisibility(true);
  for (int i = 0 ; i <NLAYERS ; ++i) {
    wireL[i]->SetVisAttributes (wireVisAtt);
  }*/

  return worldP;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CometCDCDetectorConstruction::ConstructSDandField()
{
  // G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // 
  // Sensitive detectors
  //
  //CometCDCSensitiveDetector* chamberSD = new CometCDCSensitiveDetector("ChamberSD", "ChamberHitsCollection");
  //SetSensitiveDetector("ChamberSD", chamberSD);

  // 
  // Magnetic field
  //
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
/*  G4ThreeVector fieldValue = G4ThreeVector(0.,0.,1.) * CLHEP::tesla;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);*/
  //fField = new G4UniformMagField(G4ThreeVector(0.,0.,1.)*CLHEP::tesla);
  fField = new CometCDCField();
  fFieldManager 
        = G4TransportationManager::GetTransportationManager()
                ->GetFieldManager();
  fFieldManager->SetDetectorField(fField);
  fFieldManager->CreateChordFinder(fField);

  //worldL->SetFieldManager (fFieldManager, true);

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
