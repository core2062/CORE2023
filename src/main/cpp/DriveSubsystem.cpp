#include "DriveSubsystem.h"

DriveSubsystem::DriveSubsystem() :
		ahrs(SerialPort::kUSB),
		m_analogPressureInput(0),
		m_analogSupplyVoltage(1),
		m_leftMaster(LEFT_FRONT_PORT),
		m_rightMaster(RIGHT_FRONT_PORT),
		m_leftSlave(LEFT_BACK_PORT),
		m_rightSlave(RIGHT_BACK_PORT),
		m_etherAValue("Ether A Value", .6),
        m_etherBValue("Ether B Value", .4),
		m_etherQuickTurnValue("Ether Quick Turn Value", 1.0),
        m_ticksPerInch("Ticks Per Inch", (4 * 3.1415) / 1024),
		m_driveSpeedModifier("Drive speed Modifier", 1),
		m_balanceMaxSpeed("Max speed of Balance", 0.5),
		m_balanceCalibration("Robot Pitch", 2.5),
		m_balanceMaxCalibration("max proportion of robot", 0.30),
		m_compressor(frc::PneumaticsModuleType::REVPH) {
}

void DriveSubsystem::robotInit() {
	// Registers joystick axis and buttons, does inital setup for talons
	driverJoystick->RegisterAxis(CORE::COREJoystick::LEFT_STICK_Y);
	driverJoystick->RegisterAxis(CORE::COREJoystick::RIGHT_STICK_X);
	driverJoystick->RegisterButton(CORE::COREJoystick::RIGHT_TRIGGER);
	driverJoystick->RegisterButton(CORE::COREJoystick::B_BUTTON);
	driverJoystick->RegisterButton(CORE::COREJoystick::START_BUTTON);
    InitTalons();
}

void DriveSubsystem::teleopInit() {
	// Sets ether drive values, inits talons
	COREEtherDrive::SetAB(m_etherAValue.Get(), m_etherBValue.Get());
	COREEtherDrive::SetQuickturn(m_etherQuickTurnValue.Get());
	InitTalons();
	m_compressor.EnableDigital();
	SmartDashboard::PutString("Drive Controls", " Forward/Back: Left Stick \n Right/Left: Right Stick \n Shift: Right Trigger");
}

void DriveSubsystem::teleop() {
	// Code for teleop. Sets motor speed based on the values for the joystick, runs compressor,
	// Toggles gears
    double mag = -driverJoystick->GetAxis(CORE::COREJoystick::JoystickAxis::LEFT_STICK_Y);
	double rot = driverJoystick->GetAxis(CORE::COREJoystick::JoystickAxis::RIGHT_STICK_X);

	VelocityPair speeds = COREEtherDrive::Calculate(mag, rot, .1);
	setMotorSpeed(speeds.left, speeds.right);
	SmartDashboard::PutNumber("Left side speed", speeds.left);
	SmartDashboard::PutNumber("Right side speed", speeds.right);
	SmartDashboard::PutNumber("Left side encoder", m_leftMaster.GetSelectedSensorPosition(0));
	SmartDashboard::PutNumber("Right side encoder", m_rightMaster.GetSelectedSensorPosition(0));

	SmartDashboard::PutNumber("Robot Heading", ahrs.GetFusedHeading());
	
	SmartDashboard::PutNumber("Pressure", (250* (m_analogPressureInput.GetVoltage()/m_analogSupplyVoltage.GetVoltage())-25));
	// SetTalonMode(NeutralMode::Coast);
	if (driverJoystick->GetRisingEdge(CORE::COREJoystick::START_BUTTON)){
		SetTalonMode(NeutralMode::Coast);
	}
	if (driverJoystick->GetButton(CORE::COREJoystick::B_BUTTON)){
		SetTalonMode(NeutralMode::Brake);
		Balance();
	}
}

void DriveSubsystem::setMotorSpeed(double speedInFraction, DriveSide whichSide) {
	// Sets motor speed based on drive side and desired speed
	if (whichSide == DriveSide::BOTH || whichSide == DriveSide::RIGHT) {
		m_rightMaster.Set(ControlMode::PercentOutput, speedInFraction);
		m_rightSlave.Set(ControlMode::PercentOutput, speedInFraction);
	}
	if (whichSide == DriveSide::BOTH || whichSide == DriveSide::LEFT) {
		m_leftMaster.Set(ControlMode::PercentOutput, speedInFraction);
		m_leftSlave.Set(ControlMode::PercentOutput, speedInFraction);
	}
}

void DriveSubsystem::setMotorSpeed(double leftPercent, double rightPercent) {
	// Sets speed based on percent output desired
	setMotorSpeed(leftPercent, DriveSide::LEFT);
	setMotorSpeed(rightPercent, DriveSide::RIGHT);
}

double DriveSubsystem::getRobotPosition() {
	return m_rightMaster.GetSelectedSensorPosition(0);
}

void DriveSubsystem::InitTalons() {
	// Sets up talons
	m_leftMaster.Set(ControlMode::PercentOutput, 0);
	m_leftSlave.Set(ControlMode::PercentOutput, 0);
	m_rightMaster.Set(ControlMode::PercentOutput, 0);
	m_rightSlave.Set(ControlMode::PercentOutput, 0);

	// Encoder Functions
    m_leftMaster.SetStatusFramePeriod(StatusFrameEnhanced::Status_1_General, 10, 0);
    m_rightMaster.SetStatusFramePeriod(StatusFrameEnhanced::Status_1_General, 10, 0);

    m_leftMaster.ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::IntegratedSensor, 0, 0);
    m_rightMaster.ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::IntegratedSensor, 0, 0);

	// m_rightMaster.SetSelectedSensorPosition(0.0);
	// m_leftMaster.SetSelectedSensorPosition(0.0);
	
	SetTalonMode(NeutralMode::Coast);

	m_leftMaster.SetSensorPhase(false);
    m_rightMaster.SetSensorPhase(false);

	// Motor Inversion
	m_leftMaster.SetInverted(false);
	m_leftSlave.SetInverted(false);
	m_rightMaster.SetInverted(true);
	m_rightSlave.SetInverted(true);
 
	SetTalonMode(NeutralMode::Coast);
}


void DriveSubsystem::resetEncoder(){
	cout << "Reset encoder error value" <<  m_rightMaster.SetSelectedSensorPosition(0.0, 0, 25);
}

void DriveSubsystem::SetTalonMode(NeutralMode mode){
	m_rightMaster.SetNeutralMode(mode);
	m_rightSlave.SetNeutralMode(mode);
	m_leftMaster.SetNeutralMode(mode);
	m_leftSlave.SetNeutralMode(mode);
}


void DriveSubsystem::Balance(){
	m_currentPitch = ahrs.GetPitch() + m_balanceCalibration.Get(); 
	std::cout << "the pitch is: " << m_currentPitch << endl;
	if (abs(m_currentPitch) >= 0.7){
	double Proportion = m_currentPitch/15.0;
	if (Proportion >= m_balanceMaxCalibration.Get()){
		Proportion = m_balanceMaxCalibration.Get();
	} else if (Proportion <= -m_balanceMaxCalibration.Get()){
		Proportion = -m_balanceMaxCalibration.Get();
	}
	std::cout << "The proportional value is: " << Proportion << endl;
	double motorSpeed = m_balanceMaxSpeed.Get() * Proportion;
	std::cout << "The motor is being set to: " << motorSpeed << endl;
	setMotorSpeed(motorSpeed, motorSpeed);
	}
}