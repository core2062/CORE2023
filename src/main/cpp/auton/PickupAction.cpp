#include "auton/PickupAction.h"
#include "Robot.h"

PickupAction::PickupAction(pickupAction requestedPickupAction) {
    m_pickupAction = requestedPickupAction;
}

void PickupAction::ActionInit() {

}

CORE::COREAutonAction::actionStatus PickupAction::Action() {
    switch(m_pickupAction) {
        case PICKUP:
            Robot::GetInstance()->scoringAssembly.SetWantedState(WANT_TO_PICKUP);
            break;
    }
    return COREAutonAction::actionStatus::END;
}

void PickupAction::ActionEnd() {
    
}