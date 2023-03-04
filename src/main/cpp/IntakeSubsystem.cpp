#include "IntakeSubsystem.h"


IntakeSubsystem::IntakeSubsystem() :
        m_leftIntakeMotor(LEFT_INTAKE),
        m_rightIntakeMotor(RIGHT_INTAKE),
        m_intake(frc::PneumaticsModuleType::REVPH, INTAKE_IN_PORT, INTAKE_OUT_PORT),
        m_operatorJoystick(OPERATOR_JOYSTICK),
        m_intakeConeSpeed("Intake Cone Speed", 0.65),
        m_intakeCubeSpeed("Intake Cube Speed", 0.4),
        m_intakeTimeSet("Intake timer set time", 2){
}

void IntakeSubsystem::robotInit(){
    m_rightIntakeMotor.Set(ControlMode::PercentOutput, 0);
    m_leftIntakeMotor.Set(ControlMode::PercentOutput, 0);
    driverJoystick->RegisterButton(CORE::COREJoystick::A_BUTTON);
    driverJoystick->RegisterButton(CORE::COREJoystick::RIGHT_BUTTON);
    driverJoystick->RegisterButton(CORE::COREJoystick::RIGHT_TRIGGER);
    driverJoystick->RegisterButton(CORE::COREJoystick::LEFT_TRIGGER);
    m_intake.Set(DoubleSolenoid::kForward);
}


void IntakeSubsystem::teleopInit() {}

void IntakeSubsystem::teleop()
{
    if(m_intakeActive)
    {
        if(driverJoystick->GetButton(CORE::COREJoystick::JoystickButton::RIGHT_TRIGGER)) {
            SetIntake(-m_intakeConeSpeed.Get());
        } else if (driverJoystick->GetButton(CORE::COREJoystick::JoystickButton::RIGHT_BUTTON)) {
            SetIntake(m_intakeConeSpeed.Get());
        } else if (driverJoystick->GetButton(CORE::COREJoystick::JoystickButton::LEFT_TRIGGER)) {
            SetIntake(-m_intakeCubeSpeed.Get());
        } else {
            SetIntake(0);
        }
    } else {
        SetIntake(0);    
    }

    if (driverJoystick->GetRisingEdge(CORE::COREJoystick::JoystickButton::A_BUTTON) && m_intakeActive == false){
		m_intake.Set(DoubleSolenoid::kReverse);
		m_intakeActive = true;
        m_intakeTimer.Reset();
        m_intakeTimer.Start();
	} else if (driverJoystick->GetRisingEdge(CORE::COREJoystick::JoystickButton::A_BUTTON) && m_intakeActive == true){
		m_intake.Set(DoubleSolenoid::kForward);
		m_intakeActive = false;
    }
    if (m_intakeTimer.Get() > m_intakeTimeSet.Get() && m_intakeActive) {
        m_intake.Set(DoubleSolenoid::kOff);
        m_intakeTimer.Reset();
        m_intakeTimer.Stop();
    }
}

void IntakeSubsystem::SetIntake(double intakeSpeed) {
    m_leftIntakeMotor.Set(ControlMode::PercentOutput, -intakeSpeed);
    m_rightIntakeMotor.Set(ControlMode::PercentOutput, intakeSpeed);
}
void IntakeSubsystem::SetIntakeForward() {
    SetIntake(m_intakeConeSpeed.Get());
}
void IntakeSubsystem::SetIntakeReverse() {
    SetIntake(-m_intakeConeSpeed.Get());
}