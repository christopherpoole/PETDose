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

// GEANT4 //
#include "globals.hh"
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
    GantryParameterisation(G4VPhysicalVolume* mother_physical);
    virtual ~GantryParameterisation();
    
    using G4VNestedParameterisation::ComputeMaterial;
    using G4VNestedParameterisation::ComputeDimensions;

  public:
    virtual void Construct(G4ThreeVector position, G4RotationMatrix* rotation);
    G4Material* ComputeMaterial(G4VPhysicalVolume *physical_volume,
            const G4int copy_number, const G4VTouchable *parent_touchable);
    void ComputeTransformation(const G4int copyNo, G4VPhysicalVolume *physVol) const;
    G4int GetNumberOfMaterials() const;
    G4Material* GetMaterial(G4int i) const;

  private:
    G4VPhysicalVolume* mother_physical;
    G4LogicalVolume* mother_logical;

    G4LogicalVolume* head_logical;
    G4LogicalVolume* row_logical;
    G4LogicalVolume* crystal_logical;

    G4double radius;

    G4double length;
    G4double width;

    G4int crystals_x;
    G4int crystals_y;
    
    G4int blocks_x;
    G4int blocks_y;
    
    G4int heads;
};

#endif // GANTRYPARAMETERISATION_HH
