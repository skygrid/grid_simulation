/* Copyright (c) 2006-2014. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

package LHCb;

import com.opencsv.CSVReader;
import org.simgrid.msg.*;
import org.simgrid.msg.Process;

import java.io.FileReader;
import java.util.*;


public class Scheduler extends Process {
	public HashMap<String, List<Task>> schedulingFile = new HashMap<>();

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

	// Check if there are any free host(tier1)

	// Return all available free hosts (tier1s)
	public ArrayList<Host> getFreeHost(ArrayList<Host> hosts){
		ArrayList<Host> freeHosts = new ArrayList<>();
		for (Host host : hosts){
			if (host.getProperty("busy").equals("no")){
				freeHosts.add(host);
			}
		}
		return freeHosts;
	}

	// Return fastest machine from the arraylist of hosts
	public Host getFastestHost(ArrayList<Host> hosts){
		HashMap<Double, Host> powerHost = new HashMap<>();
		for (int j = 0; j < hosts.size(); j++){
			powerHost.put(hosts.get(j).getSpeed(), hosts.get(j));
		}
		Host fastestHost = powerHost.get(Collections.max(powerHost.keySet()));
		return fastestHost;
	}
	public Host getFastestHost(HashSet<Host> hosts){
		HashMap<Double, Host> powerHost = new HashMap<>();
		for (Host host: hosts){
			powerHost.put(host.getSpeed(), host);
		}
		Host fastestHost = powerHost.get(Collections.max(powerHost.keySet()));
		return fastestHost;
	}

	public Host getNearestHost(Host fasthost, ArrayList<Host> locHosts){
		Host xHost = fasthost;
		double maxBandwidth = 0.0;
		for (Host host: locHosts){
			if(getBandWidthLink(new HashSet(Arrays.asList(fasthost.getName(), host.getName()))) > maxBandwidth){
				maxBandwidth = getBandWidthLink(new HashSet(Arrays.asList(fasthost.getName(), host.getName())));
				xHost = host;
			}
		}
		return xHost;
	}
	public ArrayList<Host> getFastestLinkBetweenHost(ArrayList<Host> freeHosts, ArrayList<Host> locHosts){
		ArrayList<Host> srcDstHost = new ArrayList<>();
		double maxBandwidth = 0.0;
		for (int i = 0; i < freeHosts.size(); i++){
			for (int j = 0; j < locHosts.size(); j++){
				if (getBandWidthLink(new HashSet(Arrays.asList(freeHosts.get(i), locHosts.get(j)))) > maxBandwidth){
					srcDstHost.removeAll(srcDstHost);
					maxBandwidth = getBandWidthLink(new HashSet(Arrays.asList(freeHosts.get(i), locHosts.get(j))));
					srcDstHost.add(freeHosts.get(i));
					srcDstHost.add(locHosts.get(j));
				}
			}
		}
		return srcDstHost;
	}


	public Double getBandWidthLink(HashSet hashSet){
		HashSet<String> hashSet1112 = new HashSet<>(Arrays.asList("Tier1_1", "Tier1_2"));
		HashSet<String> hashSet1113 = new HashSet<>(Arrays.asList("Tier1_1", "Tier1_3"));
		HashSet<String> hashSet1114 = new HashSet<>(Arrays.asList("Tier1_1", "Tier1_4"));
		HashSet<String> hashSet1115 = new HashSet<>(Arrays.asList("Tier1_1", "Tier1_5"));
		HashSet<String> hashSet1116 = new HashSet<>(Arrays.asList("Tier1_1", "Tier1_6"));
		HashSet<String> hashSet1117 = new HashSet<>(Arrays.asList("Tier1_1", "Tier1_7"));

		HashSet<String> hashSet1213 = new HashSet<>(Arrays.asList("Tier1_2", "Tier1_3"));
		HashSet<String> hashSet1214 = new HashSet<>(Arrays.asList("Tier1_2", "Tier1_4"));
		HashSet<String> hashSet1215 = new HashSet<>(Arrays.asList("Tier1_2", "Tier1_5"));
		HashSet<String> hashSet1216 = new HashSet<>(Arrays.asList("Tier1_2", "Tier1_6"));
		HashSet<String> hashSet1217 = new HashSet<>(Arrays.asList("Tier1_2", "Tier1_7"));

		HashSet<String> hashSet1314 = new HashSet<>(Arrays.asList("Tier1_3", "Tier1_4"));
		HashSet<String> hashSet1315 = new HashSet<>(Arrays.asList("Tier1_3", "Tier1_5"));
		HashSet<String> hashSet1316 = new HashSet<>(Arrays.asList("Tier1_3", "Tier1_6"));
		HashSet<String> hashSet1317 = new HashSet<>(Arrays.asList("Tier1_3", "Tier1_7"));

		HashSet<String> hashSet1415 = new HashSet<>(Arrays.asList("Tier1_4", "Tier1_5"));
		HashSet<String> hashSet1416 = new HashSet<>(Arrays.asList("Tier1_4", "Tier1_6"));
		HashSet<String> hashSet1417 = new HashSet<>(Arrays.asList("Tier1_4", "Tier1_7"));

		HashSet<String> hashSet1516 = new HashSet<>(Arrays.asList("Tier1_5", "Tier1_6"));
		HashSet<String> hashSet1517 = new HashSet<>(Arrays.asList("Tier1_5", "Tier1_7"));

		HashSet<String> hashSet1617 = new HashSet<>(Arrays.asList("Tier1_6", "Tier1_7"));



		Double bandWidth = 0.0;
		if (hashSet.equals(hashSet1112)){
			bandWidth = 13.430125;
		}
		else if (hashSet.equals(hashSet1113)){
			bandWidth = 31.430125;
		}
		else if (hashSet.equals(hashSet1114)){
			bandWidth = 23.430125;
		}
		else if (hashSet.equals(hashSet1115)){
			bandWidth = 45.430125;
		}
		else if (hashSet.equals(hashSet1116)){
			bandWidth = 41.430125;
		}
		else if (hashSet.equals(hashSet1117)){
			bandWidth = 18.430125;
		}
		else if (hashSet.equals(hashSet1213)){
			bandWidth = 12.430125;
		}
		else if (hashSet.equals(hashSet1214)){
			bandWidth = 37.430125;
		}
		else if (hashSet.equals(hashSet1215)){
			bandWidth = 22.45;
		}
		else if (hashSet.equals(hashSet1216)){
			bandWidth = 38.45;
		}
		else if (hashSet.equals(hashSet1217)){
			bandWidth = 28.98;
		}
		else if (hashSet.equals(hashSet1314)){
			bandWidth = 19.99;
		}
		else if (hashSet.equals(hashSet1315)){
			bandWidth = 13.56;
		}
		else if (hashSet.equals(hashSet1316)){
			bandWidth = 25.56;
		}
		else if (hashSet.equals(hashSet1317)){
			bandWidth = 29.33;
		}
		else if (hashSet.equals(hashSet1415)){
			bandWidth = 38.88;
		}
		else if (hashSet.equals(hashSet1416)){
			bandWidth = 39.03;
		}
		else if (hashSet.equals(hashSet1417)){
			bandWidth = 31.23;
		}
		else if (hashSet.equals(hashSet1516)){
			bandWidth = 27.77;
		}
		else if (hashSet.equals(hashSet1517)){
			bandWidth = 15.45;
		}
		else if (hashSet.equals(hashSet1617)){
			bandWidth = 19.022;
		}
		return bandWidth;
	}

   	public void main(String[] args) throws MsgException {


      	if (args.length < 3) {
	 		Msg.info("Scheduler needs 3 arguments (input mailbox, first output mailbox, last one)");
	 		Msg.info("Got "+args.length+" instead");
	 		System.exit(1);
      	}
		AppendToFile.delete();
		try {
			createInput();
		}catch (Exception e){
			System.out.println("Error of reading file");
		}
		Task releaseTask;
	  	int input = Integer.valueOf(args[0]);
	  	int firstOutput = Integer.valueOf(args[1]);
	  	int lastOutput = Integer.valueOf(args[2]);
      	int taskCount = compSize.size();
      	int tier1Count = lastOutput - firstOutput + 1;
      	Msg.info("Scheduler starts to work");

		Host[] hosts = Host.all();
		for (Host host: hosts){
			host.setProperty("busy", "no");
		}

		//Find the fastest tier1 from a given location
		for (int  i = 0; i < taskCount; i++){
			while (true) {
				ArrayList<Host> hostList = getFreeHost(locations.get(i));
				if (!hostList.isEmpty()){
					Host destination = getFastestHost(hostList);
					InstructionTask instructionTask = new InstructionTask(taskName.get(i), compSize.get(i), commSize.get(i), destination.getName());
					Msg.info("Sending " + taskName.get(i) + " to " + destination);
					instructionTask.dsend(destination.getName());
					Host.getByName(destination.getName()).setProperty("busy", "yes");
					break;
				}
				ArrayList<Host> nonhostList = getFreeHost(nonLocations.get(i));
				if (!nonhostList.isEmpty()) {
					Host destination = getFastestHost(nonhostList);
					String downloadFrom = getNearestHost(destination, locations.get(i)).getName();
					Msg.info("Sending " + taskName.get(i) + " to " + destination + " Download from " + downloadFrom + " " + Host.getByName(downloadFrom).getProperty("busy"));
					InstructionTask instructionTask = new InstructionTask(taskName.get(i), compSize.get(i), commSize.get(i), downloadFrom);
					instructionTask.dsend(destination.getName());
					Host.getByName(destination.getName()).setProperty("busy", "yes");
					break;
				}
				GiveMeReleasedHosts giveMeReleasedHosts = new GiveMeReleasedHosts();

				giveMeReleasedHosts.send("Tier0'");
				releaseTask = Task.receive("Tier0");

				if (releaseTask instanceof ReleasedHostTask){
					ReleasedHostTask rht = (ReleasedHostTask) releaseTask;
					ArrayList<Host> common = new ArrayList<Host>(locations.get(i));
					common.retainAll(rht.releasedHosts);
					if (!common.isEmpty()){
						Host destination = getFastestHost(common);
						InstructionTask instructionTask = new InstructionTask(taskName.get(i), compSize.get(i), commSize.get(i), destination.getName());
						Msg.info("Sending " + taskName.get(i) + " to " + destination);
						instructionTask.dsend(destination.getName());
						Host.getByName(destination.getName()).setProperty("busy", "yes");
					}else {
						Host destination = getFastestHost(rht.releasedHosts);
						String downloadFrom = getNearestHost(destination, locations.get(i)).getName();
						InstructionTask instructionTask = new InstructionTask(taskName.get(i), compSize.get(i), commSize.get(i), downloadFrom);
						Msg.info("Sending " + taskName.get(i) + " to " + destination + " Download from " + downloadFrom + " " + Host.getByName(downloadFrom).getProperty("busy"));
						instructionTask.dsend(destination.getName());
						Host.getByName(destination.getName()).setProperty("busy", "yes");
					}
					break;
				}
			}
		}

		Msg.info("All tasks have been dispatched. Let's tell everybody the computation is over.");
		for (int i = 1; i <= tier1Count; i++){
			FinalizeTask task = new FinalizeTask();
			FinalizeTask task1 = new FinalizeTask();
			task.send("Tier1_" + i);
			task1.send("Tier1_" + i + "'");
		}
		FinalizeTask finalizeTask = new FinalizeTask();
		finalizeTask.send("Tier0'");
		Msg.info("I'm done. See you!");
   }
}

