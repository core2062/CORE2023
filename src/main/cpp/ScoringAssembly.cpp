#include "ScoringAssembly.h"
#include "Robot.h"

ScoringAssembly::ScoringAssembly() : m_transitTransitionTimeout("Transit Transition Timeout",4),
                                    m_scoreMidTransitionTimeout("Score Mid Transition Timeout",4),
                                    m_armThreshold("Arm Threshold",2),
                                    m_elevatorThreshold("Elevator Threshold",2)
{}

void ScoringAssembly::RobotInitTask()
{
    m_armSubsystem = &Robot::GetInstance()->armSubsystem;
    m_elevatorSubsystem = &Robot::GetInstance()->elevatorSubsystem;
    m_grabberSubsystem = &Robot::GetInstance()->grabberSubsystem;
    m_intakeSubsystem = &Robot::GetInstance()->intakeSubsystem;

    m_armInElevatorUp = false;
    m_systemWithinThreshold = false;
}

void ScoringAssembly::AutonInitTask()
{
    // Sets up wantedState and systemState
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
}

/* If this actually runs, I believe it runs after the teleop() calls
    This is where the magic happen, actually changing the state of the robot */
void ScoringAssembly::PostLoopTask()
{
    SystemState newState  = m_systemState;
    switch (m_systemState)
    {
        case SystemState::TRANSIT:
            // cout << "Handling transit" << endl;
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
        case SystemState::STARTING_HEIGHT:
            newState = HandleStartingHeight();
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
    m_timeoutTimer.Reset();
    m_timeoutTimer.Start();
}

bool ScoringAssembly::IsTransit(){
    return (m_systemState == SystemState::TRANSIT);
}

bool ScoringAssembly::IsGrabbing(){
    return (m_systemState == SystemState::GRABBING);
}

bool ScoringAssembly::IsScoringMid(){
    return (m_systemState == SystemState::SCORING_MID);
}

bool ScoringAssembly::IsScoringHigh(){
    return (m_systemState == SystemState::SCORING_HIGH);
}

bool ScoringAssembly::IsStartingHeight(){
    return (m_systemState == SystemState::STARTING_HEIGHT);
}

WantedState ScoringAssembly::GetWantedState(){
    return m_wantedState;
}

/*  
    Transit handles the moving of the robot components
    It checks the wanted state of the robot sets the different subsystems
*/
ScoringAssembly::SystemState ScoringAssembly::HandleTransit()
{
    bool reachedTarget = false;
    switch (m_wantedState)
    {
        case WantedState::WANT_TO_PICKUP:
            m_grabberSubsystem->SetClaw(true);
            m_intakeSubsystem->SetIntakeActive(true);
            if ((m_armSubsystem->IsArmIn() && m_elevatorSubsystem->IsMaxAutoExtension()) || m_armInElevatorUp)
            {
                std::cout << "At second phase" << endl;
                m_armInElevatorUp = true;
                m_armSubsystem->SetWristDown(); // To pickup gamepieces, the arm had to be fully retracted and rotated down
                m_elevatorSubsystem->SetPickupHeight(); // Sets the requested height of the elevator to the lowest level
                reachedTarget = (!m_armSubsystem->IsWristUp() && m_elevatorSubsystem->IsPickupHeight() && m_armSubsystem->IsArmIn()); // Checks if the individual subsystems have reached their destination.
            } else {
                m_armSubsystem->SetWristUp(); // To pickup gamepieces, the arm had to be fully retracted and rotated down
                m_elevatorSubsystem->SetMaxHeight();
                m_armSubsystem->SetArmIn();
            }
            reachedTarget = reachedTarget || (m_timeoutTimer.Get() > m_transitTransitionTimeout.Get());
            break;
        case WantedState::WANT_TO_SCORE_MID:
        std::cout << "MiIIiiIIIiiiId!1!!!!!111!!!!1!!" << endl;
            if ((m_armSubsystem->GetArmDist() < m_armThreshold.Get() && !m_armSubsystem->IsWristUp()) || m_systemWithinThreshold)
            {
                m_systemWithinThreshold = true;
                if (m_elevatorSubsystem->IsMaxAutoExtension())
                {
                    m_armSubsystem->SetWristUp();
                    m_systemWithinThreshold = false;
                } else {
                    m_elevatorSubsystem->SetMaxHeight();
                } 
            } else {
                std::cout << "Else also!!!!!1!!11!!1!" << endl;
                m_armSubsystem->SetMediumDist(); // To score at the mid level, the arm had to rotate up and extend partially
                m_elevatorSubsystem->SetMediumHeight(); // Set the requested
                m_armSubsystem->SetWristUp();
                reachedTarget = (m_armSubsystem->IsMediumDist() && m_elevatorSubsystem->IsMediumHeight());
            }
            reachedTarget = reachedTarget || (m_timeoutTimer.Get() > m_scoreMidTransitionTimeout.Get());
            break;
        case WantedState::WANT_TO_SCORE_HIGH:
            std::cout << "scoring high!!!" << endl;
            if ((m_armSubsystem->GetArmDist() < m_armThreshold.Get() && !m_armSubsystem->IsWristUp()) || m_systemWithinThreshold)
            {
                m_systemWithinThreshold = true;
                if (m_elevatorSubsystem->IsMaxAutoExtension())
                {
                    m_armSubsystem->SetWristUp();
                    m_systemWithinThreshold = false;
                } else {
                    m_elevatorSubsystem->SetMaxHeight();
                } 
            } else {
                std::cout << "Else!!!!!1!!11!!1!" << endl;
                m_armSubsystem->SetHighDist(); // To score at the mid level, the arm had to rotate up and extend partially
                m_elevatorSubsystem->SetHighHeight(); // Set the requested
                reachedTarget = (m_armSubsystem->IsHighDist() && m_elevatorSubsystem->IsHighHeight());
            }
            reachedTarget = reachedTarget || (m_timeoutTimer.Get() > m_scoreMidTransitionTimeout.Get());
            break;
        case WANT_STARTING_HEIGHT:
            m_elevatorSubsystem->SetStartingHeight();
            m_armInElevatorUp = false;
            m_systemWithinThreshold = false;
            reachedTarget = (m_elevatorSubsystem->IsStartingHeight() || (m_timeoutTimer.Get() > m_scoreMidTransitionTimeout.Get()));
            break;
        case WantedState::MANUAL: // In case you wanted to manually move the scoring assembly
            m_armInElevatorUp = false;
            m_systemWithinThreshold = false;
            reachedTarget = false;
            break;
    }
    SmartDashboard::PutBoolean("Reached Target",reachedTarget);

    if (reachedTarget)
    {
        m_armInElevatorUp = false;
        m_systemWithinThreshold = false;
    }
    

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

// The functions below handle keeping the scoring assembly in wanted state 

ScoringAssembly::SystemState ScoringAssembly::HandleGrabbing()
{
    // std::cout << "Handle Grabbing called" << endl;
    m_armSubsystem->SetWristDown();
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

ScoringAssembly::SystemState ScoringAssembly::HandleStartingHeight()
{
    m_elevatorSubsystem->SetStartingHeight();
    switch (m_wantedState)
    {
    case WantedState::WANT_STARTING_HEIGHT:
        return SystemState::STARTING_HEIGHT;
    default:
        return SystemState::TRANSIT;
    }
}