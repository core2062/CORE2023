#pragma once

#include <CORERobotLib.h>

using namespace CORE;

class DelayAction : public COREAutonAction {
public:
    DelayAction();
    void ActionInit() override;
    CORE::COREAutonAction::actionStatus Action() override;
    void ActionEnd() override;
};