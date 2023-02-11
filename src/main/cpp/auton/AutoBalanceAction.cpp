#include "auton/AutoBalanceAction.h"
#include "Robot.h"

AutoBalanceAction::AutoBalanceAction(autoBalanceAction requestedAutoBalanceAction) {
    m_autoBalanceAction = requestedAutoBalanceAction;
}

void AutoBalanceAction::ActionInit() {

}

CORE::COREAutonAction::actionStatus AutoBalanceAction::Action() {
    switch(m_autoBalanceAction) {
        case AUTO_BALANCE:
            Robot::GetInstance()->driveSubsystem.Balance();
            break;
    }
    return COREAutonAction::actionStatus::END;
}

void AutoBalanceAction::ActionEnd() {
    
}