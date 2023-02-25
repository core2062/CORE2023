#include "IntakeSubsystem.h"


IntakeSubsystem::IntakeSubsystem() :
                                    intakeSpeed("Intake Speed", 0.3),
                                    m_leftIntakeMotor(LEFT_INTAKE),
                                    m_rightIntakeMotor(RIGHT_INTAKE),
                                    m_intakeTimeSet("Intake timer set time", 2),
                                    m_intake(frc::PneumaticsModuleType::REVPH, INTAKE_IN_PORT, INTAKE_OUT_PORT){
}

void IntakeSubsystem::robotInit(){
    m_rightIntakeMotor.Set(ControlMode::PercentOutput, 0);
    m_leftIntakeMotor.Set(ControlMode::PercentOutput, 0);
    operatorJoystick->RegisterButton(CORE::COREJoystick::DPAD_N);
    operatorJoystick->RegisterButton(CORE::COREJoystick::DPAD_E);
    operatorJoystick->RegisterButton(CORE::COREJoystick::DPAD_S);
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
            SetIntake(-intakeSpeed.Get());
        }
     } else if (driverJoystick->GetButton(CORE::COREJoystick::JoystickButton::RIGHT_BUTTON)) {
        if (m_intake.Get() == DoubleSolenoid::kForward){
            SetIntake(0.0);
        } else{
            SetIntake(intakeSpeed.Get());
        }
        } else{
        SetIntake(0.0);
    }
    // if(operatorJoystick->GetRisingEdge(CORE::COREJoystick::JoystickButton::DPAD_N)) {
    //     m_intake.Set(DoubleSolenoid::kForward);
    // }
    // if(operatorJoystick->GetRisingEdge(CORE::COREJoystick::JoystickButton::DPAD_E)) {
    //     m_intake.Set(DoubleSolenoid::kOff);
    // }
    // if(operatorJoystick->GetRisingEdge(CORE::COREJoystick::JoystickButton::DPAD_S)) {
    //     m_intake.Set(DoubleSolenoid::kReverse);
    // }

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
    std::cout << "time is " << m_intakeTimer.Get() << endl;
}

void IntakeSubsystem::SetIntake(double intakeSpeed) {
    m_leftIntakeMotor.Set(ControlMode::PercentOutput, -intakeSpeed);
    m_rightIntakeMotor.Set(ControlMode::PercentOutput, intakeSpeed);
}