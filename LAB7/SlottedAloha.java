import java.io.*;
import java.net.*;
import java.util.*;
import java.lang.*;

class Slot{
  String[] buffer;
  int backoff_count;
  int CW;
  long time;
  int retransmission_attempts;
  Slot(int cw)
  {
    buffer = new String[2];
    buffer[0] = "";
    buffer[1] = "";
    backoff_count = 0;
    CW = cw;
    time = 0;
    retransmission_attempts = 0;
  };
}

public class SlottedAloha extends Thread
{ 
	static String Generate_Packet(int len){
      Random rand_generator = new Random();
      char[] packet = new char[len];
      for(int i=0;i<len;i++){
        int rand = rand_generator.nextInt(26)+1;
        packet[i] = (char)(rand+64);
        //System.out.println("random no. "+rand); 
      }
      return (new String(packet));
    };

    public static void main(String args[]) throws Exception
    {
 		int users = 2;
    	int CW = 2;
    	double PACKET_GEN_RATE = 1000;
    	int MAX_PACKETS = 10;
      	int PACKET_LENGTH = 10;
      	double PACKET_GEN_PROBABILITY = 0.5;
      	int SimTime = 0;
      	int i = 0;
    	int successful_transmission = 0;
      	Random rand_generator = new Random();
      	int counter;
      	boolean check;
      	long total_delay = 0;
      	int total_retransmission = 0;
      	ArrayList<String> transmiting_string = new ArrayList<String>();
        ArrayList<Integer> transmit = new ArrayList<Integer>();
        boolean debug = false;

      	/*
			N for no. of nodes
			M for max packets
			PR for packet genration rate
			W for window size
			L for packet length
			P for packet generation probability
			d for debug mode
      	*/
		i = 0;
		while (i<args.length)
		{
			if(args[i].equals("-d"))
       		{
	          	debug = true;
	            break;
	        }
	        i++;
		}

		i = 0;
    	while (i<args.length)
    	{
        	check = false;
      		if(args[i].equals("-N"))
       		{
	          	if(i < args.length-1)
	          	{
	             	users = Integer.parseInt(args[i+1]);
	                check = true;
	                i++;
	          	}
	       	}
	       	if(args[i].equals("-M"))
	       	{
	            if(i < args.length-1)
	  	        {
	      	       	MAX_PACKETS = Integer.parseInt(args[i+1]);
	                check = true;
	                i++;
	          	}
	       	}
	        if(args[i].equals("-PR"))
	  	    {
	      	    if(i < args.length-1)
	          	{
	             	PACKET_GEN_RATE = Double.parseDouble(args[i+1]);
	                check = true;
	                i++;
	          	}
	       	}
	        if(args[i].equals("-W"))
	        {
	            if(i < args.length-1)
	            {
	                CW = Integer.parseInt(args[i+1]);
	                check = true;
	                i++;
	            }
	        }
	        if(args[i].equals("-L"))
	        {
	            if(i < args.length-1)
	            {
	                PACKET_LENGTH = Integer.parseInt(args[i+1]);
	                check = true;
	                i++;
	            }
	        }
	        if(args[i].equals("-P"))
	        {
	            if(i < args.length-1)
	            {
	                PACKET_GEN_PROBABILITY = Double.parseDouble(args[i+1]);
	                check = true;
	                i++;
	            }
	        }
	        if(check == false)
	          {
	              if(!(args[i].equals("-N") || args[i].equals("-M") || args[i].equals("-PR") || args[i].equals("-W") || args[i].equals("-L") || args[i].equals("-P") || args[i].equals("-d")))
	              {
	                  System.out.println("Wrong Parameter passed : "+args[i].substring(1));
	                  System.exit(0);
	              }
	          }
	       	i++;
	    }
    	
    	if(debug == true)
    	{
	    	System.out.println("Debug mode is ON.");
	    	System.out.println("users = "+users);
	    	System.out.println("MAX_PACKETS = "+MAX_PACKETS);
	     	System.out.println("PACKET_GEN_RATE = "+PACKET_GEN_RATE);
	     	System.out.println("PACKET_GEN_PROBABILITY = "+PACKET_GEN_PROBABILITY);
	     	System.out.println("Collision Window = "+CW);
	     	System.out.println("PACKET_LENGTH = "+PACKET_LENGTH);
	    }

      	Slot[] user = new Slot[users];
      	for(i=0;i<users;i++)
      	{
        	user[i] = new Slot(CW);
        }
        SimTime = 0;
        
        while(true)
        {
        	counter = 0;
        	while(transmit.isEmpty() == false)
        	{
        		transmit.remove(0);
        	}
        	
        	for(i = 0; i < users; i++)
        	{
        		if(Math.random() < PACKET_GEN_PROBABILITY)
        		{
        			if(user[i].buffer[0].equals(""))
        			{
        				//Thread.sleep(1000/((long)PACKET_GEN_RATE));
        				user[i].buffer[0] = Generate_Packet(PACKET_LENGTH);
        			}
        			else if(user[i].buffer[1].equals(""))
        			{
        				//Thread.sleep(1000/((long)PACKET_GEN_RATE));
        				user[i].buffer[1] = Generate_Packet(PACKET_LENGTH);
        			}
        			if(user[i].time == 0)
        			{
        				user[i].time = System.currentTimeMillis();
        			}	
        			if(user[i].backoff_count == 0)
        			{
        				counter++;
        				transmit.add(i);
        			}
        			else
        				user[i].backoff_count--;
        		}
        	}
        	
        	if(debug == true)
	        {
	        	for( int value : transmit)
	        		System.out.println("user : "+value+"\nwhile counter : "+SimTime+"\nnode buffer 0 : "+user[value].buffer[0]+"\nnode buffer 1 :"+user[value].buffer[1]);

	        	System.out.println("counter : "+counter+"\ntransmit size : "+transmit.size());
	        }

        	if(counter == 1)
        	{
    			int value = transmit.get(0);
    			long time = System.currentTimeMillis();
    			long packet_delay = time - user[value].time;
    			total_delay = total_delay + packet_delay;
    			transmiting_string.add(user[value].buffer[0]);
    			if(debug == true)
    			{
					System.out.println("Transmiting Packet : "+user[value].buffer[0]);
    			}
    			user[value].buffer[0] = "";
    			user[value].buffer[0] = user[value].buffer[1];
    			user[value].buffer[1] = "";
    			user[value].time = 0;
    			user[value].CW = Math.max(2, (int)(user[value].CW * 0.75));
    		
        		successful_transmission++;
        		if(successful_transmission == MAX_PACKETS)
        			break;
        	}
        	else
        	{
        		for(int value : transmit)
        		{
        			int max = user[value].CW;
        			user[value].backoff_count = rand_generator.nextInt(max);
        			user[value].CW = Math.min(256, user[value].CW * 2);
        			user[value].retransmission_attempts++;
        			if(debug == true)
    				{
        				System.out.println("Retransmission Attempts for user "+value+" : "+user[value].retransmission_attempts);
        			}

        			if(user[value].retransmission_attempts > 10)
        			{	
        				System.out.println("Retransmission Limit Exceeeded");
        				System.exit(0);
        			}
        		}
        	}
        	SimTime++;
        }
        if(debug == true)
    	{
	     	i=0;
	     	for( String str : transmiting_string)
	    	{
	    		System.out.println("Transmited Packet "+i+" : "+str);
	    		i++;
	    	}

	    	for(i = 0; i < users; i++)
	    	{
	    		total_retransmission = total_retransmission + user[i].retransmission_attempts;
	    	}

	    	System.out.println("Total Retransmission Attempts :"+total_retransmission);
        	System.out.println("Simulation Time :"+SimTime);
	    }

        //System.out.print(CW+", "+PACKET_GEN_PROBABILITY+", ");
        System.out.println("No. Of Nodes(N): "+users+", Collision Window(W): "+CW+", Packet Generation Probability(P): "+PACKET_GEN_PROBABILITY);
        System.out.println("Utilization: "+(double)MAX_PACKETS/SimTime);
        //System.out.println((double)MAX_PACKETS/SimTime);
		System.out.println("Average packet delay(ms): " + (double)total_delay/MAX_PACKETS );
	}
}