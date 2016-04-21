/* Copyright (c) 2006-2014. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

package LHCb;
import org.simgrid.msg.Task;

public class FinalizeTask extends InstructionTask {
   public FinalizeTask() {
      super("finalize",0,0, "s");
   }
}
    
