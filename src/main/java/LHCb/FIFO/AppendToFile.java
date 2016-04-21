package LHCb.FIFO;

/**
 * Created by ken on 14.04.16.
 */

import java.io.*;

public class AppendToFile {
	public static void delete(){
		try(Writer writer = new BufferedWriter(new FileWriter("/home/ken/IdeaProjects/Sim2Sim/src/main/java/LHCb/FIFO/dataFiles/FIFOtaskTime.txt"))){
           writer.write("");
    	}catch(IOException e){
    		e.printStackTrace();
    	}
		try(Writer writer = new BufferedWriter(new FileWriter("/home/ken/IdeaProjects/Sim2Sim/src/main/java/LHCb/FIFO/dataFiles/FIFOschedulingTime.txt"))){
           writer.write("");
    	}catch(IOException e){
    		e.printStackTrace();
    	}
		try(Writer writer = new BufferedWriter(new FileWriter("/home/ken/IdeaProjects/Sim2Sim/src/main/java/LHCb/FIFO/dataFiles/FIFOnodeWorkload.txt"))){
           writer.write("NameTier1, WorkingTime, NumberOfTask, DownloadByte, UploadByte\n");
    	}catch(IOException e){
    		e.printStackTrace();
    	}

	}
	public static void appendScheduling(String s){
		try(Writer writer = new BufferedWriter(new FileWriter("/home/ken/IdeaProjects/Sim2Sim/src/main/java/LHCb/FIFO/dataFiles/FIFOschedulingTime.txt", true))){
           writer.write(s + "\n");
    	}catch(IOException e){
    		e.printStackTrace();
    	}
	}
    public static void appendToFile(String s) {
    	try(Writer writer = new BufferedWriter(new FileWriter("/home/ken/IdeaProjects/Sim2Sim/src/main/java/LHCb/FIFO/dataFiles/FIFOtaskTime.txt", true))){
           writer.write(s + "\n");
    	}catch(IOException e){
    		e.printStackTrace();
    	}
    }

	public static void nodeWorkload(String s) {
    	try(Writer writer = new BufferedWriter(new FileWriter("/home/ken/IdeaProjects/Sim2Sim/src/main/java/LHCb/FIFO/dataFiles/FIFOnodeWorkload.txt", true))){
           writer.write(s);
    	}catch(IOException e){
    		e.printStackTrace();
    	}
    }

}
