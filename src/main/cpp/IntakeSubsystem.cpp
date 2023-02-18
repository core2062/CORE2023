#include "IntakeSubsystem.h"

using namespace CORE;

IntakeSubsystem::IntakeSubsystem() :
                                    intakeSpeed("Intake Speed", 0.70),
                                    m_leftIntakeMotor(LEFT_INTAKE),
                                    m_rightIntakeMotor(RIGHT_INTAKE),
                                    m_intake(frc::PneumaticsModuleType::REVPH, INTAKE_IN_PORT, INTAKE_OUT_PORT){
}

void IntakeSubsystem::robotInit(){
    m_rightIntakeMotor.Set(ControlMode::PercentOutput, 0);
    m_leftIntakeMotor.Set(ControlMode::PercentOutput, 0);
    operatorJoystick->RegisterButton(CORE::COREJoystick::X_BUTTON);
}


void IntakeSubsystem::teleopInit() {
}

void IntakeSubsystem::teleop(){
    if(operatorJoystick->GetButton(CORE::COREJoystick::JoystickButton::RIGHT_TRIGGER)) {
       SetIntake(-intakeSpeed.Get());
     } else if (operatorJoystick->GetButton(CORE::COREJoystick::JoystickButton::RIGHT_BUTTON)) {
        SetIntake(intakeSpeed.Get());
        } else{
        SetIntake(0.0);
    }

    if (operatorJoystick->GetRisingEdge(CORE::COREJoystick::JoystickButton::X_BUTTON) && m_intakeActive == false){
		m_intake.Set(DoubleSolenoid::kForward);
		m_intakeActive = true;
	} else if (operatorJoystick->GetRisingEdge(CORE::COREJoystick::JoystickButton::X_BUTTON) && m_intakeActive == true){
		m_intake.Set(DoubleSolenoid::kReverse);
		m_intakeActive = false;
    }
}

void IntakeSubsystem::SetIntake(double intakeSpeed) {
    m_leftIntakeMotor.Set(ControlMode::PercentOutput, -intakeSpeed);
    m_rightIntakeMotor.Set(ControlMode::PercentOutput, intakeSpeed);
}