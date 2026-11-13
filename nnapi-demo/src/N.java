import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.midlet.MIDlet;

public class N extends MIDlet implements CommandListener, Runnable {

	private static Form f;
	private String mpgramUid;
	private boolean started;

	protected void destroyApp(boolean unconditional) {
	}

	protected void pauseApp() {
	}

	protected void startApp() {
		if (started) return;
		started = true;
		
		f = new Form("test");
		f.addCommand(new Command("Exit", Command.EXIT, 0));
		f.addCommand(new Command("Start MPGram", Command.SCREEN, 1));
		f.addCommand(new Command("Start X-Plore", Command.SCREEN, 2));
		f.setCommandListener(this);
		Display.getDisplay(this).setCurrent(f);
		
		new Thread(this).start();
	}
	
	public void run() {
		try {
			f.append("version: " + System.getProperty("cc.nnproject.symbianext.version") + " (" + System.getProperty("cc.nnproject.symbianext.variant") + ")\n");
			f.append("ram: " + System.getProperty("com.nokia.memoryramfree") + "\n");
			f.append("battery: " + System.getProperty("com.nokia.mid.batterylevel") + "\n");
			f.append("tls: " + System.getProperty("cc.nnproject.symbianext.checkTLS12") + "\n");
			f.append("xplore: " + System.getProperty("cc.nnproject.symbianext.getInstalledSisVersion:0xa0000bcd") + "\n");
			f.append("mpgram: " + System.getProperty("cc.nnproject.symbianext.getMidletSuiteVersion:MPGram;nnproject") + "\n");
			mpgramUid = System.getProperty("cc.nnproject.symbianext.getMidletUid:MPGram;nnproject");
			f.append("mpgram uid: " + mpgramUid + "\n");
		} catch (Exception e) {
			e.printStackTrace();
			f.append(e.toString());
		}
	}

	public void commandAction(Command c, Displayable arg1) {
		if(c.getCommandType() == Command.EXIT) {
			notifyDestroyed();
		} else if (c.getCommandType() == Command.SCREEN) {
			switch (c.getPriority()) {
			case 1: {
				if ("true".equals(System.getProperty("cc.nnproject.symbianext.isAppRunning:" + mpgramUid))) {
					f.append("foreground mpgram: " + System.getProperty("cc.nnproject.symbianext.bringAppToForeground:" + mpgramUid) + "\n");
				} else {
					f.append("launch mpgram: " + System.getProperty("cc.nnproject.symbianext.launchMidlet:MPGram;nnproject;1;-Durl=https://t.me/nnmidlets") + "\n");
				}
				break;
			}
			case 2: {
				if ("true".equals(System.getProperty("cc.nnproject.symbianext.isAppRunning:0xa0000bcd"))) {
					f.append("foreground x-plore: " + System.getProperty("cc.nnproject.symbianext.bringAppToForeground:0xa0000bcd") + "\n");
				} else {
					f.append("launch x-plore: " + System.getProperty("cc.nnproject.symbianext.launchApp:0xa0000bcd") + "\n");
				}
				break;
			}
			}
		}
	}

}
