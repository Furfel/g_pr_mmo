package pg.grupag.mapeditor;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.LayoutManager;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.io.File;

import javax.imageio.ImageIO;
import javax.swing.JPanel;

public class Edit extends JPanel implements MouseMotionListener, MouseListener, KeyListener {

	public int x=-1, y=-1;
	public int offsetX=0, offsetY=0;
	public static final int VIEW_W=16, VIEW_H=16;
	private Map map = new Map();
	private Image sprites = null;
	private int spritesW=4, spritesH=2;
	private byte object=0;
	
	public Edit() {
		this.setPreferredSize(new Dimension(VIEW_W*32, VIEW_H*32));
		this.setFocusable(true);
		this.addMouseListener(this);
		this.addMouseMotionListener(this);
		this.addKeyListener(this);
		
		File imageFile = new File("../Klient/res/tiles.png");
		if(imageFile.exists()) try {
			sprites = ImageIO.read(imageFile);
			spritesW = sprites.getWidth(null)/32;
			spritesH = sprites.getHeight(null)/32;
		} catch(Exception e) {
			System.err.println("Couldn\'t load sprites!");
		}
	}

	public void paint(Graphics g) {
		g.clearRect(0, 0, getWidth(), getHeight());
		if(sprites != null) g.drawImage(sprites, 0, 0, getWidth(), getHeight(), 0, 0, 32, 32, null);
		g.setColor(Color.GRAY);
		for(int y=offsetY;y<=offsetY+VIEW_H;y++)
			for(int x=offsetX;x<=offsetX+VIEW_W;x++) {
				g.drawRect((x-offsetX)*32, (y-offsetY)*32, 32, 32);
					if(y>=0 && x>=0 && y<map.getHeight() && x<map.getWidth()) {
						byte[] spr = map.getObjects(x, y);
						for(int i=0;i<spr.length;i++) {
							if(spr[i]>0)
								g.drawImage(sprites, (x-offsetX)*32, (y-offsetY)*32, (x-offsetX)*32+32, (y-offsetY)*32+32, spr[i]%spritesW*32, spr[i]/spritesW*32, spr[i]%spritesW*32+32, spr[i]/spritesW*32+32, null);
						}
					}
				}
		if(this.x>0 && this.y>0) {
			int x = (this.x/32)*32;
			int y = (this.y/32)*32;
			g.setColor(Color.WHITE);
			g.drawImage(sprites, x, y, x+32, y+32, object%spritesW*32, object/spritesW*32, object%spritesW*32+32, object/spritesW*32+32, null);
			g.drawRect(x, y, 32, 32);
		}
	}

	@Override
	public void keyPressed(KeyEvent e) {
		if(e.getKeyCode()==KeyEvent.VK_DOWN) {
			if(offsetY<map.getHeight()-VIEW_H-2)
				offsetY++;
		} else if(e.getKeyCode()==KeyEvent.VK_UP) {
			if(offsetY>-VIEW_H+2)
				offsetY--;
		} else if(e.getKeyCode()==KeyEvent.VK_LEFT) {
			if(offsetX>-VIEW_W+2)
				offsetX--;
		} else if(e.getKeyCode()==KeyEvent.VK_RIGHT) {
			if(offsetX<map.getWidth()-VIEW_W-2)
				offsetX++;
		} else if(e.getKeyCode()==KeyEvent.VK_A) {
			if(object>0) object--;
		} else if(e.getKeyCode()==KeyEvent.VK_Z) {
			if(object<120) object++;
		}
		repaint();
	}

	@Override
	public void keyReleased(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void keyTyped(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseClicked(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseEntered(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseExited(MouseEvent e) {
		x=-1; y=-1;
	}

	@Override
	public void mousePressed(MouseEvent e) {
		requestFocus();
		requestFocusInWindow();
		int x = e.getX()/32;
		int y = e.getY()/32;
		map.putObject(object, x+offsetX, y+offsetY);
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseDragged(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mouseMoved(MouseEvent e) {
		x=e.getX(); y=e.getY();
		repaint();
	}
	
	public interface MapInterface {
		
	}
	
	
}
