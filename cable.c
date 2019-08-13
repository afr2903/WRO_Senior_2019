#pragma config(Sensor, S4,     , sensorEV3_GenericI2C)
int gyro;
int  Kp = 110;
int offset = 25; //light value
int offset_r= 31;
int Tp = 70; //speed
int turn =0;
int powerRight=0;
int powerLeft=0;
int powerRight2=0;
int powerLeft2=0;
int error=0;
int light_value=0;


task turnoff(){
	waitUntil(getButtonPress(buttonRight)==1);
	stopAllTasks();
}
void stopp(){
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(100);
}

void av(int dg, int vl){
	int aux= dg>0? 1:-1;
	setMotorTarget(motorB, getMotorEncoder(motorB)+dg, vl);
	setMotorSpeed(motorC, vl*aux);
	waitUntilMotorStop(motorB);
	setMotorSpeed(motorC, 0);
	delay(100);
}
void lf(){
	light_value= getColorReflected(S1);
	error= light_value - offset;
	turn= Kp*error;
	turn=turn/100;
	powerRight= Tp-turn;
	powerLeft= Tp+turn;
	displayBigTextLine(1, "luz: %d",light_value);
	setMotorSpeed(motorB, powerRight);
	setMotorSpeed(motorC, powerLeft);
}
void lf_slw(){
	if(getColorReflected(S1)<15){
		setMotorSpeed(motorB, 30);
		setMotorSpeed(motorC, 0);
	}else{
		setMotorSpeed(motorB, 0);
		setMotorSpeed(motorC, 30);
	}
}
void lf_r(){
	light_value= getColorReflected(S2);
	error= light_value - offset_r;
	turn= Kp*error;
	turn=turn/100;
	powerLeft= (Tp-turn)*0.16;
	powerRight= (Tp+turn)*0.16;
	displayBigTextLine(9, "luz: %d",light_value);

	gyro= getGyroDegrees(S3);
	string gyr= getGyroDegrees(S3);
	displayBigTextLine(5, gyr);
	powerRight2= (70+(gyro*2))*0.44;
	powerLeft2= (70-(gyro*2))*0.44;

	setMotorSpeed(motorB, powerRight+powerRight2);
	setMotorSpeed(motorC, powerLeft+powerLeft2);
}
void spin(int dg){
	while(getGyroDegrees(S3)<dg-1||getGyroDegrees(S3)>dg+1){

		gyro= getGyroDegrees(S3);
		string gyr= getGyroDegrees(S3);
		displayBigTextLine(5, gyr);
		setMotorSpeed(motorB, -(dg-gyro));
		setMotorSpeed(motorC, (dg-gyro));
	}
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	resetGyro(S3);
	delay(300);
}
void av_line(){
	clearTimer(T1);
	while(time1[T1]<200) lf_r();
	while(getColorReflected(S1)>15) lf_r();
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(200);
}




task main(){
	startTask(turnoff);
	resetGyro(S3);
	setMotorSpeed(motorA, -10);
	setMotorSpeed(motorD, 20);
	delay(500);
	setMotorReversed(motorC, true);
	av_line();
	setMotorSpeed(motorD, -100);
	delay(800);
	setMotorTarget(motorD, getMotorEncoder(motorD)+100, 50);
	waitUntilMotorStop(motorD);
	while(getColorReflected(S1)>15) setMotorSpeed(motorB, 70);
	setMotorSpeed(motorB, 0);
	delay(100);
	clearTimer(T1);
	while(time1(T1)<1200) lf_slw();
	stopp();
	spin(0);
	setMotorSpeed(motorD, 30);
	delay(1000);

	spin(180);
	while(getColorReflected(S1)>15)setMotorSpeed(motorB, 50);
	delay(50);
	while(getColorReflected(S1)<15)setMotorSpeed(motorB, 50);
	setMotorSpeed(motorB, 0);
	delay(200);
	while(getColorReflected(S1)>15)setMotorSpeed(motorC, 50);
	setMotorSpeed(motorC, 0);
	delay(400);

	clearTimer(T1);
	while(time1(T1)<500) lf();
	stopp();
	while(getColorReflected(S2)>15) lf();
	stopp();
	//setMotorSpeed(motorB, -20);
	//while(getColorReflected(S2)>15) setMotorSpeed(motorC, -20);
	//stopp();

	setMotorSpeed(motorC, 50);
	delay(400);
	while(getColorReflected(S2)>15) setMotorSpeed(motorC, 50);
	delay(50);
	while(getColorReflected(S2)<15) setMotorSpeed(motorC, 50);
	setMotorSpeed(motorC, 0);
	delay(200);
	spin(90);

	while(getColorReflected(S1)>15) lf_r();
	delay(50);
	av_line();
	av_line();
	setMotorSpeed(motorB, -20);
	while(getColorReflected(S2)>15) setMotorSpeed(motorC, -20);
	stopp();

	setMotorSpeed(motorC, -50);
	delay(400);
	while(getColorReflected(S1)>15) setMotorSpeed(motorC, -60);
	stopp();
	clearTimer(T1);
	while(time1(T1)<800) lf_slw();
	stopp();
	spin(-87);
	while(getColorReflected(S2)>15) lf();
	stopp();
	spin(4);
	setMotorSpeed(motorB, -20);
	while(getColorReflected(S2)<15) setMotorSpeed(motorC, -20);
	stopp();
	av(30, 40);
	spin(0);

	setMotorSpeed(motorD, -30);
	delay(900);
	setMotorSpeed(motorD, 0);
	av(-250, 50);
	setMotorSpeed(motorD, 50);
	delay(1000);
	spin(0);






	while(getColorReflected(S2)>15) setMotorSpeed(motorB, -50);
	delay(50);
	while(getColorReflected(S2)<15) setMotorSpeed(motorB, -40);
	stopp();
	spin(90);
	av_line();
	av_line();

	setMotorSpeed(motorC, 50);
	delay(300);
	while(getColorReflected(S1)>15) setMotorSpeed(motorC, 50);
	stopp();

	while(getColorReflected(S2)>15) lf();
	stopp();
	setMotorSpeed(motorD, -100);
	delay(800);
	setMotorTarget(motorD, getMotorEncoder(motorD)+100, 50);
	waitUntilMotorStop(motorD);
	setMotorSpeed(motorC, -30);
	delay(100);
	stopp();
	clearTimer(T1);
	while(time1(T1)<1200) lf_slw();
	stopp();
	spin(90);
	setMotorSpeed(motorD, 30);
	delay(1000);

	spin(180);
	while(getColorReflected(S2)>15) setMotorSpeed(motorB, 50);
	delay(50);
	stopp();
	repeat(5) av_line();
	setMotorSpeed(motorB, 40);
	delay(400);
	repeat(2){
		while(getColorReflected(S2)>15) setMotorSpeed(motorB,40);
		delay(50);
		while(getColorReflected(S2)<15) setMotorSpeed(motorB,40);
		stopp();
		while(getColorReflected(S2)>15) setMotorSpeed(motorC,40);
		stopp();
	}

	spin(-90);
	av_line();
	av_line();
	setMotorSpeed(motorB, -20);
	while(getColorReflected(S2)>15) setMotorSpeed(motorC, -20);
	stopp();

	setMotorSpeed(motorC, -50);
	delay(400);
	while(getColorReflected(S1)>15) setMotorSpeed(motorC, -60);
	stopp();
	clearTimer(T1);
	while(time1(T1)<900) lf_slw();
	stopp();

	spin(-93);
	while(getColorReflected(S2)>15) lf();
	stopp();
	spin(4);
	setMotorSpeed(motorB, -20);
	while(getColorReflected(S2)<15) setMotorSpeed(motorC, -20);
	stopp();
	av(30, 40);
	spin(0);

	setMotorSpeed(motorD, -30);
	delay(900);
	setMotorSpeed(motorD, 0);
	av(-300, 50);
	setMotorSpeed(motorD, 50);
	delay(1000);

	stopAllTasks();
}
