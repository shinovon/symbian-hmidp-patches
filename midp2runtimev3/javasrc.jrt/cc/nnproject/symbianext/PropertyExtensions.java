package cc.nnproject.symbianext;

import com.nokia.mj.impl.vmport.VmPort;

public class PropertyExtensions {

	static final String VERSION = "1.0";
	
	public static String getProperty(String property) {
		try {
			if ("cc.nnproject.symbianext.version".equals(property)) {
				return VERSION;
			}
			if ("cc.nnproject.symbianext.variant".equals(property)) {
				return "jrt1";
			}
			if ("cc.nnproject.symbianext.checkTLS12".equals(property)) {
				return _checkTLS12() == 1 ? "mbedtls" : "false";
			}
			if (property.startsWith("cc.nnproject.symbianext.isAppInstalled:")) {
				String[] params = parseParams(property, 1, 1);
				if (params == null) return null;
				
				return _isAppInstalled(getIntUid(params[0])) == 1 ? "true" : "false";
			}
			if (property.startsWith("cc.nnproject.symbianext.isAppRunning:")) {
				String[] params = parseParams(property, 1, 1);
				if (params == null) return null;

				return _isAppRunning(getIntUid(params[0])) == 1 ? "true" : "false";
			}
			if (property.startsWith("cc.nnproject.symbianext.launchApp:")) {
				String[] params = parseParams(property, 2, 1);
				if (params == null) return null;
				
				return Integer.toString(_launchApp(getIntUid(params[0]), params[1]));
			}
			if (property.startsWith("cc.nnproject.symbianext.bringAppToForeground:")) {
				String[] params = parseParams(property, 3, 1);
				if (params == null) return null;

				boolean sendMessage = false;
				int message = 0;
				if (params[1] != null) {
					message = Integer.parseInt(params[1]);
					sendMessage = true;
				}

				return Integer.toString(_bringAppToForeground(getIntUid(params[0]), sendMessage, message, params[2]));
			}
			if (property.startsWith("cc.nnproject.symbianext.getMidletUid:")) {
				String[] params = parseParams(property, 3, 2);
				if (params == null) return null;
				if (params[0] == null || params[1] == null) return null;
				
				int midletNum = 1;
				if (params[2] != null) {
					midletNum = Integer.parseInt(params[2]);
				}
				
				int r = _getMidletUid(params[0], params[1], midletNum);
				if (r == 0) return null;
				
				return "0x".concat(Integer.toString(r, 16));
			}
			if (property.startsWith("cc.nnproject.symbianext.getMidletSuiteVersion:")) {
				String[] params = parseParams(property, 2, 2);
				if (params == null) return null;
				if (params[0] == null || params[1] == null) return null;

				int[] ver = new int[3];
				int r = _getMidletSuiteVersion(params[0], params[1], ver);
				if (r != 0) return null;
				
				return ver[0] + "." + ver[1] + "." + ver[2];
			}
			if (property.startsWith("cc.nnproject.symbianext.launchMidlet:")) {
				String[] params = parseParams(property, 4, 2);
				if (params == null) return null;
				if (params[0] == null || params[1] == null) return null;
				
				int midletNum = 1;
				if (params[2] != null) {
					midletNum = Integer.parseInt(params[2]);
				}
				
				StringBuffer cmd = new StringBuffer();
				cmd.append(params[0]).append(';').append(params[1]).append(';').append(midletNum);
				if (params[3] != null) {
					cmd.append(';').append(params[3]);
				}
				
				return Integer.toString(_launchMidlet(params[0], params[1], midletNum, cmd.toString()));
			}
			if (property.startsWith("cc.nnproject.symbianext.bringMidletToForeground:")) {
				String[] params = parseParams(property, 3, 2);
				if (params == null) return null;
				if (params[0] == null || params[1] == null) return null;

				int midletNum = 1;
				if (params[2] != null) {
					midletNum = Integer.parseInt(params[2]);
				}

				return Integer.toString(_bringMidletToForeground(params[0], params[1], midletNum));
			}
			if (property.startsWith("cc.nnproject.symbianext.createGlobalData:")) {
				String[] params = parseParams(property, 1, 1);
				if (params == null || params[0] == null) return null;
				if (params[0].length() == 0 || params[0].length() > 32) return null;

				return Integer.toString(_createGlobalData("MIDP2Data." + params[0]));
			}
			if (property.startsWith("cc.nnproject.symbianext.setGlobalData:")) {
				String[] params = parseParams(property, 2, 2);
				if (params == null) return null;
				if (params[0] == null || params[1] == null) return null;
				if (params[0].length() == 0 || params[0].length() > 32 || params[1].length() > 1024) return null;

				return Integer.toString(_setGlobalData("MIDP2Data." + params[0], params[1]));
			}
			if (property.startsWith("cc.nnproject.symbianext.getGlobalData:")) {
				String[] params = parseParams(property, 1, 1);
				if (params == null || params[0] == null) return null;
				if (params[0].length() == 0 || params[0].length() > 32) return null;

				return _getGlobalData("MIDP2Data." + params[0]);
			}
			if (property.startsWith("cc.nnproject.symbianext.getInstalledSisVersion:")) {
				String[] params = parseParams(property, 1, 1);
				if (params == null) return null;

				int[] ver = new int[3];
				int r = _getInstalledSisVersion(getIntUid(params[0]), ver);
				if (r != 0) return null;
				
				return ver[0] + "." + ver[1] + "." + ver[2];
			}
		} catch (Throwable ignored) {
			ignored.printStackTrace();
			return ignored.toString();
		}
		return null;
	}
	
	private static String[] parseParams(String property, int total, int required) {
		int i = property.indexOf(':');
		if (i == -1 || i + 1 == property.length()) return null;
		
		String[] res = new String[total];
		int j, num = 0;
		while (num < total) {
			if (num == total - 1 || (j = property.indexOf(';', i + 1)) == -1) {
				res[num++] = property.substring(i + 1);
				break;
			}
			
			res[num++] = property.substring(i + 1, j);
			i = j;
		}
		
		if (num < required) return null;
		
		return res;
	}
	
	private static int getIntUid(String uid) {
		if (uid.startsWith("0x")) {
			return (int) Long.parseLong(uid.substring(2), 16);
		}
		return Integer.parseInt(uid);
	}
	
	private static native int _checkTLS12();
	
	private static native int _isAppInstalled(int uid);
	private static native int _isAppRunning(int uid);
	private static native int _launchApp(int uid, String doc);
	private static native int _bringAppToForeground(int uid, boolean sendMessage, int message, String data);
	
	private static native int _getMidletUid(String name, String vendor, int num);
	private static native int _getMidletSuiteVersion(String name, String vendor, int[] out);
	private static native int _launchMidlet(String name, String vendor, int num, String cmd);
	private static native int _bringMidletToForeground(String name, String vendor, int num);
	private static native int _createGlobalData(String name);
	private static native int _setGlobalData(String name, String data);
	private static native String _getGlobalData(String name);
	
	private static native int _getInstalledSisVersion(int uid, int[] out);
	
	static {
		try {
			VmPort.getInstance().System_loadLibrary("nnapi93");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
