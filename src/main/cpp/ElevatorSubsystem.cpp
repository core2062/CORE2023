#include "ElevatorSubsystem.h"
#include "Robot.h"

ElevatorSubsystem::ElevatorSubsystem() : 
        m_leftLiftMotor(LEFT_LIFT_PORT),
        m_rightLiftMotor(RIGHT_LIFT_PORT),
        m_bottomLimitSwitch(ELEVATOR_BOTTOM_LIMIT_SWITCH_PORT),
        m_topLimitSwitch(ELEVATOR_TOP_LIMIT_SWITCH_PORT),
        m_operatorJoystick(OPERATOR_JOYSTICK),
        m_pickUpHeight("Elevator Pick Up Height"),
        m_mediumHeight("Elevator Mid-Level Height"),
        m_highHeight("Elevator High-Level Height"),
        m_safeRotateHeight("Safe rotation height",0.49541015982),
        m_ticksPerMeter("Elevator Ticks Per Meter",27343), 
        m_bottomLimit("Elevator Bottom Limit",0),
        m_topLimit("Elevator Top Limit",0.586757),
        m_cruiseVel("Elevator Cruise Velocity"),
        m_maxAcel("Elevator Max Acceleration"),
        m_liftUpSpeedMod("Elevator Up Speed ", 0.5),
        m_liftDownSpeedMod("Elevator Down Speed ", 0.3)
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
    m_leftLiftMotor.SetSelectedSensorPosition(0, 0, 0);

    m_leftLiftMotor.SetSensorPhase(true);
    
    // operatorJoystick->RegisterAxis(CORE::COREJoystick::JoystickAxis::LEFT_STICK_Y);

}

void ElevatorSubsystem::teleopInit(){
    SetRequestedPosition(GetElevatorMeters()); // Sets requested position to current position
    m_leftLiftMotor.ConfigMotionCruiseVelocity(m_cruiseVel.Get(), 0);
    m_leftLiftMotor.ConfigMotionAcceleration(m_maxAcel.Get(), 0);
}

void ElevatorSubsystem::teleop(){}

// Will probably run after PostLoopTask() in scoring assembly
void ElevatorSubsystem::PostLoopTask(){
    SmartDashboard::PutNumber("Elevator Position Meters", GetElevatorMeters());
    SmartDashboard::PutNumber("Elevator Velocity", m_leftLiftMotor.GetSelectedSensorVelocity(0));
    SmartDashboard::PutNumber("Requested Elevator Position", m_requestedPosition);
    SmartDashboard::PutBoolean("Elevator safe rotation height", IsSafeRotateHeight());

    double elevatorPosition = GetElevatorMeters();

    SetRequestedSpeed(-m_operatorJoystick.GetRawAxis(1));

    SmartDashboard::PutNumber("Elevator Speed", m_requestedSpeed);

    // Deadbands joystick input
    if(m_requestedSpeed < -0.01 || m_requestedSpeed > 0.1)
    {
        if(m_requestedSpeed < 0)
            m_requestedSpeed *= m_liftDownSpeedMod.Get();
        else
            m_requestedSpeed *= m_liftUpSpeedMod.Get();
        SetRequestedPosition(elevatorPosition);
        Robot::GetInstance()->scoringAssembly.SetWantedState(WantedState::MANUAL);
    }

    // Softstops the elevator
    if(m_requestedSpeed > 0.0 && (/*ElevatorUp() || */GetElevatorMeters() > m_topLimit.Get()))
    {
	    // std::cout << "Softstopped" << endl;
        m_requestedSpeed = 0;
        SetRequestedPosition(m_topLimit.Get());
    } 
    else if(!ElevatorDown())
    {
        
        if(m_requestedSpeed < 0)
        {
            m_requestedSpeed = 0;
            SetRequestedPosition(0);
        }
        ResetEncoders();
    }
    // std::cout << "Button 2: " << m_operatorJoystick.GetRawButton(2) << endl;
    if (m_operatorJoystick.GetRawButton(2))
    {
        m_requestedSpeed = 0.2;
    }

    // Sets the motors, if requested speed is within deadbands will move manually else motionmagic will move it to the requested position
    if (m_requestedSpeed < -0.01 || m_requestedSpeed > 0.1)
    {
        
        m_leftLiftMotor.Set(ControlMode::PercentOutput, m_requestedSpeed);
    } else
    {
        m_leftLiftMotor.Set(ControlMode::MotionMagic, m_requestedPosition);
    }
    
    m_requestedSpeed = 0;
    SmartDashboard::PutNumber("Elevator", elevatorPosition);
}

void ElevatorSubsystem::SetRequestedPosition(double positionInMeters)
{
    // Sets the requested position after converting to ticks; Used for moving manually
    auto position = (int)(positionInMeters * m_ticksPerMeter.Get());
    position = max(position,0);
    position = min(position, (int)(m_topLimit.Get()*m_ticksPerMeter.Get()));
    m_requestedPosition = position;
}

void ElevatorSubsystem::SetRequestedSpeed(double speed){
    m_requestedSpeed = speed;
}

void ElevatorSubsystem::SetMaxHeight(){
    SetRequestedPosition(m_topLimit.Get());
}

void ElevatorSubsystem::SetHighHeight(){
    SetRequestedPosition(m_highHeight.Get());
}

void ElevatorSubsystem::SetMediumHeight(){
    SetRequestedPosition(m_mediumHeight.Get());
}

void ElevatorSubsystem::SetPickupHeight(){
    SetRequestedPosition(m_pickUpHeight.Get());
}

int ElevatorSubsystem::GetElevatorPosition(){
    return m_leftLiftMotor.GetSelectedSensorPosition();
}

double ElevatorSubsystem::GetElevatorMeters(){
    return GetElevatorPosition() / m_ticksPerMeter.Get();
}

bool ElevatorSubsystem::ElevatorDown(){
    return m_bottomLimitSwitch.Get();
}

bool ElevatorSubsystem::ElevatorUp(){
    return m_topLimitSwitch.Get();
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

bool ElevatorSubsystem::IsSafeRotateHeight(){
    // return GetElevatorMeters() >= m_safeRotateHeight.Get();
    return true;
}

void ElevatorSubsystem::ResetEncoders(){
    m_leftLiftMotor.SetSelectedSensorPosition(0,0,0);
}
