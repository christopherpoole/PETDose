#!/bin/bash

cd /media/nfsroot/software/cern/geant4.9.6.p02/bin
source geant4.sh
cd ~

export G4LEDATA=/media/nfsroot/software/cern/geant4.9.6/data/G4EMLOW6.32
export PYTHONPATH=$PYTHONPATH:/media/nfsroot/software/cern/geant4.9.6.p02/python/lib

# Other
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib

cd /media/nfsroot/software/PETDose

python -B main.py $macro $dicom_dir $pet_aq $ct_aq $histories $PBS_ARRAYID

