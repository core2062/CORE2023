#pragma once

#include <frc/Compressor.h>
#include <frc/DoubleSolenoid.h>
#include <frc/AnalogInput.h>
#include <ctre/Phoenix.h>
#include <ctre/phoenix/motorcontrol/TalonFXSensorCollection.h>
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
	void RobotInit() override;
	void TeleopInit() override;
	void Teleop() override;
	
	double getRobotPosition();
	void InitTalons();
	void setMotorSpeed(double speedInFraction, DriveSide whichSide);
	void setMotorSpeed(double leftPercent, double rightPercent);
	void resetEncoder();
	void SetTalonMode(NeutralMode mode);
	AHRS ahrs;

private:
	//m_analogSupplyVoltage used in calculation to get current PSI
	AnalogInput m_analogPressureInput, m_analogSupplyVoltage;
    TalonSRX m_leftMaster, m_rightMaster, m_leftSlave, m_rightSlave;

    COREConstant<double> m_etherAValue, m_etherBValue, m_etherQuickTurnValue, m_ticksPerInch, m_driveSpeedModifier;
    Compressor m_compressor;
};
