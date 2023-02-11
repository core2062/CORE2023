#pragma once

#include <CORERobotLib.h>
#include "ScoringAssembly.h"

using namespace CORE;

enum scoreMidAction{
    SCORE_MID
};

class ScoreMidAction : public COREAutonAction {
    public:
        ScoreMidAction(scoreMidAction requestedScoreMidAction);
        void ActionInit() override;
        CORE::COREAutonAction::actionStatus Action() override;
        void ActionEnd() override;
    private:
        scoreMidAction m_scoreMidAction;
};