package LHCb;

/**
 * Created by ken on 12.04.16.
 */
import java.io.*;

public class AppendToFile {
	public static void delete(){
		try(Writer writer = new BufferedWriter(new FileWriter("/home/ken/IdeaProjects/Sim2Sim/src/main/java/LHCb/dataFiles/taskTime.txt"))){
           writer.write("");
    	}catch(IOException e){
    		e.printStackTrace();
    	}
		try(Writer writer = new BufferedWriter(new FileWriter("/home/ken/IdeaProjects/Sim2Sim/src/main/java/LHCb/dataFiles/schedulingTime.txt"))){
           writer.write("");
    	}catch(IOException e){
    		e.printStackTrace();
    	}
		try(Writer writer = new BufferedWriter(new FileWriter("/home/ken/IdeaProjects/Sim2Sim/src/main/java/LHCb/dataFiles/nodeWorkload.txt"))){
           writer.write("NameTier1, WorkingTime, NumberOfTask, DownloadByte, UploadByte\n");
    	}catch(IOException e){
    		e.printStackTrace();
    	}

	}
    public static void appendToFile(String s, double num1, double num2, double num3, double num4) {
		String formatStr = "%s, %.4f, %.4f, %.4f, %.1f%n";
    	try(Writer writer = new BufferedWriter(new FileWriter("/home/ken/IdeaProjects/Sim2Sim/src/main/java/LHCb/dataFiles/taskTime.txt", true))){
           writer.write(String.format(formatStr, s, num1, num2, num3, num4));
    	}catch(IOException e){
    		e.printStackTrace();
    	}
    }

	public static void nodeWorkload(String s) {
    	try(Writer writer = new BufferedWriter(new FileWriter("/home/ken/IdeaProjects/Sim2Sim/src/main/java/LHCb/dataFiles/nodeWorkload.txt", true))){
           writer.write(s);
    	}catch(IOException e){
    		e.printStackTrace();
    	}
    }

}
