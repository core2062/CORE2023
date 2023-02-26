#pragma once

#include <frc/Compressor.h>
#include <frc/AnalogInput.h>
#include <ctre/Phoenix.h>
#include <AHRS.h>
#include <CORERobotLib.h>
#include <COREFramework/COREScheduler.h>
#include <COREUtilities/CORETimer.h>

#include "Config.h"

using namespace CORE;
using namespace frc;

enum class DriveSide{LEFT = 1, RIGHT = 2, BOTH = 3};

class DriveSubsystem : public CORESubsystem {
public:
	DriveSubsystem();
	void robotInit() override;
	void teleopInit() override;
	void teleop() override;
	
	double getRobotPosition();
	void InitTalons();
	void setMotorSpeed(double speedInFraction, DriveSide whichSide);
	void setMotorSpeed(double leftPercent, double rightPercent);
	void resetEncoder();
	void SetTalonMode(NeutralMode mode);
	void Balance();
	AHRS ahrs;

private:
	//m_analogSupplyVoltage used in calculation to get current PSI
	// AnalogInput m_analogPressureInput, m_analogSupplyVoltage;
    TalonFX m_leftPrimary, m_rightPrimary, m_leftSecondary, m_rightSecondary;
	double m_currentPitch;
    COREConstant<double> m_etherAValue, m_etherBValue, m_etherQuickTurnValue, m_ticksPerInch, m_driveSpeedModifier, m_driveSpeedModifierSlow, m_balanceMaxSpeed, m_balanceCalibration, m_balanceMaxCalibration;

	// CORETimer m_timeoutTimer;
    Compressor m_compressor;
};
