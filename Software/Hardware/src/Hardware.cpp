#include "Hardware.h"

#define BUZZ_PIN 13

Hardware::Hardware() {
	/// init IMU
	imu = new LSM9DS0(0x6B, 0x1D);
	uint16_t imuResult = imu->begin();

	/// init motors
	motors.standby(false);

	/// init adc
    adc_i2c = new mraa::I2c(1);
    adc = new ads1015(adc_i2c, 0x48);
    adc->setRange(_4_096V);

    /// init buzzer
    buzz = new upm::Buzzer(BUZZ_PIN);
}

TDVect Hardware::getGyro() {
	TDVect returnVect;
	imu->readGyro();
	returnVect.xComp = imu->calcGyro(imu->gx);
	returnVect.yComp = imu->calcGyro(imu->gy);
	returnVect.zComp = imu->calcGyro(imu->gz);
	return returnVect;
}

TDVect Hardware::getAccel() {
	TDVect returnVect;
	imu->readAccel();
	returnVect.xComp = imu->calcAccel(imu->ax);
	returnVect.yComp = imu->calcAccel(imu->ax);
	returnVect.zComp = imu->calcAccel(imu->ax);
	return returnVect;
}

TDVect Hardware::getCompass() {
	TDVect returnVect;
	imu->readMag();
	returnVect.xComp = imu->calcMag(imu->mx);
	returnVect.yComp = imu->calcMag(imu->my);
	returnVect.zComp = imu->calcMag(imu->mz);
	return returnVect;
}

void Hardware::setMotors(int motorOne, int motorTwo) {
	motors.shortBrake(motorOne==0,motorTwo==0);
	double scaleOne = ((double)motorOne)/100.0;
	double scaleTwo = ((double)motorTwo)/100.0;
	motors.diffDrive(scaleOne,scaleTwo);
}

void Hardware::chirp(double volume, int frequency, int time) {
	buzz->setVolume(volume);
	buzz->playSound(frequency,time);
}

int Hardware::getVis() {
	return adc->getRawResult(0);
}

int Hardware::getMicrophone() {
	return adc->getRawResult(1);
}

int Hardware::getIR() {
	return adc->getRawResult(2);
}

void Hardware::setLed(int r, int g, int b, int i) {
}

int Hardware::getTemperature() {
	imu->readTemp();
	return imu->temperature;
}

Hardware::~Hardware() {
	buzz->stopSound();
	motors.standby(true);
}

