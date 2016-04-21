package LHCb;

import org.simgrid.msg.*;
import org.simgrid.msg.Process;
import org.simgrid.trace.Trace;

/**
 * Created by ken on 12.04.16.
 */
public class SenderDownloader extends Process {
    public SenderDownloader(Host host, String name, String args[]){
        super(host, name, args);
    }
    public void main(String args[]) throws MsgException{
        if (args.length < 1) {
			Msg.info("Tier1 needs 1 argument (its number)");
			System.exit(1);
		}
        int num = Integer.valueOf(args[0]);
        double uploadByte = 0.0;
        while (true){
            Task task = Task.receive("Tier1_"+num + "'");
            if (task instanceof GiveMeDataTask){
                GiveMeDataTask giveMeData = (GiveMeDataTask) task;
				Msg.info("Get request from " + giveMeData.requesterName);
				DataOtherHostTask sendDataTask = new DataOtherHostTask(giveMeData.getName(), giveMeData.executeSize, giveMeData.byteSize, getHost().getName(), giveMeData.timeStart);
				Msg.info("Start send data to " + giveMeData.requesterName);
				sendDataTask.send(giveMeData.requesterName);
                uploadByte += sendDataTask.getMessageSize();
            }
            if (task instanceof FinalizeTask) {
				break;
			}
        }
        AppendToFile.nodeWorkload(uploadByte + "\n");
        Msg.info("Receive Finalize. I am done");
    }
}
