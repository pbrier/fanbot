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
    isOn = true;
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


class FanBot
{
  Led[] led;
  float servo=0, act_servo; // position between 0.0 and 1.0
  FanBot() 
  {
    led = new Led[7];
    led[0] = new Led(30,30,color(0,255,0)); // left eye, green
    led[1] = new Led(150,30,color(0,255,0)); // right eye, green
    led[2] = new Led(50,120,color(255,255,0)); // left mouth, yellow
    led[3] = new Led(70,127,color(255,0,0)); // red
    led[4] = new Led(90,130,color(255,0,0)); // center: red
    led[5] = new Led(110,127,color(255,0,0)); // red
    led[6] = new Led(130,120,color(255,255,0)); // right mouth, yellow
  }

  void display()
  {
    for(int i=0; i<led.length; i++)
      led[i].display();
    fill(128);
    rect(0,0,width,10);
    fill(255);
    rect(act_servo*width-5,0,10,10);
    act_servo = 0.7 * act_servo + 0.3*servo;
  }

  void click(float x, float y)
  {
    for(int i=0; i<led.length; i++)
      if ( led[i].intersect(x,y) )
        led[i].toggle();
  }
  
  int value()
  {
    int val=0;
    for(int i=0; i<led.length; i++)
      if ( led[i].isOn ) 
        val |= 1<<i;
    return val;
  }
  
  void update()
  {    
    int s = (int)(servo * 255.0);
    if ( s < 0 ) s = 0;
    if ( s > 255 ) s = 255;
    open("D:\\peterb\\git\\fanbot\\software\\hidapi-0.7.0\\windows\\fanbot " + value() + " " +  s); 
  }
  
  void setServo(float v) 
  {
     servo = v;
     update();
  }
  
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
    if ( mouseY < 10 ) fanBot.setServo((float)mouseX/width);
 
    fanBot.click(mouseX, mouseY);    
    fanBot.update();   
  }
  if ( !mousePressed ) 
    waitUp = false;
  
  
  
}
