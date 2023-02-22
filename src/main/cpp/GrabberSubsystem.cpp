// #include "GrabberSubsystem.h"

// using namespace CORE;

GrabberSubsystem::GrabberSubsystem(): m_clawActuator(frc::PneumaticsModuleType::REVPH, CLAW_IN_PORT, CLAW_OUT_PORT){}


void GrabberSubsystem::robotInit(){
    operatorJoystick->RegisterButton(CORE::COREJoystick::LEFT_BUTTON);
}


void GrabberSubsystem::teleopInit() {
}

void GrabberSubsystem::teleop(){   
    if (operatorJoystick->GetRisingEdge(CORE::COREJoystick::JoystickButton::LEFT_BUTTON) && m_clawActive == false){
		m_clawActuator.Set(DoubleSolenoid::kForward);
		m_clawActive = true;
	} else if (operatorJoystick->GetRisingEdge(CORE::COREJoystick::JoystickButton::LEFT_BUTTON) && m_clawActive == true){
		m_clawActuator.Set(DoubleSolenoid::kReverse);
		m_clawActive = false;
    }
	SmartDashboard::PutBoolean("Claw Open", m_clawActive);
}