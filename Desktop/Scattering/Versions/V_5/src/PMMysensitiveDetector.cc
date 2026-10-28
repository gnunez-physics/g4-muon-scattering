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
    G4ParticleDefinition *particleDef = aStep->GetTrack()->GetDefinition();
    G4String particleName = particleDef->GetParticleName();

    if (particleName != "mu-") 
    {
        return false;
    }

    G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();
    G4int copyNo = aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber();
    G4String detName = SensitiveDetectorName;

    const PMEventAction* eventAction = static_cast<const PMEventAction*>(
        G4RunManager::GetRunManager()->GetUserEventAction()
    );

    PMEventAction* mutableEventAction = const_cast<PMEventAction*>(eventAction);

    if (mutableEventAction)
    {
        if (detName == "EntryDetector1")
        {
            if (mutableEventAction->GetEntry1Id() == -1)
            {
                mutableEventAction->SetEntry1Data(pos.x(), pos.y(), pos.z(), copyNo);
            }
        }
        else if (detName == "EntryDetector2")
        {
            if (mutableEventAction->GetEntry2Id() == -1)
            {
                mutableEventAction->SetEntry2Data(pos.x(), pos.y(), pos.z(), copyNo);
            }
        }
        else if (detName == "ExitDetector1")
        {
            if (mutableEventAction->GetExit1Id() == -1)
            {
                mutableEventAction->SetExit1Data(pos.x(), pos.y(), pos.z(), copyNo);
            }
        }
        else if (detName == "ExitDetector2")
        {
            if (mutableEventAction->GetExit2Id() == -1)
            {
                mutableEventAction->SetExit2Data(pos.x(), pos.y(), pos.z(), copyNo);
            }
        }
    }

    return true;
}