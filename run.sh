#!/bin/bash

cd /media/nfsroot/software/cern/geant4.9.6/bin
source geant4.sh
cd ~

export G4LEDATA=/media/nfsroot/software/cern/geant4.9.6/data/G4EMLOW6.32
export PYTHONPATH=$PYTHONPATH:/media/nfsroot/software/cern/geant4.9.6/python/lib

# Other
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib

cd /media/nfsroot/software/PETDose

python -B main.py --run_id=$PBS_ARRAYID --radius=400 --width=5 --length=10 --crystals_phi=1 --crystals_axis=100 --heads=360 --histories=10000 --dicom=/media/nfsroot/software/PETDose/data --ct_acquisition=2 --save=True --gun_x=$gunx --gun_y=$guny --gun_z=$gunz
