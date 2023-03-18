#pragma once

#include <CORERobotLib.h>
#include "ArmSubsystem.h"

using namespace CORE;

enum armAction{
    WRIST_UP,
    WRIST_DOWN,
    ARM_OUT_MID,
    ARM_OUT_HIGH,
    ARM_IN
};

class ArmAction : public COREAutonAction {
    public:
        ArmAction(armAction requestedaArmAction);
        void ActionInit() override;
        CORE::COREAutonAction::actionStatus Action() override;
        void ActionEnd() override;
    private:
        armAction m_armAction;
        ArmSubsystem* m_armSubsystem;
};