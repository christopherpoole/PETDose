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


// USER //
#include "DetectorConstruction.hh"
#include "ParallelDetectorConstruction.hh"

// GEANT4 //
#include "globals.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4UIcommand.hh"

// G4VoxelData //
#include "G4VoxelDataParameterisation.hh"


ParallelDetectorConstruction::ParallelDetectorConstruction()
    : G4VUserParallelWorld("parallel world")
{
    G4cout << "initialising parallel world" << G4endl;
}

ParallelDetectorConstruction::~ParallelDetectorConstruction()
{
}


void ParallelDetectorConstruction::Construct()
{
    G4cout << "constructing parallel world" << G4endl;

    G4ThreeVector shape = G4ThreeVector(200, 200, 200);
    G4ThreeVector spacing = G4ThreeVector(15*mm, 15*mm, 15*mm);

    G4VoxelArray<uint8_t>* array = new G4VoxelArray<uint8_t>(shape, spacing);
    
    G4NistManager* nist_manager = G4NistManager::Instance();
    std::map<uint8_t, G4Material*> materials;
    materials[0] = nist_manager->FindOrBuildMaterial("G4_AIR");

    G4VoxelDataParameterisation<uint8_t>* voxeldata_param =
        new G4VoxelDataParameterisation<uint8_t>(array, materials, GetWorld());
    voxeldata_param->Construct(G4ThreeVector(), new G4RotationMatrix());
    voxeldata_param->SetVisibility(false);

    scorer = new G4VoxelDetector<double>("detector", shape, spacing);
    scorer->SetDebug(true); 
 
    G4SDManager* sensitive_detector_manager = G4SDManager::GetSDMpointer(); 
    sensitive_detector_manager->AddNewDetector(scorer); 
    voxeldata_param->GetLogicalVolume()->SetSensitiveDetector(scorer); 
}

