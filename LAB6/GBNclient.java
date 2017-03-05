import java.io.*;
import java.net.*;

class GBNclient
{
   
   /*
   PACKET_LENGTH=argv[]; first byte contains sequence no.
   PACKET_GEN_RATE = argv[];
   MAX_BUFFER_SIZE
   */
   /*
   Check for all parameters before starting the program
   */
   /* create thread to generate packets periodically AND store them in buffer. Drop newly generated packet if buffer is full */
   /* add sequence number to packet only if it's added to buffer */
   /* RTT = Round Trip Time */
   /* transmit based on windows size condition and start timeout 
      timeout=100ms for first 10 packets then,
      timeout= 2*RTT.average (milliseconds)
   */

   /* sort out all the command line pa */
  // public void in_parameter(String )


   public static void main(String args[]) throws Exception
   {  boolean debug=false;
      int i=0;
      int PACKET_LENGTH = 0;
      int PACKET_GEN_RATE = 0;
      int MAX_BUFFER_SIZE = 0;
      int WINDOW_SIZE = 1;
      int MAX_PACKETS = 10;
      int RETRANSMISSION_ATTEMPTS = 0;
      float RTTave = 0;
      String IPADDRESS = "localhost";
      int PORTNO = 4567;

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
      
      //while(true)
      //{
         BufferedReader inFromUser =
            new BufferedReader(new InputStreamReader(System.in));
         DatagramSocket clientSocket = new DatagramSocket();
         System.out.println(args.length);
         InetAddress IPAddress = InetAddress.getByName(IPADDRESS);
         int looprun = 1;
      while(true){   
         if (looprun > 3)
            break;
         else looprun++;
         byte[] sendData = new byte[1024];
         byte[] receiveData = new byte[1024];
         String sentence = inFromUser.readLine();
         sendData = sentence.getBytes();
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
         
      }
      System.out.println("PACKET_GEN_RATE = "+PACKET_GEN_RATE);
      System.out.println("PACKET_LENGTH = "+PACKET_LENGTH);
      System.out.println("RETRANSMISSION_RATIO = "+PACKET_GEN_RATE);
      clientSocket.close();
   }
}