// #pragma once

// #include <ctre/Phoenix.h>
// #include <CORERobotLib.h>
// #include "Config.h"

// using namespace CORE;
// using namespace frc;

// class ArmSubsystem : public CORESubsystem {
// public:
//     ArmSubsystem();
//     void RobotInit() override;
//     void TeleopInit() override;
//     void Teleop() override;

//     void SetArmMotorSpeed(double percent);
//     double GetArmMotorPosition();
//     void ResetEncoders();
//     void InitTalons();

// private:
//     COREConstant<double> m_armMotorSpeedModifier;
//     TalonSRX m_armMotor;
    
// };