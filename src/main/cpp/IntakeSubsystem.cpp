#include "IntakeSubsystem.h"


IntakeSubsystem::IntakeSubsystem() :
        m_leftIntakeMotor(LEFT_INTAKE),
        m_rightIntakeMotor(RIGHT_INTAKE),
        m_intake(frc::PneumaticsModuleType::REVPH, INTAKE_IN_PORT, INTAKE_OUT_PORT),
        m_turnTableMotor(TURNTABLE),
        m_operatorJoystick(OPERATOR_JOYSTICK),
        m_intakeConeSpeed("Intake Cone Speed", 0.5),
        m_intakeCubeSpeed("Intake Cube Speed", 0.4),
        m_lowerIntakeConeSpeed("Low Bar Intake Cone Speed", 0.75),
        m_lowerIntakeCubeSpeed("Low Bar Intake Cube Speed", 0.75),
        m_intakeTimeSet("Intake timer set time", 0.5),
        m_turnTableSpeed("Turn Table Speed", 0.4){
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
            SetIntake(-m_intakeConeSpeed.Get(), -m_lowerIntakeConeSpeed.Get());
        } else if (driverJoystick->GetButton(CORE::COREJoystick::JoystickButton::RIGHT_BUTTON)) {
            SetIntake(m_intakeConeSpeed.Get(), m_lowerIntakeConeSpeed.Get());
        } else if (driverJoystick->GetButton(CORE::COREJoystick::JoystickButton::LEFT_TRIGGER)) {
            SetIntake(-m_intakeCubeSpeed.Get(), -m_lowerIntakeCubeSpeed.Get());
        } else {
            SetIntake(0, 0);
        }
        if(m_operatorJoystick.GetPOV() == 90) {
            SetTurnTable(-m_turnTableSpeed.Get());
        } else if(m_operatorJoystick.GetPOV() == 270) {
            SetTurnTable(m_turnTableSpeed.Get());
        } else {
            SetTurnTable(0);
        }
    } else {
        SetIntake(0, 0);    
    }

    if (driverJoystick->GetRisingEdge(CORE::COREJoystick::JoystickButton::A_BUTTON) && m_intakeActive == false){
		SetIntakeActive(true);
	} else if (driverJoystick->GetRisingEdge(CORE::COREJoystick::JoystickButton::A_BUTTON) && m_intakeActive == true){
		SetIntakeActive(false);
    }
    if (m_intakeTimer.Get() > m_intakeTimeSet.Get() && m_intakeActive) {
        m_intake.Set(DoubleSolenoid::kOff);
        m_intakeTimer.Reset();
        m_intakeTimer.Stop();
    }
}

void IntakeSubsystem::SetIntake(double upperIntakeSpeed, double lowerIntakeSpeed) {
    m_leftIntakeMotor.Set(ControlMode::PercentOutput, -lowerIntakeSpeed);
    m_rightIntakeMotor.Set(ControlMode::PercentOutput, upperIntakeSpeed);
}
void IntakeSubsystem::SetIntakeForward() {
    SetIntake(m_intakeConeSpeed.Get(), m_lowerIntakeConeSpeed.Get());
}
void IntakeSubsystem::SetIntakeReverse() {
    SetIntake(-m_intakeConeSpeed.Get(), -m_lowerIntakeConeSpeed.Get());
}


void IntakeSubsystem::SetIntakeActive(bool active){
    if (active && m_intakeActive == false){
		m_intake.Set(DoubleSolenoid::kReverse);
		m_intakeActive = true;
        m_intakeTimer.Reset();
        m_intakeTimer.Start();
	} else if (!active && m_intakeActive == true){
		m_intake.Set(DoubleSolenoid::kForward);
		m_intakeActive = false;
    }
}

void IntakeSubsystem::SetTurnTable(double turnTableSpeed) {
    m_turnTableMotor.Set(ControlMode::PercentOutput, turnTableSpeed);
}