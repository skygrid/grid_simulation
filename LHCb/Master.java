package LHCb;

/**
 * Created by ken on 27.03.16.
 */
import org.simgrid.msg.Host;
import org.simgrid.msg.Msg;
import org.simgrid.msg.MsgException;
import org.simgrid.msg.Task;
import org.simgrid.msg.Process;

public class Master extends Process {
	public Master(Host host, String name, String[]args) {
		super(host,name,args);
	}
	public void main(String[] args) throws MsgException {
		if (args.length < 4) {
			Msg.info("Master needs 4 arguments");
			System.exit(1);
		}

		int tasksCount = Integer.valueOf(args[0]);
		double taskComputeSize = Double.valueOf(args[1]);
		double taskCommunicateSize = Double.valueOf(args[2]);

		int tier1Count = Integer.valueOf(args[3]);

		Msg.info("Hello! Got "+  tier1Count + " Tier1s and "+tasksCount+" tasks to process");

		for (int i = 1; i <= tasksCount; i++) {
			Task task = new Task("Task_" + i, taskComputeSize, taskCommunicateSize);
			//Msg.info("Sending \"" + task.getName()+ "\" to \"slave_" + i % slavesCount + "\"");
			task.send("task");
		}

		Msg.info("All tasks have been dispatched. Let's tell everybody the computation is over.");

		for (int i = 0; i < tier1Count; i++) {
			FinalizeTask task = new FinalizeTask();
			task.send("task");
		}

		Msg.info("Goodbye now!");
	}
}