#include "ArmSubsystem.h"
#include "Robot.h"

ArmSubsystem::ArmSubsystem() : 
        m_telescopeMotorL(LEFT_ARM_MOTOR),
        m_telescopeMotorR(RIGHT_ARM_MOTOR),
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
    m_wristUp = false;
    m_telescopeMotorL.SetInverted(true);

    m_telescopeMotorR.Set(ControlMode::PercentOutput, 0);
    m_telescopeMotorL.Set(ControlMode::PercentOutput, 0);

    m_telescopeMotorR.SetNeutralMode(NeutralMode::Brake);
    m_telescopeMotorL.SetNeutralMode(NeutralMode::Brake);

    m_telescopeMotorR.Follow(m_telescopeMotorL);

    m_telescopeMotorL.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, 0);
    m_telescopeMotorL.SetSelectedSensorPosition(0, 0, 0);

    operatorJoystick->RegisterAxis(CORE::COREJoystick::JoystickAxis::RIGHT_STICK_Y);
    operatorJoystick->RegisterButton(CORE::COREJoystick::JoystickButton::BACK_BUTTON);

}

void ArmSubsystem::teleopInit()
{
    SetRequestedPosition(GetTelescopeArmMeters()); // Sets requested position to current position
    m_telescopeMotorL.ConfigMotionCruiseVelocity(m_cruiseTelescopeVel.Get(), 0);
    m_telescopeMotorL.ConfigMotionAcceleration(m_maxTelescopeAcel.Get(), 0);
}

void ArmSubsystem::teleop()
{
    if(operatorJoystick->GetRisingEdge(CORE::COREJoystick::JoystickButton::BACK_BUTTON))
    {
        if(m_wristUp)
        {
            m_armPiston.Set(DoubleSolenoid::kForward);
        } else {
            m_armPiston.Set(DoubleSolenoid::kReverse);
        }
        m_wristUp = !m_wristUp;
    }
}

// Will probably run after PostLoopTask() in scoring assembly
void ArmSubsystem::PostLoopTask()
{
    SmartDashboard::PutNumber("Arm Telescope Position", m_telescopeMotorL.GetSelectedSensorPosition(0));
    SmartDashboard::PutNumber("Arm Telescope LEFT Velocity", m_telescopeMotorL.GetSelectedSensorVelocity(0));
    SmartDashboard::PutNumber("Arm Telescope RIGHT Velocity", m_telescopeMotorR.GetSelectedSensorVelocity(0));
    SmartDashboard::PutNumber("Arm Telescope Requested Position", m_requestedDist);

    SmartDashboard::PutBoolean("Arm Rotation Out", IsArmUp());
    SmartDashboard::PutBoolean("Arm Rotation Requested Position", m_requestedRotOut);

    double telescopePosition = GetTelescopeArmMeters();
    bool armRotation = IsArmUp();

    double joystickValue = -operatorJoystick->GetAxis(CORE::COREJoystick::JoystickAxis::RIGHT_STICK_Y);

    SetDistRequestedSpeed(joystickValue);

    SmartDashboard::PutNumber("Requested Telescope Speed",m_requestedTelescopeSpeed);

    // Deadbands joystick input
    if(m_requestedTelescopeSpeed < -0.1 || m_requestedTelescopeSpeed > 0.1)
    {
        m_requestedTelescopeSpeed *= 0.5;
        // SetRequestedPosition(telescopePosition);
        Robot::GetInstance()->scoringAssembly.SetWantedState(WantedState::MANUAL);
        m_wantedState = WantedArmState::MANUAL;
    }

    // Softstop for telescoping arm extend & rotation
    // if(m_requestedTelescopeSpeed > 0 && GetTelescopeArmMeters() - m_outerLimit.Get() > 2)
    // {
    //         m_requestedTelescopeSpeed = 0;
    //         SetRequestedPosition(m_outerLimit.Get());
    // } else if(!ArmIn())
    // {
    //     if (m_requestedTelescopeSpeed < 0)
    //     {
    //         m_requestedTelescopeSpeed = 0;
    //         SetRequestedPosition(0);
    //     }
    //     ResetEncoders();
    // }
    
    switch (m_wantedState)
    {
        case MANUAL:
            if(Robot::GetInstance()->elevatorSubsystem.IsSafeRotateHeight())
            {
                if(m_requestedTelescopeSpeed < 0) // Rotates before moving
                {
                    if (armRotation) // If arm rotated, then arm can extend
                    {
                        m_telescopeMotorL.Set(ControlMode::PercentOutput,m_requestedTelescopeSpeed);
                    } else
                    {
                        // m_armPiston.Set(DoubleSolenoid::kForward);
                        m_telescopeMotorL.Set(ControlMode::PercentOutput,0);
                    }
                } else // Moves before rotating
                {
                    // if (telescopePosition > 2)  // If telescoping arm is in, then arm can rotate
                    // {
                        // m_armPiston.Set(DoubleSolenoid::kReverse);
                    // m_telescopeMotorL.Set(ControlMode::PercentOutput,0);
                    // } else
                    // {
                    //     double telescopeRequestedPosition = m_requestedDist;
                    m_telescopeMotorL.Set(ControlMode::PercentOutput,m_requestedTelescopeSpeed);
                    // }
                }
            } else {
                m_telescopeMotorL.Set(ControlMode::PercentOutput,0);
            }
            break;
        case WANT_TO_SCORE: // Rotates before moving
            if(Robot::GetInstance()->elevatorSubsystem.IsSafeRotateHeight())
            {
                if (armRotation) // If arm rotated, then arm can extend
                {
                    double telescopeRequestedPosition = m_requestedDist;
                    m_telescopeMotorL.Set(ControlMode::MotionProfile,telescopeRequestedPosition);
                } else
                {
                    // m_armPiston.Set(DoubleSolenoid::kForward);
                    m_telescopeMotorL.Set(ControlMode::PercentOutput,0);
                }
            }
            break;
        case WANT_TO_PICKUP: // Moves before rotating
            if(Robot::GetInstance()->elevatorSubsystem.IsSafeRotateHeight())
            {
                if (telescopePosition < 2)  // If telescoping arm is in, then arm can rotate
                {
                    // m_armPiston.Set(DoubleSolenoid::kReverse);
                    m_telescopeMotorL.Set(ControlMode::PercentOutput,0);
                } else
                {
                    double telescopeRequestedPosition = m_requestedDist;
                    m_telescopeMotorL.Set(ControlMode::MotionProfile,telescopeRequestedPosition);
                }
            }
            break;
    }
}

void ArmSubsystem::SetRequestedPosition(double positionInMeters)
{
    // Sets the requested position after converting to ticks; Used for moving manually
    auto position = (int)(positionInMeters * m_ticksPerMeter.Get());
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
    return m_telescopeMotorL.GetSelectedSensorPosition(0);
}

double ArmSubsystem::GetTelescopeArmMeters()
{
    return GetArmDist() / m_ticksPerMeter.Get();
}

// Returns whether or not the arm is retracted to the inner limit
bool ArmSubsystem::ArmIn()
{
    return m_inLimitSwitch.Get();
}

bool ArmSubsystem::IsHighDist()
{
    return abs(GetTelescopeArmMeters() - m_highDist.Get()) < 2;
}

bool ArmSubsystem::IsMediumDist()
{
    return abs(GetTelescopeArmMeters() - m_mediumDist.Get()) < 2;
}

bool ArmSubsystem::IsArmUp()
{
    return m_armPiston.Get();
}

void ArmSubsystem::ResetEncoders()
{
    m_telescopeMotorL.SetSelectedSensorPosition(0,0,0);
}

bool ArmSubsystem::IsArmFullyIn()
{
    // return ArmIn() && !IsArmUp();
    return true;
}