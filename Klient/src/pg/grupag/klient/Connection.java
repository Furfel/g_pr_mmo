package pg.grupag.klient;

import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

public class Connection {
	private Socket socket;
	public static final int PORT=9528;
	
	public Connection(String server) throws Exception {
		try {
			socket = new Socket(server,PORT);
		} catch(UnknownHostException e) {
			System.err.println("Unknown host.");
			throw new Exception("Host unknown.");
		} catch(IOException e) {
			System.err.println(e.getMessage());
			throw new Exception("IOError");
		}
	}
	
	class ConnectionThread extends Thread {
		public ConnectionThread() {
			
		}
	}
}
