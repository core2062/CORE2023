#pragma once

#include <ctre/Phoenix.h>
#include <frc/DoubleSolenoid.h>
#include <frc/DigitalInput.h>
#include <CORERobotLib.h>
#include "Config.h"


using namespace CORE;
using namespace frc;


class ElevatorSubsystem : public CORESubsystem, public CORETask {
public:
	ElevatorSubsystem();
    void robotInit() override;
    void teleopInit() override;
    void teleop() override;
    void PostLoopTask() override;

    void SetRequestedPosition(double positionInInches);
    void SetRequestedSpeed(double speed);

    void SetHighHeight();
    void SetMediumHeight();
    void SetPickupHeight();

    int GetElevatorPosition();
    double GetElevatorInches();
    bool LiftDown();
    bool LiftTop();

    bool IsHighHeight();
    bool IsMediumHeight();
    bool IsPickupHeight();

    void ResetEncoder();

private:
	TalonSRX m_leftLiftMotor, m_rightLiftMotor;
    DigitalInput m_bottomLimitSwitch, m_topLimitSwitch;
    
    COREConstant<double> m_pickUpHeight, m_mediumHeight, m_highHeight;
    COREConstant<double> m_ticksPerInch;
    COREConstant<int> m_cruiseVel, m_maxAcel;

    int m_requestedPosition;
    double m_requestedSpeed;

};