package LHCb;

import org.simgrid.msg.Task;

/**
 * Created by ken on 14.04.16.
 */
// The instanses of this class Tier1 sends to Scheduler2
public class ReleaseTask extends Task {
    String releasedHost;
    public ReleaseTask(String releasedHost){
        super("", 0.0, Common.SCHE_COMM_SIZE);
        this.releasedHost = releasedHost;
    }
}
