#include "auton/DelayAction.h"
#include "Robot.h"

DelayAction::DelayAction() {}

void DelayAction::ActionInit() {}

CORE::COREAutonAction::actionStatus DelayAction::Action() {
    return COREAutonAction::actionStatus::CONTINUE;
}

void DelayAction::ActionEnd() {}