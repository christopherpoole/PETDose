//////////////////////////////////////////////////////////////////////////
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


#ifndef SENSITIVEDETECTOR_HH
#define	SENSITIVEDETECTOR_HH


// GEANT4 //
#include "globals.hh"

#include "G4VSensitiveDetector.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SDManager.hh"

class G4Step;
class G4TouchableHistory;
class G4HCofThisEvent;
class DetectorConstruction;

// STL //
#include <vector>


class SensitiveDetector : public G4VSensitiveDetector {
  public:
    SensitiveDetector(const G4String& name);
    virtual ~SensitiveDetector();

    void Init();
    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);
    void clear();
    void PrintAll();

  public:
    G4int hits;

    std::vector<double>* primaries;
    std::vector<double>* secondaries;
};

#endif	// SENSITIVEDETECTOR_HH

