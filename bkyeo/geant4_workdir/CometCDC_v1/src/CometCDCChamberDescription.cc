#ifndef ROOTANAL
#include "CometCDCChamberDescription.hh"
#include "CLHEP/Units/PhysicalConstants.h"
#else
#include "../include/CometCDCChamberDescription.hh"
#endif
#include "globals.hh"
#include <assert.h>
#include "TFile.h"
#include "TNtuple.h"


CometCDCChamberDescription::CometCDCChamberDescription() {
  worldHalfLength = 1000.*CLHEP::mm;
  chamberRO = 831. * CLHEP::mm;
  chamberRI = 500. * CLHEP::mm;
  chamberHLO = 1740.9*0.5 * CLHEP::mm;
  chamberHLI = 1500.0*0.5 * CLHEP::mm;
  chamberOWallT = 5.* CLHEP::mm;
  chamberIWallT = 0.1 * CLHEP::mm;
  chamberEPlateT = 10. * CLHEP::mm;
  protonAbsorberHalfLength = 450. * CLHEP::mm;
  protonAbsorberThickness  = 1. * CLHEP::mm;
  protonAbsorberInnerRadius = 470. * CLHEP::mm;
  hodoscopeHalfLength = 150. * CLHEP::mm;
  hodoscopeInnerRadius = protonAbsorberInnerRadius;
  hodoscopeOuterRadius = chamberRI - chamberIWallT;
  hodoscopeZCenter = protonAbsorberHalfLength + hodoscopeHalfLength;
  hodoscopeMeanRadius = 0.5 * (hodoscopeInnerRadius + hodoscopeOuterRadius);
  targetRadius = 100. * CLHEP::mm;
  targetDistance = 50. * CLHEP::mm;
  targetThickness = 0.2 * CLHEP::mm;
  numberOfTargets = 17;
  numberOfHodoscope = 2;
  numberOfHodoscopeSection = 64;


  fEndPlateRadius[0] = 51.40;
  fEndPlateRadius[1] = 52.20;
  fEndPlateRadius[2] = 53.00;
  fEndPlateRadius[3] = 53.80;
  fEndPlateRadius[4] = 54.60;
  fEndPlateRadius[5] = 55.40;
  fEndPlateRadius[6] = 56.20;
  fEndPlateRadius[7] = 57.00;
  fEndPlateRadius[8] = 57.80;
  fEndPlateRadius[9] = 58.60;
  fEndPlateRadius[10] = 59.40;
  fEndPlateRadius[11] = 60.20;
  fEndPlateRadius[12] = 61.00;
  fEndPlateRadius[13] = 61.80;
  fEndPlateRadius[14] = 62.60;
  fEndPlateRadius[15] = 63.40;
  fEndPlateRadius[16] = 64.20;
  fEndPlateRadius[17] = 65.00;
  fEndPlateRadius[18] = 65.80;
  fEndPlateRadius[19] = 66.60;
  fEndPlateRadius[20] = 67.40;
  fEndPlateRadius[21] = 68.20;
  fEndPlateRadius[22] = 69.00;
  fEndPlateRadius[23] = 69.80;
  fEndPlateRadius[24] = 70.60;
  fEndPlateRadius[25] = 71.40;
  fEndPlateRadius[26] = 72.20;
  fEndPlateRadius[27] = 73.00;
  fEndPlateRadius[28] = 73.80;
  fEndPlateRadius[29] = 74.60;
  fEndPlateRadius[30] = 75.40;
  fEndPlateRadius[31] = 76.20;
  fEndPlateRadius[32] = 77.00;
  fEndPlateRadius[33] = 77.80;
  fEndPlateRadius[34] = 78.60;
  fEndPlateRadius[35] = 79.40;
  fEndPlateRadius[36] = 80.20;
  fEndPlateRadius[37] = 81.00;
  fEndPlateRadius[38] = 81.80; // in cm

  fNumberOfWires[0] = 396;
  fNumberOfWires[1] = 396;
  fNumberOfWires[2] = 396;
  fNumberOfWires[3] = 408;
  fNumberOfWires[4] = 408;
  fNumberOfWires[5] = 420;
  fNumberOfWires[6] = 420;
  fNumberOfWires[7] = 432;
  fNumberOfWires[8] = 432;
  fNumberOfWires[9] = 444;
  fNumberOfWires[10] = 444;
  fNumberOfWires[11] = 456;
  fNumberOfWires[12] = 456;
  fNumberOfWires[13] = 468;
  fNumberOfWires[14] = 468;
  fNumberOfWires[15] = 480;
  fNumberOfWires[16] = 480;
  fNumberOfWires[17] = 492;
  fNumberOfWires[18] = 492;
  fNumberOfWires[19] = 504;
  fNumberOfWires[20] = 504;
  fNumberOfWires[21] = 516;
  fNumberOfWires[22] = 516;
  fNumberOfWires[23] = 528;
  fNumberOfWires[24] = 528;
  fNumberOfWires[25] = 540;
  fNumberOfWires[26] = 540;
  fNumberOfWires[27] = 552;
  fNumberOfWires[28] = 552;
  fNumberOfWires[29] = 564;
  fNumberOfWires[30] = 564;
  fNumberOfWires[31] = 576;
  fNumberOfWires[32] = 576;
  fNumberOfWires[33] = 588;
  fNumberOfWires[34] = 588;
  fNumberOfWires[35] = 600;
  fNumberOfWires[36] = 600;
  fNumberOfWires[37] = 612;
  fNumberOfWires[38] = 612;

  fNumberOfHoleSkip[0] = 12;
  fNumberOfHoleSkip[1] = -10;
  fNumberOfHoleSkip[2] = -10;
  fNumberOfHoleSkip[3] = 12;
  fNumberOfHoleSkip[4] = 12;
  fNumberOfHoleSkip[5] = -12;
  fNumberOfHoleSkip[6] = -12;
  fNumberOfHoleSkip[7] = 12;
  fNumberOfHoleSkip[8] = 12;
  fNumberOfHoleSkip[9] = -12;
  fNumberOfHoleSkip[10] = -12;
  fNumberOfHoleSkip[11] = 12;
  fNumberOfHoleSkip[12] = 12;
  fNumberOfHoleSkip[13] = -12;
  fNumberOfHoleSkip[14] = -12;
  fNumberOfHoleSkip[15] = 12;
  fNumberOfHoleSkip[16] = 12;
  fNumberOfHoleSkip[17] = -12;
  fNumberOfHoleSkip[18] = -12;
  fNumberOfHoleSkip[19] = 12;
  fNumberOfHoleSkip[20] = 12;
  fNumberOfHoleSkip[21] = -12;
  fNumberOfHoleSkip[22] = -12;
  fNumberOfHoleSkip[23] = 12;
  fNumberOfHoleSkip[24] = -12;
  fNumberOfHoleSkip[25] = -12;
  fNumberOfHoleSkip[26] = 12;
  fNumberOfHoleSkip[27] = 12;
  fNumberOfHoleSkip[28] = -14;
  fNumberOfHoleSkip[29] = -14;
  fNumberOfHoleSkip[30] = 14;
  fNumberOfHoleSkip[31] = 14;
  fNumberOfHoleSkip[32] = -14;
  fNumberOfHoleSkip[33] = -14;
  fNumberOfHoleSkip[34] = 14;
  fNumberOfHoleSkip[35] = 14;
  fNumberOfHoleSkip[36] = -14;
  fNumberOfHoleSkip[37] = -14;

  fStereoAngle[0] = 65.829;
  fStereoAngle[1] = -55.542;
  fStereoAngle[2] = -56.172;
  fStereoAngle[3] = 66.102;
  fStereoAngle[4] = 66.823;
  fStereoAngle[5] = -65.620;
  fStereoAngle[6] = -66.310;
  fStereoAngle[7] = 65.144;
  fStereoAngle[8] = 65.805;
  fStereoAngle[9] = -64.674;
  fStereoAngle[10] = -65.307;
  fStereoAngle[11] = 64.209;
  fStereoAngle[12] = 64.817;
  fStereoAngle[13] = -63.750;
  fStereoAngle[14] = -64.334;
  fStereoAngle[15] = 63.297;
  fStereoAngle[16] = 63.858;
  fStereoAngle[17] = -62.850;
  fStereoAngle[18] = -63.389;
  fStereoAngle[19] = 62.408;
  fStereoAngle[20] = 62.926;
  fStereoAngle[21] = -61.972;
  fStereoAngle[22] = -62.471;
  fStereoAngle[23] = 61.541;
  fStereoAngle[24] = 62.022;
  fStereoAngle[25] = -61.116;
  fStereoAngle[26] = -61.579;
  fStereoAngle[27] = 60.696;
  fStereoAngle[28] = 61.143;
  fStereoAngle[29] = -70.279;
  fStereoAngle[30] = -70.781;
  fStereoAngle[31] = 69.803;
  fStereoAngle[32] = 70.288;
  fStereoAngle[33] = -69.332;
  fStereoAngle[34] = -69.801;
  fStereoAngle[35] = 68.868;
  fStereoAngle[36] = 69.321;
  fStereoAngle[37] = -68.410;
  fStereoAngle[38] = -68.848;  // in mrad

  fFirstHoleRotateAngle[0] = 0.000;
  fFirstHoleRotateAngle[1] = 0.000;
  fFirstHoleRotateAngle[2] = 0.000;
  fFirstHoleRotateAngle[3] = 15.400;
  fFirstHoleRotateAngle[4] = 15.400;
  fFirstHoleRotateAngle[5] = 0.000;
  fFirstHoleRotateAngle[6] = 0.000;
  fFirstHoleRotateAngle[7] = 14.544;
  fFirstHoleRotateAngle[8] = 14.544;
  fFirstHoleRotateAngle[9] = 0.000;
  fFirstHoleRotateAngle[10] = 0.000;
  fFirstHoleRotateAngle[11] = 0.000;
  fFirstHoleRotateAngle[12] = 0.000;
  fFirstHoleRotateAngle[13] = 13.426;
  fFirstHoleRotateAngle[14] = 13.426;
  fFirstHoleRotateAngle[15] = 0.000;
  fFirstHoleRotateAngle[16] = 0.000;
  fFirstHoleRotateAngle[17] = 12.771;
  fFirstHoleRotateAngle[18] = 12.771;
  fFirstHoleRotateAngle[19] = 0.000;
  fFirstHoleRotateAngle[20] = 0.000;
  fFirstHoleRotateAngle[21] = 12.177;
  fFirstHoleRotateAngle[22] = 12.177;
  fFirstHoleRotateAngle[23] = 0.000;
  fFirstHoleRotateAngle[24] = 0.000;
  fFirstHoleRotateAngle[25] = 11.636;
  fFirstHoleRotateAngle[26] = 11.636;
  fFirstHoleRotateAngle[27] = 0.000;
  fFirstHoleRotateAngle[28] = 0.000;
  fFirstHoleRotateAngle[29] = 0.000;
  fFirstHoleRotateAngle[30] = 0.000;
  fFirstHoleRotateAngle[31] = 0.000;
  fFirstHoleRotateAngle[32] = 0.000;
  fFirstHoleRotateAngle[33] = 10.686;
  fFirstHoleRotateAngle[34] = 10.686;
  fFirstHoleRotateAngle[35] = 0.000;
  fFirstHoleRotateAngle[36] = 0.000;
  fFirstHoleRotateAngle[37] = 10.267;
  fFirstHoleRotateAngle[38] = 10.267; // in mrad

  for (int i = 0 ; i < NLAYERS ; ++i) {
    fEndPlateRadius[i] *= CLHEP::cm;
    fStereoAngle[i] *= CLHEP::mrad;
    fFirstHoleRotateAngle[i] *= CLHEP::mrad;
  }

  fWireNumberSum[0] = fNumberOfWires[0];
  for (int i = 1 ; i <NLAYERS ; ++i) {
    fWireNumberSum [i] = fWireNumberSum[i-1] + fNumberOfWires[i];
  }

  int k = 0;
  for (int i = 0 ; i <NLAYERS ; ++i) {
    G4double hl = (chamberHLO - chamberHLI) / (chamberRO - chamberRI) * ( fEndPlateRadius[i] - chamberRI ) + chamberHLI;
    G4cout << "*** layer " << i << " half length = " << hl <<", ";
    G4double deltaAngle = CLHEP::twopi / (double)(getNumberOfWires(i));
    G4double firstAngle = getFirstHoleRotateAngle(i);
    G4double radius = getEndPlateRadius(i);
    int j;
    for (j = 0 ; j < fNumberOfWires[i] ; ++j) {
      G4double startAngle = firstAngle + j * deltaAngle;
      G4double endAngle   = startAngle + getNumberOfHoleSkip(i) * deltaAngle;

      WireDataType thisWire;
      thisWire.StartPosition.set(radius * cos(startAngle), radius * sin(startAngle), hl); 
      thisWire.EndPosition.set  (radius * cos(endAngle), radius * sin(endAngle), -hl); 
      if (startAngle < endAngle) { thisWire.phi0 = startAngle; thisWire.phi1 = endAngle; }
      else                       { thisWire.phi0 = endAngle;   thisWire.phi1 = startAngle; }

      fWires[i].push_back(thisWire);
      ++k;
//      G4cout << "Adding wire " << i << " " << j << G4endl;
    }
    G4cout << "total " << j <<" wires constrcted" << G4endl;
  }
  G4cout << "Total number of wires = " << k << G4endl;

  print();

  //target position
  G4int numberOfTargetsInOneSide = (numberOfTargets-1)/2;
  for (G4int i = 0 ; i < numberOfTargets ; ++i) {
    targetPositions.push_back((i-numberOfTargetsInOneSide)*targetDistance);
  }

  // sense layers
  senseLayers.clear();
  for (int i = 0 ; i < NLAYERS ; ++i) {
    if (getWireType(i) == SENSE_WIRE) {
      senseLayers.push_back(i);
      senseLayerRadius.push_back(fEndPlateRadius[i]);
    }
  }

  //G4cerr << "senseLayers.size = " << senseLayers.size() << G4endl;


  
}

CometCDCChamberDescription::~CometCDCChamberDescription() {;}


G4int CometCDCChamberDescription::getLayerNumber (G4int wireNumber) const{ 
  if (wireNumber <0) return -1;
  for (int i = 0 ; i <NLAYERS ; ++i) {
    if (wireNumber < fWireNumberSum[i]) return i;
  }
  return -1;
}

G4int CometCDCChamberDescription::getCellNumber (G4int layerNumber) const {
  if (layerNumber == 0) { return 0; }
  else if (layerNumber <NLAYERS) { return (layerNumber+1)/2; }
  else { assert(0); }
}

WireType CometCDCChamberDescription::getWireType (G4int layerNumber) const {
  if (layerNumber == 0 || layerNumber == NLAYERS-1) { return GUARD_WIRE; }
  else if (1 <= layerNumber && layerNumber < NLAYERS-1) { 
    if ((layerNumber+1) % 2 == 0) return FIELD_WIRE;
    else return  SENSE_WIRE;
  }
  else { assert(0); }
}

char CometCDCChamberDescription::getWireTypeChar (G4int layerNumber) const {
  if (layerNumber == 0 || layerNumber == NLAYERS-1) { return 'G'; }
  else if (1 <= layerNumber && layerNumber < NLAYERS-1) { 
    if ((layerNumber+1) % 2 == 0) return 'F';
    else return  'S';
  }
  else { assert(0); }
}


G4double CometCDCChamberDescription::getEndPlateRadius (G4int layerNumber) const {
  assert (0<=layerNumber && layerNumber <NLAYERS);
  return fEndPlateRadius[layerNumber] ; 
}
  
G4int CometCDCChamberDescription::getNumberOfWires (G4int layerNumber) const {
  assert (0<=layerNumber && layerNumber <NLAYERS);
  return fNumberOfWires[layerNumber];
}

G4int CometCDCChamberDescription::getNumberOfHoleSkip (G4int layerNumber) const {
  assert (0<=layerNumber && layerNumber <NLAYERS);
  return fNumberOfHoleSkip[layerNumber];
}

G4double CometCDCChamberDescription::getStereoAngle (G4int layerNumber) const {
  assert (0<=layerNumber && layerNumber <NLAYERS);
  return fStereoAngle[layerNumber] ; 
}

G4double CometCDCChamberDescription::getFirstHoleRotateAngle (G4int layerNumber) const {
  assert (0<=layerNumber && layerNumber <NLAYERS);
  return fFirstHoleRotateAngle[layerNumber] ; 
}

G4int CometCDCChamberDescription::getWireInLayer (G4int wireNumber) const {
  if (wireNumber <0) return -1;
  if (wireNumber < fWireNumberSum[0]) return wireNumber;
  for (int i = 1 ; i <NLAYERS ; ++i) {
    if (wireNumber < fWireNumberSum[i]) return wireNumber - fWireNumberSum[i-1];
  }
  return -1;
}

G4int CometCDCChamberDescription::getWireNumber (G4int layerNumber, G4int wireNumberInLayer) const {
  if (layerNumber<0) return -1;
  else if (layerNumber==0) return wireNumberInLayer;
  else if (layerNumber <NLAYERS) return fWireNumberSum[layerNumber-1] + wireNumberInLayer;
  else return -1;
}

void CometCDCChamberDescription::print () {
  G4cout  << "******************  Chamber wire information ***********************************\n";
  G4cout  << "Layer\tcell\tType\tRadius\tN(wire)\tSum(wire)\tN(skip)\teta\tphi\tN(wire,rec)\n";
  G4cout  << "--------------------------------------------------------------------------------\n";
  for (int i = 0 ; i < NLAYERS ; ++i) {
    G4cout  << i << "\t" 
            << getCellNumber(i) << "\t"
            << getWireTypeChar(i) << "\t"
            << getEndPlateRadius(i) << "\t"
            << getNumberOfWires(i) << "\t"
            << fWireNumberSum [i] <<"\t"
            << getNumberOfHoleSkip(i) << "\t"
            << getStereoAngle(i) << "\t"
            << getFirstHoleRotateAngle(i) << "\t"
            << fWires[i].size() << "\n";
  }
  G4cout  << "--------------------------------------------------------------------------------\n";
/*
  TFile *f = new TFile ("wire_configuration.root", "recreate");
  f->cd();
  TNtuple *nt = new TNtuple ("nt", "wire configuration", "lay:wir:sx:sy:sz:ex:ey:ez");
  for (int i = 0 ; i <NLAYERS ; ++i) {
    for (unsigned int j = 0 ; j <fWires[i].size() ; ++j) {
      nt->Fill(i,j,fWires[i][j].StartPosition.x(), fWires[i][j].StartPosition.y(), fWires[i][j].StartPosition.z(), fWires[i][j].EndPosition.x(), fWires[i][j].EndPosition.y(), fWires[i][j].EndPosition.z());
    }
  }
  nt->Write();
  f->Close();
*/
}

G4ThreeVector CometCDCChamberDescription::getMeanPositionOfWire(int lay, int wir) const{
  const WireDataType & thisWire = getWire(lay, wir);
  return 0.5* (thisWire.StartPosition + thisWire.EndPosition);
}

G4ThreeVector CometCDCChamberDescription::getMeanPositionOfHodoscopeModule (int sec1, int sec2) const {
  G4ThreeVector ret(0,0,0);
  if (sec1 == 0)  ret.setZ(hodoscopeZCenter * -1.);
  else if (sec1 == 1) ret.setZ(hodoscopeZCenter);
  else return ret;
  G4double phi = ((double)sec2 + 0.5) * CLHEP::twopi / double(numberOfHodoscopeSection);
  ret.setX (hodoscopeMeanRadius*std::cos(phi));
  ret.setY (hodoscopeMeanRadius*std::sin(phi));
  return ret;
}




