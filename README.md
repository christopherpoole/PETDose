# PETDose

Transport radionuclide activity through a CT dataset.

## Compilation

The GEANT4 simulation requires compilation as a Python library.
To do this perform the following:

* cd ./g4lib/g4
* mkdir build
* cd build
* cmake ..
* make

## Usage

You will need a DICOM PET/CT:

    python main.py macros/vis.mac <dicom dir> <pet aquisition> <ct aquisition> <histories> <run id>
   
