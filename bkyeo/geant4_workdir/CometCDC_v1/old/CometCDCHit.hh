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
// $Id: CometCDCHit.hh 69223 2013-04-23 12:36:10Z gcosmo $
//
/// \file CometCDCHit.hh
/// \brief Definition of the CometCDCHit class

#ifndef CometCDCHit_h
#define CometCDCHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// Calorimeter hit class
///
/// It defines data members to store the the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength

class CometCDCHit : public G4VHit
{
  public:
    CometCDCHit();
    CometCDCHit(const CometCDCHit&);
    virtual ~CometCDCHit();

    // operators
    const CometCDCHit& operator=(const CometCDCHit&);
    G4int operator==(const CometCDCHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    // methods to handle data
    void Add(G4double de, G4double dl);

    // get methods
    G4double GetEdep() const;
    G4double GetTrackLength() const;
      
  private:
    G4double fEdep;        ///< Energy deposit in the sensitive volume
    G4double fTrackLength; ///< Track length in the  sensitive volume
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<CometCDCHit> CometCDCHitsCollection;

extern G4ThreadLocal G4Allocator<CometCDCHit>* CometCDCHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* CometCDCHit::operator new(size_t)
{
  if(!CometCDCHitAllocator)
    CometCDCHitAllocator = new G4Allocator<CometCDCHit>;
  void *hit;
  hit = (void *) CometCDCHitAllocator->MallocSingle();
  return hit;
}

inline void CometCDCHit::operator delete(void *hit)
{
  if(!CometCDCHitAllocator)
    CometCDCHitAllocator = new G4Allocator<CometCDCHit>;
  CometCDCHitAllocator->FreeSingle((CometCDCHit*) hit);
}

inline void CometCDCHit::Add(G4double de, G4double dl) {
  fEdep += de; 
  fTrackLength += dl;
}

inline G4double CometCDCHit::GetEdep() const { 
  return fEdep; 
}

inline G4double CometCDCHit::GetTrackLength() const { 
  return fTrackLength; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
