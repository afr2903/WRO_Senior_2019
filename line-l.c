int  Kp = 100;
int offset = 28; //light value
int Tp = 80; //speed
int turn =0;
int powerRight=0;
int powerLeft=0;
int error=0;
int light_value=0;
//2 48

void lf(){
	light_value= getColorReflected(S3);
	error= light_value - offset;
	turn= Kp*error;
	turn=turn/100;
	powerRight= Tp-turn;
	powerLeft= Tp+turn;
	displayBigTextLine(3, "luz: %d",light_value);
	setMotorSpeed(motorB, powerRight);
	setMotorSpeed(motorC, powerLeft);
}
task main(){
	setMotorReversed(motorC, true);
	while(true) lf();
}
