import java.io.*;
import java.net.*;
import java.util.Random;
import java.lang.*;

class Slot{
  String[] buffer;
  Slot()
  {
    buffer = new String[2];
    //if(Math.random() > p_gen_rate)
    //  Generate_Packet(5);
  };
  void Generate_Packet(int len){
    int j=0;
    Random rand_generator = new Random();
    char[] packet = new char[len];
    for(int i=0;i<len;i++){
      int rand = rand_generator.nextInt(26)+1;
      packet[i] = (char)(rand+64);
      //System.out.println("random no. "+rand); 
    }
    if(buffer[0] != null)
    {
      if(buffer[1] == null)
      {
        buffer[1] = new String(packet);
        j=2;
      }
    }
    else
    {
      buffer[0] = new String(packet);
      j=1;
    }
    if(j==1)
      System.out.println(buffer[0]);
    else if (j==2) 
      System.out.println(buffer[1]);
    else
      System.out.println("Buffer is full.");
  };
}

public class SlottedAloha extends Thread
{
   public static void main(String args[]) throws Exception
   {
 		 	int users=2;
    	int CW = 2;
    	double PACKET_GEN_RATE = 1000;
    	int MAX_PACKETS = 10;
      int PACKET_LENGTH = 10;
      int SimTime = 0;
    	int i=0;
    	while (i<args.length)
    	{
      	if(args[i].equals("-N"))
       	{
          	if(i < args.length-1)
          	{
             		users = Integer.parseInt(args[i+1]);
          	}
       	}
       	if(args[i].equals("-M"))
       	{
            if(i < args.length-1)
  	        {
      	       	MAX_PACKETS = Integer.parseInt(args[i+1]);
          	}
       	}
        if(args[i].equals("-P"))
  	    {
      	    if(i < args.length-1)
          	{
             		PACKET_GEN_RATE = Double.parseDouble(args[i+1]);
          	}
       	}
        if(args[i].equals("-W"))
        {
            if(i < args.length-1)
            {
                CW = Integer.parseInt(args[i+1]);
            }
        }
        if(args[i].equals("-L"))
        {
            if(i < args.length-1)
            {
                PACKET_LENGTH = Integer.parseInt(args[i+1]);
            }
        }
       	i++;
    	}
    	
    	System.out.println("users = "+users);
    	System.out.println("MAX_PACKETS = "+MAX_PACKETS);
      System.out.println("PACKET_GEN_RATE = "+PACKET_GEN_RATE);
      System.out.println("Collision Window = "+CW);
    	
      Slot[] user = new Slot[users];
      
      for(int j=0;j<users;j++)
          user[j] = new Slot();
     	
      while(true)
      {
        SimTime++;
        //System.out.println("Random = " + Math.random() );
        //System.out.println( user[] );
        for(i=0;i<users; i++)
          user[i].Generate_Packet(PACKET_LENGTH);
        if(SimTime > 2)
          break;
        Thread.sleep(1000);
      }
   	}
}