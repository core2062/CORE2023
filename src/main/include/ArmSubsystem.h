// #pragma once

#include <frc/DigitalInput.h>
#include <frc/DoubleSolenoid.h>
#include <ctre/Phoenix.h>
#include <CORERobotLib.h>
#include "Config.h"
#include "ElevatorSubsystem.h"

// using namespace CORE;
// using namespace frc;

class ArmSubsystem : public CORESubsystem, public CORETask {
public:
    ArmSubsystem();
    void robotInit() override;
    void teleopInit() override;
    void teleop() override;
    void PostLoopTask() override;

    void SetRequestedPosition(double positionInMeters);
    void SetRequestedRotation(bool rot);
    void SetDistRequestedSpeed(double speed);

    void SetHighDist();
    void SetMediumDist();
    void SetRotDown();

    int GetArmDist();
    double GetTelescopeArmMeters();
    
    bool ArmIn();
    bool ArmDown();

    bool IsHighDist();
    bool IsMediumDist();
    // bool IsArmOut();
    bool IsArmUp();
    bool IsArmFullyIn();

    void ResetEncoders();

private:
    enum WantedArmState // This is used to determine which order the arm assembly will move in; (Rot before extend vise versa)
    {
        WANT_TO_SCORE,
        WANT_TO_PICKUP,
        MANUAL
    };


    TalonSRX m_telescopeMotorL, m_telescopeMotorR;
    DoubleSolenoid m_armPiston;
    DigitalInput m_inLimitSwitch, m_downLimitSwitch;
    
    COREConstant<double> m_mediumDist, m_highDist;
    COREConstant<double> m_ticksPerMeter;
    COREConstant<int> m_outerLimit, m_rotationLimit;
    COREConstant<int> m_cruiseTelescopeVel, m_maxTelescopeAcel;

    int m_requestedDist;
    double m_requestedTelescopeSpeed;

    bool m_requestedRotOut;

    WantedArmState m_wantedState;
    
    bool m_wristUp;

    // ElevatorSubsystem* m_elevatorSubsystem;

};