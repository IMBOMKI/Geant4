#include "CometCDCField.hh"
#include "globals.hh"

void CometCDCField::GetFieldValue (const G4double Point[4], G4double *Bfield) const {
  Bfield[0] = 1.*CLHEP::tesla;
  Bfield[1] = 0.;
  Bfield[2] = 0.;
}
//G4bool CometCDCField::DoesFieldChangeEnergy ()  { return true; }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
