package LHCb;

import org.simgrid.msg.*;
import org.simgrid.msg.Process;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;

/**
 * Created by ken on 14.04.16.
 */
public class Schdeuler2 extends Process {
    public Schdeuler2(Host host, String name, String args[]){
        super(host, name, args);
    }
    public void main(String args[]) throws MsgException{
        Task task;
        HashSet<Host> releasedHosts = new HashSet<>();
        while (true){
            task = Task.receive("Tier0'");
            if (task instanceof ReleaseTask){
                ReleaseTask rt = (ReleaseTask) task;
                releasedHosts.add(getHost().getByName(rt.releasedHost));
            }
            if (task instanceof GiveMeReleasedHosts){
                Msg.info("THERE ARE FREE " + releasedHosts);
                for (Host rhost: releasedHosts){
                    if (rhost.getProperty("busy").equals("yes")){
                        releasedHosts.remove(rhost);
                    }
                }
                if (!releasedHosts.isEmpty()){
                    ReleasedHostTask releasedHostTask = new ReleasedHostTask(releasedHosts);
                    releasedHostTask.dsend("Tier0");
                    releasedHosts.clear();
                }else {
                    Task listenTask = Task.receive("Tier0'");
                    if (listenTask instanceof ReleaseTask) {
                        ReleaseTask lt = (ReleaseTask) listenTask;
                        releasedHosts.add(getHost().getByName(lt.releasedHost));
                        ReleasedHostTask releasedHostTask = new ReleasedHostTask(releasedHosts);
                        releasedHostTask.send("Tier0");
                        releasedHosts.clear();
                    }
                }
            }
            if (task instanceof FinalizeTask){
                Msg.info("Receive Finalize. Buy");
                break;
            }
        }
    }
}
