#ifndef CometCDCField_h
#define CometCDCField_h 1

#include "G4MagneticField.hh"
#include "G4TransportationManager.hh"
#include "globals.hh"

//
class CometCDCField : public G4MagneticField {
  public:
    void GetFieldValue (const G4double Point[4], G4double *Bfield) const;
};

#endif

