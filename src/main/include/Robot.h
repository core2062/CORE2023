// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <string>
#include <iostream>
#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <COREFramework/COREScheduler.h>
#include <CORERobotLib.h>


using namespace CORE;
using namespace std;


class Robot : public CORERobot {
 public:
  Robot();
  void RobotInit() override;
  void TeleopInit() override;
  void Teleop() override;
  void Test() override;
  void TestInit() override;
  void SimulationInit() override;
  void SimulationPeriodic() override;
  static Robot * GetInstance();

 private:
 
  static Robot * m_instance;
};
