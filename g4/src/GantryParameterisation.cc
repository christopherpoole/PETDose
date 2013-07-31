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


#include "GantryParameterisation.hh"


GantryParameterisation::GantryParameterisation(G4VPhysicalVolume* mother_physical) {
    this->mother_physical = mother_physical;
    this->mother_logical = mother_physical->GetLogicalVolume();

    radius = 800*mm;

    length = 10*mm;
    width = 5*mm;
    
    crystals_x = 10;
    crystals_y = 10;
    
    blocks_x = 5;
    blocks_y = 5;

    heads = 10;
}


GantryParameterisation::~GantryParameterisation() {
}


void GantryParameterisation::Construct(G4ThreeVector position, G4RotationMatrix* rotation) {
    G4NistManager* nist_manager = G4NistManager::Instance();
    G4Material* air = nist_manager->FindOrBuildMaterial("G4_AIR");

    // Head of rows
    G4Box* head_solid =
        new G4Box("head_solid", width*crystals_x*blocks_x/2.,
                                width*crystals_y*blocks_y/2.,
                                length/2.);
    head_logical =
        new G4LogicalVolume(head_solid, air, "head_logical", 0, 0, 0);

    // Row of crystals
    G4VSolid* row_solid =
        new G4Box("row_solid", width*crystals_x*blocks_x/2.,
                               width/2.,
                               length/2.);
    row_logical = new G4LogicalVolume(row_solid, air, "row_logical");
    new G4PVReplica("row_replica", row_logical, head_logical,
                    kYAxis, crystals_y*blocks_y, width);

    // Crystal
    G4VSolid* crystal_solid =
        new G4Box("crystal_solid", width/2.,
                                   width/2.,
                                   length/2.);
    crystal_logical = new G4LogicalVolume(crystal_solid, air, "crystal_logical");
    new G4PVReplica("crystal_replica", crystal_logical, row_logical,
                    kXAxis, crystals_x*blocks_x, width);

    // Ring of heads 
    new G4PVParameterised("heads", head_logical, mother_logical, kPhi, heads, this);
}


G4Material* GantryParameterisation::ComputeMaterial(G4VPhysicalVolume *physical_volume,
        const G4int copy_number, const G4VTouchable *parent_touchable)
{
    G4NistManager* nist_manager = G4NistManager::Instance();
    return nist_manager->FindOrBuildMaterial("G4_AIR");
}


void GantryParameterisation::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume *physVol) const
{
    G4double angle = copyNo * (360./heads)*deg;
    G4RotationMatrix* rm = new G4RotationMatrix();
    rm->rotateX(angle);

    physVol->SetTranslation(G4ThreeVector(0, radius*sin(angle), radius*cos(angle)));
    physVol->SetRotation(rm);
}


G4int GantryParameterisation::GetNumberOfMaterials() const
{
    return 1;
}


G4Material* GantryParameterisation::GetMaterial(G4int i) const
{
    G4NistManager* nist_manager = G4NistManager::Instance();
    return nist_manager->FindOrBuildMaterial("G4_AIR");
}

