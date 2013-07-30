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
#include "GantryParameterisation.hh"

// GEANT4 //
#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"


DetectorConstruction::DetectorConstruction()
{
    hounsfield.push_back(Hounsfield(-1050, "G4_AIR", 0.001 ));
    hounsfield.push_back(Hounsfield(-950,"G4_AIR", 0.044));
    hounsfield.push_back(Hounsfield(-700,"G4_LUNG_ICRP", 0.302));
    hounsfield.push_back(Hounsfield(125,"G4_TISSUE_SOFT_ICRP", 1.101));
    hounsfield.push_back(Hounsfield(2500,"G4_BONE_CORTICAL_ICRP", 2.088));
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

    GantryParameterisation* gantry_param = new GantryParameterisation(world_physical);
    gantry_param->Construct(G4ThreeVector(), 0);
 
    //phantom_solid = new G4Box("phantom_solid", 1.0*m, 1.0*m, 1.0*m);
    //phantom_logical = new G4LogicalVolume(phantom_solid, air, "phantom_logical", 0, 0, 0);
    //phantom_physical = new G4PVPlacement(0, G4ThreeVector(), phantom_logical, 
    //                                   "phantom_physical", world_logical, false, 0);

    world_logical->SetUserLimits(new G4UserLimits(5*mm));

    /* 
    // Make a mapping between the data in array and G4Materials
    // at increaments of 25 HU.
    G4int increment = 25;
    std::map<int16_t, G4Material*> materials = MakeMaterialsMap(increment);

    G4VoxelDataParameterisation<int16_t>* voxeldata_param =
        new G4VoxelDataParameterisation<int16_t>(array, materials, phantom_physical);

    G4RotationMatrix* rotation = new G4RotationMatrix();

    voxeldata_param->Construct(G4ThreeVector(), rotation);
    voxeldata_param->SetRounding(25, -1000, 2000);
    voxeldata_param->ShowMidPlanes();
    voxeldata_param->ShowZPlanes(15, 0); // every 15th slice

    std::map<int16_t, G4Colour*> colours;
    for (int i=-2500; i<5000; i++) {
        double gray = (double) (i + 2500) / 7500.;
        double alpha = 1;

        if (i < -500) {
            gray = 0;
            alpha = 0;
        }

        if (gray > 1)
            gray = 1;

        colours[i] = new G4Colour(gray, gray, gray, alpha);
    }
    voxeldata_param->SetColourMap(colours);
    voxeldata_param->SetVisibility(false);

    scorer = new G4VoxelDetector<double>("detector",
            G4ThreeVector(201, 201, 201), G4ThreeVector(10*mm, 10*mm, 10*mm));
    scorer->SetDebug(false);

    G4SDManager* sensitive_detector_manager = G4SDManager::GetSDMpointer();
    sensitive_detector_manager->AddNewDetector(scorer);
    phantom_logical->SetSensitiveDetector(scorer);
    voxeldata_param->GetLogicalVolume()->SetSensitiveDetector(scorer);
    */

    return world_physical;
}


std::map<int16_t, G4Material*> DetectorConstruction::MakeMaterialsMap(G4int increment) {
    // Our materials map or ramp
    std::map<int16_t, G4Material*> ramp;

    // Calculate intermediate points in each segment
    for (unsigned int i=0; i <hounsfield.size()-1; i++) {
        G4double hounsfield_rise = hounsfield[i+1].density - hounsfield[i].density;
        G4double density_run = hounsfield[i+1].value - hounsfield[i].value;
        G4double gradient = hounsfield_rise / density_run;

        // Add each increment in the current segment to the ramp  
        int count = 0;
        for (int hf=hounsfield[i].value; hf<hounsfield[i+1].value; hf+=increment) {
            G4double density = count*increment*gradient + hounsfield[i].density;
            ramp[hf] = MakeNewMaterial(hounsfield[i].material_name, density);
            count++;
        }
    }
    // Add the last setpoint to the ramp
    ramp[hounsfield.back().value] = MakeNewMaterial(hounsfield.back().material_name,
                                                    hounsfield.back().density);
    return ramp;
}


G4Material* DetectorConstruction::MakeNewMaterial(G4String base_material_name, G4double density) {
    G4NistManager* nist_manager = G4NistManager::Instance();
    G4String new_name = base_material_name + G4UIcommand::ConvertToString(density);

    return nist_manager->BuildMaterialWithNewDensity(new_name, base_material_name, density*g/cm3);
}

