#include "ArmSubsystem.h"

ArmSubsystem::ArmSubsystem() :  m_armMotorSpeedModifier("Arm Speed Modifier", 0.5),
                                m_armMotor(ARM_MOTOR_PORT)
{}

void ArmSubsystem::RobotInit()
{
    InitTalons();
}

void ArmSubsystem::TeleopInit()
{
	m_armMotor.Set(ControlMode::PercentOutput, 0);
}

void ArmSubsystem::Teleop()
{
    SmartDashboard::PutNumber("Arm motor encoder position",m_armMotor.GetSelectedSensorPosition(0));
}


void ArmSubsystem::SetArmMotorSpeed(double speed)
{
    m_armMotor.Set(ControlMode::PercentOutput,(speed*m_armMotorSpeedModifier.Get()));
}

double ArmSubsystem::GetArmMotorPosition()
{
    return m_armMotor.GetSelectedSensorPosition(0);
}

void ArmSubsystem::ResetEncoders()
{
    m_armMotor.SetSelectedSensorPosition(0, 0, 10);
}

void ArmSubsystem::InitTalons() {

    m_armMotor.SetNeutralMode(NeutralMode::Brake);

	// Sets up talons
	m_armMotor.Set(ControlMode::PercentOutput, 0);

    // Zero the sensor
    m_armMotor.SetSelectedSensorPosition(0, 0, 10);
    // m_armMotor.SetInverted(true);
    m_armMotor.SetSensorPhase(true);
}