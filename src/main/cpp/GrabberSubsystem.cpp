#include "GrabberSubsystem.h"

GrabberSubsystem::GrabberSubsystem(): 
        m_operatorJoystick(OPERATOR_JOYSTICK),
		m_clawActuator(frc::PneumaticsModuleType::REVPH, CLAW_IN_PORT, CLAW_OUT_PORT)
{}

void GrabberSubsystem::robotInit(){
}

void GrabberSubsystem::teleopInit() {
	m_clawActuator.Set(DoubleSolenoid::kReverse);
}

void GrabberSubsystem::teleop(){   
    if (m_operatorJoystick.GetRawButtonReleased(1)){
		if (m_clawActive)
		{
			SetClaw(false);
		} else {
			SetClaw(true);
		}
	} 
	SmartDashboard::PutBoolean("Claw Open", m_clawActive);
}

void GrabberSubsystem::SetClaw(bool open){
	if(open)
	{
		m_clawActuator.Set(DoubleSolenoid::kReverse);
	} else {
		m_clawActuator.Set(DoubleSolenoid::kForward);
	}
	m_clawActive = open;
}