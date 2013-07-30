//////////////////////////////////////////////////////////////////////////
// G4VoxelData
// ===========
// A general interface for loading voxelised data as geometry in GEANT4.
//
// Author:  Christopher M Poole <mail@christopherpoole.net>
// Source:  http://github.com/christopherpoole/G4VoxelData
//
// License & Copyright
// ===================
// 
// Copyright 2013 Christopher M Poole <mail@christopherpoole.net>
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


#ifndef GANTRYPARAMETERISATION_HH
#define GANTRYPARAMETERISATION_HH

// STL //
#include <vector>
#include <map>

// GEANT4 //
#include "globals.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4PVParameterised.hh"
#include "G4VNestedParameterisation.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"


class GantryParameterisation : public G4VNestedParameterisation
{
public:

    GantryParameterisation(G4VPhysicalVolume* mother_physical)
    {
        this->mother_physical = mother_physical;

        length = 10*mm;
        width = 5*mm;
        
        crystals_x = 14;
        crystals_y = 14;
        
        blocks_x = 4;
        blocks_y = 4;

        heads = 12;
    };

    virtual ~GantryParameterisation(){
    };

    virtual void Construct(G4ThreeVector position, G4RotationMatrix* rotation) {
        G4NistManager* nist_manager = G4NistManager::Instance();
        G4Material* air = nist_manager->FindOrBuildMaterial("G4_AIR");

        G4Box* block_solid =
            new G4Box("block_solid", width*crystals_x*blocks_x/2.,
                                         width*crystals_y*blocks_y/2.,
                                         length/2.);
        block_logical =
            new G4LogicalVolume(block_solid, air, "block_logical", 0, 0, 0);
        new G4PVPlacement(rotation, position,
            "block_physical", block_logical, mother_physical, 0, false, 0);

        // Y //
        G4VSolid* y_solid =
            new G4Box("y_solid", width*crystals_x*blocks_x/2.,
                                 width/2.,
                                 length/2.);
        y_logical = new G4LogicalVolume(y_solid, air, "y_logical");
        new G4PVReplica("y_replica", y_logical, block_logical,
                        kYAxis, crystals_y*blocks_y, width);

        // X //
        G4VSolid* x_solid =
            new G4Box("x_solid", width/2.,
                                 width*crystals_y*blocks_y/2.,
                                 length/2.);
        x_logical = new G4LogicalVolume(x_solid, air, "x_logical");
        new G4PVReplica("x_replica", x_logical, y_logical,
                        kXAxis, crystals_x*blocks_x, width);

        // CRYSTAL //
        G4VSolid* crystal_solid =
            new G4Box("crystal_solid", width/2.,
                                       width/2.,
                                       length/2.);
        crystal_logical = new G4LogicalVolume(crystal_solid, air, "crystal_logical");
        
        new G4PVParameterised("heads", crystal_logical, x_logical, kPhi, 1, this);
    };

    using G4VNestedParameterisation::ComputeMaterial;
    G4Material* ComputeMaterial(G4VPhysicalVolume *physical_volume,
            const G4int copy_number, const G4VTouchable *parent_touchable)
    {
        G4NistManager* nist_manager = G4NistManager::Instance();
        return nist_manager->FindOrBuildMaterial("G4_AIR");
    };

    void ComputeTransformation(const G4int copyNo, G4VPhysicalVolume *physVol) const
    {
        G4double angle = copyNo * (360/12.)*deg;
        G4RotationMatrix* rm = new G4RotationMatrix();
        rm->rotateX(angle);
        
        physVol->SetRotation(rm);
    };

    using G4VNestedParameterisation::ComputeDimensions;

    G4int GetNumberOfMaterials() const
    {
        return 1;
    };

    G4Material* GetMaterial(G4int i) const
    {
        G4NistManager* nist_manager = G4NistManager::Instance();
        return nist_manager->FindOrBuildMaterial("G4_AIR");
    };


  private:
    G4VPhysicalVolume* mother_physical;

    G4LogicalVolume* block_logical;
    G4LogicalVolume* x_logical;
    G4LogicalVolume* y_logical;
    G4LogicalVolume* crystal_logical;

    G4double length;
    G4double width;

    G4int crystals_x;
    G4int crystals_y;
    
    G4int blocks_x;
    G4int blocks_y;
    
    G4int heads;
};

#endif // GANTRYPARAMETERISATION_HH
