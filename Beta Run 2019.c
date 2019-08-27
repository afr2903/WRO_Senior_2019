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
	delay(50);
}
void spin(int dg){
	while(getGyroDegrees(S3)<dg-1||getGyroDegrees(S3)>dg+1){
		gyro= getGyroDegrees(S3);
		string gyr= getGyroDegrees(S3);
		displayBigTextLine(5, gyr);
		setMotorSpeed(motorB, -(dg-gyro));
		setMotorSpeed(motorC, (dg-gyro));
	}
	stopp();
	resetGyro(S3);
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
	stopp();
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
			displayBigTextLine(1, "%d", j);
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
task spin_claw(){
	repeat(dev){
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

void regreso(){
	av(-100,100);
	while(getGyroDegrees(S3)<175)setMotorSpeed(motorB, -100);
	stopp();
	while(getColorReflected(S1)>15) slw_line_r();
	stopp();
	av(100, 100);
	spin(93);
}

void regreso_2(){
	av(-250,100);
	stopp();
	spin(90);
	clearTimer(T1);
	while(time1[T1]<2500){
		gyro= getGyroDegrees(S3);
		string gyr= getGyroDegrees(S3);
		displayBigTextLine(5, gyr);
		powerRight2= (90-(gyro*2))*1;
		powerLeft2= (90+(gyro*2))*1;
		setMotorSpeed(motorB, -powerRight2);
		setMotorSpeed(motorC, -powerLeft2);
	}
	stopp();
	resetGyro(S3);
	delay(300);
	av(200,100);
	spin(90);
	while(getColorReflected(S1)>15) slw_line_r();
}


void pick(int l){
	av_line();
	if(l==3)av(-100,100);

	while(getGyroDegrees(S3)<75) setMotorSpeed(motorB, -100);
	delay(50);
	while(getColorReflected(S1)>15) setMotorSpeed(motorB, -50);
	stopp();

	av(-150,100);
	while(getColorReflected(S2)>15) slw_line();
	stopp();
	//spin(90);
	//stopp();
	setMotorSpeed(motorD, -50);
	delay(1100);

	setMotorTarget(motorD, getMotorEncoder(motorD)+90, 50);
	waitUntilMotorStop(motorD);

	av(30,50);
	spin(90);
	setMotorSpeed(motorD, -100);
	delay(800);

	stopp();
	dev= cubes[cdev];
	startTask(spin_claw);
	setMotorSpeed(motorD, 10);
	delay(500);
	startTask(spin_claw);

	delay(1000);
	setMotorSpeed(motorD, 0);
	delay(50);
	setMotorSpeed(motorD, 60);
	delay(400);
}

void put(int opc){
	if(opc == 1){
		av(-100,100);
		spin(170);
		while(getColorReflected(S1)>15) setMotorSpeed(motorB,-50);
		stopp();

		while(true){
			displayBigTextLine(7, "color:%d", getColorName(S2));
			if(getColorName(S2)==5 || getColorName(S2)==2)break;
			slw_line();
		}
		stopp();

		//spin(11);
		av(-60,70);
		spin(11);
		setMotorSpeed(motorD, -30);
		delay(600);
		setMotorSpeed(motorD, -100);
		delay(500);
		setMotorSpeed(motorD, 35);
		delay(500);
		spin(0);
	}
	if(opc==2){
		while(getGyroDegrees(S3)>-170) setMotorSpeed(motorC, -100);
		delay(50);
		while(getColorReflected(S1)>15) setMotorSpeed(motorC, -60);
		while(true){
			if(getColorName(S2)==5 || getColorName(S2)==2)break;
			slw_line();
		}
		stopp();
		av(-60,70);
		spin(-180);
		setMotorSpeed(motorD, -40);
		delay(500);
		setMotorSpeed(motorD, -100);
		delay(500);
		setMotorSpeed(motorD, 35);
		delay(500);
		spin(0);
	}
	if(opc==3){
		while(getGyroDegrees(S3)>-170) setMotorSpeed(motorC, -100);
		delay(50);
		while(getColorReflected(S1)>15) setMotorSpeed(motorC, -60);
		while(true){
			if(getColorName(S2)==2 || getColorName(S2)==2)break;
			slw_line();
		}
		stopp();
		spin(180);
		av(-60,70);
		spin(0);
		setMotorSpeed(motorD, -40);
		delay(500);
		setMotorSpeed(motorD, -100);
		delay(500);
		setMotorSpeed(motorD, 35);
		delay(500);
		spin(0);
	}
}
void av_line2(){
	Tp=60;
	Kp=80;
	clearTimer(T1);
	while(time1[T1]<150) lf();
	while(getColorReflected(S2)>15) lf();
	stopp();
	Tp=70;
}

void av_line3(){
	Tp=60;
	Kp=80;
	clearTimer(T1);
	while(time1[T1]<150) lf2();
	while(getColorReflected(S1)>15) lf2();
	stopp();
	Tp=70;
}
void put_tono2(int opc){
	if(opc == 3){
		av(100,90);
		stopp();

		while(getGyroDegrees(S3)>-87){
			setMotorSpeed(motorC, -90);
		}
		stopp();
		spin(-90);
		stopp();

		clearTimer(T1);
		while(time1[T1]<1500){
			gyro= getGyroDegrees(S3);
			string gyr= getGyroDegrees(S3);
			displayBigTextLine(5, gyr);
			powerRight2= (80-(gyro*2))*1;
			powerLeft2= (80+(gyro*2))*1;

			setMotorSpeed(motorB, -powerRight2);
			setMotorSpeed(motorC, -powerLeft2);
		}
		stopp();
		resetGyro(S3);
		delay(500);
		av(300,70);
		while(getGyroDegrees(S3)>-70) setMotorSpeed(motorC, -70);
		while(getColorReflected(S1)>15) setMotorSpeed(motorC,-70);
		stopp();
		clearTimer(T1);
		while(time1[T1]<900)slw_line();
		stopp();
		spin(-90);
		av(50,70);
		repeat(4){
			av_line2();
		}
		stopp();
		av(100,70);
		spin(90);
		stopp();

		setMotorSpeed(motorB, -80);
		setMotorSpeed(motorC, -80);
		delay(1200);
		stopp();
		resetGyro(S3);
		delay(500);

		av(180,70);
		setMotorSpeed(motorD, -80);
		delay(800);
		setMotorSpeed(motorD, 30);
		delay(500);
		av(-60,60);
		spin(90);
		repeat(4)av_line3();
		av(60,60);
		spin(-90);
	}
	if(opc ==4){
		av(100,70);
		stopp();

		while(getGyroDegrees(S3)<90){
			setMotorSpeed(motorB, -70);
		}
		stopp();
		spin(90);
		stopp();

		clearTimer(T1);
		while(time1[T1]<1500){
			gyro= getGyroDegrees(S3);
			string gyr= getGyroDegrees(S3);
			displayBigTextLine(5, gyr);
			powerRight2= (80-(gyro*2))*1;
			powerLeft2= (80+(gyro*2))*1;

			setMotorSpeed(motorB, -powerRight2);
			setMotorSpeed(motorC, -powerLeft2);
		}
		stopp();
		resetGyro(S3);
		delay(500);
		av(300,70);


		while(getGyroDegrees(S3)<70) setMotorSpeed(motorB, -70);
		while(getColorReflected(S1)>15) setMotorSpeed(motorB,-70);
		stopp();
		clearTimer(T1);
		while(time1[T1]<1400){
			slw_line_r();
		}
		stopp();
		spin(90);
		av(50,70);
		repeat(4)av_line3();

		stopp();
		av(100,70);
		spin(-90);
		stopp();

		setMotorSpeed(motorB, -80);
		setMotorSpeed(motorC, -80);
		delay(1200);
		stopp();
		resetGyro(S3);
		delay(500);
		av(180,70);
		setMotorSpeed(motorD, -80);
		delay(800);
		setMotorSpeed(motorD, 30);
		delay(500);
	}
}


task turnoff(){
	waitUntil(getButtonPress(buttonRight)==1);
	stopAllTasks();
}

task main(){
	//READ CUBES.......................................................
	resetGyro(S3);
	delay(300);
	setMotorReversed(motorC, true);
	startTask(turnoff);
	setMotorSpeed(motorD, 20);
	delay(500);
	setSoundVolume(40);
	//setMotorSpeed(motorD, 20);

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


		if(dev==0){
			int opc= devpos[cdev]==l? 1 : devpos[cdev]<l? 2 : 3;
			pick(l);
			put(opc);
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
			if(cdev==2) break;
			regreso();
			cdev++;
			curr_line= curr_line+ (opc==1? -1: opc==2? -2 : 0);
			//break;
		}




	}
	regreso_2();
	//FIN PRUEBAS DEJA...............................
	//DEJAR CABLE 1..................................
	av_line();
	while(getColorReflected(S2)>15) setMotorSpeed(motorB, 40);
	stopp();
	while(getColorReflected(S1)>15) setMotorSpeed(motorC, 40);
	stopp();
	spin(0);
	av(-200, 50);
	setMotorSpeed(motorD, -100);
	delay(800);
	setMotorTarget(motorD, getMotorEncoder(motorD)+100, 50);
	waitUntilMotorStop(motorD);
	//while(getColorReflected(S1)>15) setMotorSpeed(motorB, 70);
	setMotorSpeed(motorB, 0);
	delay(100);
	clearTimer(T1);
	while(time1(T1)<1300) slw_line();
	stopp();
	spin(0);
	setMotorSpeed(motorD, 30);
	delay(1500);

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
	while(time1(T1)<600) slw_line();
	stopp();
	spin(0);
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
	while(time1(T1)<800) slw_line();
	stopp();
	spin(-90);
	while(getColorReflected(S2)>15) lf();
	stopp();
	spin(4);
	setMotorSpeed(motorB, -20);
	while(getColorReflected(S2)<15) setMotorSpeed(motorC, -20);
	stopp();
	av(30, 40);
	spin(5);

	setMotorSpeed(motorD, -30);
	delay(900);
	setMotorSpeed(motorD, 0);
	av(-250, 50);
	setMotorSpeed(motorD, 50);
	delay(1000);

	//DEJAR CABLE 2...................................................

	spin(-90);
	clearTimer(T1);
	while(time1[T1]<1600){
		gyro= getGyroDegrees(S3);
		string gyr= getGyroDegrees(S3);
		displayBigTextLine(5, gyr);
		powerRight2= (70-(gyro*2))*1;
		powerLeft2= (70+(gyro*2))*1;

		setMotorSpeed(motorB, -powerRight2);
		setMotorSpeed(motorC, -powerLeft2);
	}
	stopp();
	resetGyro(S3);
	delay(500);

	while(getGyroDegrees(S3)>-70) setMotorSpeed(motorB, 70);
	delay(50);
	while(getColorReflected(S1)>15) setMotorSpeed(motorB, 50);
	delay(50);
	while(getColorReflected(S1)<15) setMotorSpeed(motorB, 30);
	stopp();
	spin(-90);

	clearTimer(T1);
	while(time1[T1]<600) lf();
	while(getColorReflected(S2)>15) lf();
	stopp();

	while(getColorReflected(S1)>15) setMotorSpeed(motorC, 70);
	stopp();
	while(getColorReflected(S1)<15) setMotorSpeed(motorC, 70);
	stopp();
	while(getColorReflected(S1)>15) setMotorSpeed(motorB, 70);
	stopp();
	while(getColorReflected(S1)<15) setMotorSpeed(motorB, 70);
	stopp();
	av(-280, 40);
	setMotorSpeed(motorD, -100);
	delay(800);
	setMotorTarget(motorD, getMotorEncoder(motorD)+100, 50);
	waitUntilMotorStop(motorD);
	clearTimer(T1);
	while(time1(T1)<1200) slw_line();
	stopp();
	//spin(0);
	setMotorSpeed(motorD, 20);
	delay(1500);

	spin(180);
	av(100, 50);
	while(getColorReflected(S1)>15) setMotorSpeed(motorC, 40);
	stopp();
	while(getColorReflected(S2)>15) setMotorSpeed(motorB, 40);
	stopp();
	clearTimer(T1);
	while(time1[T1] < 800) lf_r();
	stopp();
	spin(0);
	Tp=60;
	Kp=90;
	repeat(4){
		clearTimer(T1);
		while(time1[T1] < 400) lf2();
		while(getColorReflected(S1)>20) lf2();
	}
	stopp();
	Kp = 110;
	Tp = 70;


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
	while(getColorReflected(S1)>15) setMotorSpeed(motorC, -20);
	stopp();

	setMotorSpeed(motorC, -50);
	delay(400);
	while(getColorReflected(S1)>15) setMotorSpeed(motorC, -60);
	stopp();
	clearTimer(T1);
	while(time1(T1)<900) slw_line();
	stopp();

	spin(-88);
	while(getColorReflected(S2)>15) lf();
	stopp();
	spin(-2);
	setMotorSpeed(motorB, -20);
	while(getColorReflected(S2)<15) setMotorSpeed(motorC, -20);
	stopp();
	av(30, 40);
	spin(5);

	setMotorSpeed(motorD, -30);
	delay(900);
	setMotorSpeed(motorD, 0);
	av(-300, 50);
	setMotorSpeed(motorD, 50);
	delay(1000);

	//REGRESAR A LINEA...........................................

	spin(-90);

	clearTimer(T1);
	while(time1[T1]<2000){
	gyro= getGyroDegrees(S3);
	string gyr= getGyroDegrees(S3);
	displayBigTextLine(5, gyr);
	powerRight2= (70-(gyro*2))*1;
	powerLeft2= (70+(gyro*2))*1;

	setMotorSpeed(motorB, -powerRight2);
	setMotorSpeed(motorC, -powerLeft2);
	}
	av(160,60);
	spin(90);
	repeat(5) av_line3();
  av(30,60);
  spin(-90);

  //DEVICES 2................................................
  cdev=3;
  curr_line=0;
  for(int l=1; l<=3; l++){
		for(;curr_line<l; curr_line++) {

			av_line();
		}
		setMotorSpeed(motorB, 0);
		setMotorSpeed(motorC, 0);
		delay(100);


		dev= SensorValue(S4);
		string dv= SensorValue(S4);
		displayBigTextLine(12,dv);
		delay(200);



		if(dev==0){
			pick(l);
			put_tono2(cdev);
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
			if(cdev==4) break;
			cdev++;
			curr_line= 0;
			//break;
		}
	}

	av(-100,70);
	spin(90);
	setMotorSpeed(motorB, 80);
	setMotorSpeed(motorC, 90);
	delay(2000);


	stopAllTasks();
}
