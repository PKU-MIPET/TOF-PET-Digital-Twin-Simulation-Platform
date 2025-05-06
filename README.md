# TOF-PET-Digital-Twin-Simulation-Platform
A TOF-PET digital twin framework based on Geant4 v11.3 and Python3 is developed, capable of simulating diverse detector configurations by alternating  parameters of scintillator, SiPM and front-end electronics. 

The Geant4 toolkit was employed to simulate the complete physical processes of gamma photons, including: physical interactions during gamma-ray propagation, interaction processes within the scintillation crystal, interactions in the optical coupling silicone, and photon capture and absorption by SiPMs. Subsequently, Python3 was employed for digital processing, which involved: generating SiPM output waveforms, simulating low-pass filtering effects, and introducing electronic noise of front-end electronic circuits. 

Project Overview
--------
This platform is a digital twin framework for TOF-PET detectors based on Geant4 v11.3 and Python3, including:
- End-to-end physical process simulation with Geant4
- Digital signal processing with Python
- Configurable detector parameter modules

Core Features
--------
■ Physical Process Simulation (Geant4)
  ✓ Gamma photon propagation physics
  ✓ Scintillation light generation and transmission
  ✓ Optical coupling in silicone layer
  ✓ Photon detection by SiPMs

■ Signal Processing (Python3)
  ✓ SiPM waveform generation
  ✓ Front-end electronics low-pass filtering
  ✓ Electronic noise simulation
  ✓ Time walk effect compensation

Directory Structure
--------
.
├── code/
│   ├── geant4_simulation/    # Geant4 simulation source code
│   │   └── build1/           # Build directory
│   │       └── StepChecktest.txt  # Raw output file
│   └── python_simulation.py  # Main signal processing program
└── results/
    ├── Geant4_output/        # Intermediate physics simulation data
    │   └── 1.txt             # Preprocessed photon data
    └── Python_output/         # Digital waveform output
        ├── 1.txt             # Time-domain waveform data
        └── 1.png             # Waveform visualization

Data Format Specification
------------
► Geant4 Raw Output (StepChecktest.txt)
  Each line contains 6 tab-separated columns:
  [X coordinate(mm)] [Y coordinate(mm)] [Z coordinate(mm)]
  [Photon generation time(ns)] [Photon emission time(ns)] [Event ID]

  Example:
  -0.13698    0.893376    5.1    19.2621    19.3167    1

► Preprocessed Data (1.txt)
  Each line contains 5 space-separated columns:
  [X coordinate(mm)] [Y coordinate(mm)] [Z coordinate(mm)]
  [Emission time(ns)] [Gamma energy(eV)]

  Example:
  0.978766 0.972246 5.1 12.6058 511000

User Guide
--------
1. Run Geant4 simulation:
   - Compile code: cd code/geant4_simulation/build1 && make
   - Execute program: ./executable

2. Configure Python processing:
   Modify paths in python_simulation.py:
   INPUT_FILE = "./results/Geant4_output/1.txt"
   OUTPUT_DIR = "./results/Python_output/"

3. Generate digital waveforms:
   python python_simulation.py

Performance Parameters
--------
○ Typical runtime: 1-2 minutes/event
○ Supported platforms: Linux/Windows (Geant4), Cross-platform (Python)
