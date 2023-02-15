#include "auton/AutoBalanceAction.h"
#include "Robot.h"

AutoBalanceAction::AutoBalanceAction(autoBalanceAction requestedAutoBalanceAction, int requestedTime) {
    m_autoBalanceAction = requestedAutoBalanceAction;
    m_timer = requestedTime;
}

void AutoBalanceAction::ActionInit() {

}

CORE::COREAutonAction::actionStatus AutoBalanceAction::Action() {
    switch(m_autoBalanceAction) {
        case AUTO_BALANCE:
        
        if ((m_timer - 1) > 0) {
            Robot::GetInstance()->driveSubsystem.Balance();
            return COREAutonAction::actionStatus::CONTINUE;
            m_timer -= 1;
        } else {
          
        } 
        break;
    }
    return COREAutonAction::actionStatus::END;
}

void AutoBalanceAction::ActionEnd() {
    
}