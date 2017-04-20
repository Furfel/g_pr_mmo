package pg.grupag.mapeditor;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.concurrent.locks.Lock;

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
		try(DataOutputStream out = new DataOutputStream(new FileOutputStream(f))) {
			out.writeShort(width);
			out.writeShort(height);
			for(int y=0;y<getHeight();++y)
				for(int x=0;x<getWidth();++x)
					for(int z=0;z<24;z++)
						out.writeByte(objects[x][y][z]);
			out.close();
		} catch(Exception e) {
			System.err.println(e.getMessage());
		}
	}
	
	public void flatten() {
		for(int x=0;x<getWidth();++x)
			for(int y=0;y<getHeight();++y)
				for(int i=1;i<24;++i)
					objects[x][y][i]=0;
	}
	
	public synchronized void load(File f) {
		try(DataInputStream in = new DataInputStream(new FileInputStream(f))) {
			width = in.readShort();
			height = in.readShort();
			objects = new byte[width][height][24];
			for(int y=0;y<getHeight();++y)
				for(int x=0;x<getWidth();++x)
					for(int z=0;z<24;z++)
						objects[x][y][z]=in.readByte();
			in.close();
		} catch(Exception e) {
			System.err.println(e.getMessage());
		} finally {
		}
		//Czytanie!
	}

}
