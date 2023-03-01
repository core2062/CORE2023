#include "ArmSubsystem.h"
#include "Robot.h"

//TODO: Ask Sid which button he wants to actuate the wrist & implement
// 5 Pcikup
// 6 high
// 4 Med
// Pad W awrist

ArmSubsystem::ArmSubsystem() : 
        m_telescopeMotorL(LEFT_ARM_MOTOR),
        m_telescopeMotorR(RIGHT_ARM_MOTOR),
        m_armPiston(frc::PneumaticsModuleType::REVPH, ARM_IN_PORT, ARM_OUT_PORT),
        m_topLimitSwitch(ARM_IN_LIMIT_SWITCH_PORT),
        m_operatorJoystick(OPERATOR_JOYSTICK),
        m_mediumDist("Arm Mid-Level Distance In Ticks",12000),
        m_highDist("Arm High-Level Distance In Ticks",23000),
        m_outerLimit("Arm Outer Limit In Ticks",24054),
        m_cruiseTelescopeVel("Telescope Arm Cruise Velocity",4),
        m_maxTelescopeAcel("Telescope Arm Max Acceleration",5),
        m_armSpeed("Arm Speed", 0.7)
{}

void ArmSubsystem::robotInit()
{
    m_wristUp = true;
    m_telescopeMotorL.SetInverted(true);

    m_telescopeMotorR.Set(ControlMode::PercentOutput, 0);
    m_telescopeMotorL.Set(ControlMode::PercentOutput, 0);

    m_telescopeMotorR.SetNeutralMode(NeutralMode::Brake);
    m_telescopeMotorL.SetNeutralMode(NeutralMode::Brake);

    m_telescopeMotorR.Follow(m_telescopeMotorL);

    m_telescopeMotorL.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, 0);
    m_telescopeMotorL.SetSelectedSensorPosition(0, 0, 0);

    m_telescopeMotorL.SetSensorPhase(true);

    // operatorJoystick->RegisterAxis(CORE::COREJoystick::JoystickAxis::RIGHT_STICK_Y);
    // operatorJoystick->RegisterButton(CORE::COREJoystick::JoystickButton::BACK_BUTTON);

}

void ArmSubsystem::teleopInit()
{
    SetRequestedPosition(GetArmDist()); // Sets requested position to current position
    m_telescopeMotorL.ConfigMotionCruiseVelocity(m_cruiseTelescopeVel.Get(), 0);
    m_telescopeMotorL.ConfigMotionAcceleration(m_maxTelescopeAcel.Get(), 0);
}

void ArmSubsystem::teleop()
{
    if(m_operatorJoystick.GetRawButtonReleased(3))
    {
        m_requestedRotUp = !m_requestedRotUp;
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
    SmartDashboard::PutBoolean("Arm Rotation Requested Position", m_requestedRotUp);

    double telescopePosition = GetArmDist();

    SetDistRequestedSpeed(m_operatorJoystick.GetRawAxis(0));

    SmartDashboard::PutNumber("Requested Telescope Speed",m_requestedTelescopeSpeed);

    // Deadbands joystick input
    if(m_requestedTelescopeSpeed < -0.1 || m_requestedTelescopeSpeed > 0.1)
    {
        // m_requestedTelescopeSpeed *= 0.5;
        SetRequestedPosition(telescopePosition);
        Robot::GetInstance()->scoringAssembly.SetWantedState(WantedState::MANUAL);
    } else {
        m_requestedTelescopeSpeed = 0;
    }

    // Softstop for telescoping arm extend & rotation
    if(m_requestedTelescopeSpeed > 0 && (IsArmIn()/* || (GetArmDist() - m_outerLimit.Get()) > 100*/))
    {
        std::cout << "Softstopping arm out" << endl;
        m_requestedTelescopeSpeed = 0;
        SetRequestedPosition(m_outerLimit.Get());
    } else if(GetArmDist() < 100)
    {
        if (m_requestedTelescopeSpeed < 0)
        {
            std::cout << "Softstopping arm in" << endl;
            m_requestedTelescopeSpeed = 0;
            SetRequestedPosition(0);
        }
    }

    if(m_requestedTelescopeSpeed < -0.1 || m_requestedTelescopeSpeed > 0.1)
    {
        m_telescopeMotorL.Set(ControlMode::PercentOutput,m_requestedTelescopeSpeed);
    } else {
        m_telescopeMotorL.Set(ControlMode::MotionMagic,m_requestedDist);
    }

    if(m_requestedRotUp)
    {
        m_armPiston.Set(DoubleSolenoid::kReverse);
    } else {
        m_armPiston.Set(DoubleSolenoid::kForward);
    }
}

void ArmSubsystem::SetRequestedPosition(int position)
{
    // Sets the requested position after converting to ticks; Used for moving manually
    position = max(position,0);
    position = min(position, (int)m_outerLimit.Get());
    m_requestedDist = position;
}

void ArmSubsystem::SetRequestedRotation(bool rot)
{
    m_requestedRotUp = rot;
}

void ArmSubsystem::SetDistRequestedSpeed(double speed)
{
    m_requestedTelescopeSpeed = speed * m_armSpeed.Get();
}

void ArmSubsystem::SetHighDist()
{
    SetRequestedPosition(m_highDist.Get());
    SetRequestedRotation(true);
}

void ArmSubsystem::SetMediumDist()
{
    SetRequestedPosition(m_mediumDist.Get());
    SetRequestedRotation(true);
}

void ArmSubsystem::SetArmIn(){
    SetRequestedPosition(0);
}

void ArmSubsystem::SetRotUp()
{
    SetRequestedRotation(true);
}

void ArmSubsystem::SetRotDown()
{
    SetRequestedRotation(false);
}

int ArmSubsystem::GetArmDist()
{
    return m_telescopeMotorL.GetSelectedSensorPosition(0);
}


bool ArmSubsystem::IsHighDist()
{
    return abs(GetArmDist() - m_highDist.Get()) < 2;
}

bool ArmSubsystem::IsMediumDist()
{
    return abs(GetArmDist() - m_mediumDist.Get()) < 2;
}

bool ArmSubsystem::IsArmUp()
{
    return m_wristUp;
}

bool ArmSubsystem::IsArmIn()
{
    return !m_topLimitSwitch.Get();
}

void ArmSubsystem::ResetEncoders()
{
    m_telescopeMotorL.SetSelectedSensorPosition(0,0,0);
}