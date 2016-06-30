/**
 * Example of using a CountdownTimer with it's basic functions.
 */
 
import com.dhchoi.CountdownTimer;
import com.dhchoi.CountdownTimerService;

CountdownTimer timer;
String timerCallbackInfo = "";

void setup() {
  size(300, 300);

  // create and start a timer that has been configured to trigger onTickEvents every 100 ms and run for 5000 ms
  timer = CountdownTimerService.getNewCountdownTimer(this).configure(100, 5000).start();
}

void draw() {
  background(255);
  fill(0);
  textAlign(LEFT, TOP);

  // show the status of the timer
  text("timer.isRunning():" + timer.isRunning(), 15, 10);
  text("timer.isPaused():" + timer.isPaused(), 15, 25);
  text("tickInterval=" + timer.getTickInterval() + ", timerDuration=" + timer.getTimerDuration(), 15, 40);

  // show the info of event callbacks
  textAlign(CENTER, CENTER);
  text(timerCallbackInfo, width/2, height/2);
}

void onTickEvent(CountdownTimer t, long timeLeftUntilFinish) {
  timerCallbackInfo = "[tick] - timeLeft: " + timeLeftUntilFinish + "ms";
}

void onFinishEvent(CountdownTimer t) {
  timerCallbackInfo = "[finished]";
}

void keyPressed() {
  // user interface for operating the timer
  switch(key) {
    case 'a':
      println("Starting timer...");
      timer.start();
      break;
    case 's':
      println("Stopping timer...");
      timer.stop(CountdownTimer.StopBehavior.STOP_AFTER_INTERVAL);
      break;
    case 'r':
      println("Resetting timer...");
      timer.reset(CountdownTimer.StopBehavior.STOP_AFTER_INTERVAL);
      break;
    case 'q':
      println("getTimeLeftUntilNextEvent: " + timer.getTimeLeftUntilNextEvent());
      break;
    case 'w':
      println("getTimeLeftUntilFinish: " + timer.getTimeLeftUntilFinish());
      break;
  }
}
