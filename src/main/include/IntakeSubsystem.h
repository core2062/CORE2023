#pragma once

#include <ctre/Phoenix.h>
#include <CORERobotLib.h>
#include "Config.h"


using namespace CORE;
using namespace frc;


class IntakeSubsystem : public CORESubsystem {
public:
	IntakeSubsystem();
    void RobotInit();
    void TeleopInit();
    void Teleop();
    void SetIntake(double intakeSpeed);

    COREConstant<double> intakeSpeed;
private:
    TalonSRX m_leftIntakeMotor, m_rightIntakeMotor;
};
