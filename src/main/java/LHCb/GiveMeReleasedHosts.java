package LHCb;

import org.simgrid.msg.Task;

/**
 * Created by ken on 14.04.16.
 */
public class GiveMeReleasedHosts extends Task {
    public GiveMeReleasedHosts(){
        super("", 0, Common.SCHE_COMM_SIZE);
    }
}
