package pg.grupag.mapeditor;

import java.io.File;

public class Map {

	private short width = 32, height = 32;
	
	byte[][][] objects = new byte[32][32][24];
	
	public short getWidth() {return width;}
	public short getHeight() {return height;}
	
	public Map() {
		
	}
	
	public Map(short w, short h) {
		width = w; height = h;
		objects = new byte[w][h][24];
	}
	
	public void putObject(byte object, int x, int y) {
		int free = 0;
		while(free<23 && objects[x][y][free]!=0)
			free++;
		if(free<24) objects[x][y][free] = object;
	}
	
	public void popObject(int x, int y) {
		int free = 0;
		while(free<23 && objects[x][y][free]!=0)
			free++;
		if(free-1>=0)
			objects[x][y][free-1] = 0;
	}
	
	public byte[] getObjects(int x, int y) {
		return objects[x][y];
	}
	
	public void save(File f) {
		//ZAPISYWANIE
	}
	
	public void load(File f) {
		//Czytanie!
	}

}
