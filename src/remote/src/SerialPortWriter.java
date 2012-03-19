import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;

public class SerialPortWriter {
        CommPortIdentifier pid;
        SerialPort serial;
        java.io.OutputStream outstream;
        
        public SerialPortWriter(){
            try {
            	pid = CommPortIdentifier.getPortIdentifier("/dev/cu.usbserial-A4008e3N");        
            }catch (Exception ee){
            	
            }
            try {
            	serial = (SerialPort)pid.open("crawler",2000);        	
            }catch (Exception ee){
            	
            }
        }
        
        
        public void writeToPort(int servo, int motor) throws Exception
        {


        	
          serial.setSerialPortParams(57600,SerialPort.DATABITS_8,SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);
          serial.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);
          outstream=serial.getOutputStream();
          
          char start = (char)255;
          
          outstream.write(start);
          outstream.write(servo);
          outstream.write(motor);
        }
}
