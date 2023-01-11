// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

Robot* Robot::m_instance;

Robot::Robot() {
    CORELog::LogInfo("Starting up Robot!");
    m_instance = this;
}

void Robot::Teleop() {}

void Robot::RobotInit() {}

void Robot::TeleopInit() {}

void Robot::Test() {}

void Robot::TestInit() {}

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