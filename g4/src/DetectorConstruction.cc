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
#include "G4NistManager.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"


DetectorConstruction::DetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
    G4NistManager* nist_manager = G4NistManager::Instance();
    G4Material* air = nist_manager->FindOrBuildMaterial("G4_AIR");
    G4Material* water = nist_manager->FindOrBuildMaterial("G4_WATER");

    world_solid = new G4Box("world_solid", 1.5*m, 1.5*m, 1.5*m);
    world_logical = new G4LogicalVolume(world_solid, air, "world_logical", 0, 0, 0);
    world_physical = new G4PVPlacement(0, G4ThreeVector(), world_logical, 
                                       "world_physical", 0, false, 0);
    world_logical->SetVisAttributes(G4VisAttributes::Invisible);
    world_logical->SetUserLimits(new G4UserLimits(5*mm));

    phantom_solid = new G4Box("phantom_solid", 1.0*m, 1.0*m, 1.0*m);
    phantom_logical = new G4LogicalVolume(phantom_solid, air, "phantom_logical", 0, 0, 0);
    phantom_physical = new G4PVPlacement(0, G4ThreeVector(), phantom_logical, 
                                       "phantom_physical", world_logical, false, 0);
    phantom_logical->SetVisAttributes(G4VisAttributes::Invisible);
    
    gantry_param = new GantryParameterisation(phantom_physical);
    gantry_param->SetRadius(radius);
    gantry_param->SetCrystalLength(crystal_length);
    gantry_param->SetCrystalWidth(crystal_width);
    gantry_param->SetNumberOfCrystals(crystals_x, crystals_y);
    gantry_param->SetNumberOfBlocks(blocks_x, blocks_y);
    gantry_param->SetNumberOfHeads(heads);
    gantry_param->Construct(G4ThreeVector(), 0);

    sensitive_detector = new SensitiveDetector("crystal_hits");

    G4SDManager* sensitive_detector_manager = G4SDManager::GetSDMpointer();
    sensitive_detector_manager->AddNewDetector(sensitive_detector);
    gantry_param->GetLogicalVolume()->SetSensitiveDetector(sensitive_detector);

    return world_physical;
}

