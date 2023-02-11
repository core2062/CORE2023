#include "DriveSubsystem.h"

DriveSubsystem::DriveSubsystem() :
		ahrs(SerialPort::kUSB),
		m_analogPressureInput(0),
		m_analogSupplyVoltage(1),
		m_leftPrimary(LEFT_FRONT_PORT),
		m_rightPrimary(RIGHT_FRONT_PORT),
		m_leftSecondary(LEFT_BACK_PORT),
		m_rightSecondary(RIGHT_BACK_PORT),
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
	// m_compressor.EnableDigital();
	SmartDashboard::PutString("Drive Controls", " Forward/Back: Left Stick \n Right/Left: Right Stick \n Shift: Right Trigger");
}

void DriveSubsystem::teleop() {
	// Code for teleop. Sets motor speed based on the values for the joystick, runs compressor,
	// Toggles gears
    double rot = -driverJoystick->GetAxis(CORE::COREJoystick::JoystickAxis::RIGHT_STICK_X);
	double mag = driverJoystick->GetAxis(CORE::COREJoystick::JoystickAxis::LEFT_STICK_Y);

	
	VelocityPair speeds = COREEtherDrive::Calculate(mag, rot, .1);
	setMotorSpeed(speeds.left, speeds.right);
	SmartDashboard::PutNumber("Left side speed", speeds.left);
	SmartDashboard::PutNumber("Right side speed", speeds.right);
	SmartDashboard::PutNumber("Left side encoder", m_leftPrimary.GetSelectedSensorPosition(0));
	SmartDashboard::PutNumber("Right side encoder", m_rightPrimary.GetSelectedSensorPosition(0));

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
		m_rightPrimary.Set(ControlMode::PercentOutput, speedInFraction);
		m_rightSecondary.Set(ControlMode::PercentOutput, speedInFraction);
	}
	if (whichSide == DriveSide::BOTH || whichSide == DriveSide::LEFT) {
		m_leftPrimary.Set(ControlMode::PercentOutput, speedInFraction);
		m_leftSecondary.Set(ControlMode::PercentOutput, speedInFraction);
	}
}

void DriveSubsystem::setMotorSpeed(double leftPercent, double rightPercent) {
	// Sets speed based on percent output desired
	setMotorSpeed(leftPercent, DriveSide::LEFT);
	setMotorSpeed(rightPercent, DriveSide::RIGHT);
}

double DriveSubsystem::getRobotPosition() {
	return m_rightPrimary.GetSelectedSensorPosition(0);
}

void DriveSubsystem::InitTalons() {
	// Sets up talons
	m_leftPrimary.Set(ControlMode::PercentOutput, 0);
	m_leftSecondary.Set(ControlMode::PercentOutput, 0);
	m_rightPrimary.Set(ControlMode::PercentOutput, 0);
	m_rightSecondary.Set(ControlMode::PercentOutput, 0);

	// Encoder Functions
    m_leftPrimary.SetStatusFramePeriod(StatusFrameEnhanced::Status_1_General, 10, 0);
    m_rightPrimary.SetStatusFramePeriod(StatusFrameEnhanced::Status_1_General, 10, 0);

    m_leftPrimary.ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::IntegratedSensor, 0, 0);
    m_rightPrimary.ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::IntegratedSensor, 0, 0);

	m_leftPrimary.SetSelectedSensorPosition(0.0);
	m_rightPrimary.SetSelectedSensorPosition(0.0);
	
	SetTalonMode(NeutralMode::Coast);

	m_leftPrimary.SetSensorPhase(false);
    m_rightPrimary.SetSensorPhase(true);

	// Motor Inversion
	m_leftPrimary.SetInverted(false);
	m_leftSecondary.SetInverted(false);
	m_rightPrimary.SetInverted(true);
	m_rightSecondary.SetInverted(true);
 
	SetTalonMode(NeutralMode::Coast);
}


void DriveSubsystem::resetEncoder(){
	cout << "Reset encoder error value" <<  m_rightPrimary.SetSelectedSensorPosition(0.0, 0, 25);
}

void DriveSubsystem::SetTalonMode(NeutralMode mode){
	m_rightPrimary.SetNeutralMode(mode);
	m_rightSecondary.SetNeutralMode(mode);
	m_leftPrimary.SetNeutralMode(mode);
	m_leftSecondary.SetNeutralMode(mode);
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