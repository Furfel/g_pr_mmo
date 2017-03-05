package pg.grupag.klient;

import java.awt.Dimension;
import java.awt.Graphics;

import javax.swing.JPanel;

public class JDisplay extends JPanel implements Runnable {

	private boolean draw=true; //Czy rysujemy czy konczymy watek
	public static final int PLAYER_VIEW_RADIUS=3;
	public static final int SCREEN_SQUARE_SIZE = (2*PLAYER_VIEW_RADIUS+1)*32;
	
	public JDisplay() {
		setPreferredSize(new Dimension(SCREEN_SQUARE_SIZE,SCREEN_SQUARE_SIZE));
	}
	
	public void paint(Graphics g) {
		g.clearRect(0, 0, getWidth(), getHeight());
	}
	
	public void run() {
		while(draw) {
			repaint();
			try {Thread.sleep(33);}catch(Exception e) {}
		}
	}

}
