/*
#pragma once

#include <ctre/Phoenix.h>
#include <CORERobotLib.h>

#include "ArmSubsystem.h"
#include "ElevatorSubsystem.h"

using namespace CORE;
using namespace frc;

enum WantedState
{
    WANT_TO_PICKUP, // We want to pick up a gamepiece
    WANT_TO_SCORE_MID, // We want to score on the mid level
    WANT_TO_SCORE_HIGH, // We want to score on the high level
    MANUAL // We want manual control
};


class ScoringAssembly : public CORETask {
public:
	ScoringAssembly();
    void RobotInitTask() override;
    void AutonInitTask() override;
    void TeleopInitTask() override;
    void PostLoopTask() override;


private:
    ArmSubsystem* m_armSubsystem;
    ElevatorSubsystem* m_elevatorSubsystem;

    // int m_currentLevel;
    // double m_currentElevPosition,m_currentArmPosition;
};
*/