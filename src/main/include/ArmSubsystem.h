#pragma once

#include <frc/DigitalInput.h>
#include <ctre/Phoenix.h>
#include <CORERobotLib.h>
#include "Config.h"

using namespace CORE;
using namespace frc;

class ArmSubsystem : public CORESubsystem, public CORETask {
public:
    ArmSubsystem();
    void robotInit() override;
    void teleopInit() override;
    void teleop() override;
    void PostLoopTask() override;

    void SetRequestedPosition(double positionInInches);
    void SetRequestedRotation(double rotationInDegrees);
    void SetDistRequestedSpeed(double speed);
    void SetRotRequestedSpeed(double speed);

    void SetHighDist();
    void SetMediumDist();
    void SetRotDown();

    int GetArmDist();
    int GetArmRotation();
    double GetTelescopeArmInches();
    double GetRotationArmDegrees();
    
    bool ArmIn();
    bool ArmDown();

    bool IsHighDist();
    bool IsMediumDist();
    bool IsArmOut();
    bool IsArmUp();

    void ResetRotationEncoder();
    void ResetDistanceEncoder();
    void ResetEncoders();

private:
    enum WantedArmState
    {
        WANT_TO_SCORE,
        WANT_TO_PICKUP,
        MANUAL
    };


    TalonSRX m_armMotor, m_armLift;
    DigitalInput m_inLimitSwitch, m_downLimitSwitch;
    
    COREConstant<double> m_rotationDist, m_mediumDist, m_highDist;
    COREConstant<double> m_ticksPerMeter, m_ticksPerDegree;
    // When Telescope @ m_innerLimit & rotation @ m_rotationLimit, it can either extend or rotate
    COREConstant<int> m_innerLimit, m_outerLimit, m_rotationLimit;
    COREConstant<int> m_cruiseTelescopeVel, m_maxTelescopeAcel;
    COREConstant<int> m_cruiseRotationVel, m_maxRotationAcel;

    int m_requestedDist;
    double m_requestedTelescopeSpeed;

    int m_requestedRot;
    double m_requestedRotSpeed;

    WantedArmState m_wantedState;
};