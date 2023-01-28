#include "ArmSubsystem.h"
#include "Robot.h"

ArmSubsystem::ArmSubsystem() : 
        m_armMotor(ARM_MOTOR_PORT),
        m_armLift(ARM_LIFT_MOTOR_PORT),
        m_inLimitSwitch(ARM_IN_LIMIT_SWITCH_PORT),
        m_downLimitSwitch(ARM_DOWN_LIMIT_SWITCH_PORT),
        m_rotationDist("Arm Rotation Dist"), // Arm rot out degrees
        m_mediumDist("Arm Mid-Level Distance"),
        m_highDist("Arm High-Level Distance"),
        m_ticksPerMeter("Arm Ticks Per Meter"),
        m_ticksPerDegree("Arm Ticks Per Degree"),
        m_innerLimit("Arm Inner Limit"),
        m_outerLimit("Arm Outer Limit"),
        m_rotationLimit("Arm Rotation Limit"),
        m_cruiseTelescopeVel("Telescope Arm Cruise Velocity"),
        m_maxTelescopeAcel("Telescope Arm Max Acceleration"),
        m_cruiseRotationVel("Arm Cruise Rotation Velocity"),
        m_maxRotationAcel("Arm Max Rotation Acceleration")
{}

void ArmSubsystem::robotInit()
{
    // Lines 20-33 config the talons
    m_armMotor.Set(ControlMode::PercentOutput, 0);
    m_armLift.Set(ControlMode::PercentOutput, 0);

    m_armMotor.SetNeutralMode(NeutralMode::Brake);
    m_armLift.SetNeutralMode(NeutralMode::Brake);

    // m_armMotor.SetInverted(true);

    m_armMotor.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,0);
    m_armMotor.SetSelectedSensorPosition(0,0,0);

    m_armLift.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,0);
    m_armLift.SetSelectedSensorPosition(0,0,0);

    operatorJoystick->RegisterAxis(CORE::COREJoystick::JoystickAxis::LEFT_STICK_X);

}

void ArmSubsystem::teleopInit()
{}

void ArmSubsystem::teleop()
{}

void ArmSubsystem::PostLoopTask()
{
    SmartDashboard::PutNumber("Arm Telescope Position", m_armMotor.GetSelectedSensorPosition(0));
    SmartDashboard::PutNumber("Arm Telescope Velocity", m_armMotor.GetSelectedSensorVelocity(0));
    SmartDashboard::PutNumber("Arm Telescope Requested Position", m_requestedDist);

    SmartDashboard::PutNumber("Arm Rotation Position", m_armLift.GetSelectedSensorPosition(0));
    SmartDashboard::PutNumber("Arm Rotation Velocity", m_armLift.GetSelectedSensorVelocity(0));
    SmartDashboard::PutNumber("Arm Rotation Requested Position", m_requestedRot);

    double telescopePosition = GetTelescopeArmInches();

    switch (m_wantedState)
    {
        case MANUAL:

            break;
        case WANT_TO_SCORE:
            if (GetRotationArmDegrees() - m_rotationDist.Get() < 1)
            {
                /* code */
            } else
            {
                /* code */
            }
            break;
        case WANT_TO_PICKUP:
            if (GetTelescopeArmInches() - m_rotationDist.Get() < 1)
            {
                /* code */
            } else
            {
                /* code */
            }
            break;
    }
}

void ArmSubsystem::SetRequestedPosition(double positionInInches)
{
    auto position = (int)(positionInInches * m_ticksPerMeter.Get());
    position = max(position,0);
    position = min(position, (int)(m_outerLimit.Get()*m_ticksPerMeter.Get()));
    m_requestedDist = position;
}

void ArmSubsystem::SetRequestedRotation(double rotationInDegrees)
{
    auto rotation = (int)(rotationInDegrees * m_ticksPerDegree.Get());
    rotation = max(rotation,0);
    rotation = min(rotation, (int)(m_rotationLimit.Get()*m_ticksPerDegree.Get()));
    m_requestedRot = rotation;
}

void ArmSubsystem::SetDistRequestedSpeed(double speed)
{
    m_requestedTelescopeSpeed = speed;
}

void ArmSubsystem::SetRotRequestedSpeed(double speed)
{
    m_requestedRotSpeed = speed;
}

void ArmSubsystem::SetHighDist()
{
    m_wantedState = WantedArmState::WANT_TO_SCORE;
    SetRequestedPosition(m_highDist.Get());
    SetRequestedRotation(m_rotationDist.Get());
}

void ArmSubsystem::SetMediumDist()
{
    m_wantedState = WantedArmState::WANT_TO_SCORE;
    SetRequestedPosition(m_mediumDist.Get());
    SetRequestedRotation(m_rotationDist.Get());
}

void ArmSubsystem::SetRotDown()
{
    m_wantedState = WantedArmState::WANT_TO_PICKUP;
    SetRequestedPosition(0);
    SetRequestedRotation(0);
}

int ArmSubsystem::GetArmDist()
{
    return m_armMotor.GetSelectedSensorPosition(0);
}

int ArmSubsystem::GetArmRotation()
{
    return m_armLift.GetSelectedSensorPosition(0);
}

double ArmSubsystem::GetTelescopeArmInches()
{
    return GetArmDist() / m_ticksPerMeter.Get();
}

double ArmSubsystem::GetRotationArmDegrees()
{
    return GetArmRotation() / m_ticksPerDegree.Get();
}

bool ArmSubsystem::ArmIn()
{
    return m_inLimitSwitch.Get();
}

bool ArmSubsystem::ArmDown()
{
    return m_downLimitSwitch.Get();
}

bool ArmSubsystem::IsHighDist()
{
    return abs(GetTelescopeArmInches() - m_highDist.Get()) < 2;
}

bool ArmSubsystem::IsMediumDist()
{
    return abs(GetTelescopeArmInches() - m_mediumDist.Get()) < 2;
}

bool ArmSubsystem::IsArmUp()
{
    return abs(GetRotationArmDegrees() - m_rotationDist.Get()) < 4;
}

void ArmSubsystem::ResetRotationEncoder()
{
    m_armLift.SetSelectedSensorPosition(0,0,0);
}

void ArmSubsystem::ResetDistanceEncoder()
{
    m_armMotor.SetSelectedSensorPosition(0,0,0);
}

void ArmSubsystem::ResetEncoders()
{
    ResetRotationEncoder();
    ResetDistanceEncoder();
}
