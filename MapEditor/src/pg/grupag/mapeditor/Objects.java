package pg.grupag.mapeditor;

import java.awt.Graphics;
import java.awt.Image;

public class Objects {

	Obj[] objects = new Obj[64];
	
	public Objects() {
		objects[0] = new Obj(Obj.SMALL_OBJECT,new int[][]{{0}},false); //space
		objects[1] = new Obj(Obj.SMALL_OBJECT,new int[][]{{1}},false); //grass
		objects[2] = new Obj(Obj.SMALL_OBJECT,new int[][]{{2}},false); //wood
		objects[3] = new Obj(Obj.SMALL_OBJECT,new int[][]{{3}},false); //gravel
		objects[4] = new Obj(Obj.SMALL_OBJECT,new int[][]{{5}},true); //box
		objects[5] = new Obj(Obj.SMALL_OBJECT,new int[][]{{6},{7}},false); //fire
		objects[6] = new Obj(Obj.SMALL_OBJECT,new int[][]{{8}},true); //table
		objects[7] = new Obj(Obj.SMALL_OBJECT,new int[][]{{9}},true); //barrel
		objects[8] = new Obj(Obj.SMALL_OBJECT,new int[][]{{10}},false); //dirt
		objects[9] = new Obj(Obj.SMALL_OBJECT,new int[][]{{11}},false); //white marble
		objects[10] = new Obj(Obj.SMALL_OBJECT,new int[][]{{12}},false); //black marble
		objects[11] = new Obj(Obj.SMALL_OBJECT,new int[][]{{15}},false); //bag
		objects[12] = new Obj(Obj.SMALL_OBJECT,new int[][]{{16},{17}},true); //basin
		objects[13] = new Obj(Obj.SMALL_OBJECT,new int[][]{{18}},false); //statue
		objects[14] = new Obj(Obj.LARGE_OBJECT,new int[][]{{42,41,32,31}},false); //winter tree
		objects[15] = new Obj(Obj.LARGE_OBJECT,new int[][]{{44,43,34,33}},false); //blue stone
		objects[16] = new Obj(Obj.LARGE_OBJECT,new int[][]{{46,45,36,35}},false); //red stone
		objects[17] = new Obj(Obj.LARGE_OBJECT,new int[][]{{48,47,38,37}},false); //chojinka
		objects[18] = new Obj(Obj.SMALL_OBJECT,new int[][]{{20},{21},{22},{23}},false); //woda
	}
	
	public int drawSprites(int i, Graphics g, Image sprites, int x, int y, int offsetX, int offsetY, int spritesW, int elevation) {
		if(objects[i]==null) return -1;
		int[] spr = objects[i].getSprites();
		if(spr.length>1) {
			g.drawImage(sprites,
				(x-offsetX)*32-elevation*8, //lewy gorny rog kwadratu w oknie
				(y-offsetY)*32-elevation*8,
				(x-offsetX)*32+32-elevation*8,
				(y-offsetY)*32+32-elevation*8,
				spr[0]%spritesW*32, //lewy gorny rog kwadratu w pliku .png
				spr[0]/spritesW*32,
				spr[0]%spritesW*32+32,
				spr[0]/spritesW*32+32,
			null);
			g.drawImage(sprites,
					(x-offsetX-1)*32-elevation*8, //lewy gorny rog kwadratu w oknie
					(y-offsetY)*32-elevation*8,
					(x-offsetX-1)*32+32-elevation*8,
					(y-offsetY)*32+32-elevation*8,
					spr[1]%spritesW*32, //lewy gorny rog kwadratu w pliku .png
					spr[1]/spritesW*32,
					spr[1]%spritesW*32+32,
					spr[1]/spritesW*32+32,
			null);
			g.drawImage(sprites,
					(x-offsetX)*32-elevation*8, //lewy gorny rog kwadratu w oknie
					(y-offsetY-1)*32-elevation*8,
					(x-offsetX)*32+32-elevation*8,
					(y-offsetY-1)*32+32-elevation*8,
					spr[2]%spritesW*32, //lewy gorny rog kwadratu w pliku .png
					spr[2]/spritesW*32,
					spr[2]%spritesW*32+32,
					spr[2]/spritesW*32+32,
			null);
			g.drawImage(sprites,
					(x-offsetX-1)*32-elevation*8, //lewy gorny rog kwadratu w oknie
					(y-offsetY-1)*32-elevation*8,
					(x-offsetX-1)*32+32-elevation*8,
					(y-offsetY-1)*32+32-elevation*8,
					spr[3]%spritesW*32, //lewy gorny rog kwadratu w pliku .png
					spr[3]/spritesW*32,
					spr[3]%spritesW*32+32,
					spr[3]/spritesW*32+32,
			null);
		} else {
			g.drawImage(sprites,
					(x-offsetX)*32-elevation*8, //lewy gorny rog kwadratu w oknie
					(y-offsetY)*32-elevation*8,
					(x-offsetX)*32+32-elevation*8,
					(y-offsetY)*32+32-elevation*8,
					spr[0]%spritesW*32, //lewy gorny rog kwadratu w pliku .png
					spr[0]/spritesW*32,
					spr[0]%spritesW*32+32,
					spr[0]/spritesW*32+32,
				null);
		}
		if(objects[i].elevation) return elevation+1;
		return elevation;
	}
	

	public class Obj {
		public static final int SMALL_OBJECT=0;
		public static final int LARGE_OBJECT=1;
		
		public int size;
		public int[][] frames;
		private boolean elevation=false;
		public Obj(int size, int[][] sprites, boolean elevation) {
			this.size = size;
			int frames = sprites.length; 
			this.frames = new int[frames][size==LARGE_OBJECT?4:1];
			for(int i=0;i<frames;i++)
				for(int j=0;j<this.frames[i].length;j++)
				this.frames[i][j] = sprites[i][j];
			this.elevation = elevation;
		}
		
		public int[] getSprites() {
			return frames[(int) ((System.currentTimeMillis()/1000)%frames.length)];
		}
	}
}
