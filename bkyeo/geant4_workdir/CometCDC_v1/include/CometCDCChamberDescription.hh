#ifndef CometCDCChamberDescription_h
#define CometCDCChamberDescription_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include "CLHEP/Units/PhysicalConstants.h"

// layerNumber ranges from 0 to 38
// wireNumber ranges from 0


const int NLAYERS = 39;

enum WireType {
  UNDEFINED_WIRE = -1, GUARD_WIRE = 0, FIELD_WIRE, SENSE_WIRE
};

class WireDataType {
  public:
    WireDataType(){;}
    ~WireDataType() {;}

    G4double phi0, phi1;
    G4ThreeVector StartPosition;
    G4ThreeVector EndPosition;
};


class CometCDCChamberDescription 
{
  public:
    CometCDCChamberDescription();
    virtual ~CometCDCChamberDescription();
    
    G4int     getLayerNumber            (G4int wireNumber) const;
    G4int     getCellNumber             (G4int layerNumber) const;
    WireType  getWireType               (G4int layerNumber) const;
    char      getWireTypeChar           (G4int layerNumber) const;
    G4double  getEndPlateRadius         (G4int layerNumber) const;
    G4int     getNumberOfWires          (G4int layerNumber) const;
    G4int     getNumberOfHoleSkip       (G4int layerNumber) const;
    G4double  getStereoAngle            (G4int layerNumber) const;
    G4double  getFirstHoleRotateAngle   (G4int layerNumber) const;

    G4int     getWireInLayer            (G4int wireNumber) const;
    G4int     getWireNumber             (G4int layerNumber, G4int wireNumberInLayer) const;
    G4int     getTotalWireNumber        () const { return fWireNumberSum[NLAYERS-1]; }
    void      print (void) ;
    /*G4double  getOuterHalfLength () const {return fRout;}
    G4double  getInnerHalfLength () const {return fRin;}
    G4double  getOuterRadius () const {return fHLout;}
    G4double  getInnerRadius () const {return fHLin;}*/

    WireDataType getWire (int lay, int wir) const {return fWires[lay][wir];}
    std::vector<WireDataType> getWires(int lay) const { return fWires[lay]; }

    G4ThreeVector getMeanPositionOfWire (int lay, int wir) const;
    G4ThreeVector getMeanPositionOfHodoscopeModule (int sec1, int sec2) const;

  private:
//    G4double fRin, fRout, fHLin, fHLout;
    G4double fEndPlateRadius [NLAYERS];
    G4int fNumberOfWires [NLAYERS];
    G4int fNumberOfHoleSkip [NLAYERS];
    G4double fStereoAngle [NLAYERS];
    G4double fFirstHoleRotateAngle [NLAYERS];
    G4int fWireNumberSum [NLAYERS];
    std::vector<WireDataType> fWires[NLAYERS];

  public:
    G4double worldHalfLength;
    G4double chamberRO;
    G4double chamberRI;
    G4double chamberHLO;
    G4double chamberHLI;
    G4double chamberOWallT;
    G4double chamberIWallT;
    G4double chamberEPlateT;
    G4double protonAbsorberHalfLength;
    G4double protonAbsorberThickness;
    G4double protonAbsorberInnerRadius;
    G4double hodoscopeHalfLength; 
    G4double hodoscopeInnerRadius;
    G4double hodoscopeOuterRadius;
    G4double hodoscopeZCenter;
    G4double hodoscopeMeanRadius;
    G4double targetRadius;
    G4double targetDistance;
    G4double targetThickness;
    G4int numberOfTargets;
    G4int numberOfHodoscope;
    G4int numberOfHodoscopeSection;


    std::vector<G4double> targetPositions;
    std::vector<G4int> senseLayers;
    std::vector<G4double> senseLayerRadius;
    
    
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

