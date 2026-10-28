#include "PMRun.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"

PMRunAction::PMRunAction()
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetNtupleMerging(true); 

    analysisManager->CreateNtuple("MuonData", "Datos de seguimiento de 4 planos para MCS de muones");
    
    // Entrada 1 (Cerca del bloque)
    analysisManager->CreateNtupleDColumn("x_in1");
    analysisManager->CreateNtupleDColumn("y_in1");
    analysisManager->CreateNtupleDColumn("z_in1");
    analysisManager->CreateNtupleIColumn("id_in1");
    
    // Entrada 2 (Más alejada / Superior)
    analysisManager->CreateNtupleDColumn("x_in2");
    analysisManager->CreateNtupleDColumn("y_in2");
    analysisManager->CreateNtupleDColumn("z_in2");
    analysisManager->CreateNtupleIColumn("id_in2");
    
    // Salida 1 (Cerca del bloque)
    analysisManager->CreateNtupleDColumn("x_out1");
    analysisManager->CreateNtupleDColumn("y_out1");
    analysisManager->CreateNtupleDColumn("z_out1");
    analysisManager->CreateNtupleIColumn("id_out1");
    
    // Salida 2 (Más alejada / Inferior)
    analysisManager->CreateNtupleDColumn("x_out2");
    analysisManager->CreateNtupleDColumn("y_out2");
    analysisManager->CreateNtupleDColumn("z_out2");
    analysisManager->CreateNtupleIColumn("id_out2");
    
    analysisManager->FinishNtuple();
}

PMRunAction::~PMRunAction()
{
}

void PMRunAction::BeginOfRunAction(const G4Run* /*run*/)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    G4String fileName = "muon_mcs_results.root";
    analysisManager->OpenFile(fileName);
}

void PMRunAction::EndOfRunAction(const G4Run* /*run*/)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}