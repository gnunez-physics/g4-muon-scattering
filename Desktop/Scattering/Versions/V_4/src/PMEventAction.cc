#include "PMEventAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"

PMEventAction::PMEventAction()
: fX_in1(0.), fY_in1(0.), fZ_in1(0.), fId_in1(-1),
  fX_in2(0.), fY_in2(0.), fZ_in2(0.), fId_in2(-1),
  fX_out1(0.), fY_out1(0.), fZ_out1(0.), fId_out1(-1),
  fX_out2(0.), fY_out2(0.), fZ_out2(0.), fId_out2(-1)
{}

PMEventAction::~PMEventAction()
{}

void PMEventAction::BeginOfEventAction(const G4Event* /*event*/)
{
    fX_in1 = 0.; fY_in1 = 0.; fZ_in1 = 0.; fId_in1 = -1;
    fX_in2 = 0.; fY_in2 = 0.; fZ_in2 = 0.; fId_in2 = -1;
    fX_out1 = 0.; fY_out1 = 0.; fZ_out1 = 0.; fId_out1 = -1;
    fX_out2 = 0.; fY_out2 = 0.; fZ_out2 = 0.; fId_out2 = -1;
}

void PMEventAction::SetEntry1Data(G4double x, G4double y, G4double z, G4int id)
{
    fX_in1 = x; fY_in1 = y; fZ_in1 = z; fId_in1 = id;
}

void PMEventAction::SetEntry2Data(G4double x, G4double y, G4double z, G4int id)
{
    fX_in2 = x; fY_in2 = y; fZ_in2 = z; fId_in2 = id;
}

void PMEventAction::SetExit1Data(G4double x, G4double y, G4double z, G4int id)
{
    fX_out1 = x; fY_out1 = y; fZ_out1 = z; fId_out1 = id;
}

void PMEventAction::SetExit2Data(G4double x, G4double y, G4double z, G4int id)
{
    fX_out2 = x; fY_out2 = y; fZ_out2 = z; fId_out2 = id;
}

void PMEventAction::EndOfEventAction(const G4Event* /*event*/)
{
    // Guardar en el ROOT solo si el muón atravesó exitosamente los 4 planos
    if (fId_in1 != -1 && fId_in2 != -1 && fId_out1 != -1 && fId_out2 != -1)
    {
        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        
        // Columna 0 a 3: Entrada 1
        analysisManager->FillNtupleDColumn(0, fX_in1);
        analysisManager->FillNtupleDColumn(1, fY_in1);
        analysisManager->FillNtupleDColumn(2, fZ_in1);
        analysisManager->FillNtupleIColumn(3, fId_in1);
        
        // Columna 4 a 7: Entrada 2
        analysisManager->FillNtupleDColumn(4, fX_in2);
        analysisManager->FillNtupleDColumn(5, fY_in2);
        analysisManager->FillNtupleDColumn(6, fZ_in2);
        analysisManager->FillNtupleIColumn(7, fId_in2);
        
        // Columna 8 a 11: Salida 1
        analysisManager->FillNtupleDColumn(8, fX_out1);
        analysisManager->FillNtupleDColumn(9, fY_out1);
        analysisManager->FillNtupleDColumn(10, fZ_out1);
        analysisManager->FillNtupleIColumn(11, fId_out1);
        
        // Columna 12 a 15: Salida 2
        analysisManager->FillNtupleDColumn(12, fX_out2);
        analysisManager->FillNtupleDColumn(13, fY_out2);
        analysisManager->FillNtupleDColumn(14, fZ_out2);
        analysisManager->FillNtupleIColumn(15, fId_out2);
        
        analysisManager->AddNtupleRow();
    }
}