#pragma config(Sensor, S4,     , sensorEV3_GenericI2C)

int deg= 320;
int dev;
int gyro;
int  Kp = 110;
int offset = 28; //light value
int offset_r= 34;
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


task turnoff(){
	waitUntil(getButtonPress(buttonRight)==1);
	stopAllTasks();
}


void stopp(){
	setMotorSpeed(motorB,0);
	setMotorSpeed(motorC,0);
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

void av_line2(){
	Tp=60;
	Kp=80;
	clearTimer(T1);
	while(time1[T1]<150) lf();
	while(getColorReflected(S2)>15) lf();
	//stopp();
	Tp=70;
}

void av_line3(){
	Tp=60;
	Kp=80;
	clearTimer(T1);
	while(time1[T1]<150) lf2();
	while(getColorReflected(S1)>15) lf2();
	//stopp();
	Tp=70;
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

	while(getColorReflected(S2)>15) slw_line();
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(300);

	av(30,40); //si hace falta avanzar para agarrarlo
	spin(90);
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(300);
	setMotorSpeed(motorD, -100);
	delay(1500);

	stopp();
	setMotorSpeed(motorD, 60);
	delay(900);
}



void put(){
	int dif= devpos[cdev]-curr_line;
	if(dif>0){
		av(-50,60);
		setMotorSpeed(motorB, 60);
		delay(500);
		while(getColorReflected(S1)>15) setMotorSpeed(motorB, 50);
		setMotorSpeed(motorB, 0);
		delay(300);

		av_line();
		setMotorSpeed(motorC, -50);
		delay(500);
		while(getColorReflected(S1)>15) setMotorSpeed(motorC, -50);
		setMotorSpeed(motorB, 0);
		delay(300);
	}
	else if(dif<0){
		av(50,60);
		setMotorSpeed(motorC, -50);
		delay(200);
		while(getColorReflected(S1)>15) setMotorSpeed(motorC, -50);
		delay(100);
		while(getColorReflected(S1)<15) setMotorSpeed(motorC, -50);
		delay(100);
		while(getColorReflected(S1)>15) setMotorSpeed(motorC, -50);
		setMotorSpeed(motorB, 0);
		delay(300);

		setMotorSpeed(motorC, -50);
		delay(500);
		while(getColorReflected(S1)>15) setMotorSpeed(motorC, -50);
		setMotorSpeed(motorB, 0);
		delay(300);
	}
	else{
		av(-50,60);
		setMotorSpeed(motorB, 60);
		delay(500);
		while(getColorReflected(S1)>15) setMotorSpeed(motorB, 50);
		setMotorSpeed(motorB, 0);
		delay(300);

		setMotorSpeed(motorC, -50);
		delay(500);
		while(getColorReflected(S1)>15) setMotorSpeed(motorC, -50);
		setMotorSpeed(motorB, 0);
		delay(300);
	}

	delay(200);
	int luz= getColorReflected(S2);
	while( (luz>15&&luz<40)||(luz>50) )lf();
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(300);
	av(0,40);
	spin(180);
	setMotorSpeed(motorD, -80);
	delay(800);
	setMotorSpeed(motorD, 30);
	delay(500);

	av(-100,70);
	while(getColorReflected(S2)>15) setMotorSpeed(motorB, -50);
	setMotorSpeed(motorB, 0);
	delay(300);
	spin(90);

	curr_line= devpos[cdev];
	cdev++;
}

void put_tono(int opc){
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
	}

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
	/*
	while(getColorReflected(S2)>15)setMotorSpeed(motorB, -30);
	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(100);
	while(getColorReflected(S2)<15)setMotorSpeed(motorB, -30);

	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	delay(300);



*/
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

task main(){
	//setMotorSpeed(motorA,-20);

	startTask(turnoff);
  setMotorReversed(motorC, true);

	resetGyro(S3);

	setMotorSpeed(motorD, 20);
	delay(500);
  stopp();
  av(100,70);
  stopp();

	while(getGyroDegrees(S3)>-90){
	setMotorSpeed(motorC, -70);
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
	av(300,70);
	while(getGyroDegrees(S3)>-70) setMotorSpeed(motorC, -70);
	while(getColorReflected(S1)>15) setMotorSpeed(motorC,-70);
	stopp();
	clearTimer(T1);
	while(time1[T1]<900){
		slw_line();
  }
	stopp();
	av(50,70);
	repeat(4){
		av_line2();
	}
   stopp();
   av(100,70);
   spin(0);
   stopp();

   setMotorSpeed(motorB, -80);
	 setMotorSpeed(motorC, -80);
   delay(1200);
   stopp();
   av(160,70);
   setMotorSpeed(motorD, -80);
	delay(800);
	setMotorSpeed(motorD, 30);
	delay(500);
	av(-60,60)
   spin(90);
   repeat(4){
   av_line3();
   }
   stopalltasks();
}
