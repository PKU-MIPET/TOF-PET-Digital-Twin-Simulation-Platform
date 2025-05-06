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
/// \file B1SteppingAction.hh
/// \brief Definition of the B1SteppingAction class

#ifndef B1SteppingAction_h
#define B1SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"
#include "G4String.hh"
class B1EventAction;

class G4LogicalVolume;

/// Stepping action class
/// 

class OutputPathMessenger : public G4UImessenger {
private:
    G4UIdirectory* customDir;               // 自定义目录
    G4UIcmdWithAString* setOutputPathCmd;   // 设置路径命令
    G4String outputPath;

public:
    OutputPathMessenger() {
        // 创建自定义目录
        customDir = new G4UIdirectory("/custom/");
        customDir->SetGuidance("Custom commands for output control");

        // 创建设置路径命令
        setOutputPathCmd = new G4UIcmdWithAString("/custom/setOutputPath", this);
        setOutputPathCmd->SetGuidance("Set the output file path.");
        setOutputPathCmd->SetParameterName("outputPath", false);
        setOutputPathCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    }

    ~OutputPathMessenger() {
        delete setOutputPathCmd;
        delete customDir;
    }

    void SetNewValue(G4UIcommand* command, G4String newValue) override {
        if (command == setOutputPathCmd) {
            outputPath = newValue;
            G4cout << "Output path set to: " << outputPath << G4endl;
        }
    }

    const G4String& GetOutputPath() const {
        return outputPath;
    }
};

//
class B1SteppingAction : public G4UserSteppingAction
{
  public:
    B1SteppingAction(B1EventAction* eventAction,OutputPathMessenger* messenger);
    virtual ~B1SteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

    // 添加构造函数，接受 messenger 作为参数
    //B1SteppingAction(OutputPathMessenger* messenger);
    OutputPathMessenger* messenger;

  private:
    B1EventAction*  fEventAction;
    G4LogicalVolume* fScoringVolume;
   // OutputPathMessenger* messenger;
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
