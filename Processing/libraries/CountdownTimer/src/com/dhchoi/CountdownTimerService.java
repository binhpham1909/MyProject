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
 * @created     Feb 7, 2015
 * @modified    Aug 16, 2015
 * @version     0.9.4 (5)
 */

package com.dhchoi;

import processing.core.PApplet;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

/**
 * The service class that manages the creation and removal of all {@link CountdownTimer}s.
 */
public class CountdownTimerService {
    // timerIdMap related fields
    private static int timerIdCounter = -1;
    private static Map<Integer, CountdownTimer> timerIdMap = new HashMap<Integer, CountdownTimer>();
    
    private CountdownTimerService() {
        // nothing to do here
    }
    
    /**
     * Returns a new {@link CountdownTimer} that can be used inside the main Processing applet.
     * The first created {@link CountdownTimer} will always have an id of 0.
     * All subsequently created {@link CountdownTimer}s will have an id that is one higher than the previously created {@link CountdownTimer}'s id.
     * 
     * (e.g. second created timer will have id 1, third created timer will have id 2, and so on)
     *
     * @param app the main Processing applet
     * @return a newly created {@link CountdownTimer}
     */
    public static CountdownTimer getNewCountdownTimer(PApplet app) {
        timerIdCounter++;
        timerIdMap.put(timerIdCounter, new CountdownTimer(app, timerIdCounter));

        return getCountdownTimerForId(timerIdCounter);
    }

    /**
     * Returns the {@link CountdownTimer} associated with the corresponding id.
     *
     * @param id id of the desired {@link CountdownTimer}
     * @return the {@link CountdownTimer} associated with the corresponding id
     */
    public static CountdownTimer getCountdownTimerForId(int id) {
        return timerIdMap.get(id);
    }

    /**
     * Returns a set of {@link CountdownTimer} ids that have been created.
     *
     * @return a set of {@link CountdownTimer} ids that have been created
     */
    public static HashSet<Integer> getTimerIds() {
        return new HashSet<Integer>(timerIdMap.keySet());
    }

    /**
     * Removes a {@link CountdownTimer} from the created set of {@link CountdownTimer}s.
     * If the {@link CountdownTimer} is still running, the {@link CountdownTimer} will not be removed.
     *
     * @param timer the intended {@link CountdownTimer} to be removed
     * @return true if the {@link CountdownTimer} was successfully removed
     */
    public static boolean removeCountdownTimer(CountdownTimer timer) {
        if(timer == null || timer.isRunning()) {
            return false;
        }

        timer = null;
        timerIdMap.remove(timer.getId());

        return true;
    }

    /**
     * Removes a {@link CountdownTimer} with the corresponding id from the created set of {@link CountdownTimer}s.
     * If the {@link CountdownTimer} is still running, the {@link CountdownTimer} will not be removed.
     * 
     * @param id id of the intended {@link CountdownTimer} to be removed
     * @return true if the {@link CountdownTimer} was successfully removed
     */
    public static boolean removeCountdownTimerWithId(int id) {
        return removeCountdownTimer(timerIdMap.get(id));
    }
}
