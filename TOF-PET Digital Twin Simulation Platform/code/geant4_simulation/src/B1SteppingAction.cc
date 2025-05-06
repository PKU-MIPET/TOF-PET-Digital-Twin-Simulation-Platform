//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include <vector>
#include <math.h>
#include <mutex>

using namespace std;
G4double energy_record; // 
extern G4double counts_for_p;
extern G4double len_box;

extern G4double array_record[500][500]; // 

// extern G4double Evnet_energy;

extern G4int total_countA2;
extern G4int total_countA3;
extern G4double total_counts_for_in;
mutex temp_lock, temp_lock2, temp_lock3;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction *eventAction,OutputPathMessenger* mess)
    : G4UserSteppingAction(),
      fEventAction(eventAction),
      fScoringVolume(0)
     {
            this->messenger = mess;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step *step)
{

  if (step->GetTrack()->GetParticleDefinition()->GetParticleName() == "opticalphoton")
  {
    if (step->GetTrack()->GetCurrentStepNumber() == 1)
    {
      fEventAction->PhotonStartTime = step->GetPreStepPoint()->GetGlobalTime();

      // temp_lock2.lock();
      // ofstream StepCheck2;
      // StepCheck2.open("./data/StepCheck2.txt", ios::app);
      // StepCheck2 << step->GetPreStepPoint()->GetKineticEnergy() << G4endl;
      // StepCheck2.close();
      // temp_lock2.unlock();
    }

    if (step->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName() == "SG")
    {
      if (step->GetPostStepPoint()->GetPhysicalVolume())
      {
        if (step->GetPostStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName() == "A2")
        {
          // G4double xpos = step->GetPreStepPoint()->GetPosition().getX();
          // G4double ypos = step->GetPreStepPoint()->GetPosition().getY();
          // G4double zpos = step->GetPreStepPoint()->GetPosition().getZ();

          G4double xpos = step->GetPostStepPoint()->GetPosition().getX();
          G4double ypos = step->GetPostStepPoint()->GetPosition().getY();
          G4double zpos = step->GetPostStepPoint()->GetPosition().getZ();

          temp_lock.lock();
          ofstream StepCheck;
          StepCheck.open(messenger->GetOutputPath(), ios::app);
          StepCheck << xpos << "\t";
          StepCheck << ypos << "\t";
          StepCheck << zpos << "\t";
          StepCheck << fEventAction->PhotonStartTime << "\t";
          StepCheck << step->GetPreStepPoint()->GetGlobalTime() << "\t";
          StepCheck << fEventAction->EevntIdCheck << G4endl;
          temp_lock.unlock();
        }
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
