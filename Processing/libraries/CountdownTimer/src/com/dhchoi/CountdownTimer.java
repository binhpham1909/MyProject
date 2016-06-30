/**
 * CountdownTimer
 * A countdown timer for Processing which can trigger callback events during a user-defined set of tick intervals and duration.
 * https://github.com/dhchoi/processing-countdowntimer
 *
 * Copyright (c) 2015 Dong Hyun Choi
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 *
 * @author      Dong Hyun Choi
 * @created     Mar 26, 2014
 * @modified    Aug 16, 2015
 * @version     0.9.4 (5)
 */

package com.dhchoi;

import processing.core.*;

import java.util.HashSet;
import java.util.concurrent.*;
import java.lang.reflect.Method;

public class CountdownTimer {
    public enum StopBehavior {
        STOP_IMMEDIATELY, STOP_AFTER_INTERVAL
    }
    
    // processing related fields
    private final PApplet mApp;
    // callback event related fields
    private static final String ON_TICK_EVENT_NAME = "onTickEvent";
    private static final String ON_FINISH_EVENT_NAME = "onFinishEvent";
    private Method onTickEvent;
    private Method onFinishEvent;
    private boolean isLegacyTickEvent = false;
    private boolean isLegacyFinishEvent = false;
    // timer related fields
    private final int mId;
    private final TimeUnit mTimeUnit = TimeUnit.MILLISECONDS;
    private ScheduledExecutorService mScheduledExecutorService = getNewScheduledExecutorService();
    private long mTimerDuration = 0;
    private long mTickInterval = 0;
    private long mTimeLeftAtStartOfTick = 0;
    private long mFinishTimeInFuture;
    private boolean mIsRunning = false;
    private boolean mDoReset = false;
    private final Runnable mCallableTask = new Runnable() {
        @Override
        public void run() {
            mTimeLeftAtStartOfTick = mFinishTimeInFuture - getSystemTimeMillis();

            if(mTimeLeftAtStartOfTick <= 0) {
                if(isLegacyFinishEvent) {
                    invokeMethod(onFinishEvent, mId);
                }
                else {
                    invokeMethod(onFinishEvent, CountdownTimer.this);
                }
                stop(StopBehavior.STOP_AFTER_INTERVAL);
            }
            else {
                // perform tick and get tick performance time
                // the tick for the ongoing interval before the timer being stopped will always be performed
                long tickStartTime = getSystemTimeMillis();
                if(isLegacyTickEvent) {
                    invokeMethod(onTickEvent, mId, mTimeLeftAtStartOfTick);
                }
                else {
                    invokeMethod(onTickEvent, CountdownTimer.this, mTimeLeftAtStartOfTick);
                }
                long tickPerformanceTime = getSystemTimeMillis() - tickStartTime;

                // no time left, so finish timer
                if(tickPerformanceTime > mTimeLeftAtStartOfTick) {
                    if(isLegacyFinishEvent) {
                        invokeMethod(onFinishEvent, mId);
                    }
                    else {
                        invokeMethod(onFinishEvent, CountdownTimer.this);
                    }
                    stop(StopBehavior.STOP_AFTER_INTERVAL);
                }
                else {
                    // get next closest scheduled time
                    long nextScheduledTime = Math.min(mTimeLeftAtStartOfTick, mTickInterval) - tickPerformanceTime;
                    // in case mTickInterval < tickPerformanceTime < mTimeLeftAtStartOfTick, skip to next available interval
                    while (nextScheduledTime < 0) {
                        nextScheduledTime += mTickInterval;
                    }

                    // schedule new service only if timer is running and doesn't require reset
                    if(mIsRunning && !mDoReset) {
                        mScheduledExecutorService.schedule(mCallableTask, nextScheduledTime, mTimeUnit);
                    }
                }
            }
        }
    };

    /**
     * Constructor for creating a CountdownTimer.
     * 
     * @param app the main Processing applet
     * @param id the id for the timer
     */
    CountdownTimer(PApplet app, int id) {
        mId = id;
        mApp = app;
        mApp.registerMethod("dispose", this);

        try {
            onTickEvent = mApp.getClass().getMethod(ON_TICK_EVENT_NAME, int.class, long.class);
            isLegacyTickEvent = true;
            System.err.println("Please use the new \"void " + ON_TICK_EVENT_NAME + "(CountdownTimer t, long timeLeftUntilFinish)\" callback for better method chaining.");
        } catch (NoSuchMethodException e1) {
            try {
                onTickEvent = mApp.getClass().getMethod(ON_TICK_EVENT_NAME, CountdownTimer.class, long.class);
            } catch (NoSuchMethodException e2) {
                System.err.println("Applet needs to implement \"void " + ON_TICK_EVENT_NAME + "(CountdownTimer t, long timeLeftUntilFinish)\"");
                e2.printStackTrace();
            }
        }

        try {
            onFinishEvent = mApp.getClass().getMethod(ON_FINISH_EVENT_NAME, int.class);
            isLegacyFinishEvent = true;
            System.err.println("Please use the new \"void " + ON_FINISH_EVENT_NAME + "(CountdownTimer t)\" callback for better method chaining.");
        } catch (NoSuchMethodException e1) {
            try {
                onFinishEvent = mApp.getClass().getMethod(ON_FINISH_EVENT_NAME, CountdownTimer.class);
            } catch (NoSuchMethodException e2) {
                System.err.println("Applet needs to implement \"void " + ON_FINISH_EVENT_NAME + "(CountdownTimer t)\"");
                e2.printStackTrace();
            }
        }
    }

    /**
     * Configures the tick interval and timer duration in milliseconds.
     * The timer must be configured first before calling the start() method.
     *
     * @param tickIntervalMillis the tick interval (in milliseconds)
     * @param timerDurationMillis the total timer duration (in milliseconds)
     * @return the CountdownTimer that has been configured
     * @throws IllegalArgumentException if timerDurationMillis or tickIntervalMillis is not greater than zero, or if tickIntervalMillis is greater than timerDurationMillis
     * @throws IllegalStateException when attempted to configure a running or paused timer
     */
    public synchronized final CountdownTimer configure(long tickIntervalMillis, long timerDurationMillis) {
        if(tickIntervalMillis <= 0) {
            System.err.println("tickIntervalMillis has to be greater than zero");
            throw new IllegalArgumentException("tickIntervalMillis has to be greater than zero");
        }

        if(timerDurationMillis <= 0) {
            System.err.println("timerDurationMillis has to be greater than zero");
            throw new IllegalArgumentException("timerDurationMillis has to be greater than zero");
        }

        if(timerDurationMillis < tickIntervalMillis) {
            System.err.println("tickIntervalMillis cannot be longer than timerDurationMillis");
            throw new IllegalArgumentException("tickIntervalMillis cannot be longer than timerDurationMillis");
        }

        if(mIsRunning) {
            System.err.println("cannot configure timer when it is already running");
            throw new IllegalStateException("cannot configure timer when it is already running");
        }

        if(isPaused()) {
            System.err.println("cannot configure timer when it has paused");
            throw new IllegalStateException("cannot configure timer when it has paused");
        }

        mTickInterval = tickIntervalMillis;
        mTimerDuration = timerDurationMillis;

        return this;
    }

    /**
     * Starts the timer with the most recent tick interval and timer duration configuration.
     * If the timer was stopped before the finish time, the method call will resume the timer from where it was stopped.
     * Starting an already running timer will have no effect.
     *
     * @return the CountdownTimer that has been started
     * @throws IllegalStateException if timer has not been configured before the initial call to the method
     */
    public synchronized final CountdownTimer start() {
        if(isPaused()) {
            return start(mTimeLeftAtStartOfTick);
        }

        return start(mTimerDuration); // case when timer has finished, has been reset, or is the first time starting
    }

    private synchronized CountdownTimer start(long duration) {
        if(mTimerDuration == 0 || mTickInterval == 0) {
            System.err.println("CountdownTimer has not been configured yet");
            throw new IllegalStateException("CountdownTimer has not been configured yet");
        }

        if(!mIsRunning) {
            if(mScheduledExecutorService.isShutdown()) {
                mScheduledExecutorService = getNewScheduledExecutorService();
            }

            try {
                mIsRunning = true;
                mDoReset = false;

                mFinishTimeInFuture = getSystemTimeMillis() + duration;
                mScheduledExecutorService.schedule(mCallableTask, mTickInterval, mTimeUnit);
            }
            catch (Exception e) {
                System.err.println("Failed to start the timer");
                e.printStackTrace();
            }
        }

        return this;
    }

    /**
     * This is effectively the same as calling stop(StopBehavior.STOP_AFTER_INTERVAL).
     *
     * @return the CountdownTimer that has been stopped
     */
    @Deprecated
    public synchronized final CountdownTimer stop() {
        return stop(StopBehavior.STOP_AFTER_INTERVAL);
    }

    /**
     * Interrupts the timer to stop with the behavior depending on the given StopBehavior.
     * Based on the parameter, the timer may stop immediately without waiting for the currently running interval to complete,
     * or it may stop after the currently running interval has completed.
     * Attempting to stop a timer that was already stopped or reset will have no effect.
     * 
     * In case the StopBehavior was set to stop the timer immediately,
     * there are no guarantees beyond best-effort attempts to stop processing actively executing tasks.
     * For example, typical implementations will cancel via Thread.interrupt(), so any task that fails to respond to interrupts may never terminate.
     * This functional limit is bound to the limits of Java.
     *
     * @param stopBehavior the intended stop behavior of the timer
     * @return the CountdownTimer that has been stopped
     */
    public synchronized final CountdownTimer stop(StopBehavior stopBehavior) {
        mIsRunning = false;
        
        if(stopBehavior == StopBehavior.STOP_AFTER_INTERVAL) {
            mScheduledExecutorService.shutdown();
        }
        else { // StopConfig.STOP_IMMEDIATELY
            mScheduledExecutorService.shutdownNow();
        }

        return this;
    }

    /**
     * This is effectively the same as calling reset(StopBehavior.STOP_AFTER_INTERVAL).
     *
     * @return the CountdownTimer that has been reset
     */
    @Deprecated
    public synchronized final CountdownTimer reset() {
        return reset(StopBehavior.STOP_AFTER_INTERVAL);
    }

    /**
     * Stops the timer and resets it to the most recent configuration.
     * If the method was called while the timer was running, it will first stop the timer by effectively calling stop(StopBehavior).
     * Attempting to reset a timer that was already reset or stopped will have no effect.
     *
     * @param stopBehavior the intended stop behavior of the timer
     * @return the CountdownTimer that has been reset
     */
    public synchronized final CountdownTimer reset(StopBehavior stopBehavior) {
        mDoReset = true;

        return stop(stopBehavior);
    }

    /**
     * Returns true if the timer is currently running.
     *
     * @return true if the timer is currently running
     */
    public final boolean isRunning() {
        return mIsRunning;
    }

    /**
     * Returns true if the timer was stopped before being finished.
     *
     * @return true if the timer was stopped before being finished
     */
    public final boolean isPaused() {
        return !mIsRunning && (mTimeLeftAtStartOfTick > 0) && !mDoReset;
    }

    /**
     * Returns the id of the timer.
     *
     * @return the id of the timer
     */
    public final int getId() {
        return mId;
    }

    /**
     * Returns the configured tick interval in milliseconds.
     * 
     * @return the configured tick interval in milliseconds
     */
    public final long getTickInterval() {
        return mTickInterval;
    }

    /**
     * Returns the configured timer duration in milliseconds.
     * 
     * @return the configured timer duration in milliseconds
     */
    public final long getTimerDuration() {
        return mTimerDuration;
    }

    /**
     * Returns the current time left until the upcoming tick or finish event in milliseconds.
     *
     * @return the current time left until the upcoming tick or finish event in milliseconds
     */
    public final long getTimeLeftUntilNextEvent() {
        if(isRunning()) {
            return (mFinishTimeInFuture - getSystemTimeMillis()) % mTickInterval;
        }

        return 0;
    }

    /**
     * Returns the current time left until the timer finishes in milliseconds.
     *
     * @return the current time left until the timer finishes in milliseconds
     */
    public final long getTimeLeftUntilFinish() {
        if(isRunning()) {
            return mFinishTimeInFuture - getSystemTimeMillis();
        }

        return 0;
    }

    /**
     *  Method used by the main Processing applet to clear up any resources used by the timer when closing the applet.
     */
    public void dispose() {
        mScheduledExecutorService.shutdownNow();
    }

    @Override
    public String toString() {
        return "CountdownTimer[" + getId() + "](tickInterval:" + mTickInterval + ", timerDuration:" + mTimerDuration + ")";
    }

    /**
     * Returns a new timer that can be used inside the main Processing applet.
     * The first created timer will always have an id of 0.
     * All subsequently created timers will have an id that is 1 higher than the previously created timer's id
     * (e.g. second created timer will have id 1, third created timer will have id 2, and so on).
     * 
     * (DEPRECATED: use the CountdownTimerService's method instead)
     *
     * @param app the main Processing applet
     * @return a newly created CountdownTimer
     */
    @Deprecated
    public static CountdownTimer getNewCountdownTimer(PApplet app) {
        return CountdownTimerService.getNewCountdownTimer(app);
    }

    /**
     * Returns the timer associated with the corresponding id.
     * 
     * (DEPRECATED: use the CountdownTimerService's method instead)
     *
     * @param id id of the desired timer
     * @return the CountdownTimer associated with the corresponding id
     */
    @Deprecated
    public static CountdownTimer getCountdownTimerForId(int id) {
        return CountdownTimerService.getCountdownTimerForId(id);
    }

    /**
     * Returns a set of timer ids that have been created.
     * 
     * (DEPRECATED: use the CountdownTimerService's method instead)
     *
     * @return a set of timer ids that have been created
     */
    @Deprecated
    public static HashSet<Integer> getTimerIds() {
        return CountdownTimerService.getTimerIds();
    }

    private long getSystemTimeMillis() {
        return TimeUnit.NANOSECONDS.toMillis(System.nanoTime());
    }

    private ScheduledExecutorService getNewScheduledExecutorService() {
        return Executors.newSingleThreadScheduledExecutor();
    }

    private void invokeMethod(Method method, Object... args) {
        if(method != null) {
            try {
                method.invoke(mApp, args);
            } catch (Exception e) {
                System.err.println("failed to call method " + method.toString() + " inside main app");
                e.printStackTrace();
            }
        }
    }
}
