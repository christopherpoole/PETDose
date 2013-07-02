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

// GEANT4 //
#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"


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
    //world_logical->SetVisAttributes(G4VisAttributes::Invisible);
 
    phantom_solid = new G4Box("phantom_solid", 1*m, 1*m, 1*m);
    phantom_logical = new G4LogicalVolume(phantom_solid, water, "phantom_logical", 0, 0, 0);
    phantom_physical = new G4PVPlacement(0, G4ThreeVector(), phantom_logical, 
                                         "phantom_physical", world_logical, false, 0);

    scorer = new G4VoxelDetector<double>("detector",
            G4ThreeVector(300, 300, 300), G4ThreeVector(10, 10, 10));
    scorer->SetDebug(false);

    G4SDManager* sensitive_detector_manager = G4SDManager::GetSDMpointer();
    sensitive_detector_manager->AddNewDetector(scorer);
    phantom_logical->SetSensitiveDetector(scorer);

    return world_physical;
}

