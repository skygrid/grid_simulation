package LHCb;

import org.simgrid.msg.Task;

/**
 * Created by ken on 11.04.16.
 */
public class DataOtherHostTask extends Task {
    public String senderName;
    public double timeStart;
    public double executeSize;

    public DataOtherHostTask(String taskName, double executeSize, double byteSize, String senderName, double timeStart){
        super(taskName, executeSize, byteSize);
        this.senderName = senderName;
        this.timeStart = timeStart;
        this.executeSize =executeSize;
    }
}
