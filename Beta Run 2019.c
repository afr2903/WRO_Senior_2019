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
int offset = 28;
int offset_r = 34; //light value
int Tp = 70; //speed
int turn =0;
int powerRight=0;
int powerLeft=0;
int powerRight2=0;
int powerLeft2=0;
int error=0;
int light_value=0;

int curr_line=0;
int cdev=1;
int devpos[]={0,1,3};

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
	powerLeft= (Tp-turn)*0.13;
	powerRight= (Tp+turn)*0.13;
	displayBigTextLine(9, "luz: %d",light_value);

	gyro= getGyroDegrees(S3);
	string gyr= getGyroDegrees(S3);
	displayBigTextLine(5, gyr);
	powerRight2= (70+(gyro*2))*0.47;
	powerLeft2= (70-(gyro*2))*0.47;

	setMotorSpeed(motorB, powerRight+powerRight2);
	setMotorSpeed(motorC, powerLeft+powerLeft2);
}
void lf2(){
	light_value= getColorReflected(S2);
	error= light_value - offset;
	turn= Kp*error;
	turn=turn/100;
	powerLeft= Tp-turn;
	powerRight= Tp+turn;
	//displayBigTextLine(1, "luz2: %d",light_value);
	setMotorSpeed(motorB, powerRight);
	setMotorSpeed(motorC, powerLeft);
}
void av_line(){
	clearTimer(T1);
	while(time1[T1]<200) lf_r();
	while(getColorReflected(S1)>15) lf_r();
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(200);
}

void slw_line(){
	if(getColorReflected(S1)<15){
		setMotorSpeed(motorB, 30);
		setMotorSpeed(motorC, 0);
	}
	if(getColorReflected(S1)>15){
		setMotorSpeed(motorC, 30);
		setMotorSpeed(motorB, 0);
	}
}

void slw_line_r(){
	if(getColorReflected(S2)>15){
		setMotorSpeed(motorB, 30);
		setMotorSpeed(motorC, 0);
	}
	if(getColorReflected(S2)<15){
		setMotorSpeed(motorC, 30);
		setMotorSpeed(motorB, 0);
	}
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

void regreso(){
	av(-100,60);
	int aux;
	aux=getColorReflected(S2);
	delay(400);
	while(getGyroDegrees(S3)<180)setMotorSpeed(motorB, -60);
	delay(50);
	stopp();
	while(getColorReflected(S1)>15) slw_line_r();
	stopp();
	av(100, 60);
	spin(90);
}

void regreso_2(){
	av(-250,60);
	stopp();
	spin(90);
	clearTimer(T1);
	while(time1[T1]<3000){
		gyro= getGyroDegrees(S3);
		string gyr= getGyroDegrees(S3);
		displayBigTextLine(5, gyr);
		powerRight2= (70-(gyro*2))*1;
		powerLeft2= (70+(gyro*2))*1;
		setMotorSpeed(motorB, -powerRight2);
		setMotorSpeed(motorC, -powerLeft2);
	}
	stopp();
	av(170,60);
	spin(90);
	while(getColorReflected(S1)>15) slw_line_r();
}


void pick(int l){
	av_line();
	if(l==3)av(-100,60);



	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(200);

	while(getGyroDegrees(S3)<85){
		setMotorSpeed(motorB, -60);
	}
	setMotorSpeed(motorB, 0);
	delay(200);

	av(-150,60);
	while(getColorReflected(S2)>15) slw_line();
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(300);


	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(300);
	//spin(90);
	//av(160,20); //si hace falta avanzar para agarrarlo
	//spin(0);
	//av(-140,40);
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(300);

	setMotorSpeed(motorD, -50);
	delay(1300);

	stopp();
	setMotorTarget(motorD, getMotorEncoder(motorD)+90, 50);
	waitUntilMotorStop(motorD);
	//delay(200);

	av(30,50);
	spin(90);
	setMotorSpeed(motorD, -100);
	delay(800);

	stopp();
	setMotorSpeed(motorD, 10);
	delay(1500);
	setMotorSpeed(motorD, 0);
	delay(50);
	setMotorSpeed(motorD, 60);
	delay(400);

	repeat(cubes[cdev]){
		setMotorSpeed(motorA,100);
		delay(300);
		setMotorSpeed(motorA,0);
		delay(100);
		setMotorSpeed(motorA,-100);
		delay(300);
		setMotorSpeed(motorA,0);
		delay(100);

	}

}

void put(int opc){
	if(opc == 1){
		av(-100,70);
		spin(170);

		while(true){
			displayBigTextLine(7, "color:%d", getColorName(S2));
			if(getColorName(S2)==5 || getColorName(S2)==2)break;
			slw_line();
		}

		setMotorSpeed(motorB, 0);
		setMotorSpeed(motorC, 0);
		delay(1000);


		setMotorSpeed(motorB, 0);
		setMotorSpeed(motorC, 0);
		delay(300);
		spin(10);
		av(-60,60);
		spin(0);
		setMotorSpeed(motorD, -40);
		delay(500);
		setMotorSpeed(motorD, -100);
		delay(500);
		setMotorSpeed(motorD, 25);
		delay(1000);
		spin(0);
	}
	if(opc==2){
		while(getGyroDegrees(S3)>-180)setMotorSpeed(motorC, -60);
		while(true){
			if(getColorName(S2)==5 || getColorName(S2)==2)break;
			slw_line();
		}
		setMotorSpeed(motorB, 0);
		setMotorSpeed(motorC, 0);
		delay(300);
		av(-20,60);
		spin(-180);
		setMotorSpeed(motorD, -40);
		delay(500);
		setMotorSpeed(motorD, -100);
		delay(500);
		setMotorSpeed(motorD, 80);
		delay(900);
		spin(0);
	}
	if(opc==3){
		while(getGyroDegrees(S3)<160)setMotorSpeed(motorB, -60);
		delay(50);
		while(getColorReflected(S1)>15)setMotorSpeed(motorB, -40);
		while(true){
			if(getColorName(S2)==2 || getColorName(S2)==2)break;
			slw_line();
		}
		stopp();
		spin(180);
		av(-60,60);
		spin(0);
		setMotorSpeed(motorD, -40);
		delay(500);
		setMotorSpeed(motorD, -100);
		delay(500);
		setMotorSpeed(motorD, 80);
		delay(900);
		spin(0);
	}

}


task turnoff(){
	waitUntil(getButtonPress(buttonRight)==1);
	stopAllTasks();
}

task main(){
	//READ CUBES.......................................................

	setMotorReversed(motorC, true);
	startTask(turnoff);
	setMotorSpeed(motorD, 20);
	delay(500);
	setSoundVolume(40);
	//setMotorSpeed(motorD, 20);
	resetGyro(S3);

	setMotorSpeed(motorC, 50);
	while(getColorReflected(S2)>20) setMotorSpeed(motorB, 50);
	delay(200);
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	repeat(1){
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
	spin(0);
	//av(-200, 50);


	startTask(read_cubes);
	while(!finish&&getColorReflected(S1)>10) lf_r();
	stopp();
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
	pos[2]= (pos[2]+1)%4;
	pos[3]= (pos[3]+0)%4;
	pos[4]= (pos[4]+2)%4;
	pos[5]= (pos[5]+1)%4;

	cubes[1]=pos[5];
	for(int j=2; j<=4; j++) cubes[j]=pos[j];

	av(-100, 50);
	setMotorSpeed(motorB,40);
	setMotorSpeed(motorC,0);
	delay(500);
	while(getColorReflected(S2)>20) setMotorSpeed(motorB, 40);
	spin(-90);

	clearTimer(T1);
	while(time1[T1] < 500) lf_r();
	stopp();
	Tp=60;
	Kp=80;
	repeat(4){
		clearTimer(T1);
		while(time1[T1] < 300) lf2();
		while(getColorReflected(S1)>20) lf2();
	}
	Kp = 110;
	Tp = 70;


	setMotorSpeed(motorB,0);
	setMotorSpeed(motorC,0);
	//FIN programa read cube.......................
	//.............................................
	//Programa pruebas deja........................
	resetGyro(S3);
	av(85, 60);
	spin(-90);

	for(int l=1; l<=3; l++){
		for(;curr_line<l; curr_line++) {
			av_line();
		}
		setMotorSpeed(motorB, 0);
		setMotorSpeed(motorC, 0);
		delay(100);

		clearTimer(T1);
		while(time1[T1]<50){
			gyro= getGyroDegrees(S3);
			string gyr= getGyroDegrees(S3);
			displayBigTextLine(5, gyr);
			powerRight2= (70+(gyro*2))*1;
			powerLeft2= (70-(gyro*2))*1;

			setMotorSpeed(motorB, +powerRight2);
			setMotorSpeed(motorC, +powerLeft2);
		}
		stopp();
		dev= SensorValue(S4);
		string dv= SensorValue(S4);
		displayBigTextLine(12,dv);
		delay(200);
		srand(nSysTime);//borrar es para random


		if(dev==0){
			int opc= devpos[cdev]==l? 1 : devpos[cdev]<l? 2 : 3;
			pick(l);
			put(opc);
			if(cdev==2) break;
			regreso();
			repeat((4-cubes[cdev])%4){
				setMotorSpeed(motorA,100);
				delay(300);
				setMotorSpeed(motorA,0);
				delay(100);
				setMotorSpeed(motorA,-100);
				delay(300);
				setMotorSpeed(motorA,0);
				delay(100);

			}
			cdev++;
			curr_line= curr_line+ (opc==1? -1: opc==2? -2 : 0);
			//break;
		}




	}
	regreso_2();
	stopAllTasks();
	//FIN PRUEBAS DEJA...............................
}
