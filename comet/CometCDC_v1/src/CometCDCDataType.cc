#include "CometCDCDataType.hh"
#include "CometCDCChamberDescription.hh"
#include "CometCDCDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "TFile.h"
#include "TH1.h"
#include "TNtuple.h"
#include "TTree.h"
#include <algorithm>

//********** MCParticleType *************//
MCParticleType::MCParticleType (G4int _idhep, G4int _isthep, G4ThreeVector _pos, G4ThreeVector _mom, G4double _time):
  idhep(_idhep), isthep(_isthep), pos(_pos), mom(_mom), time(_time)
{}


//********** StepPointDataType *************//
StepPointDataType::StepPointDataType():
  pos(0), mom(0), time(0), edep(0), idhep(0), isPrimary(false)
{}

StepPointDataType::StepPointDataType(G4ThreeVector _pos, G4ThreeVector _mom, G4double _time, G4double _edep, G4int _idhep, G4bool _isPrimary):
  pos(_pos), mom(_mom), time(_time), edep(_edep), idhep(_idhep), isPrimary(_isPrimary)
{}

//********** HitDataType *************//
HitDataType::HitDataType()
{
  section.first = -1;
  section.second = -1;
  stepPoint.clear();
}

HitDataType::HitDataType(G4int _section1, G4int _section2)
{
  section.first = _section1;
  section.second = _section2;
  stepPoint.clear();
}

void HitDataType::sortStepPointsByTime () {
  bool swapFlag = false;
  do {
    swapFlag = false;
    for (std::vector<StepPointDataType>::iterator it = stepPoint.begin() ;
        it != stepPoint.end()-1 ; ++it) {
      if ((*it).time > (*(it+1)).time) {
        std::iter_swap (it, it+1);
        swapFlag = true;
      }
    }
  } while (swapFlag);
}

std::pair<int, int> HitDataType::getFirstAndLastSP() {
  int first=-1, last=-1;
  G4double firstTime= 9.e100, lastTime= -9.e100;
  for (unsigned int i = 0 ; i < stepPoint.size() ; ++i) {
    G4double & cTime = stepPoint[i].time;
    if (cTime < firstTime) {
      firstTime = cTime;
      first = i;
    }
    if (cTime > lastTime) {
      lastTime = cTime;
      last = i;
    }
  }
  std::pair<int, int> ret (first, last);
  return ret;
}


//********** CometCDCDataType *************//
CometCDCDataType::CometCDCDataType (const CometCDCChamberDescription * _chamberDesc) :
  chamberDesc ( _chamberDesc)
{


  assert (chamberDesc);
  for (G4int i = 0 ; i < chamberDesc->numberOfHodoscope ; ++i) {
    for (G4int j = 0 ; j <chamberDesc->numberOfHodoscopeSection ; ++j) {
      HitDataType tmp(i,j);
      hitmap[HODOSCOPE].push_back(tmp);
    }
  }

  for (G4int i = 0 ; i < NLAYERS ; ++i) {
    for (G4int j = 0 ; j < chamberDesc->getNumberOfWires(i) ; ++j) {
      HitDataType tmp(i,j);
      hitmap[CHAMBER].push_back(tmp);
    }
  }
  G4cerr << "CometCDCDataType: " << hitmap[CHAMBER].size() << " and " << hitmap[HODOSCOPE].size() << " data space prepared" << G4endl;


  fw = new TFile ("data.root", "recreate");
  ntgen = new TNtuple ("ntgen", "gen particle", "evtid:x:y:z:px:py:pz:time:idhep:isthep");
  ntsp  = new TNtuple ("ntsp", "steppoints", "evtid:det:sec1:sec2:x:y:z:px:py:pz:t:edep");
  ntwire = new TNtuple ("ntwire", "wire configuration", "lay:wir:sx:sy:sz:ex:ey:ez:type:phi0:phi1");
  trdata = new TTree ("trdata", "data tree");

  trdata->Branch ("evtid", &brEvtid, "evtid/I"); 

  trdata->Branch ("genx", &brGenx, "genx/F");
  trdata->Branch ("geny", &brGeny, "geny/F");
  trdata->Branch ("genz", &brGenz, "genz/F");
  trdata->Branch ("genpx", &brGenpx, "genpx/F");
  trdata->Branch ("genpy", &brGenpy, "genpy/F");
  trdata->Branch ("genpz", &brGenpz, "genpz/F");
  trdata->Branch ("genpt", &brGenpt, "genpt/F");

  trdata->Branch ("nhitHOD", &(recohit[HODOSCOPE].nhit), "nhitHOD/I");
  trdata->Branch ("HODnsphit", recohit[HODOSCOPE].nsphit, "HODnsphit[nhitHOD]/I");
  trdata->Branch ("HODsec1", recohit[HODOSCOPE].sec1, "HODsec1[nhitHOD]/I");
  trdata->Branch ("HODsec2", recohit[HODOSCOPE].sec2, "HODsec2[nhitHOD]/I");
  trdata->Branch ("HODedep", recohit[HODOSCOPE].edep, "HODedep[nhitHOD]/F");
  trdata->Branch ("HODt0", recohit[HODOSCOPE].t0, "HODt0[nhitHOD]/F");
  trdata->Branch ("HODt1", recohit[HODOSCOPE].t1, "HODt1[nhitHOD]/F");
  trdata->Branch ("HODtm", recohit[HODOSCOPE].tm, "HODtm[nhitHOD]/F");
  trdata->Branch ("HODposhx", recohit[HODOSCOPE].poshx, "HODposhx[nhitHOD]/F");
  trdata->Branch ("HODposhy", recohit[HODOSCOPE].poshy, "HODposhy[nhitHOD]/F");
  trdata->Branch ("HODposhz", recohit[HODOSCOPE].poshz, "HODposhz[nhitHOD]/F");
  trdata->Branch ("HODmomhx", recohit[HODOSCOPE].momhx, "HODmomhx[nhitHOD]/F");
  trdata->Branch ("HODmomhy", recohit[HODOSCOPE].momhy, "HODmomhy[nhitHOD]/F");
  trdata->Branch ("HODmomhz", recohit[HODOSCOPE].momhz, "HODmomhz[nhitHOD]/F");
  trdata->Branch ("HODposdx", recohit[HODOSCOPE].posdx, "HODposdx[nhitHOD]/F");
  trdata->Branch ("HODposdy", recohit[HODOSCOPE].posdy, "HODposdy[nhitHOD]/F");
  trdata->Branch ("HODposdz", recohit[HODOSCOPE].posdz, "HODposdz[nhitHOD]/F");
  trdata->Branch ("HODposdphi", recohit[HODOSCOPE].posdphi, "HODposdphi[nhitHOD]/F");

  trdata->Branch ("nspHOD", &(recohit[HODOSCOPE].nsp), "nspHOD/I"); 
  trdata->Branch ("HODhitidx", recohit[HODOSCOPE].hitidx, "HODhitidx[nspHOD]/I");
  trdata->Branch ("HODspx", recohit[HODOSCOPE].spx, "HODspx[nspHOD]/F");
  trdata->Branch ("HODspy", recohit[HODOSCOPE].spy, "HODspy[nspHOD]/F");
  trdata->Branch ("HODspz", recohit[HODOSCOPE].spz, "HODspz[nspHOD]/F");
  trdata->Branch ("HODspt", recohit[HODOSCOPE].spt, "HODspt[nspHOD]/F");
  trdata->Branch ("HODsppx", recohit[HODOSCOPE].sppx, "HODsppx[nspHOD]/F");
  trdata->Branch ("HODsppy", recohit[HODOSCOPE].sppy, "HODsppy[nspHOD]/F");
  trdata->Branch ("HODsppz", recohit[HODOSCOPE].sppz, "HODsppz[nspHOD]/F");
  trdata->Branch ("HODspedep", recohit[HODOSCOPE].spedep, "HODspedep[nspHOD]/F");
  trdata->Branch ("HODspidhep", recohit[HODOSCOPE].spidhep, "HODspidhep[nspHOD]/I");
  trdata->Branch ("HODspprim", recohit[HODOSCOPE].spprim, "HODspprim[nspHOD]/O");

  trdata->Branch ("nhitCHM", &(recohit[CHAMBER].nhit), "nhitCHM/I");
  trdata->Branch ("CHMnsphit", recohit[CHAMBER].nsphit, "CHMnsphit[nhitCHM]/I");
  trdata->Branch ("CHMsec1", recohit[CHAMBER].sec1, "CHMsec1[nhitCHM]/I");
  trdata->Branch ("CHMsec2", recohit[CHAMBER].sec2, "CHMsec2[nhitCHM]/I");
  trdata->Branch ("CHMedep", recohit[CHAMBER].edep, "CHMedep[nhitCHM]/F");
  trdata->Branch ("CHMt0", recohit[CHAMBER].t0, "CHMt0[nhitCHM]/F");
  trdata->Branch ("CHMt1", recohit[CHAMBER].t1, "CHMt1[nhitCHM]/F");
  trdata->Branch ("CHMtm", recohit[CHAMBER].tm, "CHMtm[nhitCHM]/F");
  trdata->Branch ("CHMposhx", recohit[CHAMBER].poshx, "CHMposhx[nhitCHM]/F");
  trdata->Branch ("CHMposhy", recohit[CHAMBER].poshy, "CHMposhy[nhitCHM]/F");
  trdata->Branch ("CHMposhz", recohit[CHAMBER].poshz, "CHMposhz[nhitCHM]/F");
  trdata->Branch ("CHMmomhx", recohit[CHAMBER].momhx, "CHMmomhx[nhitCHM]/F");
  trdata->Branch ("CHMmomhy", recohit[CHAMBER].momhy, "CHMmomhy[nhitCHM]/F");
  trdata->Branch ("CHMmomhz", recohit[CHAMBER].momhz, "CHMmomhz[nhitCHM]/F");
  trdata->Branch ("CHMposdx", recohit[CHAMBER].posdx, "CHMposdx[nhitCHM]/F");
  trdata->Branch ("CHMposdy", recohit[CHAMBER].posdy, "CHMposdy[nhitCHM]/F");
  trdata->Branch ("CHMposdz", recohit[CHAMBER].posdz, "CHMposdz[nhitCHM]/F");
  trdata->Branch ("CHMposdphi", recohit[CHAMBER].posdphi, "CHMposdphi[nhitCHM]/F");

  trdata->Branch ("nspCHM", &(recohit[CHAMBER].nsp), "nspCHM/I"); 
  trdata->Branch ("CHMhitidx", recohit[CHAMBER].hitidx, "CHMhitidx[nspCHM]/I");
  trdata->Branch ("CHMspx", recohit[CHAMBER].spx, "CHMspx[nspCHM]/F");
  trdata->Branch ("CHMspy", recohit[CHAMBER].spy, "CHMspy[nspCHM]/F");
  trdata->Branch ("CHMspz", recohit[CHAMBER].spz, "CHMspz[nspCHM]/F");
  trdata->Branch ("CHMspt", recohit[CHAMBER].spt, "CHMspt[nspCHM]/F");
  trdata->Branch ("CHMsppx", recohit[CHAMBER].sppx, "CHMsppx[nspCHM]/F");
  trdata->Branch ("CHMsppy", recohit[CHAMBER].sppy, "CHMsppy[nspCHM]/F");
  trdata->Branch ("CHMsppz", recohit[CHAMBER].sppz, "CHMsppz[nspCHM]/F");
  trdata->Branch ("CHMspedep", recohit[CHAMBER].spedep, "CHMspedep[nspCHM]/F");
  trdata->Branch ("CHMspidhep", recohit[CHAMBER].spidhep, "CHMspidhep[nspCHM]/I");
  trdata->Branch ("CHMspprim", recohit[CHAMBER].spprim, "CHMspprim[nspCHM]/O");
}

void CometCDCDataType::writeFiles () {
  fw->cd();
  fillChamberDescriptionNtuple();
  ntwire->Write();
  ntgen->Write();
  ntsp->Write();
  trdata->Write();
  fw->Close();
}

void CometCDCDataType::clear () {
  for (G4int i = 0 ; i <NDETECTORS ; ++i) {
    for (std::vector<HitDataType>::iterator it = hitmap[i].begin(); it != hitmap[i].end() ; ++it) {
      (*it).clear();
    }
    recohit[i].nhit = 0;
  }
  stereohit.nhit = 0;
  mcptl.clear();
}

G4bool CometCDCDataType::addStepPoint (DetectorType _det, G4int _sec1, G4int _sec2, G4ThreeVector _pos, G4ThreeVector _mom, G4double _time, G4double _edep, G4int _idhep, G4bool _isPrimary) {
  std::vector<HitDataType> & shits = hitmap[_det];
  int tag = -1;
  assert (chamberDesc);
  if (_det == CHAMBER) tag = chamberDesc->getWireNumber(_sec1, _sec2);
  else if (_det == HODOSCOPE) tag = _sec1 * chamberDesc->numberOfHodoscopeSection + _sec2;
  else {
    G4cerr << "Warning: strange detector type " << _det << " at CometCDCDataType::addStepPoint" << G4endl;
    return false;
  }

  if (shits[tag].section.first == _sec1 && shits[tag].section.second == _sec2) {
    StepPointDataType tmp (_pos, _mom, _time, _edep, _idhep, _isPrimary);
    shits[tag].addPoint(tmp);
    return true;
  }
  else {
    G4cout << "Warning: strange hitmap structure at CometCDCDataType::addStepPoint" << G4endl;
    G4cout << "tag=" << tag << ", det=" << _det << ", sec1=" << _sec1 << ", sec2=" << _sec2 << G4endl;

    for (std::vector<HitDataType>::iterator it = shits.begin() ; it != shits.end() ; ++it) {
      HitDataType &hit = *it;
      if (hit.section.first == _sec1 && hit.section.second == _sec2) {
        StepPointDataType tmp (_pos, _mom, _time, _edep, _idhep, _isPrimary);
        hit.addPoint(tmp);
        return true;
      }
    }
    G4cerr << "Warning: Cannot find corresponding hitmap at CometCDCDataType::addStepPoint. Ignoring." << G4endl;
    return false;
  }
}

G4bool CometCDCDataType::addMCParticle (G4int _idhep, G4int _isthep, G4ThreeVector _pos, G4ThreeVector _mom, G4double _time) {
  MCParticleType tmp (_idhep, _isthep, _pos, _mom, _time);
  mcptl.push_back(tmp);
  return true;
}

int CometCDCDataType::fillChamberDescriptionNtuple() {
  int n = 0;
  for (int i = 0 ; i <NLAYERS ; ++i) {
    for (unsigned int j = 0 ; j <(chamberDesc->getWires(i)).size() ; ++j) {
      const WireDataType & thisWire = chamberDesc->getWire(i,j);
      ntwire->Fill(i,j,
          thisWire.StartPosition.x(), 
          thisWire.StartPosition.y(), 
          thisWire.StartPosition.z(), 
          thisWire.EndPosition.x(), 
          thisWire.EndPosition.y(), 
          thisWire.EndPosition.z(),
          chamberDesc->getWireType(i),
          thisWire.phi0,
          thisWire.phi1);
      ++n;
    }
  }
  return n;
}

int CometCDCDataType::fillGenParticleNtuple(G4int evtid) {
  int n = 0;
  for (std::vector<MCParticleType>::iterator it = mcptl.begin() ;
        it != mcptl.end() ; ++it) {
    MCParticleType & tmp = *it;
    ntgen->Fill (evtid, tmp.pos.x(), tmp.pos.y(), tmp.pos.z(), tmp.mom.x(), tmp.mom.y(), tmp.mom.z(), tmp.time, tmp.idhep, tmp.isthep); 
    ++n;
  }
  return n;
}

std::pair<int,int> CometCDCDataType::fillStepPointsNtuple(G4int evtid) {
  G4int k[NDETECTORS];
  for (int i = 0 ; i <NDETECTORS ; ++i) k[i] = 0;
  for (int i = 0 ; i <NDETECTORS ; ++i) {
    for (std::vector<HitDataType>::iterator it = hitmap[i].begin() ; 
        it != hitmap[i].end() ; ++it) {
      HitDataType & hit = *it;
      if (hit.stepPoint.size()>0) {
        for (std::vector<StepPointDataType>::iterator it2 = hit.stepPoint.begin();
            it2 != hit.stepPoint.end() ; ++it2) {
          StepPointDataType & track = *it2;
          ntsp->Fill(evtid, i, hit.section.first, hit.section.second, 
              track.pos.x(), track.pos.y(), track.pos.z(),
              track.mom.x(), track.mom.y(), track.mom.z(),
              track.time, track.edep);
          ++k[i];
        }
      }
    }
  }
  return std::pair<int,int>(k[0],k[1]);
}

int CometCDCDataType::reconstructHitmapData(G4int evtid, DetectorType det) {
  if (det != HODOSCOPE && det != CHAMBER) return 0;
  int i = 0;
  RecoDataType & rec = recohit[det];
  int nsp = 0;
  int j = 0;
  for (std::vector<HitDataType>::iterator it = hitmap[det].begin() ;
       it != hitmap[det].end() ; ++it) {
    // TODO : add time clustering
    HitDataType & hit = *it;
    if (hit.stepPoint.size() <=0) continue;
    //hit.sortStepPointsByTime();
    rec.nsphit[i] = hit.stepPoint.size();
    nsp += (hit.stepPoint.size());

    G4double sumedep = 0, sumtedep = 0;
    G4ThreeVector momsum(0,0,0);
    G4ThreeVector possum(0,0,0);

    if (hit.stepPoint.size()>0) {
      for (std::vector<StepPointDataType>::iterator it2 = hit.stepPoint.begin();
          it2 != hit.stepPoint.end() ; ++it2) {
        StepPointDataType & track = *it2;
        rec.hitidx[j] = i;
        rec.spx[j] = track.pos.x();
        rec.spy[j] = track.pos.y();
        rec.spz[j] = track.pos.z();
        rec.spt[j] = track.time;
        rec.sppx[j] = track.mom.x();
        rec.sppy[j] = track.mom.y();
        rec.sppz[j] = track.mom.z();
        rec.spedep[j] = track.edep;
        rec.spidhep[j] = track.idhep;
        rec.spprim[j] = track.isPrimary;
        //if (rec.spidhep[j]>100 || rec.spprim[j] >100) rec.dump(i, j);
        //rec.dump(i,j);
        possum += (track.pos);
        momsum += (track.mom);
        sumedep += (track.edep);
        sumtedep += ((track.edep) * (track.time));
        ++j;
      }
    }

    rec.sec1[i] = hit.section.first;
    rec.sec2[i] = hit.section.second;
    std::pair<int, int> firstAndLast = hit.getFirstAndLastSP();
    int firstid, lastid;
    //StepPointDataType & firstSP = *(hit.stepPoint.begin());
    //StepPointDataType & lastSP = *(hit.stepPoint.end()-1);
    if (firstAndLast.first <0) {
      G4cerr << "Error: Cannot find first point in SP, assumed to be first" << G4endl;
      firstid = 0;
    }
    else {firstid = firstAndLast.first;}
    if (firstAndLast.second <0) { lastid = hit.stepPoint.size()-1; }
    else { lastid = firstAndLast.second; }
    rec.t0[i] = hit.stepPoint[firstid].time;
    rec.t1[i] = hit.stepPoint[lastid].time;

    /*StepPointDataType &firstSP = hit.stepPoint[firstid];
    rec.pos0x[i] = firstSP.pos.x();
    rec.pos0y[i] = firstSP.pos.y();
    rec.pos0z[i] = firstSP.pos.z();
    rec.mom0x[i] = firstSP.mom.x();
    rec.mom0y[i] = firstSP.mom.y();
    rec.mom0z[i] = firstSP.mom.z();
    rec.t0[i] = firstSP.time;
    if (lastid >=0) {
      StepPointDataType &lastSP = hit.stepPoint[lastid];
      rec.pos1x[i] = lastSP.pos.x();
      rec.pos1y[i] = lastSP.pos.y();
      rec.pos1z[i] = lastSP.pos.z();
      rec.mom1x[i] = lastSP.mom.x();
      rec.mom1y[i] = lastSP.mom.y();
      rec.mom1z[i] = lastSP.mom.z();
      rec.t1[i] = lastSP.time;
    }
    else {
      rec.pos1x[i] = lastSP.pos.x();
      rec.pos1y[i] = lastSP.pos.y();
      rec.pos1z[i] = lastSP.pos.z();
      rec.mom1x[i] = lastSP.mom.x();
      rec.mom1y[i] = lastSP.mom.y();
      rec.mom1z[i] = lastSP.mom.z();
      rec.t1[i] = lastSP.time;
    } //TODO
    */

    rec.tm[i] = sumtedep/sumedep;
    rec.edep[i] = sumedep;
    possum /= (hit.stepPoint.size());
    momsum /= (hit.stepPoint.size());
    rec.poshx[i] = possum.x();
    rec.poshy[i] = possum.y();
    rec.poshz[i] = possum.z();
    rec.momhx[i] = momsum.x();
    rec.momhy[i] = momsum.y();
    rec.momhz[i] = momsum.z();
    G4ThreeVector posd;
    if (det == CHAMBER) {
      posd = chamberDesc->getMeanPositionOfWire(hit.section.first, hit.section.second);
      rec.posdx[i] = posd.x();
      rec.posdy[i] = posd.y();
      rec.posdz[i] = 0;
    }
    else if (det == HODOSCOPE) { 
      posd = chamberDesc->getMeanPositionOfHodoscopeModule(hit.section.first, hit.section.second);
      rec.posdx[i] = posd.x();
      rec.posdy[i] = posd.y();
      rec.posdz[i] = posd.z();
    }
    else {return 0;}
    rec.posdphi[i] = posd.phi();
    ++i;
  }
  rec.nhit = i;
  rec.nsp = nsp;
  if (i >= NMAXRECODATA) {
    G4cerr << "Warning: NMAXRECODATA reached. Rest hits ignored." << G4endl;
    return i;
  }
  /*
  G4cout << "----------------- Event " << evtid ;
  if (det == HODOSCOPE) G4cout << " Hodoscope hit ---------------" << G4endl;
  if (det == CHAMBER)   G4cout << " Chamber hit ---------------" << G4endl;
  G4cout << "--------------------------------------------------------------" << G4endl
    << "id\tsec1\tsec2\tedep\t\tt0\t\tt1\t\ttm\t\tpos0z\t\tpos1z\t\tmom0\t\tmom1" << G4endl
    << "--------------------------------------------------------------" << G4endl;
  for (int j = 0 ; j < rec.nhit ; ++j) {
    G4cout << j << "\t" << std::scientific << std::setprecision(2)
           << rec.sec1[j] << "\t"
           << rec.sec2[j] << "\t"
           << rec.edep[j] << "\t"
           << rec.t0[j] << "\t"
           << rec.t1[j] << "\t"
           << rec.tm[j] << "\t"
           << rec.pos0z[j] << "\t"
           << rec.pos1z[j] << "\t"
           << sqrt(rec.mom0x[j]*rec.mom0x[j]+rec.mom0y[j]*rec.mom0y[j]+rec.mom0z[j]*rec.mom0z[j]) << "\t"
           << sqrt(rec.mom1x[j]*rec.mom1x[j]+rec.mom1y[j]*rec.mom1y[j]+rec.mom1z[j]*rec.mom1z[j]) << G4endl;
  }
  G4cout << "--------------------------------------------------------------" << G4endl;*/
  return i;
}

int CometCDCDataType::reconstructStereoHit () { //TODO : reconstrucStereo
  stereohit.nhit = 0;
  /*for (std::vector<RecoDataType>::iterator it = recohit[CHAMBER].begin() ;
      it != recohit[CHAMBER].end() ; ++it) {
    ++n;
  }*/
  return 0;
}

int CometCDCDataType::fillDataTree (G4int evtid) { 
  brEvtid = evtid;
  brGenx = mcptl[0].pos.x();
  brGeny = mcptl[0].pos.y();
  brGenz = mcptl[0].pos.z();
  brGenpx = mcptl[0].mom.x();
  brGenpy = mcptl[0].mom.y();
  brGenpz = mcptl[0].mom.z();
  brGenpt = mcptl[0].mom.perp();
  if (recohit[HODOSCOPE].nhit != 0 || recohit[CHAMBER].nhit !=0 || stereohit.nhit !=0) 
    trdata->Fill();
  return evtid;
}


