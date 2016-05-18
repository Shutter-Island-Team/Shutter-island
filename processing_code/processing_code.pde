import java.util.Random;
import java.util.Iterator;

// One MovableBoidSystem
MovableBoidSystem ps;
// Random generator
Random rd;

int distToWall = 50;


void setup() {
  size(600,360);
  ps = new MovableBoidSystem();
  for (int i = 0; i < 10; i++) {
    ps.addLapin();
  }
  ps.addLoup(width/2, height/2);
  rd = new Random();
}

void draw() {
  background(255);
  line(distToWall, 0, distToWall, height);
  line(width - distToWall, 0, width - distToWall, height);
  line(0, distToWall, width, distToWall);
  line(0, height - distToWall, width, height - distToWall);
  ps.run();
}


// The MovableBoidSystem manages all the Particles and the Lapins.
class MovableBoidSystem {
  ArrayList<MovableBoid> movableBoids;

  MovableBoidSystem() {
    movableBoids = new ArrayList<MovableBoid>();
  }
  
  void addLapin() {
    addLapin(random(width), random(height));
  }
  
  void addLapin(float x, float y) {
    movableBoids.add(new Lapin(x, y));
  }
  
  void addLoup() {
    addLoup(random(width), random(height));
  }
  
  void addLoup(float x, float y) {
    movableBoids.add(new Loup(x, y));
  }
  
  void addLoup(Loup l) {
    movableBoids.add(l);
  }

  // Applying a force as a PVector
  void applyForce(PVector f) {
    for (MovableBoid v: movableBoids) {
      v.applyForce(f);
    }
  }

  void run() {
    Iterator<MovableBoid> itv = movableBoids.iterator();
    while (itv.hasNext()) {
      MovableBoid v = (MovableBoid) itv.next();
      v.run(movableBoids);
    }
  }
}

public enum Type {
  LOUP,
  LAPIN
}

abstract class MovableBoid {
  protected PVector location;
  protected PVector velocity;
  protected PVector acceleration;
  
  protected Type type; 
  
  // Additional variable for size
  protected float maxforce;
  protected float maxspeed;
  
  protected float distStartSlowingDown = 100;
  
  // Wandering variable
  protected final static float rCircleWander = 20;
  protected final static float distToCircle = 60;
    
  public MovableBoid(float x, float y, Type t) {
    this(x, y, 0, 0, t);
  }
  
  public MovableBoid(float x, float y, float vx, float vy, Type t) {
    this(x, y, vx, vy, 2f, 2f, t);
  }
  
  public MovableBoid(float x, float y, float vx, float vy, float maxspeed, float maxforce, Type t) {
    acceleration = new PVector(0,0);
    velocity = new PVector(vx,vy);
    location = new PVector(x,y);
    this.maxspeed = maxspeed;
    this.maxforce = maxforce;
    type = t;
  }
  
  public void setMaxSpeed(float maxSpeed) {
    this.maxspeed = maxSpeed;
  }
  
  protected void update() {
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
  
  public void run(ArrayList<MovableBoid> b) {
    applyBehaviors(b);
    update();
    display();
  }
  
  abstract protected void applyBehaviors(ArrayList<MovableBoid> pop1);
  
  protected PVector separate (ArrayList<MovableBoid> pop, float desiredSeparation) {
    PVector sum = new PVector();
    int count = 0;
    for (MovableBoid other : pop) {
      if (other.type == Type.LOUP) {
        continue;
      }
      float d = PVector.dist(location, other.location);
      if ((d > 0) && (d < desiredSeparation)) {
        PVector diff = PVector.sub(location, other.location);
        diff.normalize();
        diff.div(d);
        sum.add(diff);
        count++;
      }
    }
    if (count > 0) {
      sum.div(count);
      sum.normalize();
      sum.mult(maxspeed);
      PVector steer = PVector.sub(sum, velocity);
      steer.limit(maxforce);
      return steer;
    }
    return new PVector(0,0);

  }

  protected void applyForce(PVector force) {
    acceleration.add(force);
  }
  
  protected PVector seek(float x, float y) {
    return seek(new PVector(x, y));
  }

  protected PVector seek(PVector target) {
    PVector desired = PVector.sub(target,location);
    desired.normalize();
    desired.mult(maxspeed);
    PVector steer = PVector.sub(desired,velocity);
    steer.limit(maxforce);
    
    return steer;
  }
  
  protected PVector arrive(PVector target) {
    PVector desired = PVector.sub(target,location);

    float d = desired.mag();
    desired.normalize();
    if (d < distStartSlowingDown) {
      // Set the magnitude according to how close we are.
      float m = map(d,0,100,0,maxspeed);
      desired.mult(m);
    } else {
      // Otherwise, proceed at maximum speed.
      desired.mult(maxspeed);
    }

    // The usual steering = desired - velocity
    PVector steer = PVector.sub(desired,velocity);
    steer.limit(maxforce);
    
    return steer;
  }
  
  protected PVector wander(Random rd) {
    float randomVal = rd.nextFloat() * 2*PI;
    PVector desiredTarget = new PVector(location.x + distToCircle*velocity.x + rCircleWander*cos(randomVal), location.y + distToCircle*velocity.y + rCircleWander*sin(randomVal));
    return arrive(desiredTarget);
  }
  
  protected PVector stayWithinWalls() { // If there is a problem with movableBoid, look at this function first
    PVector steer = new PVector(0, 0); // Warning, it works because it is java (memory leak in cpp)
    if (location.x < distToWall) {
      PVector desired = new PVector (maxspeed, velocity.y);
      steer = PVector.sub (desired, velocity);
      steer.limit(maxforce);
    } else if (location.x > width - distToWall) {
      PVector desired = new PVector (maxspeed, -velocity.y);
      steer = PVector.sub (velocity, desired);
      steer.limit(maxforce);
    }
    
    if (location.y < distToWall) {
      PVector desired = new PVector (maxspeed, velocity.x);
      steer = PVector.sub (PVector.add(desired, steer), velocity);
      steer.limit(maxforce);
    } else if (location.y > height - distToWall) {
      PVector desired = new PVector (maxspeed, -velocity.x);
      steer = PVector.sub (PVector.add(velocity, steer), desired);
      steer.limit(maxforce);
    }
    
    return steer;
  }

  abstract protected void display();
}

class Loup extends MovableBoid {
  
  final static private float r = 15f;
  
  Loup(float x, float y) {
    super(x, y, Type.LOUP);
  }
  
  Loup(float x, float y, float vx, float vy) {
    super(x, y, vx, vy, Type.LOUP);
  }
  
  protected void applyBehaviors(ArrayList<MovableBoid> pop1) {
    PVector wander = wander(rd);
    PVector stayWithinWall = stayWithinWalls();
  
    wander.mult(1);
    stayWithinWall.mult(4);
    
    applyForce(wander);
    applyForce(stayWithinWall);
  }
  
  protected void display() {
    ellipse(location.x, location.y, r, r);
  }
}

class Lapin extends MovableBoid {
  
  final static private float r = 3f;
  
  Lapin(float x, float y) {
    super(x, y, Type.LAPIN);
  }
  
  Lapin(float x, float y, float vx, float vy) {
    super(x, y, vx, vy, Type.LAPIN);
  }
  
  protected void applyBehaviors(ArrayList<MovableBoid> pop1) {
    PVector separate = separate(pop1, r*6);
    PVector seek = seek(new PVector(mouseX,mouseY));
    PVector stayWithinWall = stayWithinWalls();
    PVector wander = wander(rd);
  
    separate.mult(1.5); 
    seek.mult(0);
    wander.mult(1);
    stayWithinWall.mult(4);

    
    // applyForce(arrive(new PVector(width/2, height/2)));

    applyForce(separate);
    applyForce(wander);
    applyForce(stayWithinWall);
    applyForce(seek);
  }
  
  protected void display() {
    // Lapin is a triangle pointing in
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