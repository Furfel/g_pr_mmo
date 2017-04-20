package pg.grupag.mapeditor;

import java.awt.GraphicsConfiguration;
import java.awt.HeadlessException;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.util.concurrent.locks.Lock;

import javax.swing.JCheckBoxMenuItem;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;

public class MapEditor extends JFrame implements ActionListener {

	private JMenuBar menuBar;
	private Edit edit;
	
	public MapEditor(String load) {
		
		//TODO jakies definicje obiektow niech wczytuje w jakiejs formie
		
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
		JMenu edit = new JMenu("Edit");
		JMenuItem flatten = new JMenuItem("Flatten");
		flatten.addActionListener(this);
		edit.add(flatten);
		JCheckBoxMenuItem showAmnt = new JCheckBoxMenuItem("Show depth");
		showAmnt.setState(false);
		showAmnt.addActionListener(this);
		edit.add(showAmnt);
		bar.add(edit);
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
			int ret = openFileChooser.showOpenDialog(null);
			if(ret == JFileChooser.APPROVE_OPTION)
				if(openFileChooser.getSelectedFile()!=null)
					edit.getMap().load(openFileChooser.getSelectedFile());
		} else if(e.getActionCommand().equalsIgnoreCase("save")) {
			int ret = saveFileChooser.showSaveDialog(null);
			if(ret == JFileChooser.APPROVE_OPTION)
				if(saveFileChooser.getSelectedFile()!=null)
					edit.getMap().save(saveFileChooser.getSelectedFile());
		} else if(e.getActionCommand().equals(JFileChooser.APPROVE_OPTION)) {
			if(e.getSource()==openFileChooser) {
				//Tutaj "otworz plik" zostalo wybrane w dialogu
			} else if(e.getSource()==saveFileChooser) {
				
			}
		} else if(e.getActionCommand().equalsIgnoreCase("flatten")) {
			edit.getMap().flatten();
		} else if(e.getActionCommand().equalsIgnoreCase("show depth")) {
			edit.showAmnt = !edit.showAmnt;
			((JCheckBoxMenuItem) e.getSource()).setState(edit.showAmnt);
		}
	}
	
	public void load(File f) {
		
	}
	
	public void save(File f) {
		
	}

}
