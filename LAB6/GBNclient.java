import java.io.*;
import java.net.*;
import java.util.Random;

class Thread1 extends Thread {
   private Thread t;
   public String threadName;
   public int p_length;
   private Random rand_generator = new Random();
   public char[] packet;
   public int p_gen_rate;
   public int max_buff_s;
   public String[] buffer;

   Thread1( String name, int length, int gen_rate, int max_buff) {
      threadName = name;
      p_length = length;
      packet = new char[p_length];
      p_gen_rate = gen_rate;
      max_buff_s = max_buff;
      System.out.println("Creating " +  threadName );
   }
   
   public void run() {
      System.out.println("Running " +  threadName );
      buffer = new String[max_buff_s];
      try {int j =0;
         while(true)
         {   for(int i = 0; i < p_length; i++)
            {  int rand_num = rand_generator.nextInt() % 26;
               if (rand_num < 0)
                  rand_num = rand_num - 2*rand_num;
               packet[i] = (char)(rand_num+65);
            }
            System.out.println(packet);
            if(j<max_buff_s)
            {  
               buffer[j] = new String(packet);
               buffer[j] = (char)(j+48) + buffer[j].substring(1);
            }
            Thread.sleep(1000/p_gen_rate);
            j++;
            if(j >= max_buff_s)
               break;
         }
         
      }catch (InterruptedException e) {
         System.out.println("Thread " +  threadName + " interrupted.");
      }
      System.out.println("Thread " +  threadName + " exiting.");
   }
   
   public void start () {
      System.out.println("Starting " +  threadName );
      if (t == null) {
         t = new Thread (this, threadName);
         t.start ();
      }
   }
}

class GBNclient
{
      
   public static void main(String args[]) throws Exception
   {  
      boolean debug=false;
      int i=0;
      int PACKET_LENGTH = 10;
      int PACKET_GEN_RATE = 10000;
      int MAX_BUFFER_SIZE = 10;
      int WINDOW_SIZE = 1;
      int MAX_PACKETS = 10;
      int RETRANSMISSION_ATTEMPTS = 0;
      float RTTave = 2;
      String IPADDRESS = "localhost";
      int PORTNO = 4567;
      
      Random rand_generator = new Random(PACKET_LENGTH);

      while (i<args.length)
      {
         if(args[i].equals("-d"))
         {
            debug=true;
            System.out.println("Debug Mode is On.");
         }
         i++;
      }
      i=0;
      while (i<args.length)
      {
         if(args[i].equals("-s"))
         {
            if(i < args.length-1)
            {
               IPADDRESS = args[i+1];
               if (debug == true)
                  System.out.println("IPAddress = "+IPADDRESS);
            }
         }
         if(args[i].equals("-p"))
         {  
            if(i < args.length-1)
            {
               PORTNO = Integer.parseInt(args[i+1]);
               if (debug == true)
                  System.out.println("PORTNO = "+PORTNO);
            }
         };
         if(args[i].equals("-l"))
         {  
            if(i < args.length-1)
            {
               PACKET_LENGTH = Integer.parseInt(args[i+1]);
               if (debug == true)
                  System.out.println("PACKET_LENGTH = "+PACKET_LENGTH);
            }
         };
         if(args[i].equals("-r"))
         {
            if(i < args.length-1)
            {
               PACKET_GEN_RATE = Integer.parseInt(args[i+1]);
               if (debug == true)
                  System.out.println("PACKET_GEN_RATE = "+PACKET_GEN_RATE);
            }
         };
         if(args[i].equals("-n"))
         {
            if(i < args.length-1)
            {
               MAX_PACKETS = Integer.parseInt(args[i+1]);
               if (debug == true)
                  System.out.println("MAX_PACKETS = "+MAX_PACKETS);
            }
         };
         if(args[i].equals("-w"))
         {
            if(i < args.length-1)
            {
               WINDOW_SIZE = Integer.parseInt(args[i+1]);
               if (debug == true)
                  System.out.println("WINDOW_SIZE = "+WINDOW_SIZE);
            }
         };
         if(args[i].equals("-b"))
         {
            if(i < args.length-1)
            {
               MAX_BUFFER_SIZE = Integer.parseInt(args[i+1]);
               if (debug == true)
                  System.out.println("MAX_BUFFER_SIZE = "+MAX_BUFFER_SIZE);
            }
         };
         i++;
      }
      Thread1 T1 = new Thread1( "Thread-1", PACKET_LENGTH, PACKET_GEN_RATE, MAX_BUFFER_SIZE);
      T1.setPriority(Thread.MAX_PRIORITY);
      T1.start();
      System.out.println(T1.threadName);
      BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in));
      DatagramSocket clientSocket = new DatagramSocket();
      //System.out.println(args.length);
      InetAddress IPAddress = InetAddress.getByName(IPADDRESS);
      int looprun = 1;
      while(true)
      {   
         if (looprun > MAX_PACKETS)
            break;
         else
            looprun++;

         byte[] sendData = new byte[1024];
         byte[] receiveData = new byte[1024];

         String packet = new String(T1.packet);

         System.out.println("main "+packet);
         i=0;
         while(i < T1.buffer.length)
         {
            System.out.println("main buffer "+T1.buffer[i]);
            sendData = new String(T1.buffer[i]).getBytes();
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, PORTNO);
            clientSocket.send(sendPacket);
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
            clientSocket.receive(receivePacket);
            String modifiedSentence = new String(receivePacket.getData());
            System.out.println("FROM SERVER:" + modifiedSentence);
            System.out.println("Average RTT = " + "???");
            /* this also needs to printed in debug mode.
            Seq #: Time Generated: xx:yy RTT: zz      Number of Attempts: aa
            where time is in milliseconds:microseconds format
            */
            i++;
         }
      }
      System.out.println("PACKET_GEN_RATE = "+PACKET_GEN_RATE);
      System.out.println("PACKET_LENGTH = "+PACKET_LENGTH);
      System.out.println("RETRANSMISSION_RATIO = "+"...");
      clientSocket.close();
   }
}