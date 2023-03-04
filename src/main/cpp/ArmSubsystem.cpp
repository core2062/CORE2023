#include "ArmSubsystem.h"
#include "Robot.h"

//TODO: Ask Sid which button he wants to actuate the wrist & implement
// 5 Pcikup
// 6 high
// 4 Med
// Pad W awrist

ArmSubsystem::ArmSubsystem() : 
        m_leftArmMotor(LEFT_ARM_MOTOR),
        m_rightArmMotor(RIGHT_ARM_MOTOR),
        m_armPiston(frc::PneumaticsModuleType::REVPH, ARM_IN_PORT, ARM_OUT_PORT),
        m_armLimitSwitch(ARM_IN_LIMIT_SWITCH_PORT),
        m_operatorJoystick(OPERATOR_JOYSTICK),
        m_armSpeed("Arm Speed", 0.7),
        m_mediumDist("Arm Mid-Level Distance In Ticks",12000),
        m_highDist("Arm High-Level Distance In Ticks",23000),
        m_outerLimit("Arm Outer Limit In Ticks",18600),
        m_cruiseVel("Telescope Arm Cruise Velocity",1000),
        m_maxAcel("Telescope Arm Max Acceleration",1000)
{}

void ArmSubsystem::robotInit()
{
    m_wristUp = true;
    m_rightArmMotor.SetInverted(true);

    m_rightArmMotor.Set(ControlMode::PercentOutput, 0);
    m_leftArmMotor.Set(ControlMode::PercentOutput, 0);

    m_rightArmMotor.SetNeutralMode(NeutralMode::Brake);
    m_leftArmMotor.SetNeutralMode(NeutralMode::Brake);

    m_rightArmMotor.Follow(m_leftArmMotor);

    m_leftArmMotor.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, 0);
    // m_leftArmMotor.SetSelectedSensorPosition(0, 0, 0);

    m_leftArmMotor.SetSensorPhase(true);

    m_leftArmMotor.SetStatusFramePeriod(StatusFrameEnhanced::Status_13_Base_PIDF0, 10);
    m_leftArmMotor.SetStatusFramePeriod(StatusFrameEnhanced::Status_10_MotionMagic, 10);

    m_leftArmMotor.ConfigNominalOutputForward(0);
    m_leftArmMotor.ConfigNominalOutputReverse(0);
    m_leftArmMotor.ConfigPeakOutputForward(1);
    m_leftArmMotor.ConfigPeakOutputReverse(-1);

    m_leftArmMotor.SelectProfileSlot(0,0);
    m_leftArmMotor.Config_kF(0,0,0);
    m_leftArmMotor.Config_kP(0,0.75,0);
    m_leftArmMotor.Config_kI(0,0,0);
    m_leftArmMotor.Config_kD(0,0,0);

    m_leftArmMotor.ConfigMotionCruiseVelocity(m_cruiseVel.Get(), 0);
    m_leftArmMotor.ConfigMotionAcceleration(m_maxAcel.Get(), 0);
    

    // operatorJoystick->RegisterAxis(CORE::COREJoystick::JoystickAxis::RIGHT_STICK_Y);
    // operatorJoystick->RegisterButton(CORE::COREJoystick::JoystickButton::BACK_BUTTON);

}

void ArmSubsystem::teleopInit()
{
    SetRequestedPosition(GetArmDist()); // Sets requested position to current position
}

void ArmSubsystem::teleop()
{
    if(m_operatorJoystick.GetRawButtonReleased(3))
    {
        m_requestedRotUp = !m_requestedRotUp;
        m_wristUp = !m_wristUp;
        Robot::GetInstance()->scoringAssembly.SetWantedState(WantedState::MANUAL);
    }
}

// Will probably run after PostLoopTask() in scoring assembly
void ArmSubsystem::PostLoopTask()
{
    SmartDashboard::PutNumber("Arm Telescope Position", m_leftArmMotor.GetSelectedSensorPosition(0));
    SmartDashboard::PutNumber("Arm Telescope LEFT Velocity", m_leftArmMotor.GetSelectedSensorVelocity(0));
    SmartDashboard::PutNumber("Arm Telescope RIGHT Velocity", m_rightArmMotor.GetSelectedSensorVelocity(0));
    SmartDashboard::PutNumber("Arm Telescope Requested Position", m_requestedDist);

    SmartDashboard::PutBoolean("Wrist Up", IsWristUp());
    SmartDashboard::PutBoolean("Arm Fully In", IsArmIn());
    SmartDashboard::PutBoolean("Requested Wrist Up", m_requestedRotUp);

    double telescopePosition = GetArmDist();

    if (m_operatorJoystick.GetPOV() == 0)
    {
        Robot::GetInstance()->scoringAssembly.SetWantedState(WantedState::MANUAL);
        SetDistRequestedSpeed(1);
    } else if (m_operatorJoystick.GetPOV() == 180)
    {
        Robot::GetInstance()->scoringAssembly.SetWantedState(WantedState::MANUAL);
        SetDistRequestedSpeed(-1);
    } else {
        SetDistRequestedSpeed(0);
    }
    
    
    // SetDistRequestedSpeed(m_operatorJoystick.GetRawAxis(0));

    SmartDashboard::PutNumber("Requested Telescope Speed", m_requestedTelescopeSpeed);

    // Deadbands joystick input
    // if(m_requestedTelescopeSpeed < -0.1 || m_requestedTelescopeSpeed > 0.1)
    // {
    //     // m_requestedTelescopeSpeed *= 0.5;
    //     SetRequestedPosition(telescopePosition);
    // } else {
    //     m_requestedTelescopeSpeed = 0;
    // }

    // Softstop for telescoping arm extend & rotation
    if(m_requestedTelescopeSpeed > 0 && (GetArmDist() - m_outerLimit.Get()) > 100)
    {
        // std::cout << "Softstopping arm out" << endl;
        m_requestedTelescopeSpeed = 0;
        SetRequestedPosition(m_outerLimit.Get());
    } else if(IsArmIn())
    {
        if (m_requestedTelescopeSpeed < 0)
        {
            // std::cout << "Softstopping arm in" << endl;
            m_requestedTelescopeSpeed = 0;
            SetRequestedPosition(0);
        }
        ResetEncoders();
    }

    std::cout << "Curr wanted state: " << Robot::GetInstance()->scoringAssembly.GetWantedState() << endl;
    if(m_requestedTelescopeSpeed < -0.1 || m_requestedTelescopeSpeed > 0.1)
    {
        m_leftArmMotor.Set(ControlMode::PercentOutput,m_requestedTelescopeSpeed);
    } else if (Robot::GetInstance()->scoringAssembly.GetWantedState() != WantedState::MANUAL) {
        m_leftArmMotor.Set(ControlMode::MotionMagic,m_requestedDist);
    } else {
        m_leftArmMotor.Set(ControlMode::PercentOutput,0);
    }

    if(m_requestedRotUp)
    {
        m_armPiston.Set(DoubleSolenoid::kReverse);
    } else {
        m_armPiston.Set(DoubleSolenoid::kForward);
    }
    m_wristUp = m_requestedRotUp;
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

void ArmSubsystem::SetWristUp()
{
    SetRequestedRotation(true);
}

void ArmSubsystem::SetWristDown()
{
    SetRequestedRotation(false);
}

int ArmSubsystem::GetArmDist()
{
    return m_leftArmMotor.GetSelectedSensorPosition(0);
}


bool ArmSubsystem::IsHighDist()
{
    return abs(GetArmDist() - m_highDist.Get()) < 2;
}

bool ArmSubsystem::IsMediumDist()
{
    return abs(GetArmDist() - m_mediumDist.Get()) < 2;
}

bool ArmSubsystem::IsWristUp()
{
    return m_wristUp;
}

bool ArmSubsystem::IsArmIn()
{
    return !m_armLimitSwitch.Get();
}

bool ArmSubsystem::IsArmInRange()
{
    return IsArmIn() || GetArmDist() < 200;
}

void ArmSubsystem::ResetEncoders()
{
    m_leftArmMotor.SetSelectedSensorPosition(0,0,0);
}