package LHCb;

import org.simgrid.msg.Task;

/**
 * Created by ken on 11.04.16.
 */
public class GiveMeDataTask extends Task {
    public String requesterName;
    public double executeSize;
    public double byteSize;
    public double timeStart;

    public GiveMeDataTask(String taskName, double executeSize, double byteSize, String requesterName, double timeStart){
        super(taskName, 0.0, Common.SCHE_COMM_SIZE);
        this.requesterName = requesterName;
        this.executeSize = executeSize;
        this.byteSize = byteSize;
        this.timeStart = timeStart;

    }
}
