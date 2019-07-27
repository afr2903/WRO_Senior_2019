int gyro;
task main(){
	setMotorReversed(motorC, true);
	resetGyro(S3);

	while(getGyroDegrees(S3)<88||getGyroDegrees(S3)>92){
		gyro= getGyroDegrees(S3);
		setMotorSpeed(motorB, -(90-gyro));
		setMotorSpeed(motorC, 90-gyro);
	}

}
