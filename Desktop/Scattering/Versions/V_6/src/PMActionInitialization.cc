#include "PMActionInitialization.hh"
#include "PMPrimaryGenerator.hh"
#include "PMRun.hh"
#include "PMEventAction.hh" // <--- Incluir

PMActionInitialization::PMActionInitialization()
{}

PMActionInitialization::~PMActionInitialization()
{}

void PMActionInitialization::BuildForMaster() const
{
    SetUserAction(new PMRunAction());
}

void PMActionInitialization::Build() const
{
    SetUserAction(new PMPrimaryGenerator());
    SetUserAction(new PMRunAction());
    SetUserAction(new PMEventAction()); // <--- Registrar aquí
}