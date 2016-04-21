package LHCb;

import org.simgrid.msg.Msg;
import org.simgrid.msg.Task;

/**
 * Created by ken on 08.04.16.
 */
public class InstructionTask extends Task{
    // public String schedulerName;
    public String tier1DownloadFrom; // Download data from here
    public double executeDataSize; // Amounts of flops
    public double downloadDataSize; // size of download data from other tier
    public double timeStart;


    public InstructionTask(String taskName, double executeDataSize, double downloadDataSize, String tier1DownloadFrom){
        super(taskName, 0.0, Common.SCHE_COMM_SIZE);
        this.tier1DownloadFrom = tier1DownloadFrom;
        this.downloadDataSize = downloadDataSize;
        this.executeDataSize = executeDataSize;
        this.timeStart = Msg.getClock();
    }

}
