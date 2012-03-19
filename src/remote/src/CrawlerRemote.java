/*
 */
import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.awt.event.MouseMotionListener;
import java.awt.Dimension;
import java.awt.event.MouseEvent;
import java.awt.GridLayout;
 
public class CrawlerRemote extends JPanel implements MouseMotionListener {
    ImagePanel blankArea;
    JTextArea textArea;
//    java.io.OutputStream outstream;
    static final String NEWLINE = System.getProperty("line.separator");
    
    SerialPortWriter xbee = new SerialPortWriter();
    
    public static void main(String[] args) throws Exception{
         
        /* Use an appropriate Look and Feel */
        try {
            //UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
            //UIManager.setLookAndFeel("com.sun.java.swing.plaf.gtk.GTKLookAndFeel");
            UIManager.setLookAndFeel("javax.swing.plaf.metal.MetalLookAndFeel");
        } catch (UnsupportedLookAndFeelException ex) {
            ex.printStackTrace();
        } catch (IllegalAccessException ex) {
            ex.printStackTrace();
        } catch (InstantiationException ex) {
            ex.printStackTrace();
        } catch (ClassNotFoundException ex) {
            ex.printStackTrace();
        }
        /* Turn off metal's use of bold fonts */
        UIManager.put("swing.boldMetal", Boolean.FALSE);
         
        //Schedule a job for the event dispatch thread:
        //creating and showing this application's GUI.
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                createAndShowGUI();
            }
        });
    }
     
    /**
     * Create the GUI and show it.  For thread safety,
     * this method should be invoked from the
     * event-dispatching thread.
     */
    private static void createAndShowGUI() {
        //Create and set up the window.
        JFrame frame = new JFrame("GPS Crawler Remote");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
         
        //Create and set up the content pane.
        JComponent newContentPane = new CrawlerRemote();
        newContentPane.setOpaque(true); //content panes must be opaque
        frame.setContentPane(newContentPane);
         
        //Display the window.
        frame.pack();
        frame.setVisible(true);
    }
     
    public CrawlerRemote() {
        super(new GridLayout(0,1));
        JPanel controlArea = new ImagePanel();
        add(controlArea);
         
        textArea = new JTextArea();
        textArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(textArea,
                JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
                JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        scrollPane.setPreferredSize(new Dimension(305, 200));
         
        add(scrollPane);
         
        //Register for mouse events on blankArea and panel.
        controlArea.addMouseMotionListener(this);
        addMouseMotionListener(this);
         
        setPreferredSize(new Dimension(600, 700));
        setBorder(BorderFactory.createEmptyBorder(10,10,10,10));
    }
     
    void eventOutput(String eventDescription, MouseEvent e) {
        int[] controlMap = { 
        	    0, 25, 50, 75, 100, 125, 175,
        	    200, 225, 250, 275, 300
        	};
        int servoSet = 6;
        int motorSet = 6;
        
    	if (e.getComponent().getClass().getName() == "CrawlerRemote$ImagePanel" && (e.getX() < 300) && (e.getY() < 300)){    		
    		for (int i = 0; i < controlMap.length; i = i + 1) {
            	if (e.getX() > controlMap[i]){servoSet = i;}
            	if (e.getY() > controlMap[i]){motorSet = i;}
            }
    		
    		
    		textArea.append(
                " setServo " + servoSet + " setMotor " + motorSet
                + NEWLINE);
        
    		
    		    try { 
    		       	xbee.writeToPort(servoSet, motorSet);
    		    }catch (Exception ee){
    	        }
    		
    		
    		textArea.setCaretPosition(textArea.getDocument().getLength());
    	}
    }
     
    public void mouseMoved(MouseEvent e) {
        eventOutput("Mouse moved", e);
    }
     
    public void mouseDragged(MouseEvent e) {
        eventOutput("Mouse dragged", e);
    }
    
    
    public class ImagePanel extends JPanel{
	    private BufferedImage image;
	    public ImagePanel() {
	       try {                
	          image = ImageIO.read(new File("/Users/blynch/Documents/code/gps-crawler/src/remote/resources/control.png"));
	       } catch (IOException ex) {
	            // handle exception...
	       }
	    }
	    @Override
	    public void paintComponent(Graphics g) {
	        g.drawImage(image, 0, 0, null); // see javadoc for more info on the parameters
	    }
	}
    
}