// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

Robot* Robot::m_instance;

Robot::Robot() {
    CORELog::LogInfo("Starting up Robot!");
    m_instance = this;
}

void Robot::robotInit() {
    operatorJoystick->RegisterButton(COREJoystick::JoystickButton::A_BUTTON);
    operatorJoystick->RegisterButton(COREJoystick::JoystickButton::B_BUTTON);
    operatorJoystick->RegisterButton(COREJoystick::JoystickButton::Y_BUTTON);
}

void Robot::teleopInit() {}

void Robot::teleop() {
    if (operatorJoystick->GetRisingEdge(COREJoystick::JoystickButton::A_BUTTON))
    {
        scoringAssembly.SetWantedState(WantedState::WANT_TO_PICKUP);
    }
    else if (operatorJoystick->GetRisingEdge(COREJoystick::JoystickButton::B_BUTTON))
    {
        scoringAssembly.SetWantedState(WantedState::WANT_TO_SCORE_MID);
    }
    else if (operatorJoystick->GetRisingEdge(COREJoystick::JoystickButton::Y_BUTTON))
    {
        scoringAssembly.SetWantedState(WantedState::WANT_TO_SCORE_HIGH);
    }
    
}

void Robot::test() {}

void Robot::testInit() {}

void Robot::SimulationInit() {}

void Robot::SimulationPeriodic() {}

Robot * Robot::GetInstance() {
    return m_instance;
}

#ifndef RUNNING_FRC_TESTS
int main() {
    return frc::StartRobot<Robot>();
}
 #endif