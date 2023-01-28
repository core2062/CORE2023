/*
#pragma once

#include <ctre/Phoenix.h>
#include <CORERobotLib.h>
#include "Config.h"


using namespace CORE;
using namespace frc;


class IntakeSubsystem : public CORESubsystem {
public:
	IntakeSubsystem();
    void robotInit() override;
    void teleopInit() override;
    void teleop() override;
    void SetIntake(double intakeSpeed);

    COREConstant<double> intakeSpeed;
private:
    TalonSRX m_leftIntakeMotor, m_rightIntakeMotor;
};
*/