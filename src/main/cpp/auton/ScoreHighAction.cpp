#include "auton/ScoreHighAction.h"
#include "Robot.h"

ScoreHighAction::ScoreHighAction(scoreHighAction requestedScoreHighAction) {
    m_scoreHighAction = requestedScoreHighAction;
}

void ScoreHighAction::ActionInit() {

}

CORE::COREAutonAction::actionStatus ScoreHighAction::Action() {
    switch(m_scoreHighAction) {
        case SCORE_HIGH:
            Robot::GetInstance()->scoringAssembly.SetWantedState(WANT_TO_SCORE_HIGH);
            break;
    }
    return COREAutonAction::actionStatus::END;
}

void ScoreHighAction::ActionEnd() {

}