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
#include "PrimaryGeneratorAction.hh"

// GEANT4 //
#include "globals.hh"
#include "Randomize.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    particle_gun = new G4ParticleGun();
    particle_gun->SetParticleEnergy(0); // At rest
    G4ParticleTable* particle_table = G4ParticleTable::GetParticleTable();
  
    dicom_reader = new DicomDataIO();
    activity = NULL;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete particle_gun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    if (activity) {
        GeneratePrimariesFromActivity(event);
    } else {
        ConstructIon();
        particle_gun->GeneratePrimaryVertex(event);
    }
}

void PrimaryGeneratorAction::GeneratePrimariesFromActivity(G4Event* event)
{
    G4ThreeVector voxel;
    
    double setpoint = 0;
    while(setpoint <= 100) {
        // Randomly pick a point in the activity dataset
        voxel = G4ThreeVector(
            (unsigned int) (G4UniformRand() * activity->GetShape()[0]),
            (unsigned int) (G4UniformRand() * activity->GetShape()[1]),
            (unsigned int) (G4UniformRand() * activity->GetShape()[2]));
     
        setpoint = activity->GetValue(voxel);
    }    
    G4ThreeVector position = activity->GetPosition(voxel);
    particle_gun->SetParticlePosition(position - pet_origin);

    ConstructIon();

    particle_gun->GeneratePrimaryVertex(event);

    double weight = setpoint / max_activity;
    event->GetPrimaryVertex()->SetWeight(weight);
}

void PrimaryGeneratorAction::ConstructIon() {
    // Fluorine-18
    G4int Z = 9;
    G4int A = 18;
    G4double excitation_energy = 0*keV;
       
    G4ParticleDefinition* ion = 
        G4ParticleTable::GetParticleTable()->GetIon(Z, A, excitation_energy);

    particle_gun->SetParticleDefinition(ion);
    particle_gun->SetParticleCharge(0*eplus);
}

