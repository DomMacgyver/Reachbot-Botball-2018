#include <kipr/botball.h>
#include "functions.h"
//closed: 1520
//open:1047
 int boll = 1;
int main()
{

    int closev = 1620; // servo half open valuse
 //   int somenum = -15;
    int threshold = 15;
    int linevalue = 3500;

    win();
    int open = 1000;


    //start initialization
    create_connect();
    create_full();
    enable_servos();
    msleep(10);
    //end initialization

     //set servos
    set_servo_position(1,1);
    msleep(100);

    set_servo_position(3,200);
    msleep(100);

     //wait_for_light(5);
	shut_down_in(118);

      move_Motors(200, 200);//set arm to right height
    msleep(100);

    //move out of starting box
    create_drive_direct (200,200);
    msleep(900);
    turn(-100);
    create_drive_direct (200,200);
    msleep(1000);


    //line follow to other starting box

	thread t3;
    t3 = thread_create(ms3);
    thread_start(t3);

    double sec = seconds();
    int bol = 1;

    while(seconds() - sec < 3000 && bol ==1)
    {
        if(get_create_lbump() == 1)
        {
            boll = 0;
            msleep(100);
            create_stop();
		//thread_destroy(t3);

            //bol = 0;
            create_drive_direct (-200,-200);
   			 msleep(200);
            while(analog(2) < 3000)
    {
        create_drive_direct(200,-200);
    }
    turn(3);
    create_stop();
  //t3 = thread_create(ms3);
    thread_start(t3);


    }
    }

    thread_wait(t3);
    create_stop();
    msleep(100);
    thread_destroy(t3);
    msleep(100);

    //align in other starting box
    create_drive_direct(200,200);
    msleep(1000);
    create_stop();
    max_Motors(300); //190 was the base position
    ao();
    turn(90);
    create_drive_direct(-400,-400);//back align
    msleep(1000);
    create_stop();

    //follow line in starting box
    while(analog(2) < linevalue)
    {
        if(analog(1) < linevalue)
        {
            create_drive_direct(150,200);
        }
        else
        {
            create_drive_direct(200,150);
        }
    }
    create_stop();

    //align against other wall
    create_drive_direct(-200,-200);
    msleep(300);
    turn(90);
    create_drive_direct(-400,-400);
    msleep(1000);
    create_stop();

    //opens every claw
    set_servo_position(3, open);
    set_servo_position(1,1);

    //begin getting frisbees

    //navigate to first frisbee
    create_drive_direct(75, 75);
    msleep(1350);
    turn(-103);

    while(get_create_rbump() ==0)//go forward until it hits the wall
    {
        create_drive_direct(200,200);
    }
    create_stop();
        msleep(12000);
    
    create_drive_direct(-200,-200); // backup
    msleep(400);

    turn(90);

    create_drive_direct(-200, -200);//back align
    msleep(2000);

    create_drive_direct(200,200);//go forward
    msleep(200);
    create_stop();
    msleep(100);

    // blue tape left turn begin
    int p = 1;
    int x = analog(1);
    int w = analog(1);
    while(p == 1)
    {
        x = analog(1);
        w = analog(1);

        while(w < x+threshold)
        {
            create_drive_direct(-200,200);
            x = analog(1);
            msleep(10);

            w = analog(1);

        }
        create_drive_direct(-200,200);
        msleep(10);
        w = analog(1);
        if(w > x+threshold)
        {
            p = 0;
        }
    }
    // blue tape left turn end
    turn(4);//manual turn

    while(get_create_rbump() == 0)
    {
        create_drive_direct(100,100);
    }
    create_drive_direct(-200,-200);
    msleep(1200);
    create_stop();
    move_Motors(-350, -300);


    //time to grab second frisbee
    double tempx;
    for(tempx = 200;digital(9) == 0 && tempx > 50;tempx-=0.5)
    {
        create_drive_direct((int)tempx, (int)tempx);
        msleep(1);

    }
    while(digital(9) == 0 && tempx == 50)
    {
         create_drive_direct((int)tempx, (int)tempx);
    }


    create_drive_direct(-200,-200);
    msleep(800);
    create_stop();
    turn(7);//manual turn to get rubber hook under frisbee(1)
    create_drive_direct(50,50);
    msleep(1700);
    create_stop();

  set_servo_position(3,1200);
    msleep(200);

    thread t1;
    t1 = thread_create(ms);
    thread t2;
    t2 = thread_create(ms2);
    thread_start(t2);// go up
    msleep(800);
    thread_start(t1);//grab frisbee
    msleep(2000);
    thread_destroy(t1);
    thread_destroy(t2);

    create_drive_direct(-50,-50);
    msleep(700);
    create_stop();
    smoothturn(-7,50,50);
    msleep(700); // **

    ao();
    msleep(100);
    create_drive_direct(-150,-100);//backup from first fribee pipe
    msleep(900);


    while(analog(1) < 3000)		//backup to line
    {
        create_drive_direct(-200,-200);
    }
    create_stop();
    msleep(100);
    create_drive_direct(200,200);
    msleep(500);
    create_stop();
    while(analog(1) < 3000)
    {
        create_drive_direct(200,-200);
    }
    turn(8);
    create_stop();


    // drop off frisbee lineup
    while(analog(2) < linevalue)//line follow until perpendicualr line
    {
        if(analog(1) > linevalue)
        {
            create_drive_direct(125,75);
        }
        else
        {
            create_drive_direct(75,125);
        }
    }
    create_drive_direct(50, 50);
    msleep(1800);



    //opens hook for grab
    set_servo_position(1,1100);
    msleep(100);
	create_stop();

	max_Motors(50);

    //turns to start line following
    smoothturn(100,20,100);
    create_stop();

    create_drive_direct(75,75);
    msleep(350);

    while(analog(1) < linevalue)
    {
        create_drive_direct(100, 100);
    }
    //create_drive_direct(100, 100);
    //msleep(200);//last change
    create_stop();
    /*
    turn(-10);
    create_stop();

    //drop frisbee
    move_Motors(-50,-500);
    msleep(100);
    set_servo_position(3,1460);
    msleep(200);
    max_Motors(200);
    msleep(300);
    move_Motors(-50,-500);
    msleep(100);
    max_Motors(200);
    msleep(300);
    turn(10);*/
    create_drive_direct(100, 100);
    msleep(500);

    //turns about 180 (to face the middle)
    turn(-103);
    create_drive_direct(100,100);
    msleep(200);

    create_drive_direct(0,0);
    turn(-103);
     slowclose(1100,1440,3,1);// close servo slowly around bucket
    msleep(500);

    //move until front bumper hits pvc
    int lol;
    int you = 1;
    while(you == 1)
    {
        lol = 0;
        while(lol < 100 && (get_create_lbump() == 0 && get_create_rbump() == 0))
        {

            create_drive_direct(185,200);
            msleep(1);
            lol++;
        }
        if((get_create_lbump() == 1 || get_create_rbump() == 1))
        {
            you = 0;
        }
        else
        {
            create_drive_direct(-185,-200);
            msleep(50);
        }
    }
    create_stop();
    move_Motors(-100,-150);
    set_servo_position (1,1100);
    msleep(100);
    max_Motors(150);
    msleep(200);

    //move back from pvc
    turn(2);
    create_drive_direct(-200, -200);
    msleep(2000);
    create_stop();

	set_servo_position(3,200);//set second arm serv(2) open all the way
    msleep(200);

    move_Motors(-515, -200);
    set_servo_position(1, closev);
    create_drive_direct(200, 200);
    msleep(350);
	
	
    create_stop();

    //TURNS RIGHT BEFORE STARTS LINE FOLLOWING
    create_drive_direct(200, -200);
    msleep(500);
    create_drive_direct(200, 200);
    msleep(250);
    while(analog(1)< linevalue)
    {
        create_drive_direct(-200, 200);
        msleep(1);
    }
    turn(1);//manual correction
    while( get_create_lbump() == 0)
    {

        if(analog(1) <linevalue)
        {
            create_drive_direct(75,100);
            msleep(1);
        }
        else
        {
            create_drive_direct(100,75);
            msleep(1);
        }
    }
    //goes a little bit forward so botguy will be within reach
    create_drive_direct(150, 150);
    msleep(500);

    //turns to compensate for accidental turn 
    create_drive_direct(0,150);
    msleep(1000);
    //claw closes around botguy
    create_stop();
    set_servo_position(1, 2047);
    move_Motors(10,50);
    create_stop();
	move_Motors(30,50);
    //moves backward after picking up botguy
    while(analog(2) < linevalue)
    {
        create_drive_direct(-100, -100);
        msleep(1);
    }
    //lift up botguy
    create_stop();
    max_Motors(200);
	turn(3);
    create_drive_direct(200,200);
    msleep(700);
create_drive_direct(200, -200);
    msleep(400);
   create_stop();
    while(analog(1)< linevalue)
    {
        create_drive_direct(-200, 200);
        msleep(1);
    }
    turn(1);//manual correction

    //go until it hits pipe
    while( get_create_lbump() == 0 && get_create_rbump() == 0)
    {

        if(analog(1) <linevalue)
        {
            create_drive_direct(75,100);
            msleep(1);
        }
        else
        {
            create_drive_direct(100,75);
            msleep(1);
        }
    }
     //create_drive_direct(-200,-200);//backup from pipe
    //msleep(500);

    //align botguy to be in bin
    turn(18);
    create_drive_direct(-100, -100);
    msleep(500);
    create_stop();
    move_Motors(-80,-100);
    create_drive_direct(-100, -100);
    msleep(500);

    disable_servos();
    create_disconnect();
    msleep(1000);
    return 0;
}
