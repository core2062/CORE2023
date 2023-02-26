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
            Robot::GetInstance()->grabberSubsystem.setGrabber(true);
            break;
        case GRABBER_RELEASE:
            Robot::GetInstance()->grabberSubsystem.setGrabber(false);
            break;
    }
    return COREAutonAction::actionStatus::END;
}

void GrabberAction::ActionEnd() {
    
}

void GrabberSubsystem::setGrabber(bool set) {
	if (set)
	{
		m_clawActuator.Set(DoubleSolenoid::kForward);
		m_clawActive = true;
	} else {
		m_clawActuator.Set(DoubleSolenoid::kReverse);
		m_clawActive = false;
	}
}