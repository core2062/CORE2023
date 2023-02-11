#include "auton/ScoreMidAction.h"
#include "Robot.h"

ScoreMidAction::ScoreMidAction(scoreMidAction requestedScoreMidAction) {
    m_scoreMidAction = requestedScoreMidAction;
}

void ScoreMidAction::ActionInit() {

}

CORE::COREAutonAction::actionStatus ScoreMidAction::Action() {
    switch(m_scoreMidAction) {
        case SCORE_MID:
            Robot::GetInstance()->scoringAssembly.SetWantedState(WANT_TO_SCORE_MID);
            break;
    }
    return COREAutonAction::actionStatus::END;
}

void ScoreMidAction::ActionEnd() {

}