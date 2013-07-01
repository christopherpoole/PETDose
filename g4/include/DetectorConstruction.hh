//////////////////////////////////////////////////////////////////////////
// License & Copyright
// ===================
// 
// Copyright 2012 Christopher M Poole <mail@christopherpoole.net>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//////////////////////////////////////////////////////////////////////////


#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

// G4VoxelData //
#include "G4VoxelData.hh"
#include "NumpyDataIO.hh"
#include "G4VoxelDetector.hh"

// GEANT4 //
#include "G4VUserDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "boost/python.hpp"
#include "pyublas/numpy.hpp"


class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction();

    G4VPhysicalVolume* Construct();

    void SaveEnergyHistogram(G4String filename) {
        io->Write<double>(filename, scorer->GetEnergyHistogram()->GetData());
    }

    void SaveEnergySqHistogram(G4String filename) {
        io->Write<double>(filename, scorer->GetEnergySqHistogram()->GetData());
    }

    void SaveCountsHistogram(G4String filename) {
        io->Write<double>(filename, scorer->GetCountsHistogram()->GetData());
    }

  public:
    G4Box* world_solid;
    G4LogicalVolume* world_logical;
    G4VPhysicalVolume* world_physical;

    G4Box* phantom_solid;
    G4LogicalVolume* phantom_logical;
    G4VPhysicalVolume* phantom_physical;

    NumpyDataIO* io;
    G4VoxelDetector<double>* scorer;
};

#endif

