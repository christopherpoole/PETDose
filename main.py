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
import argparse

import Geant4
import g4 

import dicom

if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument('--radius', type=float, required=True,
            help='Set the PET gantry radius.')
    parser.add_argument('--width', type=float, required=True,
            help='Set the crystal width.')
    parser.add_argument('--length', type=float, required=True,
            help='Set the crystal length.')
    parser.add_argument('--crystals', type=int, required=True,
            help='Set the number of crystals per block.')
    parser.add_argument('--blocks', type=int, required=True,
            help='Set the number of blocks per head.')
    parser.add_argument('--heads', type=int, required=True,
            help='Set the number of heads around the gantry.')

    parser.add_argument('--dicom', type=str,
            help='Optional DICOM directory from which to load PET and CT data.')
    parser.add_argument('--ct_acquisition', type=int,
            help='Set the CT acquisition to load.')
    parser.add_argument('--pet_acquisition', type=int,
            help='Set the PET acquisition to load, -1 for all.')

    parser.add_argument('--histories', type=int, default=0,
            help='Set the number of initial histories for this run.')
    parser.add_argument('--run_id', type=int, default=0,
            help='Set the ID for this run.')
    parser.add_argument('--start_session', type=bool, default=False,
            help='Start a GEANT4 session after executing a macro.')
    parser.add_argument('--macro', type=str,
            help='Optional GEANT4 run macro, this will override any GPS settings.')
   
    args = parser.parse_args()

    detector_construction = g4.DetectorConstruction()
    g4.RegisterParallelWorld(detector_construction)
    detector_construction.SetCTDirectory(args.dicom, args.ct_acquisition)
    detector_construction.SetRadius(args.radius)
    detector_construction.SetCrystalWidth(args.width)
    detector_construction.SetCrystalLength(args.length)
    detector_construction.SetNumberOfCrystals(args.crystals, args.crystals)
    detector_construction.SetNumberOfBlocks(args.blocks, args.blocks)
    detector_construction.SetNumberOfHeads(args.heads)
    Geant4.gRunManager.SetUserInitialization(detector_construction)

    physics_list = g4.PhysicsList()
    physics_list.RegisterPhysics(g4.StepLimiterBuilder())
    Geant4.gRunManager.SetUserInitialization(physics_list)

    primary_generator = g4.PrimaryGeneratorAction()
    primary_generator.LoadActivityData(args.dicom, detector_construction.GetCTOrigin())
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
    Geant4.gApplyUICommand("/control/execute %s" % args.macro)
    Geant4.gRunManager.BeamOn(args.histories)
    
    if args.start_session:
        Geant4.StartUISession()

    detector_construction.SaveEnergyHistogram("output/energy_%i.npy" % args.run_id)
    detector_construction.SaveCountsHistogram("output/counts_%i.npy" % args.run_id)

    stepping_action.SaveMomentumHistogram("output/momentum_%i.npy" % args.run_id)
    stepping_action.SaveStepsHistogram("output/steps_%i.npy" % args.run_id)
    raw_input("Press <enter> to exit.")

