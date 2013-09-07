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


// USER//
#include "PhysicsList.hh"

// GEANT4 //
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmDNAPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParallelWorldProcess.hh"
#include "G4ChargedGeantino.hh"



PhysicsList::PhysicsList()
{
    //RegisterPhysics(new G4EmStandardPhysics_option1());
    //RegisterPhysics(new G4EmStandardPhysics_option2());
    //RegisterPhysics(new G4EmStandardPhysics_option3());
    //RegisterPhysics(new G4EmLivermorePhysics());
    //RegisterPhysics(new G4EmPenelopePhysics());

    decay_physics = new G4DecayPhysics();
    radioactive_physics = new G4RadioactiveDecayPhysics();
    standard_physics = new G4EmStandardPhysics_option2();
}

PhysicsList::~PhysicsList()
{;}

void PhysicsList::ConstructParticle()
{
    decay_physics->ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
    AddTransportation();

    G4ParallelWorldProcess* parallel_world_process =
        new G4ParallelWorldProcess("parallel world process");
    parallel_world_process->SetParallelWorld("parallel world");
    parallel_world_process->SetLayeredMaterialFlag();

    theParticleIterator->reset();
    while((*theParticleIterator)()) {
        G4ParticleDefinition* particle = theParticleIterator->value();
        
        if(particle!=G4ChargedGeantino::Definition()) {
            G4ProcessManager* pmanager = particle->GetProcessManager();
            pmanager->AddProcess(parallel_world_process);
            
            if(parallel_world_process->IsAtRestRequired(particle)) {
                pmanager->SetProcessOrdering(parallel_world_process, idxAtRest, 9999);
            }
            
            pmanager->SetProcessOrdering(parallel_world_process, idxAlongStep, 1);
            pmanager->SetProcessOrdering(parallel_world_process, idxPostStep, 9999);
        }
    }

    standard_physics->ConstructProcess();
    decay_physics->ConstructProcess();
    radioactive_physics->ConstructProcess();
}


void PhysicsList::SetCuts()
{
    SetCutsWithDefault();
    
    //SetCutValue(5*mm, "gamma");
    //SetCutValue(5*mm, "e-");
    //SetCutValue(5*mm, "e+");
}

