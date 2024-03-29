#pragma once

#include <CORERobotLib.h>
#include "DriveSubsystem.h"

using namespace CORE;

enum autoBalanceAction{
    AUTO_BALANCE
};

class AutoBalanceAction : public COREAutonAction {
    public:
        AutoBalanceAction(autoBalanceAction requestedBalanceAction, int requestedTime);
        void ActionInit() override;
        CORE::COREAutonAction::actionStatus Action() override;
        void ActionEnd() override;
    private:
        autoBalanceAction m_autoBalanceAction;
        int m_timer;
};