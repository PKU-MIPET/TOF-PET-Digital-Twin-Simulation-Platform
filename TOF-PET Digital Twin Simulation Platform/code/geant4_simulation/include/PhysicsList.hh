//
// Created by wangm on 17-8-7.
//

#ifndef PHYSICSLIST_h
#define PHYSICSLIST_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class OpNovicePhysicsListMessenger;

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpMieHG;
class G4OpBoundaryProcess;

class PhysicsList: public G4VUserPhysicsList
{
public:
    PhysicsList();
    ~PhysicsList();

public:
    virtual void ConstructParticle();
    virtual void ConstructProcess();
    virtual void SetCuts();
    void ConstructDecay();
    void ConstructEM();
    void ConstructOp();
    void ConstructHadron();
    void ConstructNuclear();
};

#endif
