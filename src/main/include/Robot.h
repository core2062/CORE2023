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


#include "DriveSubsystem.h"
#include "IntakeSubsystem.h"
#include "GrabberSubsystem.h"
#include "ScoringAssembly.h"
#include "ArmSubsystem.h"
#include "ElevatorSubsystem.h"
#include "routines/DriveRoutine.h"

using namespace CORE;
using namespace std;


class Robot : public CORERobot {
 public:
  Robot();
  void robotInit() override;
  void teleopInit() override;
  void teleop() override;
  void test() override;
  void testInit() override;
  // void SimulationInit() override;
  // void SimulationPeriodic() override;
  static Robot * GetInstance();

  
  DriveSubsystem driveSubsystem;
  IntakeSubsystem intakeSubsystem;
  GrabberSubsystem grabberSubsystem;
  ScoringAssembly scoringAssembly;
  ArmSubsystem armSubsystem;
  ElevatorSubsystem elevatorSubsystem;
  DriveRoutine driveRoutine;
  
 private:
 
  static Robot * m_instance;
};
