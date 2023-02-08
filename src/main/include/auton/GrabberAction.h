#pragma once

#include <CORERobotLib.h>
#include "GrabberSubsystem.h"

using namespace CORE;

enum grabberAction{
    GRABBER_GRAB,
    GRABBER_RELEASE
};

class GrabberAction : public COREAutonAction {
    public:
        GrabberAction(grabberAction requestedConveyorAction);
        void ActionInit() override;
        CORE::COREAutonAction::actionStatus Action() override;
        void ActionEnd() override;
    private:
        grabberAction m_grabberAction;
};