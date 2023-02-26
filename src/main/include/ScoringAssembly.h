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

    void SetWantedState(WantedState wantedState);

private:
    enum SystemState {
        TRANSIT,
        GRABBING,
        SCORING_HIGH,
        SCORING_MID
    };

    SystemState HandleTransit();
    SystemState HandleGrabbing();
    SystemState HandleScoringHigh();
    SystemState HandleScoringMid();

    ArmSubsystem* m_armSubsystem;
    ElevatorSubsystem* m_elevatorSubsystem;

    WantedState m_wantedState;
    SystemState m_systemState;

    bool m_armInElevatorUp;

    COREConstant<double> m_transitTransitionTimeout;
    COREConstant<double> m_armThreshold;
    CORETimer m_timeoutTimer;
};