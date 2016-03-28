/* Copyright (c) 2006-2014. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

package LHCb;
import org.simgrid.msg.Host;
import org.simgrid.msg.Msg;
import org.simgrid.msg.MsgException;
import org.simgrid.msg.Task;
import org.simgrid.msg.Process;


public class Scheduler extends Process {
	public Scheduler(Host host, String name, String[]args) {
		super(host,name,args);
	}
   public void main(String[] args) throws MsgException {
      if (args.length < 3) {
	 Msg.info("Scheduler needs 3 arguments (input mailbox, first output mailbox, last one)");
	 Msg.info("Got "+args.length+" instead");
	 System.exit(1);
      }
      int input = Integer.valueOf(args[0]);
      int firstOutput = Integer.valueOf(args[1]);
      int lastOutput = Integer.valueOf(args[2]);

      int taskCount = 0;
      int tier1Count = lastOutput - firstOutput + 1;
      Msg.info("Tier0 starts to work");

      while(true) {
	  Task task = Task.receive("task");

	  if (task instanceof FinalizeTask) {
	    Msg.info("Got a finalize task. Let's forward that we're done.");

	    for (int cpt = firstOutput; cpt<=lastOutput; cpt++) {
	       Task tf = new FinalizeTask();
	       tf.send("tier1_"+cpt);
	    }
	    break;
	  }
	  int dest = firstOutput + (taskCount % tier1Count);

	  Msg.info("Sending \"" + task.getName() + "\" to \"Tier1_" + dest + "\"");
	  task.send("tier1_"+dest);

	  taskCount++;
       }


      Msg.info("I'm done. See you!");
   }
}

