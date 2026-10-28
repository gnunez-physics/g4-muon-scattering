#ifndef PMEVENTACTION_HH
#define PMEVENTACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"

class PMEventAction : public G4UserEventAction
{
public:
    PMEventAction();
    virtual ~PMEventAction();

    virtual void BeginOfEventAction(const G4Event* event) override;
    virtual void EndOfEventAction(const G4Event* event) override;

    // Setters públicos para los 4 planos
    void SetEntry1Data(G4double x, G4double y, G4double z, G4int id);
    void SetEntry2Data(G4double x, G4double y, G4double z, G4int id);
    void SetExit1Data(G4double x, G4double y, G4double z, G4int id);
    void SetExit2Data(G4double x, G4double y, G4double z, G4int id);

    // Getters públicos (necesarios para PMMysensitiveDetector)
    G4int GetEntry1Id() const { return fId_in1; }
    G4int GetEntry2Id() const { return fId_in2; }
    G4int GetExit1Id()  const { return fId_out1; }
    G4int GetExit2Id()  const { return fId_out2; }

private:
    G4double fX_in1, fY_in1, fZ_in1;
    G4int    fId_in1;
    
    G4double fX_in2, fY_in2, fZ_in2;
    G4int    fId_in2;
    
    G4double fX_out1, fY_out1, fZ_out1;
    G4int    fId_out1;
    
    G4double fX_out2, fY_out2, fZ_out2;
    G4int    fId_out2;
};

#endif