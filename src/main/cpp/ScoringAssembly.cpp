#include "ScoringAssembly.h"
#include "Robot.h"

ScoringAssembly::ScoringAssembly() : m_transitTransitionTimeout("Transit Transition Timeout")
{}

void ScoringAssembly::RobotInitTask()
{
    m_armSubsystem = &Robot::GetInstance()->armSubsystem;
    m_elevatorSubsystem = &Robot::GetInstance()->elevatorSubsystem;
}

void ScoringAssembly::AutonInitTask()
{
    m_wantedState = WantedState::MANUAL;
    m_systemState = SystemState::TRANSIT;
    m_timeoutTimer.Reset();
    m_timeoutTimer.Start();
}

void ScoringAssembly::TeleopInitTask()
{
    m_wantedState = WantedState::MANUAL;
    m_systemState = SystemState::TRANSIT;
    m_timeoutTimer.Reset();
    m_timeoutTimer.Start();
}

void ScoringAssembly::PostLoopTask()
{
    SystemState newState  = m_systemState;
    switch (m_systemState)
    {
        case SystemState::TRANSIT:
            newState = HandleTransit();
            break;
        case SystemState::GRABBING:
            newState = HandleGrabbing();
            break;
        case SystemState::SCORING_HIGH:
            newState = HandleScoringHigh();
            break;
        case SystemState::SCORING_MID:
            newState = HandleScoringMid();
            break;
    }

    if (newState != m_systemState)
    {
        CORELog::LogInfo("Changing states from " + to_string((int)m_systemState) + " to " + to_string((int)newState));
        m_systemState = newState;
        m_timeoutTimer.Reset();
        m_timeoutTimer.Start();
    }
    
}

void ScoringAssembly::SetWantedState(WantedState wantedState){
    m_wantedState = wantedState;
}

ScoringAssembly::SystemState ScoringAssembly::HandleTransit()
{
    bool reachedTarget = false;
    switch (m_wantedState)
    {
        case WantedState::WANT_TO_PICKUP:
            m_armSubsystem->SetRotDown();
            m_elevatorSubsystem->SetPickupHeight();
            reachedTarget = (m_armSubsystem->ArmDown() && m_elevatorSubsystem->IsPickupHeight());
            break;
        case WantedState::WANT_TO_SCORE_MID:
            m_armSubsystem->SetMediumDist();
            m_elevatorSubsystem->SetMediumHeight();
            reachedTarget = (m_armSubsystem->IsMediumDist() && m_elevatorSubsystem->IsMediumHeight());
            break;
        case WantedState::WANT_TO_SCORE_HIGH:
            m_armSubsystem->SetHighDist();
            m_elevatorSubsystem->SetHighHeight();
            reachedTarget = (m_armSubsystem->IsHighDist() && m_elevatorSubsystem->IsHighHeight());
            break;
        case WantedState::MANUAL:
            break;
    }

    reachedTarget = reachedTarget || m_timeoutTimer.Get() > m_transitTransitionTimeout.Get();

    // State Transition
    switch (m_wantedState)
    {
        case WantedState::WANT_TO_PICKUP:
            if (reachedTarget)
                return SystemState::GRABBING;
            else
                return SystemState::TRANSIT;
        case WantedState::WANT_TO_SCORE_HIGH:
            if (reachedTarget)
                return SystemState::SCORING_HIGH;
            else
                return SystemState::TRANSIT;
        case WantedState::WANT_TO_SCORE_MID:
            if (reachedTarget)
                return SystemState::SCORING_MID;
            else
                return SystemState::TRANSIT;
        case WantedState::MANUAL:
            return SystemState::TRANSIT;
        default:
            return SystemState::TRANSIT;
    }
}

//Im ngl Im not entirely sure why the code below exists, but it does

ScoringAssembly::SystemState ScoringAssembly::HandleGrabbing()
{
    m_armSubsystem->SetRotDown();
    m_elevatorSubsystem->SetPickupHeight();
    switch (m_wantedState)
    {
    case WantedState::WANT_TO_PICKUP:
        return SystemState::GRABBING;
    default:
        return SystemState::TRANSIT;
    }
}

ScoringAssembly::SystemState ScoringAssembly::HandleScoringHigh()
{
    m_armSubsystem->SetHighDist();
    m_elevatorSubsystem->SetHighHeight();
    switch (m_wantedState)
    {
    case WantedState::WANT_TO_SCORE_HIGH:
        return SystemState::SCORING_HIGH;
    default:
        return SystemState::TRANSIT;
    }
}

ScoringAssembly::SystemState ScoringAssembly::HandleScoringMid()
{
    m_armSubsystem->SetMediumDist();
    m_elevatorSubsystem->SetMediumHeight();
    switch (m_wantedState)
    {
    case WantedState::WANT_TO_SCORE_MID:
        return SystemState::SCORING_MID;
    default:
        return SystemState::TRANSIT;
    }
}