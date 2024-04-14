ArrayList <PVector> points = new ArrayList <PVector> ();
ArrayList <PVector> coord_robot = new ArrayList <PVector> ();
PImage Table; 
import processing.serial.*;
Serial myPort;  // The serial port
int lf = 10;    // Linefeed in ASCII
String myString = null;
float x,y,angle,r;
float f = 0.25;
int size_x = 1500;
int size_y = 900;
int midd_x = size_x/2;
int midd_y = size_y/2;
float scale = 4;
float scale_robot = 2;
PVector p1,p2,p3,p4;
 
void setup() {
  // List all the available serial ports
  //printArray(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[0], 230400);
    
  size(1500,900);
  Table = loadImage("vinyle_table_2024_FINAL_V1.png");
  points.add(new PVector(0,0));
  coord_robot.add(new PVector(0,0,0));
  //background(0);
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
  p1 = points.get(1);
  p2 = points.get(2);
  p3 = points.get(3);
  p4 = points.get(4);
  line(p1.x,p1.y,p2.x,p2.y);
  line(p2.x,p2.y,p3.x,p3.y);
  line(p3.x,p3.y,p4.x,p4.y);
  line(p4.x,p4.y,p1.x,p1.y);
  image(Table,p2.x,p2.y,300*scale,200*scale);
  for(int i=0; i<31; i++){
    stroke(255,0,0); // ellipse color: default white fill, red stroke
    strokeWeight(5);
    fill(0, 0, 0);
    textSize(3*scale);
    text(i*100,p1.x+i*10*scale-1*scale,p1.y+5*scale);
    ellipse(p1.x+i*10*scale,p1.y,0,20);
  }
  for(int i=0; i<21; i++){
     stroke(255,0,0); // ellipse color: default white fill, red stroke
    strokeWeight(5);
    fill(0, 0, 0);
    textSize(3*scale);
    text(2000-i*100,p2.x-10*scale,p2.y+i*10*scale+1*scale);
    ellipse(p1.x+i*10*scale,p1.y,0,20);
    ellipse(p2.x,p2.y+i*10*scale,20,0);
  }
  stroke(0,255,0);
  strokeWeight(10);
  point(p1.x,p1.y);
  
  //stroke(0,255,0); // ellipse color: default white fill, red stroke
  //strokeWeight(10);
  //ellipse(p1.x+100*scale,p1.y-100*scale,20,20);
}
 
void draw() {
  while (myPort.available() > 0){
    myString = myPort.readStringUntil(10);
    if (myString != null) {
      //print(myString);
      String[] q = splitTokens(myString, ";");
      if (q.length>2){
        x=float(q[0]);  // Converts and prints float
        y = float(q[1]);  // Converts and prints float
        angle = float(q[2]);  // Converts and prints float
        
       
//draw_table();
        coord_robot.add(new PVector(p1.x+x*scale/10,p1.y-y*scale/10,-angle));
        while (coord_robot.size() > 1) {
          coord_robot.remove(0);
        }
        PVector coord = coord_robot.get(coord_robot.size()-1);
        
        stroke(0,255,0); // ellipse color: default white fill, red stroke
        strokeWeight(10);
        ellipse(coord.x,coord.y,5*scale*scale_robot,5*scale*scale_robot);
        stroke(255,255,255); // ellipse color: default white fill, red stroke
        line(coord.x,coord.y,coord.x+scale*sin((coord.z+0+180)*(PI/180))*1*scale_robot,coord.y+scale*cos((coord.z+0+180)*(PI/180))*1*scale_robot);
        stroke(255,0,0);
        line(coord.x,coord.y,coord.x+scale*sin((coord.z+120+180)*(PI/180))*1*scale_robot,coord.y+scale*cos((coord.z+120+180)*(PI/180))*1*scale_robot);
        line(coord.x,coord.y,coord.x+scale*sin((coord.z+240+180)*(PI/180))*1*scale_robot,coord.y+scale*cos((coord.z+240+180)*(PI/180))*1*scale_robot);
        println(x,y,angle);
 
      }
    }
        
  
  }
}

void draw_table(){
  //background(0);
  stroke(255,255,255); // black
  strokeWeight(10);
  line(p1.x,p1.y,p2.x,p2.y);
  line(p2.x,p2.y,p3.x,p3.y);
  line(p3.x,p3.y,p4.x,p4.y);
  line(p4.x,p4.y,p1.x,p1.y);
  for(int i=0; i<31; i++){
    stroke(255,0,0); // ellipse color: default white fill, red stroke
    strokeWeight(1*scale);
    ellipse(p1.x+i*10*scale,p1.y,0,20);
  }
  for(int i=0; i<21; i++){
    stroke(255,0,0); // ellipse color: default white fill, red stroke
    strokeWeight(1*scale);
    ellipse(p2.x,p2.y+i*10*scale,20,0);
  }
  stroke(0,255,0);
  strokeWeight(10);
  point(p1.x,p1.y);
}

  
  
  
  
  
