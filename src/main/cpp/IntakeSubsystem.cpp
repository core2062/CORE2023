#include "IntakeSubsystem.h"


IntakeSubsystem::IntakeSubsystem() :
        m_leftIntakeMotor(LEFT_INTAKE),
        m_rightIntakeMotor(RIGHT_INTAKE),
        m_intake(frc::PneumaticsModuleType::REVPH, INTAKE_IN_PORT, INTAKE_OUT_PORT),
        m_operatorJoystick(OPERATOR_JOYSTICK),
        m_intakeConeSpeed("Intake Speed", 0.3),
        m_intakeCubeSpeed("Intake Soeed", 0.2),
        m_intakeTimeSet("Intake timer set time", 2){
}

void IntakeSubsystem::robotInit(){
    m_rightIntakeMotor.Set(ControlMode::PercentOutput, 0);
    m_leftIntakeMotor.Set(ControlMode::PercentOutput, 0);
    driverJoystick->RegisterButton(CORE::COREJoystick::A_BUTTON);
    driverJoystick->RegisterButton(CORE::COREJoystick::RIGHT_BUTTON);
    driverJoystick->RegisterButton(CORE::COREJoystick::RIGHT_TRIGGER);
    m_intake.Set(DoubleSolenoid::kForward);
}


void IntakeSubsystem::teleopInit() {

}

void IntakeSubsystem::teleop(){
    if(driverJoystick->GetButton(CORE::COREJoystick::JoystickButton::RIGHT_TRIGGER)) {
        if (m_intake.Get() == DoubleSolenoid::kForward){
            SetIntake(0.0);
        } else{
            SetIntake(-m_intakeConeSpeed.Get());
        }
     } else{
        SetIntake(0.0);
    }
    if (driverJoystick->GetButton(CORE::COREJoystick::JoystickButton::RIGHT_BUTTON)) {
        if (m_intake.Get() == DoubleSolenoid::kForward){
            SetIntake(0.0);
        } else{
            SetIntake(m_intakeConeSpeed.Get());
        }
        } else{
        SetIntake(0.0);
    }
    if (driverJoystick->GetButton(CORE::COREJoystick::JoystickButton::LEFT_TRIGGER)) {
        if (m_intake.Get() == DoubleSolenoid::kForward){
            SetIntake(0.0);
        } else{
            SetIntake(-m_intakeCubeSpeed.Get());
        }
        } else{
        SetIntake(0.0);
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
    // std::cout << "time is " << m_intakeTimer.Get() << endl;
}

void IntakeSubsystem::SetIntake(double intakeSpeed) {
    m_leftIntakeMotor.Set(ControlMode::PercentOutput, -intakeSpeed);
    m_rightIntakeMotor.Set(ControlMode::PercentOutput, intakeSpeed);
}