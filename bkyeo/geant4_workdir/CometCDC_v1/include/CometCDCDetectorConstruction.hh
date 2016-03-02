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
// $Id: CometCDCDetectorConstruction.hh 75215 2013-10-29 16:07:06Z gcosmo $
// 
/// \file CometCDCDetectorConstruction.hh
/// \brief Definition of the CometCDCDetectorConstruction class

#ifndef CometCDCDetectorConstruction_h
#define CometCDCDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4MagneticField.hh"
#include "G4FieldManager.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;
class CometCDCChamberDescription;

/// Detector construction class to define materials and geometry.
/// The calorimeter is a box made of a given number of layers. A layer consists
/// of an absorber plate and of a detection gap. The layer is replicated.
///
/// Four parameters define the geometry of the calorimeter :
///
/// - the thickness of an absorber plate,
/// - the thickness of a gap,
/// - the number of layers,
/// - the transverse size of the calorimeter (the input face is a square).
///
/// In ConstructSDandField() sensitive detectors of CometCDCCalorimeterSD type
/// are created and associated with the Absorber and Gap volumes.
/// In addition a transverse uniform magnetic field is defined 
/// via G4GlobalMagFieldMessenger class.


class CometCDCDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    CometCDCDetectorConstruction(const CometCDCChamberDescription * _chamberDesc);
    virtual ~CometCDCDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    G4LogicalVolume * getChamberVolume() const { return chamberL; }
    G4LogicalVolume * getHodoscopeVolume () const { return hodoscopeL; }

//    const CometCDCChamberDescription * getChamberDescriptionPointer() const { return chamberDesc; }
     
  private:
    // methods
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

    // data members
    //
    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger; 
                                      // magnetic field messenger

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps

    G4LogicalVolume * chamberL;
    G4LogicalVolume * hodoscopeL;

    const CometCDCChamberDescription  * chamberDesc;

    G4MagneticField * fField;
    G4FieldManager * fFieldManager;
    G4LogicalVolume *worldL;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//

#endif

