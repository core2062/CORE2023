#include "GrabberSubsystem.h"

GrabberSubsystem::GrabberSubsystem(): m_clawActuator(frc::PneumaticsModuleType::REVPH, CLAW_IN_PORT, CLAW_OUT_PORT){}

void GrabberSubsystem::robotInit(){
    operatorJoystick->RegisterButton(CORE::COREJoystick::LEFT_BUTTON);
}

void GrabberSubsystem::teleopInit() {
	m_clawActuator.Set(DoubleSolenoid::kForward);
}

void GrabberSubsystem::teleop(){   
    if (operatorJoystick->GetRisingEdge(CORE::COREJoystick::JoystickButton::LEFT_BUTTON)){
		m_clawActuator.Toggle();
		m_clawActive = !m_clawActive;
	} 
	SmartDashboard::PutBoolean("Claw Open", m_clawActive);
}