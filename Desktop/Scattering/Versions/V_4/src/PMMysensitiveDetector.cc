#include "PMMySensitiveDetector.hh"
#include "PMEventAction.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
}

MySensitiveDetector::~MySensitiveDetector()
{
}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory * /*ROhist*/)
{
    // 1. OBTENEMOS LA IDENTIDAD DE LA PARTÍCULA
    G4ParticleDefinition *particleDef = aStep->GetTrack()->GetDefinition();
    G4String particleName = particleDef->GetParticleName();

    // 2. FILTRO ESTRICTO: Solo aceptamos muones negativos ("mu-") 
    if (particleName != "mu-") 
    {
        return false; // Ignoramos otras partículas
    }

    // 3. Extraemos la posición exacta del paso
    G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();
    
    // 4. Obtenemos el número de copia (el píxel específico de la rejilla)
    G4int copyNo = aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber();

    // 5. Identificamos a qué volumen pertenece este detector sensible
    G4String detName = SensitiveDetectorName;

    // Buscamos el EventAction actual para reportar los datos
    const PMEventAction* eventAction = static_cast<const PMEventAction*>(
        G4RunManager::GetRunManager()->GetUserEventAction()
    );

    PMEventAction* mutableEventAction = const_cast<PMEventAction*>(eventAction);

    if (mutableEventAction)
    {
        if (detName == "EntryDetector1")
        {
            mutableEventAction->SetEntry1Data(pos.x(), pos.y(), pos.z(), copyNo);
        }
        else if (detName == "EntryDetector2")
        {
            mutableEventAction->SetEntry2Data(pos.x(), pos.y(), pos.z(), copyNo);
        }
        else if (detName == "ExitDetector1")
        {
            mutableEventAction->SetExit1Data(pos.x(), pos.y(), pos.z(), copyNo);
        }
        else if (detName == "ExitDetector2")
        {
            mutableEventAction->SetExit2Data(pos.x(), pos.y(), pos.z(), copyNo);
        }
    }

    return true;
}