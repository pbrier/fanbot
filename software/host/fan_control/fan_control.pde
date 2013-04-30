/*
 * Fanbot controller
 * (c) 2013 Peter Brier (peter.brier@kekketek.nl)
 */


/*
 * Led Class
 * A round visual element
 */
class Led
{
  float x, y, s;
  color c; 
  boolean isOn;
  Led(float xpos, float ypos, color col) {
    x = xpos;
    y = ypos;
    c = col;
    s = 15;
    isOn = false;
  }
  void display()
  {
   stroke(color( red(c)/3, green(c)/3, blue(c)/3 ) );
   fill( (isOn ? color( red(c)/2, green(c)/2, blue(c)/2 ) : color( red(c)/4, green(c)/4, blue(c)/4 ) ) );
   ellipse(x,y,s,s);
   fill( (isOn ? color(c) : color( red(c)/3, green(c)/3, blue(c)/3 ) ) );
   ellipse(x,y,s/1.1,s/1.1); 
  }
  
  boolean intersect(float xpos, float ypos)
  {
    float dx = xpos-x, dy = ypos - y; 
    return sqrt( dx*dx + dy*dy ) < s;
  }

  void toggle() { isOn = !isOn; };
  void on() { isOn = true; };
  void off() { isOn = false; };
}

/*
 * Pose Class
 * A single pose
 */
class Pose
{
  int leds;
  float servo; 
  Pose() { leds = 0; servo = 0; }
}

/* 
 * Fanbot class
 * Visual representation of the robot
 * And communication with the hardware
 */
class FanBot
{
  Led[] led;
  Led button;// abuse led as button
  float servo=0, act_servo; // position between 0.0 and 1.0
  Pose[] pose = new Pose[20];
  int frame = 0;
  boolean doPlay = false;
  int time = 0;
  
  FanBot() 
  {
    led = new Led[7];
    led[0] = new Led(30,30,color(255,200,0)); // left eye, orange
    led[1] = new Led(150,30,color(255,200,0)); // right eye, orange
    led[2] = new Led(50,120,color(255,0,0)); // left mouth, red
    led[3] = new Led(70,127,color(0,255,0)); // green
    led[4] = new Led(90,130,color(255,0,0)); // center: red
    led[5] = new Led(110,127,color(0,255,0)); // green
    led[6] = new Led(130,120,color(255,0,0)); // right mouth, red
    button = new Led(90, 75, color(0) );
    for(int i=0; i<pose.length; i++) pose[i] = new Pose();
  }

  void display()
  {
    for(int i=0; i<led.length; i++)
      led[i].display();
    button.display();
    fill(128);
    rect(0,0,width,10);
    fill(255);
    rect(act_servo*width-5,0,10,10);
    act_servo = 0.7 * act_servo + 0.3*servo;

    fill(128);
    rect(0,height-20,width,height);        
    fill(0);
    textSize(15);
    text(frame+1, width/2, height-5); 
    
    if ( doPlay && (time++ > 6) )
    {
      time = 0;
      update();
      next();
      if ( frame == pose.length-1 ) 
      {
        doPlay = false;
        frame = 0;
      }
    }
  }

  void store()
  {
     pose[frame].leds = value();
     pose[frame].servo = servo;
  }

  void restore()
  {
     for(int i=0; i<led.length; i++)
       led[i].isOn = (pose[frame].leds & 1<<i) > 0;
     servo = pose[frame].servo;
  }
  
  void next() 
  {
    store();
    frame++;
    if ( frame > pose.length-1 ) frame = pose.length-1; 
    restore();
  }
  
  void prev() 
  {
    store();
    frame--;
    if ( frame < 0 ) frame = 0;
    restore(); 
  }

  // mouse click handler
  void click(float x, float y)
  {
    if ( mouseY < 10 ) 
      fanBot.setServo((float)mouseX/width);

    if ( height - mouseY < 20) 
    {
       if ( (mouseX - width/2 ) < 0 )
         prev();
       else 
         next();
    }
 
    for(int i=0; i<led.length; i++)
      if ( led[i].intersect(x,y) )
        led[i].toggle();
    
    store();
    
    if ( button.intersect(x,y) ) 
      play();
    else
      update(); 
  }
  
  // Get Actual robot value
  int value()
  {
    int val=0;
    for(int i=0; i<led.length; i++)
      if ( led[i].isOn ) 
        val |= 1<<i;
    return val;
  }
  
  // Update current state immediately to hardware
  void update()
  {    
    int s = (int)(servo * 255.0);
    if ( s < 0 ) s = 0;
    if ( s > 255 ) s = 255;
    open("D:\\peterb\\git\\fanbot\\software\\hidapi-0.7.0\\windows\\fanbot 0 " + value() + " " +  s); 
  }
  
  // Save current program to hardware
  void save()
  {   
    String pgm = "";  
    for(int i=0; i<pose.length; i++)
      pgm = pgm + " " + pose[i].leds + " " + (int)(256 *pose[i].servo);
    print(pgm + "\n");
    open("D:\\peterb\\git\\fanbot\\software\\hidapi-0.7.0\\windows\\fanbot 1" + pgm); 
  }
  
  
  void setServo(float v) { servo = v; }
  void play() 
  {
    save(); 
    doPlay = true; 
    frame = 0; 
  }
  
  void stop() { doPlay = false; }
  
}


FanBot fanBot = new FanBot();

boolean waitUp = false;

void setup()
{
  size(190,190); 
}


void draw()
{
  fanBot.display();
  // Handle mouse event
   
  if (mousePressed == true && !waitUp) 
  {
    waitUp = true;
    fanBot.click(mouseX, mouseY);    
  
  }
  if ( !mousePressed ) 
    waitUp = false;
  
  
  
}
