/**
 * Example of a stopwatch that utilizes the CountdownTimer.
 * Though the CountdownTimer itself was designed to count time downwards,
 * it can still be used to count time upwards as this example shows.
 * 
 * The example uses the mouse to control a stopwatch which increases on each second.
 * Refer to the mousePressed() event handler for further control info.
 *
 * This example was based on source code that was kindly provided
 * by Jim van Zee from Lab Data Systems in Seattle, WA.
 */
 
import com.dhchoi.CountdownTimer;
import com.dhchoi.CountdownTimerService;

final long SECOND_IN_MILLIS = 1000;
final long HOUR_IN_MILLIS = 36000000;

CountdownTimer timer;
int elapsedTime = 0;

String timeText = "";
final int timeTextX = 5, timeTextY = 35;  // upper left corner of displayed text
color timeTextColor = color(255, 0, 0);  // color of text (red: stopped, green: running)
int timeTextSeconds = 0, timeTextMinutes = 0; // the seconds and minutes to be displayed

void setup() {
  size(100,100);
  background(128);
  textSize(32);
  noStroke();
  
  timer = CountdownTimerService.getNewCountdownTimer(this).configure(SECOND_IN_MILLIS, HOUR_IN_MILLIS);
  updateTimeText();
}

void draw() {
  background(128);
  fill(timeTextColor);  
  text(timeText, timeTextX, timeTextY + 24);
}

void updateTimeText() {
  timeTextSeconds = elapsedTime % 60;
  timeTextMinutes = elapsedTime / 60;
  timeText = nf(timeTextMinutes, 2) + ':' + nf(timeTextSeconds, 2);
}

// this is called once per second when the timer is running
void onTickEvent(CountdownTimer t, long timeLeftUntilFinish) {
  ++elapsedTime;
  updateTimeText();
}

// this will be called after the timer finishes running for an hour 
void onFinishEvent(CountdownTimer t) {
  exit();
}

// mouse button event handlers that start/stop/reset the stopwatch
void mousePressed() {
  if(mouseButton == LEFT) {
    if(timer.isRunning()) {
      // STOP_IMMEDIATELY: stop immediately as soon as button was clicked
      timer.stop(CountdownTimer.StopBehavior.STOP_IMMEDIATELY); // stop stopwatch
      timeTextColor = color(255, 0, 0);  // red: stopped
    }
    else {
      // Even though the stopwatch was previously stopped with STOP_IMMEDIATELY,
      // the stopwatch will not start from where it was stopped
      // but rather count another full second before updating the timeText.
      // This is because the CountdownTimer was initially configured
      // to have one second as the smallest unit of a tick interval.
      // Decreasing the CountdownTimer's tick interval can fine tune this behavior.  
      timer.start(); // resume stopwatch
      timeTextColor = color(0, 255, 0);  // green: running
    }
  }
  else if(mouseButton == RIGHT) {  // reset stopwatch
    // STOP_AFTER_INTERVAL: reset after full second tick has passed
    timer.reset(CountdownTimer.StopBehavior.STOP_AFTER_INTERVAL); // reset stopwatch (stops first if it was running)
    timeTextColor = color(255, 0, 0);  // red: stopped
    
    elapsedTime = 0;
    updateTimeText();
  }
  else { // mouseButton == MIDDLE    
    exit(); // exit program
  }
}
