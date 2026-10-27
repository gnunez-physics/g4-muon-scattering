#ifndef PMRUN_HH
#define PMRUN_HH

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class PMRunAction : public G4UserRunAction
{
public:
    PMRunAction();
    virtual ~PMRunAction();

    virtual void BeginOfRunAction(const G4Run* run);
    virtual void EndOfRunAction(const G4Run* run);
};

#endif