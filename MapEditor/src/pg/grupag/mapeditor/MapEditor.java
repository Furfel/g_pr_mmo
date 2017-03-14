package pg.grupag.mapeditor;

import java.awt.GraphicsConfiguration;
import java.awt.HeadlessException;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;

public class MapEditor extends JFrame implements ActionListener {

	private JMenuBar menuBar;
	
	public MapEditor() {
		menuBar = new JMenuBar();
		constructMenu(menuBar);
	}
	
	private void constructMenu(JMenuBar bar) {
		JMenu file = new JMenu();
		JMenuItem newItem = new JMenuItem("New");
		newItem.addActionListener(this);
		JMenuItem open = new JMenuItem("Open");
		open.addActionListener(this);
		JMenuItem save = new JMenuItem("Save");
		save.addActionListener(this);
		file.add(newItem); file.add(open); file.add(save);
		
	}

	public static void main(String[] args) {
		new MapEditor();
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if(e.getActionCommand().equalsIgnoreCase("new")) {
			
		} else if (e.getActionCommand().equalsIgnoreCase("open")) {
			JFileChooser filechooser = new JFileChooser();
		} else if(e.getActionCommand().equalsIgnoreCase("save")) {
			
		}
	}

}
