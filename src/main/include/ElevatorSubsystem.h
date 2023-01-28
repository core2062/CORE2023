#pragma once

#include <ctre/Phoenix.h>
#include <frc/DoubleSolenoid.h>
#include <CORERobotLib.h>
#include "Config.h"


using namespace CORE;
using namespace frc;


class ElevatorSubsystem : public CORESubsystem {
public:
	ElevatorSubsystem();
    void robotInit() override;
    void teleopInit() override;
    void teleop() override;
    void ElevatorSpeed(double);
    double GetMotorPosition();
    double liftStart;
private:
	TalonSRX m_leftLiftMotor, m_rightLiftMotor;
    COREConstant<double> m_liftSpeedModifier;
};