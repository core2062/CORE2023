#pragma once

#include <ctre/Phoenix.h>
#include <frc/DoubleSolenoid.h>
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

private:
    TalonSRX m_leftIntakeMotor, m_rightIntakeMotor;
    DoubleSolenoid m_intake;

    Joystick m_operatorJoystick;
    
    COREConstant<double> m_intakeConeSpeed, m_intakeCubeSpeed, m_intakeTimeSet;
    bool m_intakeActive;
    CORETimer m_intakeTimer;
};
