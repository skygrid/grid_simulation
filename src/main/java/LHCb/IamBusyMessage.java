package LHCb;

import org.simgrid.msg.Task;

/**
 * Created by ken on 14.04.16.
 */
public class IamBusyMessage extends Task {
    public String sender;
    public IamBusyMessage(String sender){
        super("", 0, Common.SCHE_COMM_SIZE);
        this.sender = sender;
    }
}
