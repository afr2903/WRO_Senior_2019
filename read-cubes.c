#pragma config(Sensor, S4,     colour,         sensorEV3_GenericI2C)
int colors[]={0,0,2,0,3,4,4,5,5,5,5,0,0,3,0,0,0,0,0};
int c;
bool vis[6], finish=false;
int cubes[5];
int pos[6];
//1E 2S 3W 4N
//RB 2 3 0 1
//G  1 2 3 0
//Y  3 0 1 2

int deg= 320;
int dev;
int gyro;
int  Kp = 110;
int offset_r = 34; //light value
int Tp = 70; //speed
int turn =0;
int powerRight=0;
int powerLeft=0;
int powerRight2=0;
int powerLeft2=0;
int error=0;
int light_value=0;

void av(int dg, int vl){
	int aux= dg>0? 1:-1;
	setMotorTarget(motorB, getMotorEncoder(motorB)+dg, vl);
	setMotorSpeed(motorC, vl*aux);
	waitUntilMotorStop(motorB);
	setMotorSpeed(motorC, 0);
	delay(100);
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

task read_cubes(){
	for(int j=1; j<=4; j++){
		while(true){
			string aux= j;
			displayBigTextLine(1, aux);
			c=SensorValue(colour);
			cubes[j]= colors[c];
			if(cubes[j]!=0 && vis[colors[c]]==0){
				vis[colors[c]]=true;
				break;
			}
		}
	}
	finish=true;
}
task turnoff(){
	waitUntil(getButtonPress(buttonRight)==1);
	stopAllTasks();
}

task main(){
	setMotorReversed(motorC, true);
	startTask(turnoff);
	setSoundVolume(40);
	//setMotorSpeed(motorD, 20);
	resetGyro(S3);

	setMotorSpeed(motorC, 50);
	while(getColorReflected(S2)>20) setMotorSpeed(motorB, 50);
	delay(200);
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	repeat(2){
		while(getColorReflected(S2)>20) setMotorSpeed(motorB, 40);
		delay(50);
		while(getColorReflected(S2)<20) setMotorSpeed(motorB, 40);
		setMotorSpeed(motorB, 0);
		delay(50);

		while(getColorReflected(S2)>20) setMotorSpeed(motorC, 40);
		delay(50);
		while(getColorReflected(S2)<20) setMotorSpeed(motorC, 40);
		setMotorSpeed(motorC, 0);
		delay(50);
	}
	delay(600);


	startTask(read_cubes);
	while(!finish&&getColorReflected(S1)>10) lf_r();
	delay(100);
	while(getColorReflected(S1)>10) lf_r();
	setMotorSpeed(motorB,0);
	setMotorSpeed(motorC,0);
	for(int j=1; j<=4; j++){
		if(cubes[j]==2) playSoundFile("Blue");
		if(cubes[j]==3) playSoundFile("Green");
		if(cubes[j]==4) playSoundFile("Yellow");
		if(cubes[j]==5) playSoundFile("Black");
		delay(500);
	}
	for(int j=1; j<=4; j++) pos[cubes[j]]=j;

	av(-50, 50);
	setMotorSpeed(motorB,40);
	setMotorSpeed(motorC,0);
	delay(500);
	while(getColorReflected(S2)>20) setMotorSpeed(motorB, 40);
	spin(-90);
	repeat(4){
		clearTimer(T1);
		while(time1[T1] < 500) lf_r();
		while(getColorReflected(S1)>20) lf_r();
	}
	setMotorSpeed(motorB,0);
	setMotorSpeed(motorC,0);
}
