#include "GrabberSubsystem.h"

GrabberSubsystem::GrabberSubsystem(): 
        m_operatorJoystick(OPERATOR_JOYSTICK),
		m_clawActuator(frc::PneumaticsModuleType::REVPH, CLAW_IN_PORT, CLAW_OUT_PORT)
{}

void GrabberSubsystem::robotInit(){
}

void GrabberSubsystem::teleopInit() {
	m_clawActuator.Set(DoubleSolenoid::kForward);
}

void GrabberSubsystem::teleop(){   
    if (m_operatorJoystick.GetRawButtonReleased(1)){
		m_clawActuator.Toggle();
		m_clawActive = !m_clawActive;
	} 
	SmartDashboard::PutBoolean("Claw Open", m_clawActive);
}