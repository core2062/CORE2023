#pragma once

#include <CORERobotLib.h>
#include "ScoringAssembly.h"

using namespace CORE;

enum scoreHighAction{
    SCORE_HIGH
};

class ScoreHighAction : public COREAutonAction {
    public:
        ScoreHighAction(scoreHighAction requestedScoreHighAction);
        void ActionInit() override;
        CORE::COREAutonAction::actionStatus Action() override;
        void ActionEnd() override;
    private:
        scoreHighAction m_scoreHighAction;
};