#pragma once

#include <frc/AnalogInput.h>
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
    void SetWristUp();
    void SetWristDown();

    double GetArmDist();
    double GetTelescopeArmMeters();
    
    bool WristDown();

    bool IsHighDist();
    bool IsMediumDist();
    bool IsArmIn();
    bool IsArmOut();
    bool IsWristUp();

    bool AutonIsArmMedDist();

    void ResetEncoders();

private:
    TalonSRX m_leftArmMotor, m_rightArmMotor;
    DoubleSolenoid m_armPiston;

    AnalogInput m_armPotentiometer;

    Joystick m_operatorJoystick;
    
    COREConstant<double> m_armSpeed;
    COREConstant<double> m_mediumDist, m_highDist, m_autonMediumDist;
    COREConstant<double> m_inPotentiometer, m_outPotentiometer;
    COREConstant<double> m_armkP, m_armkI, m_armkD;
    COREConstant<int> m_outerLimit;
    COREConstant<int> m_cruiseVel, m_maxAcel;

    int m_requestedDist;
    double m_requestedTelescopeSpeed;

    bool m_requestedRotUp;
    bool m_wristUp;
    bool m_overspoolButton;

    // ElevatorSubsystem* m_elevatorSubsystem;

};