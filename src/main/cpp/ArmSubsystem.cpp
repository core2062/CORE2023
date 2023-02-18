#include "ArmSubsystem.h"
#include "Robot.h"

ArmSubsystem::ArmSubsystem() : 
        m_telescopeMotor(TELESCOPING_ARM_MOTOR),
        m_armPiston(frc::PneumaticsModuleType::REVPH, ARM_IN_PORT, ARM_OUT_PORT),
        m_inLimitSwitch(ARM_IN_LIMIT_SWITCH_PORT),
        m_downLimitSwitch(ARM_DOWN_LIMIT_SWITCH_PORT),
        m_mediumDist("Arm Mid-Level Distance"),
        m_highDist("Arm High-Level Distance"),
        m_ticksPerMeter("Arm Ticks Per Meter"),
        m_outerLimit("Arm Outer Limit"),
        m_rotationLimit("Arm Rotation Limit"),
        m_cruiseTelescopeVel("Telescope Arm Cruise Velocity"),
        m_maxTelescopeAcel("Telescope Arm Max Acceleration")
{}

void ArmSubsystem::robotInit()
{
    // Lines 20-33 config the talons
    m_telescopeMotor.Set(ControlMode::PercentOutput, 0);

    m_telescopeMotor.SetNeutralMode(NeutralMode::Brake);

    // m_armMotor.SetInverted(true);

    m_telescopeMotor.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,0);
    m_telescopeMotor.SetSelectedSensorPosition(0,0,0);

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

    SmartDashboard::PutBoolean("Arm Rotation Out", IsArmUp());
    SmartDashboard::PutBoolean("Arm Rotation Requested Position", m_requestedRotOut);

    double telescopePosition = GetTelescopeArmInches();
    bool armRotation = IsArmUp();

    double joystickValue = -operatorJoystick->GetAxis(CORE::COREJoystick::JoystickAxis::LEFT_STICK_Y);

    SetDistRequestedSpeed(joystickValue);

    SmartDashboard::PutNumber("Requested Telescope Speed",m_requestedTelescopeSpeed);
    if(m_requestedTelescopeSpeed < -0.01 || m_requestedTelescopeSpeed > 0.1)
    {
        if(m_requestedTelescopeSpeed < 0)
        {
            m_requestedTelescopeSpeed *= 0.1;
        }
        else
        {
            m_requestedTelescopeSpeed *= 0.5;
        }
        SetRequestedPosition(telescopePosition);
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
        ResetEncoders();
    }
    
    switch (m_wantedState)
    {
        case MANUAL:
            if(joystickValue < 0) // Rotates before moving
            {
                if (armRotation) // If arm rotated, then arm can extend
                {
                    double telescopeRequestedPosition = m_requestedDist;
                    m_telescopeMotor.Set(ControlMode::MotionProfile,telescopeRequestedPosition);
                } else
                {
                    m_armPiston.Set(DoubleSolenoid::kForward);
                    m_telescopeMotor.Set(ControlMode::PercentOutput,0);
                }
            } else // Moves before rotating
            {
                if (telescopePosition < 2)  // If telescoping arm is in, then arm can rotate
                {
                    m_armPiston.Set(DoubleSolenoid::kReverse);
                    m_telescopeMotor.Set(ControlMode::PercentOutput,0);
                } else
                {
                    double telescopeRequestedPosition = m_requestedDist;
                    m_telescopeMotor.Set(ControlMode::MotionProfile,telescopeRequestedPosition);
                }
            }
            break;
        case WANT_TO_SCORE: // Rotates before moving
            if (armRotation) // If arm rotated, then arm can extend
            {
                double telescopeRequestedPosition = m_requestedDist;
                m_telescopeMotor.Set(ControlMode::MotionProfile,telescopeRequestedPosition);
            } else
            {
                m_armPiston.Set(DoubleSolenoid::kForward);
                m_telescopeMotor.Set(ControlMode::PercentOutput,0);
            }
            break;
        case WANT_TO_PICKUP: // Moves before rotating
            if (telescopePosition < 2)  // If telescoping arm is in, then arm can rotate
            {
                m_armPiston.Set(DoubleSolenoid::kReverse);
                m_telescopeMotor.Set(ControlMode::PercentOutput,0);
            } else
            {
                double telescopeRequestedPosition = m_requestedDist;
                m_telescopeMotor.Set(ControlMode::MotionProfile,telescopeRequestedPosition);
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

void ArmSubsystem::SetRequestedRotation(bool rot)
{
    m_requestedRotOut = rot;
}

void ArmSubsystem::SetDistRequestedSpeed(double speed)
{
    m_requestedTelescopeSpeed = speed;
}

void ArmSubsystem::SetHighDist()
{
    m_wantedState = WantedArmState::WANT_TO_SCORE;
    SetRequestedPosition(m_highDist.Get());
    SetRequestedRotation(true);
}

void ArmSubsystem::SetMediumDist()
{
    m_wantedState = WantedArmState::WANT_TO_SCORE;
    SetRequestedPosition(m_mediumDist.Get());
    SetRequestedRotation(true);
}

void ArmSubsystem::SetRotDown()
{
    m_wantedState = WantedArmState::WANT_TO_PICKUP;
    SetRequestedPosition(0);
    SetRequestedRotation(false);
}

int ArmSubsystem::GetArmDist()
{
    return m_telescopeMotor.GetSelectedSensorPosition(0);
}

double ArmSubsystem::GetTelescopeArmInches()
{
    return GetArmDist() / m_ticksPerMeter.Get();
}

bool ArmSubsystem::ArmIn()
{
    return m_inLimitSwitch.Get();
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
    return m_armPiston.Get();
}

void ArmSubsystem::ResetEncoders()
{
    m_telescopeMotor.SetSelectedSensorPosition(0,0,0);
}
