# License & Copyright
# ===================
#
# Copyright 2012 Christopher M Poole <mail@christopherpoole.net>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import sys
import os
import random

import Geant4
import g4 

import dicom

if __name__ == "__main__":
    macro = sys.argv[1]
    dicom_directory = sys.argv[2]
    pet_acquisition = int(sys.argv[3])
    ct_acquisition = int(sys.argv[4])
    histories = int(sys.argv[5])
    run_id = int(sys.argv[6])

    detector_construction = g4.DetectorConstruction()
    g4.RegisterParallelWorld(detector_construction)
    detector_construction.SetCTDirectory(dicom_directory, ct_acquisition)
    detector_construction.SetRadius(800)
    detector_construction.SetCrystalWidth(50)
    detector_construction.SetCrystalLength(50)
    detector_construction.SetNumberOfCrystals(2, 2)
    detector_construction.SetNumberOfBlocks(2, 2)
    detector_construction.SetNumberOfHeads(4)
    Geant4.gRunManager.SetUserInitialization(detector_construction)

    physics_list = g4.PhysicsList()
    physics_list.RegisterPhysics(g4.StepLimiterBuilder())
    Geant4.gRunManager.SetUserInitialization(physics_list)

    primary_generator = g4.PrimaryGeneratorAction()
    primary_generator.LoadActivityData(dicom_directory, detector_construction.GetCTOrigin())
    Geant4.gRunManager.SetUserAction(primary_generator)

    event_action = g4.EventAction()
    Geant4.gRunManager.SetUserAction(event_action)

    stepping_action = g4.SteppingAction()
    Geant4.gRunManager.SetUserAction(stepping_action)

    rand_engine= Geant4.Ranlux64Engine()
    Geant4.HepRandom.setTheEngine(rand_engine)
    seed = random.randint(0, 2**32)
    Geant4.HepRandom.setTheSeed(seed)

    Geant4.gRunManager.Initialize()
    Geant4.gVisManager.Initialize()
    Geant4.gApplyUICommand("/control/execute %s" % macro)
    Geant4.gRunManager.BeamOn(histories)
    #Geant4.StartUISession()

    detector_construction.SaveEnergyHistogram("output/energy_%i.npy" % run_id)
    detector_construction.SaveCountsHistogram("output/counts_%i.npy" % run_id)

    stepping_action.SaveMomentumHistogram("output/momentum_%i.npy" % run_id)
    stepping_action.SaveStepsHistogram("output/steps_%i.npy" % run_id)
    #raw_input("Press <enter> to exit.")

