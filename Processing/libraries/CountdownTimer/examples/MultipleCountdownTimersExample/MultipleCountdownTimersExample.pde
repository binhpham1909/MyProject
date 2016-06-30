/**
 * Example of using multiple CountdownTimers in one application.
 */

import java.util.Map;
import com.dhchoi.CountdownTimer;
import com.dhchoi.CountdownTimerService;

Circle redCircle;
Circle blueCircle;
Map<CountdownTimer, Circle> timers = new HashMap<CountdownTimer, Circle>();

class Circle {
  static final float MAX_CIRCLE_RADIUS = 150;
  
  public float radius = 0;
  public long interval;
  public long duration;
  public color clr;
  
  public Circle(int interval, int duration, color clr) {
    this.interval = interval; 
    this.duration = duration;
    this.clr = clr;
  }
  
  public String toString() {
    return "Circle(" + hex(this.clr) + ")";
  }
}

void setup() {
  size(500, 300);

  // create circles
  redCircle = new Circle(100, 6000, color(255, 0, 0));
  blueCircle = new Circle(150, 9000, color(0, 0, 255));
  
  // associate timers to circles
  timers.put(
    CountdownTimerService.getNewCountdownTimer(this).configure(redCircle.interval, redCircle.duration).start(),
    redCircle
  );
  timers.put(
    CountdownTimerService.getNewCountdownTimer(this).configure(blueCircle.interval, blueCircle.duration).start(),
    blueCircle
  );
  
  println("Mapped timers to circles: " + timers.toString());
}

void draw() {
  background(255);
  noStroke();
  ellipseMode(CENTER);

  // draw red circle
  fill(redCircle.clr);
  ellipse(width/3, height/2, redCircle.radius, redCircle.radius);

  // draw blue circle
  fill(blueCircle.clr);
  ellipse(width*2/3, height/2, blueCircle.radius, blueCircle.radius);
}

void onTickEvent(CountdownTimer t, long timeLeftUntilFinish) {
  // change the radius of the circle based on which timer it was hooked up to
  Circle c = timers.get(t);
  c.radius = map(timeLeftUntilFinish, c.duration, 0, 0, Circle.MAX_CIRCLE_RADIUS);
}

void onFinishEvent(CountdownTimer t) {
  // finalize any changes when the timer finishes
  Circle c = timers.get(t);
  c.radius = Circle.MAX_CIRCLE_RADIUS;
  
  println("Finished: " + c);
}
