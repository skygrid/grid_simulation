package LHCb.SmartTolerant;

import org.simgrid.msg.Host;
import org.simgrid.msg.MsgException;
import org.simgrid.msg.Process;

/**
 * Created by ken on 17.04.16.
 */
public class AnomalyProcess extends Process{
    public AnomalyProcess(Host host, String name, String args[]){
        super(host, name, args);
    }
    public void main(String[] args) throws MsgException{

    }
}
