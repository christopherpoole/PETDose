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


#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

// G4VoxelData
#include "G4VoxelData.hh"
#include "G4VoxelArray.hh"
#include "DicomDataIO.hh"

// GEANT4 //
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

class G4GeneralParticleSource;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event* event);
    
  public:
    void LoadActivityData(G4String directory, G4int acquisition_number)
    {
        dicom_reader->SetModality("PT");
        dicom_reader->SetSlope(1);
        dicom_reader->SetIntercept(0);

        G4VoxelData* data = dicom_reader->ReadDirectory(directory);
        activity = new G4VoxelArray<int16_t>(data);
        max_activity = activity->GetMaxValue();
        pet_origin = activity->GetOrigin();
    };

  private:
    G4ParticleGun* particle_gun;

    DicomDataIO* dicom_reader;
    G4VoxelArray<int16_t>* activity;
    double max_activity;
    G4ThreeVector pet_origin;
};

#endif

