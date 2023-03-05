#include "auton/GrabberAction.h"
#include "Robot.h"

GrabberAction::GrabberAction(grabberAction requestedGrabberAction) {
    m_grabberAction = requestedGrabberAction;
}

void GrabberAction::ActionInit() {

}

CORE::COREAutonAction::actionStatus GrabberAction::Action() {
    switch(m_grabberAction) {
        case GRABBER_GRAB:
            Robot::GetInstance()->grabberSubsystem.SetClaw(true);
            break;
        case GRABBER_RELEASE:
            Robot::GetInstance()->grabberSubsystem.SetClaw(false);
            break;
    }
    return COREAutonAction::actionStatus::END;
}

void GrabberAction::ActionEnd() {
    
}
