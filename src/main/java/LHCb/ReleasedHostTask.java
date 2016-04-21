package LHCb;

import org.simgrid.msg.Host;
import org.simgrid.msg.Task;

import java.util.ArrayList;
import java.util.HashSet;

/**
 * Created by ken on 14.04.16.
 */
// The instances of this class Scheduler2 sends to Scheduler
public class ReleasedHostTask extends Task {
    public HashSet<Host> releasedHosts;
    public ReleasedHostTask(HashSet<Host> releasedHosts){
        this.releasedHosts = releasedHosts;
    }
}
