#pragma once

#include <frc/DigitalInput.h>
#include <frc/DoubleSolenoid.h>
#include <ctre/Phoenix.h>
#include <CORERobotLib.h>
#include "Config.h"
#include "ElevatorSubsystem.h"

using namespace CORE;
using namespace frc;

class ArmSubsystem : public CORESubsystem, public CORETask {
public:
    ArmSubsystem();
    void robotInit() override;
    void teleopInit() override;
    void teleop() override;
    void PostLoopTask() override;

    void SetRequestedPosition(int position);
    void SetRequestedRotation(bool rot);
    void SetDistRequestedSpeed(double speed);

    void SetHighDist();
    void SetMediumDist();
    void SetArmIn();
    void SetRotDown();

    int GetArmDist();
    double GetTelescopeArmMeters();
    
    bool ArmIn();
    bool ArmDown();

    bool IsHighDist();
    bool IsMediumDist();
    bool IsArmUp();
    bool IsArmIn();

    void ResetEncoders();

private:
    TalonSRX m_telescopeMotorL, m_telescopeMotorR;
    DoubleSolenoid m_armPiston;

    Joystick m_operatorJoystick;
    
    COREConstant<double> m_mediumDist, m_highDist;
    COREConstant<int> m_outerLimit;
    COREConstant<int> m_cruiseTelescopeVel, m_maxTelescopeAcel;

    int m_requestedDist;
    double m_requestedTelescopeSpeed;

    bool m_requestedRotUp;
    bool m_wristUp;    

    // ElevatorSubsystem* m_elevatorSubsystem;

};