package pg.grupag.mapeditor;

import java.awt.GraphicsConfiguration;
import java.awt.HeadlessException;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;

public class MapEditor extends JFrame implements ActionListener {

	private JMenuBar menuBar;
	private Edit edit;
	
	public MapEditor(String load) {
		menuBar = new JMenuBar();
		constructMenu(menuBar);
		this.setJMenuBar(menuBar);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		edit = new Edit();
		this.add(edit);
		this.pack();
		this.setVisible(true);
		edit.requestFocus();
	}
	
	private void constructMenu(JMenuBar bar) {
		JMenu file = new JMenu("File");
		JMenuItem newItem = new JMenuItem("New");
		newItem.addActionListener(this);
		JMenuItem open = new JMenuItem("Open");
		open.addActionListener(this);
		JMenuItem save = new JMenuItem("Save");
		save.addActionListener(this);
		file.add(newItem); file.add(open); file.add(save);
		bar.add(file);
	}

	public static void main(String[] args) {
		if(args.length>=1)
			new MapEditor(args[0]);
		else new MapEditor(null);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		final JFileChooser openFileChooser = new JFileChooser();
		openFileChooser.addActionListener(this);
		final JFileChooser saveFileChooser = new JFileChooser();
		saveFileChooser.addActionListener(this);
		if(e.getActionCommand().equalsIgnoreCase("new")) {
			
		} else if (e.getActionCommand().equalsIgnoreCase("open")) {
			openFileChooser.showOpenDialog(null);
		} else if(e.getActionCommand().equalsIgnoreCase("save")) {
			saveFileChooser.showSaveDialog(null);
		} else if(e.getActionCommand().equals(JFileChooser.APPROVE_OPTION)) {
			if(e.getSource()==openFileChooser) {
				
			} else if(e.getSource()==saveFileChooser) {
				
			}
		}
	}
	
	public void load(File f) {
		
	}
	
	public void save(File f) {
		
	}

}
