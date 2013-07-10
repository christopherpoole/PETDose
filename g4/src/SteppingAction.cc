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


#include "SteppingAction.hh"
#include "DetectorConstruction.hh"

#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

SteppingAction::SteppingAction()
{
    debug = false;

    shape = G4ThreeVector(201, 201, 201);
    spacing = G4ThreeVector(15*mm, 15*mm, 15*mm);

    momentum_histogram = new G4VoxelArray<double>(shape, spacing);
    steps_histogram = new G4VoxelArray<double>(shape, spacing);

    io = new NumpyDataIO();
}

SteppingAction::~SteppingAction()
{;}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    const G4Track* aTrack = aStep->GetTrack();
    const G4String name = aTrack->GetDefinition()->GetParticleName();

    G4double energy_deposit = aTrack->GetKineticEnergy();

    G4ThreeVector position = aTrack->GetPosition();

    int x_index = std::floor((position.x() + (shape[0]/2. * spacing[0])) / spacing[0]);
    int y_index = std::floor((position.y() + (shape[1]/2. * spacing[1])) / spacing[1]);
    int z_index = std::floor((position.z() + (shape[2]/2. * spacing[2])) / spacing[2]);

    if (debug) {
        G4cout << "Solid name:       " << aTrack->GetVolume()->GetLogicalVolume()->GetName() << G4endl;
        G4cout << "Total energy:     " << aTrack->GetTotalEnergy() << G4endl;
        G4cout << "Enegy to deposit: " << energy_deposit << " MeV" << G4endl;
        G4cout << "Voxel material:   " << aTrack->GetMaterial()->GetName() << G4endl;
        //G4cout << "Voxel mass:       " << voxel_mass << G4endl;
        //G4cout << "Voxel volume:     " << volume << G4endl;
        G4cout << "Position: "
               << position.x() << " "
               << position.y() << " "
               << position.z() << " " << G4endl;
        G4cout << "Histogram index: "
               << x_index << " "
               << y_index << " "
               << z_index << " " << G4endl;
        if (x_index != std::abs(x_index)) {
            G4cout << "Error: X index" << G4endl;
        }
        if (y_index != std::abs(y_index)) {
            G4cout << "Error: Y index" << G4endl;
        }
        if (z_index != std::abs(z_index)) {
            G4cout << "Error: Z index" << G4endl;
        }
        G4cout << G4endl; // blank line
    }
    
    momentum_histogram->IncrementValue(energy_deposit, x_index, y_index, z_index);
    steps_histogram->IncrementValue(aTrack->GetWeight(), x_index, y_index, z_index);

}
