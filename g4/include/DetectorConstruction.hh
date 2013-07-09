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
#include "DicomDataIO.hh"
#include "G4VoxelDetector.hh"
#include "G4VoxelDataParameterisation.hh"

// GEANT4 //
#include "G4VUserDetectorConstruction.hh"
#include "G4UIcommand.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"


// Simple data structure for setpoints in hounsfiled -> G4Material ramp
class Hounsfield{
  public:
    Hounsfield(int value, G4String material_name, G4double density) {
        this->value = value;
        this->material_name = material_name;
        this->density = density;
    };

  public:
    G4int value;
    G4String material_name;
    G4double density;
};


class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction();

    G4VPhysicalVolume* Construct();

    std::map<int16_t, G4Material*> MakeMaterialsMap(G4int increment);
    G4Material* MakeNewMaterial(G4String base_material_name, G4double density);

  public:
    void SetCTDirectory(G4String directory, G4int ct_acquistion) {
        this->directory = directory;
        this->ct_acquisition = ct_acquistion;

        DicomDataIO* reader = new DicomDataIO(); 
        reader->SetAcquisitionNumber(ct_acquisition); 
         
        G4VoxelData* data = reader->ReadDirectory(directory); 
        array = new G4VoxelArray<int16_t>(data);

        ct_origin = array->GetOrigin();
    }

    void SaveEnergyHistogram(G4String filename) {
        io->Write<double>(filename, scorer->GetEnergyHistogram()->GetData());
    }

    void SaveEnergySqHistogram(G4String filename) {
        io->Write<double>(filename, scorer->GetEnergySqHistogram()->GetData());
    }

    void SaveCountsHistogram(G4String filename) {
        io->Write<double>(filename, scorer->GetCountsHistogram()->GetData());
    }

    G4ThreeVector GetCTOrigin() {
        return this->ct_origin;
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
    std::vector<Hounsfield> hounsfield;
    
    G4VoxelArray<int16_t>* array;
    
    G4String directory;
    G4int ct_acquisition;
    G4ThreeVector ct_origin;
};

#endif

