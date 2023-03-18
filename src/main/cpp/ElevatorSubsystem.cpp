#include "ElevatorSubsystem.h"
#include "Robot.h"

ElevatorSubsystem::ElevatorSubsystem() : 
        m_leftLiftMotor(LEFT_LIFT_PORT),
        m_rightLiftMotor(RIGHT_LIFT_PORT),
        m_bottomLimitSwitch(ELEVATOR_BOTTOM_LIMIT_SWITCH_PORT),
        m_topLimitSwitch(ELEVATOR_TOP_LIMIT_SWITCH_PORT),
        m_operatorJoystick(OPERATOR_JOYSTICK),
        m_pickUpHeight("Elevator Pick Up Height",0),
        m_mediumHeight("Elevator Mid-Level Height",0.1),
        m_highHeight("Elevator High-Level Height",0.4),
        m_startingHeight("Elevator Starting Height",0.08),
        m_safeRotateHeight("Safe rotation height",0.49541015982),
        m_ticksPerMeter("Elevator Ticks Per Meter",27343), 
        m_liftUpSpeedMod("Elevator Up Speed ", 1),
        m_liftDownSpeedMod("Elevator Down Speed ", 1),
        m_liftHoldSpeed("Elevator Hold Speed ", 0.17),
        m_bottomLimit("Elevator Bottom Limit",0),
        m_topManualLimit("Elevator Manual Top Limit",0.586757),
        m_topAutoLimit("Elevator Auto Top Limit",0.486757),
        m_liftkP("Lift kP",0.9),
        m_liftkI("Lift kI",0),
        m_liftkD("Lift kD",0),
        m_cruiseVel("Elevator Cruise Velocity",3000),
        m_maxAcel("Elevator Max Acceleration",3000)
{
    m_rightLiftMotor.SetInverted(true);
}

void ElevatorSubsystem::robotInit()
{
    m_leftLiftMotor.Set(ControlMode::PercentOutput, 0);
    m_rightLiftMotor.Set(ControlMode::PercentOutput, 0);

    m_leftLiftMotor.SetNeutralMode(NeutralMode::Brake);
    m_rightLiftMotor.SetNeutralMode(NeutralMode::Brake);

    m_rightLiftMotor.Follow(m_leftLiftMotor);

    m_leftLiftMotor.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, 0);

    m_leftLiftMotor.SetSensorPhase(true);
    
    m_leftLiftMotor.SetStatusFramePeriod(StatusFrameEnhanced::Status_13_Base_PIDF0, 10);
    m_leftLiftMotor.SetStatusFramePeriod(StatusFrameEnhanced::Status_10_MotionMagic, 10);

    m_leftLiftMotor.ConfigNominalOutputForward(0.23);
    m_leftLiftMotor.ConfigNominalOutputReverse(0);
    m_leftLiftMotor.ConfigPeakOutputForward(1);
    m_leftLiftMotor.ConfigPeakOutputReverse(-1);

    m_leftLiftMotor.ConfigMotionSCurveStrength(0);

    m_leftLiftMotor.SelectProfileSlot(0,0);
    m_leftLiftMotor.Config_kF(0,0,0);
    m_leftLiftMotor.Config_kP(0,m_liftkP.Get(),0);
    m_leftLiftMotor.Config_kI(0,m_liftkI.Get(),0);
    m_leftLiftMotor.Config_kD(0,m_liftkD.Get(),0);

    m_leftLiftMotor.ConfigMotionCruiseVelocity(m_cruiseVel.Get(), 0);
    m_leftLiftMotor.ConfigMotionAcceleration(m_maxAcel.Get(), 0);
    // operatorJoystick->RegisterAxis(CORE::COREJoystick::JoystickAxis::LEFT_STICK_Y);

}

void ElevatorSubsystem::teleopInit(){
    m_holdPosition = false;
    SetRequestedPosition(GetElevatorMeters()); // Sets requested position to current position
}

void ElevatorSubsystem::teleop(){}

// Will probably run after PostLoopTask() in scoring assembly
void ElevatorSubsystem::PostLoopTask(){
    SmartDashboard::PutNumber("Elevator Position Meters", GetElevatorMeters());
    SmartDashboard::PutNumber("Elevator Position Ticks", GetElevatorPosition());
    SmartDashboard::PutNumber("Elevator Velocity", m_leftLiftMotor.GetSelectedSensorVelocity(0));
    SmartDashboard::PutNumber("Requested Elevator Position", m_requestedPosition);

    SmartDashboard::PutBoolean("Elevator Down",IsElevatorDown());
    SmartDashboard::PutBoolean("Elevator Up",IsElevatorUp());

    double elevatorPosition = GetElevatorMeters();

    SetRequestedSpeed(-m_operatorJoystick.GetRawAxis(1));

    SmartDashboard::PutNumber("Elevator Requested Speed", m_requestedSpeed);

    // Deadbands joystick input
    if(m_requestedSpeed < -0.1 || m_requestedSpeed > 0.1)
    {
        if(m_requestedSpeed < 0)
            m_requestedSpeed *= m_liftDownSpeedMod.Get();
        else
            m_requestedSpeed *= m_liftUpSpeedMod.Get();
        SetRequestedPosition(elevatorPosition);
        Robot::GetInstance()->scoringAssembly.SetWantedState(WantedState::MANUAL);
    } else {
        m_requestedSpeed = 0;
    }

    // Softstops the elevator
    if(m_requestedSpeed > 0.0 && IsElevatorUp())
    {
	    // std::cout << "Softstopped" << endl;
        m_requestedSpeed = 0;
        SetRequestedPosition(GetElevatorMeters());
    } 
    else if(IsElevatorDown())
    {
        if(m_requestedSpeed < 0)
        {
            m_requestedSpeed = 0;
            SetRequestedPosition(0);
        }
        ResetEncoders();
    }
    // std::cout << "Button 2: " << m_operatorJoystick.GetRawButton(2) << endl;
    if (m_operatorJoystick.GetRawButtonPressed(2))
    {
        m_holdPosition = true;
    } else if (m_operatorJoystick.GetRawButtonReleased(2))
    {
        m_holdPosition = false;
    }

    // if (m_holdPosition)
    // {
    //     double throttleValue = abs(m_operatorJoystick.GetRawAxis(3)-1);
    //     SmartDashboard::PutNumber("Throttle Value",throttleValue);
    //     m_requestedSpeed = throttleValue;
    //     // m_requestedSpeed = m_liftHoldSpeed.Get();
    // }

    // Sets the motors, if requested speed is within deadbands will move manually else motionmagic will move it to the requested position
    if (m_requestedSpeed < -0.1 || m_requestedSpeed > 0.1)
    {
        m_leftLiftMotor.Set(ControlMode::PercentOutput, m_requestedSpeed);
    } else
    {
        m_leftLiftMotor.Set(ControlMode::MotionMagic, m_requestedPosition);
    }
    
    m_requestedSpeed = 0;
}

void ElevatorSubsystem::SetRequestedPosition(double positionInMeters)
{
    // Sets the requested position after converting to ticks; Used for moving manually
    auto position = (int)(positionInMeters * m_ticksPerMeter.Get());
    position = max(position,0);
    if(Robot::GetInstance()->scoringAssembly.GetWantedState() != WantedState::MANUAL)
    {
        position = min(position, (int)(m_topAutoLimit.Get()*m_ticksPerMeter.Get()));
    } else {
        position = min(position, (int)(m_topManualLimit.Get()*m_ticksPerMeter.Get()));
    }
    m_requestedPosition = position;
}

void ElevatorSubsystem::SetRequestedSpeed(double speed){
    m_requestedSpeed = speed;
}

void ElevatorSubsystem::SetMaxHeight(){
    SetRequestedPosition(m_topAutoLimit.Get());
}

void ElevatorSubsystem::SetHighHeight(){
    SetRequestedPosition(m_highHeight.Get());
}

void ElevatorSubsystem::SetMediumHeight(){
    SetRequestedPosition(m_mediumHeight.Get());
}

void ElevatorSubsystem::SetPickupHeight(){
    // std::cout << "Setting Pickup ";
    SetRequestedPosition(m_pickUpHeight.Get());
}

void ElevatorSubsystem::SetStartingHeight() {
    SetRequestedPosition(m_startingHeight.Get());
}

int ElevatorSubsystem::GetElevatorPosition(){
    return m_leftLiftMotor.GetSelectedSensorPosition();
}

double ElevatorSubsystem::GetElevatorMeters(){
    return (double)(GetElevatorPosition() / m_ticksPerMeter.Get());
}

bool ElevatorSubsystem::IsElevatorDown(){
    return !m_bottomLimitSwitch.Get();// || GetElevatorPosition() < 0;
}

bool ElevatorSubsystem::IsElevatorUp(){
    return !m_topLimitSwitch.Get();
}

bool ElevatorSubsystem::IsMaxAutoExtension()
{
    return GetElevatorPosition() >= (m_topAutoLimit.Get()-200);
}

bool ElevatorSubsystem::IsHighHeight(){
    return abs(GetElevatorMeters() - m_highHeight.Get()) < 2;
}

bool ElevatorSubsystem::IsMediumHeight(){
    return abs(GetElevatorMeters() - m_mediumHeight.Get()) < 2;
}

bool ElevatorSubsystem::IsPickupHeight(){
    return abs(GetElevatorMeters() - m_pickUpHeight.Get()) < 2;
}

bool ElevatorSubsystem::IsStartingHeight()
{
    return abs(GetElevatorMeters() - m_startingHeight.Get()) < 2;
}

void ElevatorSubsystem::ResetEncoders(){
    m_leftLiftMotor.SetSelectedSensorPosition(0,0,0);
}

void ElevatorSubsystem::MoveElevator(){
    m_leftLiftMotor.Set(ControlMode::PercentOutput, m_requestedSpeed);
    m_rightLiftMotor.Set(ControlMode::PercentOutput, m_requestedSpeed);
}