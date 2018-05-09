void win()
{
    
}

void ms3()
{
      while(analog(1) < 3500 && get_create_lbump() == 0)
    {
        if(analog(2) < 3500)
        {
            create_drive_direct(150,200);
        }
        else
        {
            create_drive_direct(200,150);
        }
    }
    return;
}

void slowclose(int first, int last, int speed,int port)
{
    set_servo_position(port,first);
    msleep(100);
    int current;
    if(first < last)
    {
    for(current = first;current < last;current++)
    {
        
        set_servo_position(port,current);
        msleep(speed);
    }
    }
    else
    {
        for(current = first;current > last;current-=1)
    {
        
        set_servo_position(port,current);
        msleep(speed);
    }
    }    
}
void ms()
{
    set_servo_position(3,1730);
    //slowclose(1200,1740,1,2);
}
void smoothturn(int degrees,int basestspeed, int maxspeed){
int speed;
    set_create_total_angle(0);
    msleep(10);

    if(degrees<0) //want to go to left
    {
        int angle = degrees * -1; //changes the number to positive which is left for the create
        int point1 = angle/5;
        int point2 = angle - point1;
        
            while(get_create_total_angle()<point1)
        {
                speed = basestspeed + (maxspeed - basestspeed) * (double) get_create_total_angle()/point1;
            create_drive_direct(-speed,speed);
        }
        
        while(get_create_total_angle()<point2)
        {
            create_drive_direct(-maxspeed,maxspeed);
        }
         while(get_create_total_angle()<angle)
        {
             speed = maxspeed - (maxspeed - basestspeed) * (double) (get_create_total_angle() - point2) / point1 ;
            create_drive_direct(-speed,speed);
        }
        create_stop();
    }
    else //wants to go right
    {
        int angle = degrees * -1; //changes the number to negative which is right for the create
         int point1 = angle/5;
        int point2 = angle - point1;
        while(get_create_total_angle()>point1)
        {
                speed = basestspeed + (maxspeed - basestspeed) * (double) get_create_total_angle()/point1;
            create_drive_direct(speed,-speed);
        }
        while(get_create_total_angle()>point2)
        {
            create_drive_direct(maxspeed,-maxspeed);
        }
          while(get_create_total_angle()>angle)
        {
             speed = maxspeed - (maxspeed - basestspeed) * (double) (get_create_total_angle() - point2) / point1;
            create_drive_direct(speed,-speed);
        }
        create_stop();
    }




}
void cleanUp ( int length, int range, double decay)
{
    int ns = length;
    double data[ns];
    double clean[ns];
    clock_t t4 = clock();
    clock_t t3;
	int iter;
    int tin = (int)length * 25 / 28;
    for(iter = 0;iter < ns;iter++)
    {
     data[iter] = analog(0);


        t3 = clock();

        while((double)(t3-t4) / CLOCKS_PER_SEC < (tin *0.001) - 0.000031) //-0.000031 is timing calibration value
        {

             t3 = clock();
		create_drive_direct(-20,20);
            msleep(1);
        }
         t4 = clock();
    }
     create_stop();
printf("Data: %d",(int)data[1]);
  // calculate coefficients
  double coefficients[range+1];
  int i;
  for (i = 0; i <= range; i++)
  {
    coefficients[i] = pow((double)decay, (double)i);
      printf("\nCoeff:%f",coefficients[i]);
  }
printf("Coeffs calcd");
  // calculate the divisor
  double divisor = 0;
  for (i = -range; i <= range; i++)
  {
    divisor += coefficients[abs(i)];
  }
printf("Divisor: %f",divisor);
  // clean data
  for (i = range; i < length - range; i++)
  {
    double temp = 0;
    int j;
    for (j = -range; j <= range; j++)
    {
      temp += data[i + j] * coefficients[abs(j)];
    }
    clean[i] = temp / divisor;
      printf("Clean: %f\n",clean[i]);
  }

  // find leading and trailing slopes
  double leadSum = 0;
  double trailSum = 0;
  int leadRef = range;
  int trailRef = length - range - 1;
  for (i = 1; i <= range; i++)
  {
    leadSum += (clean[leadRef] - clean[leadRef + i]) / i;
    trailSum += (clean[trailRef] - clean[trailRef - i]) / i;
  }

  //clean edges
  double leadSlope = leadSum / range;
  double trailSlope = trailSum / range;
  for (i = 1; i <= range; i++)
  {
    clean[leadRef - i] = clean[leadRef] + leadSlope * i;
    clean[trailRef + i] = clean[trailRef] + trailSlope * i;
  }
    for(i=0;i<length;i++)
    {
     printf("Cleaned Data: %f\n", clean[i]);
    }
    int pos = 7;
		    	int x;
		    	for(x = 8;x < length-5;x++)
		    	{
		    		if(clean[x] < clean[pos])
		    		{
		    			pos = x;
		    		}
		    	}
		    	ns = length - pos;


    t4 = clock();
	printf("Ns: %d",ns);
    for(iter = 0;iter < ns ;iter++)
    {


        t3 = clock();
        printf("5");

        while((double)(t3-t4) / CLOCKS_PER_SEC < (tin *0.001) - 0.000031) //-0.000031 is timing calibration value
        {
             t3 = clock();
create_drive_direct(20,-20);

            msleep(1);
        }
         t4 = clock();
    }
     create_stop();


}
void lock_Motors(){
 mav(0,0);
    mav(1,0);
}
void move_Motors(int distance, int speed){
	int posi = gmpc(0);
	int pos2 = posi+distance;

	if (distance < 0) {
	while(posi>pos2)
	{
    		posi = gmpc(0);
		if (gmpc(0)<gmpc(1)) {
			mav(0, speed - 20);
			mav(1, speed);
    			msleep(1);}
		else if(gmpc(1) < gmpc(0)){
			mav(0, speed);
    			mav(1, speed - 20);
    			msleep(1);}
   		else {
			mav(1, speed);
    			mav(0, speed);
    			msleep(1);} }}
	else {
		while(posi<pos2)
		{
    			posi = gmpc(0);
			if (gmpc(0)>gmpc(1)) {
				mav(0, speed - 20);
				mav(1, speed);
    				msleep(1);}
			else if(gmpc(1) > gmpc(0)){
				mav(0, speed);
    				mav(1, speed - 20);
    				msleep(1);}
   			else {
				mav(1, speed);
    			mav(0, speed);
    			msleep(1);} }}

        freeze(0);
        freeze(1);

}
void max_Motors(int speed){
	
		while(digital(4)==0)
		{
    			//posi = gmpc(http://www.kipr.org/0);
			if (gmpc(0)>gmpc(1)) {
				mav(0, speed - 20);
				mav(1, speed);
    				msleep(1);}
			else if(gmpc(1) > gmpc(0)){
				mav(0, speed);
    				mav(1, speed - 20);
    				msleep(1);}
   			else {
				mav(1, speed);
    			mav(0, speed);
    			msleep(1);} }

        freeze(0);
        freeze(1);

}


void forward(int inches)
{
    set_create_distance(0);
    msleep(10);
    int distance = inches * 23;
    while(get_create_distance()<distance)
    {
        create_drive_direct(200,200);
    }
    create_stop();
}

void turn(int degrees)
{
    set_create_total_angle(0);
    msleep(10);

    if(degrees<0) //want to go to left
    {
        int angle = degrees * -1; //changes the number to positive which is left for the create
        while(get_create_total_angle()<angle)
        {
            create_drive_direct(-100,100);
        }
        create_stop();
    }
    else //wants to go right
    {
        int angle = degrees * -1; //changes the number to negative which is right for the create
        while(get_create_total_angle()>angle)
        {
            create_drive_direct(100,-100);
        }
        create_stop();
    }


}

void no_turn(){
    int val = analog(0) - 200;

    while(val<analog(0))
    {
        create_drive_direct(20,-20);
        val = analog(0)-200;
        msleep(100);
       }
}

void no_bang_turn(){
 	int val = analog(0) - 200;

    while(val<analog(0)){
        create_drive_direct(20,-20);
        val = analog(0)-200;
        msleep(100);
       }
    printf("fail");
    while(digital(9) == 0)
    {
        create_drive_direct(50, 50);
        msleep(2);
    }
}

void reverse(){
 	int val = analog(0) - 200;

    while(val<analog(0)){
        create_drive_direct(-20,20);
        val = analog(0)-200;
        msleep(100);
    }
    create_stop();

}

void hook(){
 
	
    set_servo_position(1,1300);
    msleep(300);
    move_Motors(-10,-100);
    create_drive_direct(-80,100);
    msleep(5000);
    create_drive_direct(50,100);
    msleep(2000);
    
    
}
void ms2()
{
       max_Motors(120);
}
