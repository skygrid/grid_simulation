/* Master of a basic master/slave example in Java */

/* Copyright (c) 2006-2014. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

package LHCb;
import com.opencsv.CSVReader;
import org.simgrid.msg.*;
import org.simgrid.msg.Process;
import org.simgrid.trace.Trace;

import java.io.FileReader;
import java.util.ArrayList;

public class Collider extends Process {
	public ArrayList<Double> delays = new ArrayList<>();
    public ArrayList<Double> compSize = new ArrayList<>();
    public ArrayList<Double> commSize = new ArrayList<>();

	public Collider(Host host, String name, String[]args) {
		super(host,name,args);
	} 

	public void createInput() throws Exception{
        CSVReader reader = new CSVReader(new FileReader("/home/ken/IdeaProjects/Sim2Sim/src/main/java/LHCb/exmaple.txt"), ',', '\'', 1);
        String[] nextline;
        while ((nextline = reader.readNext()) != null){
            delays.add(Double.parseDouble(nextline[0]));
            compSize.add(Double.parseDouble(nextline[1]));
            commSize.add(Double.parseDouble(nextline[2]));
        }
    }

	public void main(String[] args) throws MsgException{
		if (args.length < 4) {
			Msg.info("Collider needs 4 arguments");
			System.exit(1);
		}
		try {
			createInput();
		}catch (Exception e){
			System.out.println("Collider Error of reading file");
		}

		Trace.hostPushState(getHost().getName(), "State", "ColliderWorking");

		int tasksCount = commSize.size();
		double taskComputeSize = Double.valueOf(args[1]);
		double taskCommunicateSize = Double.valueOf(args[2]);

		int tier1sCount = Integer.valueOf(args[3]);

		Msg.info("Hello! Got "+  tier1sCount + " tier1s and "+tasksCount+" tasks to process");

		for (int i = 0; i < tasksCount; i++) {
			Task task = new Task("Task_" + i, compSize.get(i), commSize.get(i));
			try {
				Msg.info("Collider starts sending task to scheduler");
				task.send("task");
				waitFor(delays.get(i));
			}catch (TimeoutException e){

			}

		}
		//Trace.hostPushState(getHost().getName(), "State", "ColliderWaiting");

		Msg.info("All tasks have been dispatched. Let's tell everybody the computation is over.");

		//Trace.hostPushState(getHost().getName(), "State", "ColliderWorking");

		FinalizeTask task = new FinalizeTask();
		task.send("task");

		//Trace.hostPushState(getHost().getName(), "State", "waiting");
		Trace.hostPopState(getHost().getName(), "State");
		Msg.info("Goodbye now!");
	}
}
