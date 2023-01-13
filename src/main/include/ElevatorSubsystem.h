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
    void RobotInit() override;
    void TeleopInit() override;
    void Teleop() override;
private:
};