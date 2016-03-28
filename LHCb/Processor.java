package LHCb;

/**
 * Created by ken on 27.03.16.
 */
import org.simgrid.msg.Host;
import org.simgrid.msg.HostFailureException;
import org.simgrid.msg.Msg;
import org.simgrid.msg.Task;
import org.simgrid.msg.TaskCancelledException;
import org.simgrid.msg.TimeoutException;
import org.simgrid.msg.TransferFailureException;
import org.simgrid.msg.Process;

public class Processor extends Process {
	public Processor(Host host, String name, String[]args) {
		super(host, name, args);
	}
	public void main(String[] args) throws TransferFailureException, HostFailureException, TimeoutException {
		if (args.length < 1) {
			Msg.info("Processor needs 1 argument (its number)");
			System.exit(1);
		}

		int num = Integer.valueOf(args[0]);
		//Msg.info("Receiving on 'tier_"+num+"'");

		while(true) {
			Task task = Task.receive("tier1_"+num);

			if (task instanceof FinalizeTask) {
				break;
			}
			Msg.info("Received \"" + task.getName() +  "\". Processing it.");
			try {
				task.execute();
			} catch (TaskCancelledException e) {

			}
		//	Msg.info("\"" + task.getName() + "\" done ");
		}

		Msg.info("Received Finalize. I'm done. See you!");
	}
}
