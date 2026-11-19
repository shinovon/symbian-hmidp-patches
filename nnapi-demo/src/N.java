import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.midlet.MIDlet;

public class N extends MIDlet implements CommandListener, Runnable {

	private static Form f;
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
		f.addCommand(new Command("Start Opera Mobile", Command.SCREEN, 3));
		f.addCommand(new Command("Start S60 Browser", Command.SCREEN, 4));
		f.addCommand(new Command("Start CorePlayer", Command.SCREEN, 5));
		f.setCommandListener(this);
		Display.getDisplay(this).setCurrent(f);
		
		new Thread(this).start();
	}
	
	public void run() {
		try {
			f.append("version: " + System.getProperty("cc.nnproject.symbianext.version") + " (" + System.getProperty("cc.nnproject.symbianext.variant") + ")\n");
			f.append("ram: " + System.getProperty("com.nokia.memoryramfree") + "\n");
			f.append("battery: " + System.getProperty("com.nokia.mid.batterylevel") + "\n");
			f.append("network: " + System.getProperty("com.nokia.network.access") + "\n");
			f.append("tls: " + System.getProperty("cc.nnproject.symbianext.checkTLS12") + "\n");
			f.append("xplore: " + System.getProperty("cc.nnproject.symbianext.getInstalledSisVersion:0xa0000bcd") + "\n");
			f.append("mpgram: " + System.getProperty("cc.nnproject.symbianext.getMidletSuiteVersion:MPGram;nnproject") + "\n");
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
				String r = System.getProperty("cc.nnproject.symbianext.launchMidlet:MPGram;nnproject;1;-Durl=https://t.me/nnmidlets");
				if ("-11".equals(r)) {
					f.append("set ipc: " + System.getProperty("cc.nnproject.symbianext.setGlobalData:mpgram_launchUrl;https://t.me/nnmidlets") + "\n");
					f.append("foreground mpgram: " + System.getProperty("cc.nnproject.symbianext.bringMidletToForeground:MPGram;nnproject") + "\n");
				} else {
					f.append("launch mpgram: " + r + "\n");
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
			case 3: {
				String uid = "0x2002aa96";
				if ("true".equals(System.getProperty("cc.nnproject.symbianext.isAppInstalled:0x2000ca78"))) {
					// uiq
					uid = "0x2000ca78";
				}
				if ("true".equals(System.getProperty("cc.nnproject.symbianext.isAppRunning:"+uid))) {
					f.append("open opera: " + System.getProperty("cc.nnproject.symbianext.bringAppToForeground:"+uid+";open;https://nnproject.cc/") + "\n");
				} else {
					f.append("launch opera: " + System.getProperty("cc.nnproject.symbianext.launchApp:"+uid+";https://nnproject.cc/") + "\n");
				}
				break;
			}
			case 4: {
				String uid = "0x10008d39";
				if ("true".equals(System.getProperty("cc.nnproject.symbianext.isAppRunning:"+uid))) {
					f.append("open browser: " + System.getProperty("cc.nnproject.symbianext.bringAppToForeground:"+uid+";0;4 https://nnproject.cc/") + "\n");
				} else {
					f.append("launch browser: " + System.getProperty("cc.nnproject.symbianext.launchApp:"+uid+";4 https://nnproject.cc/") + "\n");
				}
				break;
			}
			case 5: {
				String uid = "0x20012fa6";
				if ("true".equals(System.getProperty("cc.nnproject.symbianext.isAppRunning:"+uid))) {
					f.append("coreplayer already running\n");
				} else {
					f.append("launch coreplayer: " + System.getProperty("cc.nnproject.symbianext.launchApp:"+uid+";E:\\video.mp4") + "\n");
				}
				break;
			}
			}
		}
	}

}
