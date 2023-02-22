// #pragma once

// #include <ctre/Phoenix.h>
// #include <frc/DoubleSolenoid.h>
// #include <CORERobotLib.h>
// #include "Config.h"


// using namespace CORE;
// using namespace frc;


class GrabberSubsystem : public CORESubsystem {
public:
	GrabberSubsystem();
    void robotInit() override;
    void teleopInit() override;
    void teleop() override;
private:
    DoubleSolenoid m_clawActuator;
    bool m_clawActive;
};