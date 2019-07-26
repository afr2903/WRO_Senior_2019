#pragma config(Sensor, S4,     , sensorEV3_GenericI2C)

int deg= 320;
int dev;
int gyro;
int  Kp = 100;
int offset = 28; //light value
int offset_r= 39;
int offset_l= 28;
int Tp = 80; //speed
int turn =0;
int powerRight=0;
int powerLeft=0;
int error=0;
int light_value=0;

int curr_line=0;
int cdev=1;
int devpos[]={0,1,3};


task turnoff(){
	waitUntil(getButtonPress(buttonRight)==1);
	stopAllTasks();
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
void lf_r(){
	light_value= getColorReflected(S2);
	error= light_value - offset_r;
	turn= Kp*error;
	turn=turn/100;
	powerRight= Tp-turn;
	powerLeft= Tp+turn;
	displayBigTextLine(9, "luz: %d",light_value);
	setMotorSpeed(motorB, powerRight);
	setMotorSpeed(motorC, powerLeft);
}
void spin(int dg){
	while(getGyroDegrees(S3)<dg-2||getGyroDegrees(S3)>dg+2){
		gyro= getGyroDegrees(S3);
		string gyr= getGyroDegrees(S3);
		displayBigTextLine(5, gyr);
		setMotorSpeed(motorB, -(dg-gyro));
		setMotorSpeed(motorC, (dg-gyro));
	}
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	resetGyro(S3);
	delay(100);
}

void av_line(){
	clearTimer(T1);
	while(time1[T1]<200) lf_r();
	while(getColorReflected(S1)>15) lf_r();
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(200);
}
void rev_line(){
	clearTimer(T1);
	while(time1[T1]<200) lf();
	while(getColorReflected(S2)>15) lf();
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(200);
}
void pick(){
	av(100, 60);
	delay(300);
	spin(90);
	setMotorSpeed(motorD, -100);
	delay(800);
	setMotorSpeed(motorD, 30);
	delay(500);
}
void put(){
	av(0, 50);
	int dif= devpos[cdev]-curr_line;
	if(dif>0){
		spin(-90);
		av_line();
		av(120,50);
		spin(-90);
	}
	else if(dif<0){
		spin(90);
		rev_line();
		av(120,50);
		spin(90);
	}
	else spin(180);

	delay(200);
	while(getColorReflected(S2)>15)lf();
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(300);
	av(0,40);
	spin(0);
	setMotorSpeed(motorD, -80);
	delay(800);
	setMotorSpeed(motorD, 30);
	delay(500);

	av(-100,70);
	spin(180);
	while(getColorReflected(S2)>15)lf();
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(300);
	av(50,50);
	spin(-90);

	curr_line= devpos[cdev];
	cdev++;
}


task main(){
	startTask(turnoff);
	setMotorReversed(motorC, true);
	resetGyro(S3);
	setMotorSpeed(motorD, 20);
	delay(500);
	while(getColorReflected(S1)>15) lf_r();
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(100);
	spin(0);
	av(-30, 60);
	spin(-90);

	for(int l=1; l<=3; l++){
		for(;curr_line<l; curr_line++) av_line();
		av(20, 60);
		dev= SensorValue(S4);
		string dv= SensorValue(S4);
		displayBigTextLine(12,dv);
		delay(200);
		if(dev==0){
			pick();
			put();
		}
	}

}
