#pragma once

#include <CORERobotLib.h>
#include "GrabberSubsystem.h"
#include "ScoringAssembly.h"

using namespace CORE;

enum pickupAction{
    PICKUP
};

class PickupAction : public COREAutonAction {
    public:
        PickupAction(pickupAction requestedPickupAction);
        void ActionInit() override;
        CORE::COREAutonAction::actionStatus Action() override;
        void ActionEnd() override;
    private:
        pickupAction m_pickupAction;
};