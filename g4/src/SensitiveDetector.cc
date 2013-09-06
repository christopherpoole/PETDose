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
#include "SensitiveDetector.hh"

// GEANT4 //
#include "G4ProcessType.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"


SensitiveDetector::SensitiveDetector(const G4String& name)
        : G4VSensitiveDetector(name)
{
    hits = 0;

    npy_intp dims[] = {1024};
    energy = pyublas::numpy_vector<float> (1, dims);
    std::fill(energy.begin(), energy.end(), 0.0);
}


SensitiveDetector::~SensitiveDetector()
{
}


void SensitiveDetector::Initialize(G4HCofThisEvent*) 
{
}


G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* touchable)
{
/*
    G4int index0 = ((G4TouchableHistory*)
        (step->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(0);

    G4int index1 = ((G4TouchableHistory*)
        (step->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(1);

    G4int index2 = ((G4TouchableHistory*)
        (step->GetPreStepPoint()->GetTouchable()))->GetReplicaNumber(2);
*/
    //G4cout << index0 << G4endl;
    //G4cout << " " << index1 << G4endl;
    //G4cout << "  " << index2 << G4endl;

    double e = step->GetTrack()->GetKineticEnergy();
    unsigned int index = e*1000;

    if (index < 1024)
        energy.sub(index) += 1; 

    hits += 1;
    return true;
}


void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    //if (hits > 0) {
    //    G4cout << "    " << hits << G4endl << G4endl;
    //}
    
    hits = 0;
}

void SensitiveDetector::clear()
{
}

void SensitiveDetector::PrintAll()
{
}
