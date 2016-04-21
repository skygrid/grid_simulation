/* Copyright (c) 2006-2014. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

package LHCb;

import java.io.File;

import org.simgrid.msg.Msg;
import org.simgrid.msg.NativeException;
import org.simgrid.trace.Trace;

public class Masterslave {

   /* This only contains the launcher. If you do nothing more than than you can run
    *   java simgrid.msg.Msg
    * which also contains such a launcher
    */

    public static void main(String[] args) throws NativeException {
        /* initialize the MSG simulation. Must be done before anything else (even logging). */
        String[] aruments = {"/home/ken/IdeaProjects/Sim2Sim/src/main/java/LHCb/platform.xml", "/home/ken/IdeaProjects/Sim2Sim/src/main/java/LHCb/masterslaveDeployment.xml", "--cfg=tracing:yes", "--cfg=tracing/filename:simulation.trace", "--cfg=tracing/platform:yes", "--cfg=msg/debug_multiple_use:on"};
        //String[] aruments = {"platform.xml", "masterslaveDeployment.xml", "--cfg=tracing:yes", "--cfg=tracing/filename:simulation.trace", "--cfg=tracing/platform:yes"};
        Msg.init(aruments);

        //String platf  = args.length > 1 ? args[0] : "examples/java/platform.xml";
        //String deploy =  args.length > 1 ? args[1] : "examples/java/masterslave/masterslaveDeployment.xml";

        //Msg.verb("Platform: "+platf+"; Deployment:"+deploy+"; Current directory: "+new File(".").getAbsolutePath());

        /* construct the platform and deploy the application */
        Msg.createEnvironment(aruments[0]);
        Msg.deployApplication(aruments[1]);

        Trace.hostStateDeclare("State");
        Trace.hostStateDeclareValue("State", "Working", "1 0 0"); // Collider
        Trace.hostStateDeclareValue("State", "Waiting", "0 0 1");
        Trace.hostStateDeclareValue("State", "Uploading", "1 1 0");

        Msg.run();
    }
}
