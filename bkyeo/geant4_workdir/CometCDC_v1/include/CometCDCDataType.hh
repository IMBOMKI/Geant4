#ifndef CometCDCDataType_h
#define CometCDCDataType_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include "CLHEP/Units/PhysicalConstants.h"


class CometCDCChamberDescription;
class TFile;
class TH1F;
class TNtuple;
class TTree;

enum DetectorType {
  UNDEFINED_DETECTOR = -1, CHAMBER = 0, HODOSCOPE = 1, NDETECTORS = 2
};

class MCParticleType {
  public:
    MCParticleType() {;}
    MCParticleType (G4int _idhep, G4int _isthep, G4ThreeVector _pos, G4ThreeVector _mom, G4double _time);
    ~MCParticleType() {;}
    G4int idhep, isthep;
    G4ThreeVector pos, mom;
    G4double time;
};

const int NMAXRECODATA = 200;
const int NMAXSPDATA = 1000;

struct RecoDataType {
  int nhit;
  int nsphit[NMAXRECODATA];
  int sec1[NMAXRECODATA], sec2[NMAXRECODATA];
  float edep[NMAXRECODATA];
  float t0[NMAXRECODATA], t1[NMAXRECODATA], tm[NMAXRECODATA];
  float poshx[NMAXRECODATA], poshy[NMAXRECODATA], poshz[NMAXRECODATA];
  float momhx[NMAXRECODATA], momhy[NMAXRECODATA], momhz[NMAXRECODATA];
  float posdx[NMAXRECODATA], posdy[NMAXRECODATA], posdz[NMAXRECODATA], posdphi[NMAXRECODATA];
  int nsp;
  int hitidx[NMAXSPDATA];
  float spx[NMAXSPDATA], spy[NMAXSPDATA], spz[NMAXSPDATA], spt[NMAXSPDATA];
  float sppx[NMAXSPDATA], sppy[NMAXSPDATA], sppz[NMAXSPDATA], spedep[NMAXSPDATA];
  int spidhep[NMAXSPDATA];
  bool spprim[NMAXSPDATA];
  void dump(int i, int j) {
    G4cerr << "nhit="<< nhit << ", nsphit="
          << nsphit[i] <<", sec1="
          << sec1[i] <<", sec2="
          << sec2[i] <<", edep="
          << edep[i] <<", t0="
          << t0[i] <<", poshx="
          << poshx[i] <<", momhx="
          << momhx[i] <<", nsp="
          << nsp <<", hitidx="
          << hitidx[j] <<", spx="
          << spx[j] <<", spppx="
          << sppx[j] <<", spidhep="
          << spidhep[j] <<", spprim="
          << spprim[j] << G4endl;
  }
}; 


class StepPointDataType {
  public:
    StepPointDataType();
    StepPointDataType(G4ThreeVector _pos, G4ThreeVector _mom, G4double _time, G4double _edep, G4int _idhep = 0, G4bool _isPrimary = false ) ;
    ~StepPointDataType() {;}
    G4ThreeVector pos;
    G4ThreeVector mom;
    G4double time;
    G4double edep;
    G4int idhep;
    G4bool isPrimary;
};



class HitDataType {
  public:
    HitDataType();
    HitDataType(G4int _section1, G4int _section2);
    ~HitDataType() {;}
    void addPoint (StepPointDataType sp) { stepPoint.push_back(sp); }
    void clear () { stepPoint.clear(); }
    void sortStepPointsByTime();
    std::pair<int, int> getFirstAndLastSP();

    std::vector<StepPointDataType> stepPoint;
    std::pair<G4int, G4int> section;
};



class CometCDCDataType 
{
  public:
    CometCDCDataType(const CometCDCChamberDescription * _chamberDesc);
    virtual ~CometCDCDataType() {;}
    void clear();
    G4bool addStepPoint (DetectorType _det, G4int _sec1, G4int _sec2, G4ThreeVector _pos, G4ThreeVector _mom, G4double _time, G4double _edep, G4int _idhep, G4bool _isPrimary);
    G4bool addMCParticle (G4int _idhep, G4int _isthep, G4ThreeVector _pos, G4ThreeVector _mom, G4double _time);

  private:

    const CometCDCChamberDescription * chamberDesc;

    std::vector<HitDataType> hitmap[NDETECTORS];
    std::vector<MCParticleType> mcptl;
    RecoDataType recohit[NDETECTORS];
    RecoDataType stereohit;

    TFile * fw;
    TNtuple *ntgen;
    TNtuple *ntsp;
    TNtuple *ntwire;
    TTree * trdata;

  private:
    int brEvtid;
    float brGenx, brGeny, brGenz;
    float brGenpx, brGenpy, brGenpz, brGenpt;


  private:

    int fillChamberDescriptionNtuple();

  public:
    int reconstructHitmapData (G4int evtid, DetectorType det);
    int reconstructStereoHit();

    int fillDataTree (G4int evtid) ;
    int fillGenParticleNtuple(G4int evtid);
    std::pair<int,int> fillStepPointsNtuple(G4int evtid);
    void writeFiles();


    
};

#endif
