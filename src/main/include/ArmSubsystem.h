#pragma once

#include <frc/DigitalInput.h>
#include <ctre/Phoenix.h>
#include <CORERobotLib.h>
#include "Config.h"

using namespace CORE;
using namespace frc;

class ArmSubsystem : public CORESubsystem/*, public CORETask*/ {
public:
    ArmSubsystem();
    void robotInit() override;
    void teleopInit() override;
    void teleop() override;
    // void PostLoopTask() override;

    /* Will be used for testing only */
    void SetArmMotorSpeed(double percent);
    double GetArmMotorPosition();
    void ResetEncoders();
    void InitTalons();

private:
    TalonSRX m_armMotor, m_armLift;
    COREConstant<double> m_liftSpeed;
    
    DigitalInput m_inLimitSwitch, m_outLimitSwitch;

    // COREConstant<int> m_cruiseVel, m_maxAcel;
    // COREConstant<double> m_mediumDist, m_highDist;
    int m_requestedPosition;
    double m_requestedSpeed;
};