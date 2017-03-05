package pg.grupag.klient;

import java.util.Random;

import javax.swing.JFrame;

public class Client {

	private JFrame window;
	private JDisplay display;
	private Connection connection;
	
	public Client(String name, String server) {
		window = new JFrame(name+"@"+server);
		window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		window.setResizable(false);
		display = new JDisplay();
		window.add(display);
		window.pack();
		window.setVisible(true);
	}
	
	public void setConnection(Connection con) {
		connection = con;
	}
	
	public static void main(String[] args) {
		String name = "Player #"+System.currentTimeMillis()%9999;
		String server = "127.0.0.1";
		if(args.length>1)
			server = args[1];
		if(args.length>2)
			name = args[2];
		
		try {
			Connection connection = new Connection(server);
			Client client = new Client(name,server);
			client.setConnection(connection);
		} catch(Exception e) {
			System.exit(1);
		}
		
	}

}
