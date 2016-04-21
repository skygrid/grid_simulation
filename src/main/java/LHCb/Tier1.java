/* Copyright (c) 2006-2014. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

package LHCb;

import org.simgrid.msg.*;
import org.simgrid.msg.Process;
import org.simgrid.trace.Trace;

public class Tier1 extends Process {
	public Tier1(Host host, String name, String[]args) {
		super(host,name,args);
	}
	public void main(String[] args) throws TransferFailureException, HostFailureException, TimeoutException, NativeException, MsgException {
		double timeWorking = 0.0;
		if (args.length < 1) {
			Msg.info("Tier1 needs 1 argument (its number)");
			System.exit(1);
		}
		int num = Integer.valueOf(args[0]);
		//Msg.info("Receiving on 'Tier1_"+num+"'");
		int numberOfTask = 0;
		double downloadByte = 0.0;
		double uploadByte = 0.0;


		while(true) {
			Task task = Task.receive("Tier1_"+num);
			if (task instanceof FinalizeTask) {
				break;
			}
			if (task instanceof InstructionTask){
				numberOfTask++;
				InstructionTask itask = (InstructionTask) task;
				getHost().setProperty("busy", "yes");
				if (itask.tier1DownloadFrom.equals(getHost().getName())){
					try {
						Msg.info("Receive " + itask.getName()+ " I have all data. Processing it");
						Task sometask = new Task(itask.getName(), itask.executeDataSize, itask.downloadDataSize);
						double timeStart = Msg.getClock();
						sometask.execute();
						double timeFinised = Msg.getClock();
						Msg.info("\"" + sometask.getName() + "\" done ");
						getHost().setProperty("busy", "no");
						ReleaseTask releaseTask = new ReleaseTask(getHost().getName());
						releaseTask.dsend("Tier0'");
						timeWorking += (timeFinised - timeStart);
						AppendToFile.appendToFile(itask.getName(), itask.timeStart, timeStart, Msg.getClock(), itask.executeDataSize);
					} catch (TaskCancelledException e){
						}
				} else {
					Msg.info("Receive " + itask.getName() + ". I don't have data" + " Request files from " + itask.tier1DownloadFrom);
					GiveMeDataTask giveMeDataTask = new GiveMeDataTask(itask.getName(), itask.executeDataSize, itask.downloadDataSize, getHost().getName(), itask.timeStart);
					giveMeDataTask.dsend(itask.tier1DownloadFrom + "'");
					}
			}
			if (task instanceof DataOtherHostTask){
				DataOtherHostTask dataOtherHostTask = (DataOtherHostTask) task;
				try {
					Msg.info("HAVE DOWNLOADED FROM " + dataOtherHostTask.senderName + " START TO PROCESS");
					downloadByte += dataOtherHostTask.getMessageSize();
					double timeStart = Msg.getClock();
					dataOtherHostTask.execute();
					double timeFinised = Msg.getClock();
					AppendToFile.appendToFile(dataOtherHostTask.getName(), dataOtherHostTask.timeStart, timeStart, Msg.getClock(), dataOtherHostTask.executeSize);
					ReleaseTask releaseTask = new ReleaseTask(getHost().getName());
					releaseTask.dsend("Tier0'");
					timeWorking += (timeFinised - timeStart);
					Msg.info("\"" + dataOtherHostTask.getName() + "\" done ");
					getHost().setProperty("busy", "no");
				}catch (TaskCancelledException e){
				}
			}
		}
		AppendToFile.nodeWorkload(getHost().getName() + ", " + String.valueOf(timeWorking) + ", " + numberOfTask + ", " + downloadByte + ", ");
		Msg.info("Received Finalize. I'm done. See you!");
	}
}
