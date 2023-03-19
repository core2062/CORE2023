#include "auton/ScoreMidAction.h"
#include "Robot.h"

ScoreMidAction::ScoreMidAction(scoreMidAction requestedScoreMidAction) {
    m_scoreMidAction = requestedScoreMidAction;
    m_scoringAssembly = &Robot::GetInstance()->scoringAssembly;
}

void ScoreMidAction::ActionInit() {

}

CORE::COREAutonAction::actionStatus ScoreMidAction::Action() {
    switch(m_scoreMidAction) {
        case SCORE_MID:
            if (m_scoringAssembly->IsScoringMid())
            {
                break;
            } else if(m_scoringAssembly->GetWantedState() == WantedState::WANT_TO_SCORE_MID) {
                return COREAutonAction::actionStatus::CONTINUE;
            } else {
                m_scoringAssembly->SetWantedState(WantedState::WANT_TO_SCORE_MID);
                return COREAutonAction::actionStatus::CONTINUE;
            }
            break;
    }
    return COREAutonAction::actionStatus::END;
}

void ScoreMidAction::ActionEnd() {

}