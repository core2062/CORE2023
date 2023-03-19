#pragma once

#include <ctre/Phoenix.h>
#include <CORERobotLib.h>

#include "ArmSubsystem.h"
#include "ElevatorSubsystem.h"
#include "GrabberSubsystem.h"
#include "IntakeSubsystem.h"

using namespace CORE;
using namespace frc;

enum WantedState
{
    MANUAL, // We want manual control
    WANT_TO_PICKUP, // We want to pick up a gamepiece
    WANT_TO_SCORE_MID, // We want to score on the mid level
    WANT_TO_SCORE_HIGH, // We want to score on the high level
    WANT_STARTING_HEIGHT //TODO: Add descriptive comment here
};


class ScoringAssembly : public CORETask {
public:
	ScoringAssembly();
    void RobotInitTask() override;
    void AutonInitTask() override;
    void TeleopInitTask() override;
    void PostLoopTask() override;

    void SetWantedState(WantedState wantedState);

    bool IsTransit();
    bool IsGrabbing();
    bool IsScoringMid();
    bool IsScoringHigh();
    bool IsStartingHeight();

    WantedState GetWantedState();

private:
    enum SystemState {
        TRANSIT,
        GRABBING,
        SCORING_MID,
        SCORING_HIGH,
        STARTING_HEIGHT
    };

    SystemState HandleTransit();
    SystemState HandleGrabbing();
    SystemState HandleScoringHigh();
    SystemState HandleScoringMid();
    SystemState HandleStartingHeight();

    ArmSubsystem* m_armSubsystem;
    ElevatorSubsystem* m_elevatorSubsystem;
    GrabberSubsystem* m_grabberSubsystem;
    IntakeSubsystem* m_intakeSubsystem;

    WantedState m_wantedState;
    SystemState m_systemState;

    bool m_armInElevatorUp, m_systemWithinThreshold;

    COREConstant<double> m_transitTransitionTimeout,m_scoreMidTransitionTimeout;
    COREConstant<double> m_armThreshold,m_elevatorThreshold;
    CORETimer m_timeoutTimer;
};