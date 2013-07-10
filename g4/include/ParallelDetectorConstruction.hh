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


#ifndef ParallelDetectorConstruction_H
#define ParallelDetectorConstruction_H 1

// G4VoxelData//
#include "G4VoxelData.hh"
#include "G4VoxelArray.hh"
#include "G4VoxelDetector.hh"

// GEANT4 //
#include "G4VUserParallelWorld.hh"
#include "G4NistManager.hh"
#include "G4Orb.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"


class ParallelDetectorConstruction : public G4VUserParallelWorld
{
  public:
    ParallelDetectorConstruction();
    ~ParallelDetectorConstruction();

    void Construct();
  
  private:
    G4Box* world_solid;
    G4LogicalVolume* world_logical;
    G4VPhysicalVolume* world_physical;
  
  public:
    G4VoxelDetector<double>* scorer;
};
#endif

