package LHCb.FIFO;
import LHCb.FinalizeTask;
import LHCb.InstructionTask;
import com.opencsv.CSVReader;
import org.simgrid.msg.Host;
import org.simgrid.msg.Msg;
import org.simgrid.msg.MsgException;
import org.simgrid.msg.Task;
import org.simgrid.msg.Process;;import java.io.FileReader;
import java.util.ArrayList;
import java.util.Arrays;

public class Scheduler extends Process {

    public ArrayList<String> taskName = new ArrayList<>();
    public ArrayList<Double> compSize = new ArrayList<>();
    public ArrayList<Double> commSize = new ArrayList<>();
    public ArrayList<ArrayList<Host>> locations = new ArrayList<>();
	public ArrayList<ArrayList<Host>> nonLocations = new ArrayList<>();

	public Scheduler(Host host, String name, String[]args) {
		super(host,name,args);
	}
    public void createInput() throws Exception{
        CSVReader reader = new CSVReader(new FileReader("/home/ken/IdeaProjects/Sim2Sim/src/main/java/LHCb/dataFiles/data.csv"), ',', '\'', 1);
        String[] nextline;
        while ((nextline = reader.readNext()) != null){
            taskName.add(nextline[0]);
            compSize.add(Double.parseDouble(nextline[1]));
            commSize.add(Double.parseDouble(nextline[2]));
            locations.add(new ArrayList<>(Arrays.asList(new Host[]{getHost().getByName(nextline[3]), getHost().getByName(nextline[4]), getHost().getByName(nextline[5])})));
			nonLocations.add(new ArrayList<>(Arrays.asList(new Host[]{getHost().getByName(nextline[6]), getHost().getByName(nextline[7]), getHost().getByName(nextline[8]), getHost().getByName(nextline[9])})));
        }
		System.out.println();
	}
	public void main(String[] args) throws MsgException {
        try {
            createInput();
        }catch (Exception e){
        }
        AppendToFile.delete();

		int tasksCount = taskName.size();
		int tier1sCount = Integer.valueOf(args[2]) - Integer.valueOf(args[1])+1;

		for (int i = 0; i < tasksCount; i++) {
			InstructionTask task = new InstructionTask(taskName.get(i), compSize.get(i), commSize.get(i), locations.get(i).get(0).getName());
			Msg.info("Sending \"" + task.getName()+ "\" to \"Tier1_" + i % tier1sCount + "\"");
			task.send("Tier1_"+(i%tier1sCount+1));
            AppendToFile.appendScheduling(taskName.get(i) + ", " + Msg.getClock());
		}

		Msg.info("All tasks have been dispatched. Let's tell everybody the computation is over.");

		for (int i = 0; i < tier1sCount; i++) {
			FinalizeTask task = new FinalizeTask();
            FinalizeTask task1 = new FinalizeTask();
			task.send("Tier1_"+(i+1));
            task1.send("Tier1_"+(i+1) + "'");
		}
        FinalizeTask finalizeTask = new FinalizeTask();
        finalizeTask.send("Tier0'");

		Msg.info("Goodbye now!");
	}
}