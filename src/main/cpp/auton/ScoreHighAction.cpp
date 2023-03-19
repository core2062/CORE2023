#include "auton/ScoreHighAction.h"
#include "Robot.h"

ScoreHighAction::ScoreHighAction() {
    m_scoringAssembly = &Robot::GetInstance()->scoringAssembly;
}

void ScoreHighAction::ActionInit() {

}

CORE::COREAutonAction::actionStatus ScoreHighAction::Action() {
    if (m_scoringAssembly->IsScoringHigh())
    {
        std::cout << "System state is scoring high" << endl;
        return COREAutonAction::actionStatus::END;
    } else if(m_scoringAssembly->GetWantedState() == WantedState::WANT_TO_SCORE_HIGH) {
        std::cout << "Wanted state is set" << endl;
        return COREAutonAction::actionStatus::CONTINUE;
    } else {
        std::cout << "Now setting wanted state" << endl;
        m_scoringAssembly->SetWantedState(WantedState::WANT_TO_SCORE_HIGH);
        return COREAutonAction::actionStatus::CONTINUE;
    }
    return COREAutonAction::actionStatus::END;
}

void ScoreHighAction::ActionEnd() {

}