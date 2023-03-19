#pragma once

#include <CORERobotLib.h>
#include "ScoringAssembly.h"

using namespace CORE;

class ScoreHighAction : public COREAutonAction {
    public:
        ScoreHighAction();
        void ActionInit() override;
        CORE::COREAutonAction::actionStatus Action() override;
        void ActionEnd() override;
    private:
        ScoringAssembly* m_scoringAssembly;
};