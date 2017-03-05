import java.io.*;
import java.net.*;
//import java.util.Random;

class GBNserver
{
   public static void main(String args[]) throws Exception
   {  
      boolean debug = false;
      int PORTNO = 4567;
      int MAX_PACKETS = 10;
      double RANDOM_DROP_PROB = 0.2;
      int i=0;
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
         if(args[i].equals("-p"))
         {
            if(i < args.length-1)
            {
               PORTNO = Integer.parseInt(args[i+1]);
               if (debug == true)
                  System.out.println("PORTNO = "+PORTNO);
            }
         }
         if(args[i].equals("-n"))
         {
            if(i < args.length-1)
            {
               MAX_PACKETS = Integer.parseInt(args[i+1]);
               if (debug == true)
                  System.out.println("MAX_PACKETS = "+MAX_PACKETS);
            }
         }
         if(args[i].equals("-e"))
         {
            if(i < args.length-1)
            {
               RANDOM_DROP_PROB = Double.parseDouble(args[i+1]);
               if (debug == true)
                  System.out.println("MAX_PACKETS = "+MAX_PACKETS);
            }
         }
         i++;
      }
      if (debug == true)
         System.out.println(RANDOM_DROP_PROB);
      
      DatagramSocket serverSocket = new DatagramSocket(PORTNO);
      byte[] receiveData = new byte[1024];
      byte[] sendData = new byte[1024];
      //Random rand_generator = new Random();
      while(true)
         {  
            //double rand_prob = rand_generator.nextDouble();
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
            serverSocket.receive(receivePacket);
            /*if(rand_prob <= RANDOM_DROP_PROB)
            {  
               //System.out.println("i = "+i+"\t\t"+rand_prob);
               System.out.println("Packet Dropped");
               continue;
            }*/
            String sentence = new String( receivePacket.getData());
            System.out.println("RECEIVED: " + sentence);
            InetAddress IPAddress = receivePacket.getAddress();
            int port = receivePacket.getPort();
            String capitalizedSentence = sentence.toUpperCase();
            sendData = capitalizedSentence.getBytes();
            DatagramPacket sendPacket =
            new DatagramPacket(sendData, sendData.length, IPAddress, port);
            serverSocket.send(sendPacket);
            receiveData=null;
            receiveData = new byte[1024];
            sendData=null;
            sendData = new byte[1024];
         }
   }
}