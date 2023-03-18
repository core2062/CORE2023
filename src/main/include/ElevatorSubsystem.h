#pragma once

#include <ctre/Phoenix.h>
#include <frc/DoubleSolenoid.h>
#include <frc/DigitalInput.h>
#include <frc/Joystick.h>
#include <CORERobotLib.h>
#include "Config.h"
#include "ArmSubsystem.h"

using namespace CORE;
using namespace frc;


class ElevatorSubsystem : public CORESubsystem, public CORETask {
public:
	ElevatorSubsystem();
    void robotInit() override;
    void teleopInit() override;
    void teleop() override;
    void PostLoopTask() override;

    void SetRequestedPosition(double positionInMeters);
    void SetRequestedSpeed(double speed);

    void SetMaxHeight();
    void SetHighHeight();
    void SetMediumHeight();
    void SetPickupHeight();
    void SetStartingHeight();

    int GetElevatorPosition();
    double GetElevatorMeters();
    bool IsElevatorDown();
    bool IsElevatorUp();

    bool IsMaxAutoExtension();
    bool IsHighHeight();
    bool IsMediumHeight();
    bool IsPickupHeight();
    bool IsStartingHeight();
    bool IsSafeRotateHeight();

    void ResetEncoders();

    void MoveElevator();

private:
	TalonSRX m_leftLiftMotor, m_rightLiftMotor;
    DigitalInput m_bottomLimitSwitch, m_topLimitSwitch;

    Joystick m_operatorJoystick;
    
    COREConstant<double> m_pickUpHeight, m_mediumHeight, m_highHeight, m_startingHeight, m_safeRotateHeight;
    COREConstant<double> m_ticksPerMeter;
    COREConstant<double> m_liftUpSpeedMod, m_liftDownSpeedMod, m_liftHoldSpeed;
    COREConstant<double> m_bottomLimit, m_topManualLimit, m_topAutoLimit;
    COREConstant<double> m_liftkP, m_liftkI, m_liftkD;
    COREConstant<int> m_cruiseVel, m_maxAcel;

    int m_requestedPosition;
    double m_requestedSpeed;
    bool m_holdPosition;

    // ArmSubsystem* m_armSubsystem;
};