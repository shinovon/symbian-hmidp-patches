package javax.microedition.lcdui;

public class MascotMEHelper {

	public static Object getGraphicsTarget(Object graphics) {
		return ((Graphics) graphics).iTarget;
	}
	
}
