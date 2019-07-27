#pragma config(Sensor, S4,     colour,         sensorEV3_GenericI2C)
int colors[]={0,0,2,0,3,4,4,5,5,5,0,0,0,3,0,0,0,0,0};
int c;
bool vis[6], finish=false;
int cubes[5];

task read_cubes(){
	for(int j=1; j<=4; j++){
		while(true){
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

void lf(){
	int  Kp = 200;
	int offset = 54; //light value
	int Tp = 70; //speed
	int turn =0;
	int powerRight=0;
	int powerLeft=0;
	int error=0;
	int light_value=0;
	light_value = getColorReflected(S2);
	error = light_value - offset;
	turn = Kp*error;
	turn=turn/100;
	powerRight= Tp-turn;
	powerLeft= Tp+20+turn;
	displayTextLine(3, "luz: %d",light_value);
	setMotorSpeed(motorB, powerRight);
	setMotorSpeed(motorC, powerLeft);
}

task main(){
	setMotorReversed(motorC, true);
	setSoundVolume(10);
	setMotorSpeed(motorC, 1);
	while(getColorReflected(S2)>20) setMotorSpeed(motorB, 50);
	setMotorSpeed(motorB, 0);
	while(getColorReflected(S2)<20) setMotorSpeed(motorC, 30);
	while(getColorReflected(S2)>20) setMotorSpeed(motorC, 30);
	setMotorSpeed(motorC, 0);
	startTask(read_cubes);
	while(!finish) lf();
	setMotorSpeed(motorB,0);
	setMotorSpeed(motorC,0);
	for(int j=1; j<=4; j++){
		if(cubes[j]==2) playSoundFile("Blue");
		if(cubes[j]==3) playSoundFile("Green");
		if(cubes[j]==4) playSoundFile("Yellow");
		if(cubes[j]==5) playSoundFile("Black");
		delay(500);
	}
	while(getColorReflected(S1)>20) lf();
	setMotorSpeed(motorB,30);
	setMotorSpeed(motorC,0);
	delay(500);
	while(getColorReflected(S2)>20) setMotorSpeed(motorB, 40);

	while(getColorReflected(S1)>20) lf();
	repeat(4){
		clearTimer(T1);
		while(time1[T1] < 500) lf();
		while(getColorReflected(S1)>20) lf();
	}
	setMotorSpeed(motorB,0);
	setMotorSpeed(motorC,0);
}
