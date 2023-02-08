#include "GrabberSubsystem.h"

using namespace CORE;

GrabberSubsystem::GrabberSubsystem(): m_clawActuator(frc::PneumaticsModuleType::REVPH, ACTUATOR_IN_PORT, ACTUATOR_OUT_PORT){}


void GrabberSubsystem::robotInit(){
    operatorJoystick->RegisterButton(CORE::COREJoystick::LEFT_BUTTON);
}


void GrabberSubsystem::teleopInit() {
}

void GrabberSubsystem::teleop(){   
    if (operatorJoystick->GetRisingEdge(CORE::COREJoystick::JoystickButton::LEFT_BUTTON) && m_clawActive == false){
		setGrabber(true);
	} else if (operatorJoystick->GetRisingEdge(CORE::COREJoystick::JoystickButton::LEFT_BUTTON) && m_clawActive == true){
		setGrabber(false);
    }
	SmartDashboard::PutBoolean("Claw Open", m_clawActive);
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