#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4RotationMatrix.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4OpticalSurface.hh"
#include "G4NistManager.hh"
#include "G4LogicalBorderSurface.hh"

G4double len_box;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
    : G4VUserDetectorConstruction(),
      fScoringVolume(0)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *B1DetectorConstruction::Construct()
{
    // Get nist material manager
    G4NistManager *nist = G4NistManager::Instance();
    G4bool isotopes = true;

    // Envelope parameters
    //
    G4double env_sizeXY = 20 * cm, env_sizeZ = 30 * cm;
    G4Material *Air = nist->FindOrBuildMaterial("G4_AIR"); 

    // Vacuum material
    G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic"); 
    G4Material *Si_Mat = nist->FindOrBuildMaterial("G4_Si");       // Si

    // Use Geant4 elements and material libraries
    G4Element *el_C = nist->FindOrBuildElement("C", isotopes);
    G4Element *el_Ga = nist->FindOrBuildElement("Ga", isotopes);
    G4Element *el_As = nist->FindOrBuildElement("As", isotopes);
    G4Element *el_N = nist->FindOrBuildElement("N", isotopes);
    // G4Element *el_Si = nist->FindOrBuildElement("Si", isotopes);
    G4Element *el_Cs = nist->FindOrBuildElement("Cs", isotopes);
    G4Element *el_I = nist->FindOrBuildElement("I", isotopes);
    G4Element *el_H = nist->FindOrBuildElement("H", isotopes);

    // Custom materials
    // 按照Geant4的模板定义材料

    G4Material *CH =
        new G4Material("CH", 1.05 * g / cm3, 2); // 2
    CH->AddElement(el_C, 1);                     // 1
    CH->AddElement(el_H, 1);

    // define LYSO
    G4Element *O = nist->FindOrBuildElement("O");
    G4Element *Si = nist->FindOrBuildElement("Si");
    G4Element *Lu = nist->FindOrBuildElement("Lu");
    G4Element *Y = nist->FindOrBuildElement("Y");
    G4Material *LYSO = new G4Material("Lu1.8Y.2SiO5", 7.1 * g / cm3, 4);
    LYSO->AddElement(Lu, 9);
    LYSO->AddElement(Y, 1);
    LYSO->AddElement(Si, 5);
    LYSO->AddElement(O, 25);


    //define SG
    G4Element *H = nist->FindOrBuildElement("H");
    G4Material *siliconeGrease = new G4Material("SiliconeGrease", 1 * g / cm3, 3);
    siliconeGrease ->AddElement(H, 1);
    siliconeGrease ->AddElement(O, 4);
    siliconeGrease ->AddElement(Si, 5);

    // Option to switch on/off checking of volumes overlaps
    //
    G4bool checkOverlaps = true;

    //
    // World
    // All models need to be placed in this world body
    G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

    // 
    const G4int wlsnum = 21;
    // G4double wls_Energy[] = {2.9314 * eV, 2.9314 * eV, 2.9314 * eV, 2.9314 * eV, 2.9314 * eV, 2.9314 * eV};
    G4double wls_Energy[] = {
        1.9076923076923078*eV, 1.952755905511811*eV, 2.0*eV, 2.049586776859504*eV,
        2.1016949152542375*eV, 2.1565217391304348*eV, 2.2142857142857144*eV,
        2.2752293577981653*eV, 2.339622641509434*eV, 2.407766990291262*eV, 2.48*eV,
        2.556701030927835*eV, 2.6382978723404253*eV, 2.7252747252747254*eV,
        2.8181818181818183*eV, 2.9176470588235293*eV, 3.024390243902439*eV,
        3.1392405063291138*eV, 3.263157894736842*eV, 3.3972602739726026*eV,
        3.5428571428571427*eV};

    G4double rIndexPstyrene[wlsnum] = {
        1.82,1.82,1.82,1.82,1.82,1.82,1.82,
        1.82,1.82,1.82,1.82,1.82,1.82,1.82,
        1.82,1.82,1.82,1.82,1.82,1.82,1.82}; 
    G4double absorption1[wlsnum] = {
         1.2 * cm,1.2 * cm,1.2 * cm,1.2 * cm,1.2 * cm,1.2 * cm,1.2 * cm,
        1.2 * cm,1.2 * cm,1.2 * cm,1.2 * cm,1.2 * cm,1.2 * cm,1.2 * cm,
        1.2 * cm,1.2 * cm,1.2 * cm,1.2 * cm,1.2 * cm,1.2 * cm,1.2 * cm}; 
    G4double scintilFast[wlsnum] = {
        1,2,4,6,9,12,14,14,13,10,7,4,2,1,1,0,0,0,0,0,0}; // 
    G4MaterialPropertiesTable *fMPTPStyrene = new G4MaterialPropertiesTable();
    fMPTPStyrene->AddProperty("RINDEX", wls_Energy, rIndexPstyrene, wlsnum); // 
    fMPTPStyrene->AddProperty("ABSLENGTH", wls_Energy, absorption1, wlsnum); //
    fMPTPStyrene->AddProperty("SCINTILLATIONCOMPONENT1", wls_Energy, scintilFast,
                              wlsnum, false, true);
    // fMPTPStyrene->AddProperty(" SCINTILLATIONCOMPONENT1", wls_Energy, scintilFast, wlsnum);
    fMPTPStyrene->AddConstProperty("SCINTILLATIONYIELD", 30000. / MeV); //
    fMPTPStyrene->AddConstProperty("RESOLUTIONSCALE", 1.0);             //
    fMPTPStyrene->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 35.0 * ns);
    // fMPTPStyrene->AddConstProperty("SCINTILLATIONTIMECONSTANT1 ", 1.0 * ns);
    LYSO->SetMaterialPropertiesTable(fMPTPStyrene); // 

    G4Box *solidWorld =
        new G4Box("World",                    // its name
                  0.1 * m, 0.1 * m, 0.1 * m); // its size  

    G4LogicalVolume *logicWorld =
        new G4LogicalVolume(solidWorld, // its solid
                            vacuum,     // its material
                            "World");   // its name

    G4VPhysicalVolume *physWorld =
        new G4PVPlacement(0,               // no rotation
                          G4ThreeVector(), // G4ThreeVector() 
                          logicWorld,      // its logical volume
                          "World",         // its name
                          0,               // its mother  volume
                          false,           // no boolean operation
                          0,               // copy number
                          checkOverlaps);  // overlaps checking

    // 
    //  Define the target thickness for each_layer
    //  G4double thickness_layer = 1.0;

    // Define the target thickness
    G4double thickness = 5;
    G4double side = 3.0;
    len_box = side;                                                                          // Real thickness
    G4Box *A1 = new G4Box("target", (side / 2) * mm, (side / 2) * mm, (thickness / 2) * mm); // This length is only half the real length
    // G4Tubs *A1 = new G4Tubs("target", 0 * mm, side / 2 * mm, (side / 2) * mm, 0 * deg, 360 * deg); //
    // G4Tubs *A1 = new G4Tubs("target", 0.0 * mm, 9 * mm, (thickness / 2) * mm, 0 * deg, 360 * deg); //

    G4LogicalVolume *logicA1 =
        new G4LogicalVolume(A1,        // its solid
                            LYSO,    // its material
                            "target"); // its name

    G4VisAttributes *VisAtt06 = new G4VisAttributes(G4Colour(0 / 255., 0 / 255., 255 / 255., 1.0)); // blue
    VisAtt06->SetForceSolid(true);
    logicA1->SetVisAttributes(VisAtt06);

    auto physiA1 = new G4PVPlacement(0, // no rotation
                                     G4ThreeVector(0.0 * mm, 0.0 * mm, thickness / 2 * mm),
                                     logicA1,        // its logical volume
                                     "target",       // its name
                                     logicWorld,     // its mother  volume
                                     false,          // no boolean operation
                                     0,              // copy number
                                     checkOverlaps); // overlaps checking
    


    //define SG
    const G4int num = 21;
    G4double greaseIndex[num] = {
        1.46 , 1.46 , 1.46 , 1.46 , 1.46 , 1.46 , 1.46 ,
        1.46 , 1.46 , 1.46 , 1.46 , 1.46 , 1.46 , 1.46 ,
        1.46 , 1.46 , 1.46 , 1.46 , 1.46 , 1.46 , 1.46
    };
    G4double greaseAbsLength[num] = {
        20*m , 20*m , 20*m , 20*m , 20*m , 20*m , 20*m ,
        20*m , 20*m , 20*m , 20*m , 20*m , 20*m , 20*m ,
        20*m , 20*m , 20*m , 20*m , 20*m , 20*m , 20*m
    };

    G4MaterialPropertiesTable* greaseMPT = new G4MaterialPropertiesTable();
    greaseMPT->AddProperty("RINDEX",wls_Energy,greaseIndex,num);
    greaseMPT->AddProperty("ABSLENGTH",wls_Energy,greaseAbsLength,num);
    siliconeGrease->SetMaterialPropertiesTable(greaseMPT);

    G4Box *SG = new G4Box("SG", ((side / 2)) * mm, (side / 2) * mm, 0.1/2 * mm);
    G4LogicalVolume *logicSG =
        new G4LogicalVolume(SG,     // its solid
                            siliconeGrease, // its material
                            "SG");  // its name


    auto physiSG = new G4PVPlacement(0, // no rotation
                                     G4ThreeVector(0.0 * mm, 0 * mm, (thickness + 0.05) * mm),
                                     logicSG,        // its logical volume
                                     "SG",           // its name
                                     logicWorld,     // its mother  volume
                                     false,          // no boolean operation
                                     0,              // copy number
                                     checkOverlaps); // overlaps checking

    //  G4Tubs *A2 = new G4Tubs("A2", 3.5 * mm, 3.5 * mm, (2 / 2) * mm, 0 * deg, 360 * deg); //定义圆柱
    G4Box *A2 = new G4Box("A2", ((side / 2)) * mm, (side / 2) * mm, 2.5 * mm);
    G4LogicalVolume *logicA2 =
        new G4LogicalVolume(A2,     // its solid
                            Si_Mat, // its material
                            "A2");  // its name

    G4VisAttributes *VisAtt07 = new G4VisAttributes(G4Colour(225 / 255., 0 / 255., 0 / 255., 1)); // red
    VisAtt07->SetForceSolid(true);
    logicA2->SetVisAttributes(VisAtt07);

    auto physiA2 = new G4PVPlacement(0, // no rotation
                                     G4ThreeVector(0.0 * mm, 0 * mm, (thickness + 2.5 + 0.1 ) * mm),
                                     logicA2,        // its logical volume
                                     "A2",           // its name
                                     logicWorld,     // its mother  volume
                                     false,          // no boolean operation
                                     0,              // copy number
                                     checkOverlaps); // overlaps checking


    const G4int NUM = 21;

    G4double PP[NUM] = {1.77*eV, 1.83*eV, 1.88*eV, 1.94*eV, 2.00*eV, 2.07*eV, 
    2.14*eV, 2.22*eV, 2.30*eV, 2.39*eV, 2.48*eV, 2.59*eV, 2.70*eV, 2.82*eV, 
    2.96*eV, 3.11*eV, 3.27*eV, 3.45*eV, 3.65*eV, 3.88*eV, 4.14*eV};
    G4double reflectivity_cc[NUM] = {0.95,0.95,0.95,0.95,0.95,0.95,0.95,
    0.95,0.95,0.95,0.95,0.95,0.95,0.95,
    0.95,0.95,0.95,0.95,0.95,0.95,0.95}; // 

    G4double pp[NUM] = {1.77*eV, 1.83*eV, 1.88*eV, 1.94*eV, 2.00*eV, 2.07*eV, 2.14*eV, 2.22*eV, 2.30*eV, 2.39*eV, 2.48*eV, 2.59*eV, 2.70*eV, 2.82*eV, 2.96*eV, 3.11*eV, 3.27*eV, 3.45*eV, 3.65*eV, 3.88*eV, 4.14*eV};
    const G4int pnum = sizeof(pp) / sizeof(G4double);
    G4double photoCath_pde[NUM] = {0.0695, 0.0777, 0.0913, 0.1040, 0.1185, 0.1340, 0.1518, 0.1768, 0.1986, 0.2277, 0.2631, 0.3007, 0.3351, 0.3654, 0.3809, 0.3673, 0.3393, 0.3028, 0.2810, 0.2496, 0.1912};
    G4double photoCath_refl[NUM] = {0.34, 0.34, 0.34, 0.34, 0.34, 0.35, 0.35, 0.36, 0.37, 0.38, 0.39, 0.40, 0.40, 0.41, 0.41, 0.42, 0.42, 0.43, 0.43, 0.44, 0.44};
    G4double photoCath_eff[pnum] = {0};

    for( int i = 0 ; i <pnum ; i++ ){
        photoCath_eff[i] = photoCath_pde[i]/(1-photoCath_refl[i]);
    }

    G4MaterialPropertiesTable* photo_mt = new G4MaterialPropertiesTable();
    photo_mt->AddProperty("EFFICIENCY", pp, photoCath_eff, pnum);
    photo_mt->AddProperty("REFLECTIVITY", pp, photoCath_refl, pnum);

    G4OpticalSurface* photoCath_opsurf = new G4OpticalSurface("photoCath_opsurf", glisur , polished, dielectric_metal);
    photoCath_opsurf->SetMaterialPropertiesTable( photo_mt );

    G4double efficiency_cc[NUM] =
    {0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0};

    G4OpticalSurface *OpbbSurface =
        new G4OpticalSurface("CH-A2-Surface");

    //  G4LogicalBorderSurface * ccSurface =
    new G4LogicalBorderSurface("CH-A2-Surface",
                               physiSG, physiA2,
                               photoCath_opsurf);


    // G4MaterialPropertiesTable *myST2 = new G4MaterialPropertiesTable();
    // myST2->AddProperty("REFLECTIVITY", PP, reflectivity2, NUM);
    // OpbbSurface->SetMaterialPropertiesTable(myST2);

    G4OpticalSurface *OpccSurface =
        new G4OpticalSurface("CH-World-Surface");

    //  G4LogicalBorderSurface * ccSurface =
    new G4LogicalBorderSurface("CH-World-Surface",
                               physiA1, physWorld,
                               OpccSurface);
    OpccSurface->SetType(dielectric_metal);
    OpccSurface->SetFinish(polished);
    OpccSurface->SetModel(unified);
    OpccSurface->SetPolish(0.9);

    // reflection
    G4MaterialPropertiesTable *myST1 = new G4MaterialPropertiesTable();
    myST1->AddProperty("REFLECTIVITY", PP, reflectivity_cc, NUM);
    myST1->AddProperty("EFFICIENCY", PP , efficiency_cc , NUM );
    OpccSurface->SetMaterialPropertiesTable(myST1);

    return physWorld;
}