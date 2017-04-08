import java.io.*;
import java.net.*;
import java.util.*;
import java.lang.*;

public class cw{
	public static void main(String[] args) throws Exception{
		int i = 0;
		double cmd_i = 1.0;
		double cmd_m = 1.0;
		double cmd_n = 1.0;
		double cmd_f = 0.1;
		double cmd_s = 1.0;
		int cmd_T = 10;
		String outfile = "output";
		double congestion_threshold = 512.0;
		int RWS = 1024;
		int MSS = 1;
		boolean debug = false;
		boolean check = false;
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

		/*
			-i is for Ki 
			-m is for Km 
			-n is for Kn
			-f is for Kf
			-s is for Ps
			-T is for max packets
			-o is for output file
		*/
    	while (i<args.length)
    	{
        	check = false;
      		if(args[i].equals("-i"))
       		{
	          	if(i < args.length-1)
	          	{
	             	cmd_i = Double.parseDouble(args[i+1]);
	                check = true;
	                i++;
	          	}
	       	}
	       	if(args[i].equals("-m"))
	       	{
	            if(i < args.length-1)
	  	        {
	      	       	cmd_m = Double.parseDouble(args[i+1]);
	                check = true;
	                i++;
	          	}
	       	}
	        if(args[i].equals("-n"))
	  	    {
	      	    if(i < args.length-1)
	          	{
	             	cmd_n = Double.parseDouble(args[i+1]);
	                check = true;
	                i++;
	          	}
	       	}
	        if(args[i].equals("-f"))
	        {
	            if(i < args.length-1)
	            {
	                cmd_f = Double.parseDouble(args[i+1]);
	                check = true;
	                i++;
	            }
	        }
	        if(args[i].equals("-s"))
	        {
	            if(i < args.length-1)
	            {
	                cmd_s = Double.parseDouble(args[i+1]);
	                check = true;
	                i++;
	            }
	        }
	        if(args[i].equals("-T"))
	        {
	            if(i < args.length-1)
	            {
	                cmd_T = Integer.parseInt(args[i+1]);
	                check = true;
	                i++;
	            }
	        }
	        if(args[i].equals("-o"))
	        {
	            if(i < args.length-1)
	            {
	                outfile = args[i+1];
	                check = true;
	                i++;
	            }
	        }
	        if(check == false)
	          {
	              if(!(args[i].equals("-i") || args[i].equals("-m") || args[i].equals("-n") || args[i].equals("-f") || args[i].equals("-s") || args[i].equals("-T") || args[i].equals("-d") || args[i].equals("-o")))
	              {
	                  System.out.println("Wrong Parameter passed : "+args[i].substring(1));
	                  System.exit(0);
	              }
	          }
	       	i++;
	    }

	    if(debug == true)
	    {
	    	System.out.println("cmd_i = "+cmd_i);
	    	System.out.println("cmd_m = "+cmd_m);
	    	System.out.println("cmd_n = "+cmd_n);
	    	System.out.println("cmd_f = "+cmd_f);
	    	System.out.println("cmd_s = "+cmd_s);
	    	System.out.println("cmd_T = "+cmd_T);
	    	System.out.println("outfile = "+outfile);
	    }
	    int j=0;
	    i=0;

	    double congestion_window = cmd_i * MSS;
	    int SimTime=0;
	    FileWriter out = null;
	    String outstr;
	    try
	    {
	    	out = new FileWriter(outfile);
	    	while(i<cmd_T)
		    {	
		    	double N = Math.ceil(congestion_window/MSS);
		    	for(j=0;j<N;j++)
		    	{
		    		SimTime++;
		    		if(debug == true)
		    			System.out.print(SimTime+", ");

			    	if(Math.random()<cmd_s)
			    	{	
			    		if(j<congestion_threshold)
			    		{
			    			congestion_window = Math.min(congestion_window+(cmd_m*MSS), RWS);
			    			if(debug == true)
			    			{
			    				System.out.println("congestion_window_1 : "+congestion_window);
			    				//System.out.println(congestion_window);
			    			}
			    		}
			    		else
			    		{
			    			congestion_window = Math.min(congestion_window + (cmd_n*MSS*MSS/congestion_window), RWS);
			    			if(debug == true)
			    			{
			    				System.out.println("congestion_window_2 : "+congestion_window);
			    				//System.out.println(congestion_window);
			    			}
			    		}
			    		i++;
			    		outstr = SimTime + " " + congestion_window+"\n";
			    		out.write(outstr);
			    	}
			    	else
			    	{
			    		//congestion_threshold = Math.max(1,congestion_window/2);
			    		congestion_threshold = congestion_window/2;
			    		congestion_window = Math.max(1, cmd_f*congestion_window);
				    	if(debug == true)
				    	{	
				    		System.out.println("congestion_window_3 : "+congestion_window);
				    		System.out.println("congestion_threshold : "+congestion_threshold);
			    			//System.out.println(congestion_window);
			    		}
			    		outstr = SimTime + " " + congestion_window+"\n";
			    		out.write(outstr);
			    		break;
			    	}
		    	}
		    }
		    out.close();
	    }catch(IOException e){
	    	System.out.println("Exception Occurred");
	    }
	    System.out.println("Output has been saved to \'"+outfile+"\'");
	}
}