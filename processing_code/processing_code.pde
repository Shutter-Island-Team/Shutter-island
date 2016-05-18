import java.util.Random;
import java.util.Iterator;
// One BoidSystem
BoidSystem ps;
// Random generator
Random rd;

void setup() {
  size(1920,1080);
  ps = new BoidSystem(new PVector(width/2,height/2));
  rd = new Random();
  for (int i = 0; i < 200; i++) {
    ps.addVehicle(rd.nextInt() % width, rd.nextInt() % height);
  }
}

void draw() {
  background(255);
  ps.run();
}


// The BoidSystem manages all the Particles and the Vehicles.
class BoidSystem {
  ArrayList<Vehicle> vehicles;
  PVector origin;

  BoidSystem(PVector location) {
    origin = location.copy();
    vehicles = new ArrayList<Vehicle>();
  }
  
  void addVehicle() {
    vehicles.add(new Vehicle(origin.x, origin.y));
  }
  
  void addVehicle(float x, float y) {
    vehicles.add(new Vehicle(x, y));
  }

  // Applying a force as a PVector
  void applyForce(PVector f) {
    for (Vehicle v: vehicles) {
      v.applyForce(f);
    }
  }

  void run() {
    Iterator<Vehicle> itv = vehicles.iterator();
    while (itv.hasNext()) {
      Vehicle v = (Vehicle) itv.next();
      v.run();
    }
  }
}

class Vehicle {

  PVector location;
  PVector velocity;
  PVector acceleration;
  
  // Additional variable for size
  float r;
  float maxforce;
  float maxspeed;
  
  // Wandering variable
  final static float rCircleWander = 20;
  final static float distToCircle = 60;

  Vehicle(float x, float y) {
    acceleration = new PVector(0,0);
    velocity = new PVector(0,0);
    location = new PVector(x,y);
    r = 3.0;
    //[full] Arbitrary values for maxspeed and
    // force; try varying these!
    maxspeed = 2;
    maxforce = maxspeed/15;
    //[end]
  }

  // Our standard “Euler integration” motion model
  void update() {
    velocity.add(acceleration);
    velocity.limit(maxspeed);
    location.add(velocity);
    // Borderless drawing
    if (location.y < 0) {
      location.y += height;
    } else {
      location.y %= height;
    }
    if (location.x < 0) {
      location.x += width;
    } else {
      location.x %= width;
    }
    
    
    acceleration.mult(0);
  }
  
   void run() {
    // arrive(new PVector(mouseX, mouseY));
    wander();
    update();
    display();
  }

  // Newton’s second law; we could divide by mass if we wanted.
  void applyForce(PVector force) {
    acceleration.add(force);
  }

  // Our seek steering force algorithm
  void seek(PVector target) {
    PVector desired = PVector.sub(target,location);
    desired.normalize();
    desired.mult(maxspeed);
    PVector steer = PVector.sub(desired,velocity);
    steer.limit(maxforce);
    applyForce(steer);
  }
  
  void wander() {
    float randomVal = rd.nextFloat() * 2*PI;
    PVector desiredTarget = new PVector(location.x + distToCircle*velocity.x + rCircleWander*cos(randomVal), location.y + distToCircle*velocity.y + rCircleWander*sin(randomVal));
    arrive(desiredTarget);
  }
  
  void arrive(PVector target) {
    PVector desired = PVector.sub(target,location);

    // The distance is the magnitude of
    // the vector pointing from
    // location to target.
    float d = desired.mag();
    desired.normalize();
    // If we are closer than 100 pixels...
    if (d < 100) {
      //[full] ...set the magnitude
      // according to how close we are.
      float m = map(d,0,100,0,maxspeed);
      desired.mult(m);
      //[end]
    } else {
      // Otherwise, proceed at maximum speed.
      desired.mult(maxspeed);
    }

    // The usual steering = desired - velocity
    PVector steer = PVector.sub(desired,velocity);
    steer.limit(maxforce);
    applyForce(steer);
  }

  void display() {
    // Vehicle is a triangle pointing in
    // the direction of velocity; since it is drawn
    // pointing up, we rotate it an additional 90 degrees.
    float theta = velocity.heading() + PI/2;
    fill(175);
    stroke(0);
    pushMatrix();
    translate(location.x,location.y);
    rotate(theta);
    beginShape();
    vertex(0, -r*2);
    vertex(-r, r*2);
    vertex(r, r*2);
    endShape(CLOSE);
    popMatrix();
  }
}