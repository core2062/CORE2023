#include "ArmSubsystem.h"
#include "Robot.h"

ArmSubsystem::ArmSubsystem() : 
        m_telescopeMotor(TELESCOPING_ARM_MOTOR),
        m_rotateMotor(ARM_ROTATION_MOTOR),
        m_inLimitSwitch(ARM_IN_LIMIT_SWITCH_PORT),
        m_downLimitSwitch(ARM_DOWN_LIMIT_SWITCH_PORT),
        m_rotationDist("Arm Rotation Dist"), // Arm rot out degrees
        m_mediumDist("Arm Mid-Level Distance"),
        m_highDist("Arm High-Level Distance"),
        m_ticksPerMeter("Arm Ticks Per Meter"),
        m_ticksPerDegree("Arm Ticks Per Degree"),
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
    m_telescopeMotor.Set(ControlMode::PercentOutput, 0);
    m_rotateMotor.Set(ControlMode::PercentOutput, 0);

    m_telescopeMotor.SetNeutralMode(NeutralMode::Brake);
    m_rotateMotor.SetNeutralMode(NeutralMode::Brake);

    // m_armMotor.SetInverted(true);

    m_telescopeMotor.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,0);
    m_telescopeMotor.SetSelectedSensorPosition(0,0,0);

    m_rotateMotor.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,0);
    m_rotateMotor.SetSelectedSensorPosition(0,0,0);

    operatorJoystick->RegisterAxis(CORE::COREJoystick::JoystickAxis::LEFT_STICK_Y);

}

void ArmSubsystem::teleopInit()
{}

void ArmSubsystem::teleop()
{}

void ArmSubsystem::PostLoopTask()
{
    SmartDashboard::PutNumber("Arm Telescope Position", m_telescopeMotor.GetSelectedSensorPosition(0));
    SmartDashboard::PutNumber("Arm Telescope Velocity", m_telescopeMotor.GetSelectedSensorVelocity(0));
    SmartDashboard::PutNumber("Arm Telescope Requested Position", m_requestedDist);

    SmartDashboard::PutNumber("Arm Rotation Position", m_rotateMotor.GetSelectedSensorPosition(0));
    SmartDashboard::PutNumber("Arm Rotation Velocity", m_rotateMotor.GetSelectedSensorVelocity(0));
    SmartDashboard::PutNumber("Arm Rotation Requested Position", m_requestedRot);

    double telescopePosition = GetTelescopeArmInches();
    double armRotation = GetRotationArmDegrees();

    double joystickValue = -operatorJoystick->GetAxis(CORE::COREJoystick::JoystickAxis::LEFT_STICK_Y);

    SetDistRequestedSpeed(joystickValue);
    SetRotRequestedSpeed(joystickValue);

    SmartDashboard::PutNumber("Requested Telescope Speed",m_requestedTelescopeSpeed);
    if(m_requestedTelescopeSpeed < -0.01 || m_requestedTelescopeSpeed > 0.1)
    {
        if(m_requestedTelescopeSpeed < 0)
        {
            m_requestedTelescopeSpeed *= 0.1;
            m_requestedRotSpeed *= 0.1;
        }
        else
        {
            m_requestedTelescopeSpeed *= 0.5;
            m_requestedRotSpeed *= 0.5;
        }
        SetRequestedPosition(telescopePosition);
        SetRequestedRotation(armRotation);
        Robot::GetInstance()->scoringAssembly.SetWantedState(WantedState::MANUAL);
    }

    if(m_requestedTelescopeSpeed > 0 && GetTelescopeArmInches() - m_outerLimit.Get() > 2)
    {
            m_requestedTelescopeSpeed = 0;
            SetRequestedPosition(m_outerLimit.Get());
    } else if(ArmIn())
    {
        if (m_requestedTelescopeSpeed < 0)
        {
            m_requestedTelescopeSpeed = 0;
            SetRequestedPosition(0);
        }
        ResetDistanceEncoder();
    }

    if(m_requestedRotSpeed > 0 && GetRotationArmDegrees() - m_rotationDist.Get() > 2)
    {
        m_requestedRotSpeed = 0;
        SetRequestedRotation(m_rotationDist.Get());
    } else if(ArmDown())
    {
        if (m_requestedRotSpeed < 0)
        {
            m_requestedRotSpeed = 0;
            SetRequestedRotation(0);
        }
        ResetRotationEncoder();
    }
    
    switch (m_wantedState)
    {
        case MANUAL:
            if(joystickValue < 0) // Rotates before moving
            {
                if (abs(armRotation - m_rotationDist.Get()) < 1) // If arm rotated, then arm can exted
                {
                    double telescopeRequestedPosition = m_requestedDist;
                    m_telescopeMotor.Set(ControlMode::MotionProfile,telescopeRequestedPosition);
                    m_rotateMotor.Set(ControlMode::PercentOutput,0);
                } else
                {
                    double armRequestedRotation = m_requestedRot;
                    m_rotateMotor.Set(ControlMode::MotionProfile,armRequestedRotation);
                    m_telescopeMotor.Set(ControlMode::PercentOutput,0);
                }
            } else // Moves before rotating
            {
                if (telescopePosition < 2)  // If telescoping arm is in, then arm can rotate
                {
                    double armRequestedRotation = m_requestedRot;
                    m_rotateMotor.Set(ControlMode::MotionProfile,armRequestedRotation);
                    m_telescopeMotor.Set(ControlMode::PercentOutput,0);
                } else
                {
                    double telescopeRequestedPosition = m_requestedDist;
                    m_telescopeMotor.Set(ControlMode::MotionProfile,telescopeRequestedPosition);
                    m_rotateMotor.Set(ControlMode::PercentOutput,0);
                }
            }
            break;
        case WANT_TO_SCORE: // Rotates before moving
            if (abs(armRotation - m_rotationDist.Get()) < 1) // If arm rotated, then arm can exted
            {
                double telescopeRequestedPosition = m_requestedDist;
                m_telescopeMotor.Set(ControlMode::MotionProfile,telescopeRequestedPosition);
                m_rotateMotor.Set(ControlMode::PercentOutput,0);
            } else
            {
                double armRequestedRotation = m_requestedRot;
                m_rotateMotor.Set(ControlMode::MotionProfile,armRequestedRotation);
                m_telescopeMotor.Set(ControlMode::PercentOutput,0);
            }
            break;
        case WANT_TO_PICKUP: // Moves before rotating
            if (telescopePosition < 2)  // If telescoping arm is in, then arm can rotate
            {
                double armRequestedRotation = m_requestedRot;
                m_rotateMotor.Set(ControlMode::MotionProfile,armRequestedRotation);
                m_telescopeMotor.Set(ControlMode::PercentOutput,0);
            } else
            {
                double telescopeRequestedPosition = m_requestedDist;
                m_telescopeMotor.Set(ControlMode::MotionProfile,telescopeRequestedPosition);
                m_rotateMotor.Set(ControlMode::PercentOutput,0);
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
    return m_telescopeMotor.GetSelectedSensorPosition(0);
}

int ArmSubsystem::GetArmRotation()
{
    return m_rotateMotor.GetSelectedSensorPosition(0);
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
    m_rotateMotor.SetSelectedSensorPosition(0,0,0);
}

void ArmSubsystem::ResetDistanceEncoder()
{
    m_telescopeMotor.SetSelectedSensorPosition(0,0,0);
}

void ArmSubsystem::ResetEncoders()
{
    ResetRotationEncoder();
    ResetDistanceEncoder();
}
