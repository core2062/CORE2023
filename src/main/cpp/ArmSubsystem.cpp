#include "ArmSubsystem.h"
#include <frc/DigitalInput.h>
frc::DigitalInput toplimitSwitch {0};
frc::DigitalInput bottomlimitSwitch {1};

ArmSubsystem::ArmSubsystem() : // m_armMotorSpeedModifier("Arm Speed Modifier", 0.5),
                                m_armMotor(ARM_MOTOR_PORT), 
                                m_armLift(ARM_LIFT_MOTOR_PORT),
                                m_liftSpeed("Arm Lift Speed", 0.5){
}

void ArmSubsystem::RobotInit(){
    operatorJoystick->RegisterAxis(CORE::COREJoystick::RIGHT_STICK_Y);
    operatorJoystick->RegisterButton(CORE::COREJoystick::JoystickButton::LEFT_TRIGGER);
    operatorJoystick->RegisterButton(CORE::COREJoystick::JoystickButton::LEFT_BUTTON);
    InitTalons();
}

void ArmSubsystem::TeleopInit(){
	m_armMotor.Set(ControlMode::PercentOutput, 0);
}

void ArmSubsystem::Teleop(){
    SmartDashboard::PutNumber("Arm motor encoder position",m_armMotor.GetSelectedSensorPosition(0));
}


void ArmSubsystem::SetArmMotorSpeed(double speed){
    // m_armMotor.Set(ControlMode::PercentOutput,(speed*m_armMotorSpeedModifier.Get()));
}

double ArmSubsystem::GetArmMotorPosition(){
    return m_armMotor.GetSelectedSensorPosition(0);
}

void ArmSubsystem::ResetEncoders(){
    m_armMotor.SetSelectedSensorPosition(0, 0, 10);
}

void ArmSubsystem::InitTalons() {

    m_armMotor.SetNeutralMode(NeutralMode::Brake);
    m_armLift.SetNeutralMode(NeutralMode::Brake);

	// Sets up talons
	m_armMotor.Set(ControlMode::PercentOutput, 0);

    // Zero the sensor
    m_armMotor.SetSelectedSensorPosition(0, 0, 10);
    // m_armMotor.SetInverted(true);
    m_armMotor.SetSensorPhase(true);
}

void ArmSubsystem::ArmLift() {
        if (m_liftSpeed.Get() > 0) {
        if (toplimitSwitch.Get()) {
            // Arm's going up and top limit is tripped so stop
            m_armLift.Set(ControlMode::PercentOutput, 0);
        } else {
            // Arm's going up but top limit is not tripped so go at commanded speed
            m_armLift.Set(ControlMode::PercentOutput, m_liftSpeed.Get());
        }
    } else {
        if (bottomlimitSwitch.Get()) {
            // Arm's going down and bottom limit is tripped so stop
            m_armLift.Set(ControlMode::PercentOutput, 0);
        } else {
            // Arm's going down but bottom limit is not tripped so go at commanded speed
            m_armLift.Set(ControlMode::PercentOutput, -m_liftSpeed.Get());
        }
    }
}