// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

Robot* Robot::m_instance;

Robot::Robot() : m_operatorJoystick(OPERATOR_JOYSTICK)
{
    CORELog::LogInfo("Starting up Robot!");
    m_instance = this;
}

void Robot::robotInit() {
    operatorJoystick->RegisterButton(COREJoystick::JoystickButton::B_BUTTON);
    operatorJoystick->RegisterButton(COREJoystick::JoystickButton::Y_BUTTON);
}

void Robot::teleopInit() {}

/* This is where the current state of the scoring assembly is set */
void Robot::teleop() {
    if (m_operatorJoystick.GetRawButtonReleased(5))
    {
        scoringAssembly.SetWantedState(WantedState::WANT_TO_PICKUP);
    }
    else if (m_operatorJoystick.GetRawButtonReleased(4))
    {
        scoringAssembly.SetWantedState(WantedState::WANT_TO_SCORE_MID);
    }
    else if (m_operatorJoystick.GetRawButtonReleased(6))
    {
        scoringAssembly.SetWantedState(WantedState::WANT_TO_SCORE_HIGH);
    }
    else if (m_operatorJoystick.GetRawButtonReleased(7))
    {
        scoringAssembly.SetWantedState(WantedState::WANT_STARTING_HEIGHT);
    }
    
}

void Robot::test() {}

void Robot::testInit() {}

Robot * Robot::GetInstance() {
    return m_instance;
}

#ifndef RUNNING_FRC_TESTS
int main() {
    return frc::StartRobot<Robot>();
}
 #endif