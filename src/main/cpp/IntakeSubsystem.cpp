#include "IntakeSubsystem.h"

using namespace CORE;

IntakeSubsystem::IntakeSubsystem() :
                                    intakeSpeed("Intake Speed", 0.70),
                                    m_leftIntakeMotor(LEFT_INTAKE),
                                    m_rightIntakeMotor(RIGHT_INTAKE){
}

void IntakeSubsystem::RobotInit(){
    m_rightIntakeMotor.Set(ControlMode::PercentOutput, 0);
    m_leftIntakeMotor.Set(ControlMode::PercentOutput, 0);

}


void IntakeSubsystem::TeleopInit() {
}

void IntakeSubsystem::Teleop(){
    if(operatorJoystick->GetButton(CORE::COREJoystick::JoystickButton::RIGHT_TRIGGER)) {
       SetIntake(-intakeSpeed.Get());
     } else if (operatorJoystick->GetButton(CORE::COREJoystick::JoystickButton::RIGHT_BUTTON)) {
        SetIntake(intakeSpeed.Get());
        } else{
        SetIntake(0.0);
    }
    
}

void IntakeSubsystem::SetIntake(double intakeSpeed) {
    m_leftIntakeMotor.Set(ControlMode::PercentOutput, -intakeSpeed);
    m_rightIntakeMotor.Set(ControlMode::PercentOutput, intakeSpeed);
}