ArrayList <PVector> points = new ArrayList <PVector> ();
PImage Table; 
//import processing.serial.*;
//Serial myPort;  // The serial port
int lf = 10;    // Linefeed in ASCII
String myString = null;
float x,y,angle,r;
float f = 0.25;
int size_x = 1500;
int size_y = 900;
int midd_x = size_x/2;
int midd_y = size_y/2;
float scale = 4;

 
void setup() {
  // List all the available serial ports
  //printArray(Serial.list());
  // Open the port you are using at the rate you want:
  //myPort = new Serial(this, Serial.list()[0], 230400);
    
  size(1500,900);
  Table = loadImage("vinyle_table_2024_FINAL_V1.png");
  points.add(new PVector(0,0));
  stroke(0,0,0); // black
  strokeWeight(10);
  point(midd_x,midd_y); //point middle screen
  /*point(midd_x-150*scale,midd_y+100*scale);
  point(midd_x-150*scale,midd_y-100*scale);
  point(midd_x+150*scale,midd_y-100*scale);
  point(midd_x+150*scale,midd_y+100*scale);*/
  /*line(midd_x-150*scale,midd_y+100*scale,midd_x-150*scale,midd_y-100*scale);
  line(midd_x-150*scale,midd_y-100*scale,midd_x+150*scale,midd_y-100*scale);
  line(midd_x+150*scale,midd_y-100*scale,midd_x+150*scale,midd_y+100*scale);
  line(midd_x+150*scale,midd_y+100*scale,midd_x-150*scale,midd_y+100*scale);*/
 
  points.add(new PVector(midd_x-150*scale,midd_y+100*scale));
  points.add(new PVector(midd_x-150*scale,midd_y-100*scale));
  points.add(new PVector(midd_x+150*scale,midd_y-100*scale));
  points.add(new PVector(midd_x+150*scale,midd_y+100*scale));
  PVector p1 = points.get(1);
  PVector p2 = points.get(2);
  PVector p3 = points.get(3);
  PVector p4 = points.get(4);
  image(Table,p2.x,p2.y,300*scale,200*scale);
  line(p1.x,p1.y,p2.x,p2.y);
  line(p2.x,p2.y,p3.x,p3.y);
  line(p3.x,p3.y,p4.x,p4.y);
  line(p4.x,p4.y,p1.x,p1.y);
  for(int i=0; i<31; i++){
    stroke(255,0,0); // ellipse color: default white fill, red stroke
    strokeWeight(5);
    fill(0, 0, 0);
    textSize(3*scale);
    text(i*10,p1.x+i*10*scale-1*scale,p1.y+5*scale);
    ellipse(p1.x+i*10*scale,p1.y,0,20);
  }
  for(int i=0; i<21; i++){
    stroke(255,0,0); // ellipse color: default white fill, red stroke
    strokeWeight(5);
    fill(0, 0, 0);
    textSize(3*scale);
    text(200-i*10,p2.x-7*scale,p2.y+i*10*scale+1*scale);
    ellipse(p1.x+i*10*scale,p1.y,0,20);
    ellipse(p2.x,p2.y+i*10*scale,20,0);
  }
  
  stroke(0,255,0);
  strokeWeight(10);
  point(p1.x,p1.y);
  
  stroke(0,255,0); // ellipse color: default white fill, red stroke
  strokeWeight(20);
  ellipse(p1.x+100*scale,p1.y-100*scale,40,40);
  
  stroke(255,255,255);
  strokeWeight(8);
  angle = 60;
  line(p1.x+100*scale,p1.y-100*scale,p1.x+100*scale+scale*sin((angle+0+180)*(PI/180))*3,p1.y-100*scale+scale*cos((angle+0+180)*(PI/180))*3);
  stroke(255,0,0);
  line(p1.x+100*scale,p1.y-100*scale,p1.x+100*scale+scale*sin((angle+120+180)*(PI/180))*3,p1.y-100*scale+scale*cos((angle+120+180)*(PI/180))*3);
  line(p1.x+100*scale,p1.y-100*scale,p1.x+100*scale+scale*sin((angle+240+180)*(PI/180))*3,p1.y-100*scale+scale*cos((angle+240+180)*(PI/180))*3);

  
  
  
  //line(100,100,200,200);
  
}
 
void draw() {
   /* stroke(0,0,0); // ellipse color: default white fill, red stroke
    strokeWeight(3);
    point(100,100);
    line(100,100,200,200);*/
}
  
  
  
  
  
  
