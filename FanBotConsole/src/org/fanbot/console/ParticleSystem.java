/*
  Fanbot console
  Android console program for the fanbot project
  See: www.fanbot.org
  Writting for Android (tested on Samsung S3 mobile)
  
    Copyright (C) 2013 David Righart

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
package org.fanbot.console;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Environment;
import android.util.Log;
import android.widget.Toast;

public class ParticleSystem {
	private static final String DEFAULT_PORT = "2013";
	private static final String DEFAULT_IP_ADDRESS = "192.168.2.12";
	public final static int TILES_X = 50;
	public final static int TILES_Y = 20;
	public final static String CONFIG_FILE = "fanbotconsole.properties";

	private int startX = 0;
	private int startY = 0;
	private int pan = 1;
	private int sizeX = 5;
	private int sizeY = 5;
	private Paint paintPos;
	private Paint paintNeg;
	private Paint mLabelPaint;
	private final Particle[][] particles = new Particle[TILES_X][TILES_Y];
	private boolean state = true;
	private List<ManualPaint> manualPaint = new ArrayList<ManualPaint>(); 
	private List<PreSelect> preSelect = new ArrayList<PreSelect>();
	private PreSelect currentPreSelect;
	private Context context;
	
	private String serverIpAddress = DEFAULT_IP_ADDRESS;
	private int serverPort = 2013;
	private ClientThread client;
	
	public ParticleSystem(final Context context, final Paint paintPos, final Paint paintNeg, int startX, int startY,
			int pan, int sizeX, int sizeY) {
		super();
		this.startX = startX;
		this.startY = startY;
		this.pan = pan;
		this.sizeX = sizeX;
		this.sizeY = sizeY;
		this.paintNeg = paintNeg;
		this.paintPos = paintPos;
		this.context = context;
		
		mLabelPaint = new Paint();
        mLabelPaint.setTextSize(38);
        mLabelPaint.setColor(Color.GREEN);
        mLabelPaint.setAlpha(90);
        
		init();
	}
	
	private void init() {
		int x = startX;
		int y = startY;
		for (int j = 0; j < TILES_Y; j++) {
			for (int i = 0; i < TILES_X; i++) {
				final Particle particle = new Particle(x, y, sizeX, sizeY, paintPos, paintNeg);
				particles[i][j] = particle;
				x += sizeX + pan;
			}
			x = startX;
			y += sizeY + pan;
		}
		
		int xx = TILES_X * (sizeX + pan) + startX;
		int yy = TILES_Y * (sizeY + pan) + startY;
		
		manualPaint.add(new ManualPaint(xx - 250, yy + 30, 100, 50, paintPos, true));
		manualPaint.add(new ManualPaint(xx - 100, yy + 30, 100, 50, paintNeg, false));
		
		preSelect.add(new PreSelect(this, startX, yy + 30, true));
		currentPreSelect = preSelect.get(0);
		preSelect.add(new PreSelect(this, startX + 225, yy + 30, false));
		preSelect.add(new PreSelect(this, startX + 2 * 225, yy + 30, false));
		preSelect.add(new PreSelect(this, startX + 3 * 225, yy + 30, false));
	}
	
	public void draw(final Canvas canvas) {
		for (int j = 0; j < TILES_Y; j++) {
			for (int i = 0; i < TILES_X; i++) {
				particles[i][j].draw(canvas);
			}
		}
		manualPaint.get(0).draw(canvas);
		manualPaint.get(1).draw(canvas);
		
		for (PreSelect p : preSelect) {
			p.draw(canvas);
		}
		
		int xx = TILES_X * (sizeX + pan) + startX;
		int yy = TILES_Y * (sizeY + pan) + startY;
		canvas.drawText("clear preselect", xx - 250, yy + 120, mLabelPaint);
	}
	
	public void hit(final int eventX, final int eventY) {
		int x = (eventX - startX) / (pan + sizeX);
		int y = (eventY - startY) / (pan + sizeY);
		
		if (x >= 0 && x < TILES_X && y >= 0 && y < TILES_Y) {
			particles[x][y].hit(eventX, eventY, state, currentPreSelect, x, y);
			if (client != null) { 
				client.setChanged(System.currentTimeMillis());
			}
		}
		
		if (manualPaint.get(0).hit(eventX, eventY)) {
			manualPaint.get(0).setSelected(true);
			manualPaint.get(1).setSelected(false);
			state = true;
		} else if (manualPaint.get(1).hit(eventX, eventY)) {
			manualPaint.get(0).setSelected(false);
			manualPaint.get(1).setSelected(true);
			state = false;
		}
		
		for (PreSelect p : preSelect) {
			if (p.hit(eventX, eventY)) {
				for (PreSelect q : preSelect) {
					q.setSelected(false);
				}
				p.setSelected(true);
				currentPreSelect = p;
				client.setChanged(System.currentTimeMillis());
				break;
			}
		}
		
		// clear-preset clicked.
		int xx = TILES_X * (sizeX + pan) + startX;
		int yy = TILES_Y * (sizeY + pan) + startY;
		if (eventX > xx - 250 && eventX < xx && eventY > yy + 120 && eventY < yy + 120 + 37) {
			currentPreSelect.clear();
			client.setChanged(System.currentTimeMillis());
            Toast.makeText(context.getApplicationContext(), "Current preselect cleared!", Toast.LENGTH_SHORT).show();

		}

	}

	public void writeToDisk() {
		Log.i("System", Environment.getExternalStorageDirectory().getAbsolutePath());
	    final File output = new File(Environment.getExternalStorageDirectory(), CONFIG_FILE);
	    if (output.exists()) {
	      output.delete();
	    }
	    try {
			FileOutputStream fos = new FileOutputStream(output.getPath());
	    	Properties p = new Properties();
	    	int i = 1;
	    	for (PreSelect ps : preSelect) {
	    		p.put("preSelect" + i++, ps.getPropertyString());
	    	}
	    	if (!p.contains("ipAddress")) {
	    		p.put("ipAddress", DEFAULT_IP_ADDRESS);
	    	}
	    	if (!p.contains("port")) {
	    		p.put("port", DEFAULT_PORT);
	    	}
	    	p.store(fos, "Config file for FanBot console.");
	    	fos.close();
		} catch (FileNotFoundException e) {
			Log.e("System", e.getMessage());
		} catch (IOException e) {
			Log.e("System", e.getMessage());
		}
	}
	
	public void readFromDisk() {
	    final File input = new File(Environment.getExternalStorageDirectory(), CONFIG_FILE);
	    if (input.exists()) {
	    	try {
	    		FileInputStream fis = new FileInputStream(input.getPath());
	    		Properties p = new Properties();
	    		p.load(fis);

	    		int i = 1;
	    		for (PreSelect ps : preSelect) {
	    			ps.parseProperty(p.getProperty("preSelect" + i++));
	    			if (ps.isSelected()) {
	    				ps.setSystemMatrix();
	    			}
	    		}
	    		
	    		serverIpAddress = p.getProperty("ipAddress", DEFAULT_IP_ADDRESS);
	    		serverPort = Integer.parseInt(p.getProperty("port", DEFAULT_PORT));
	    		
	    		fis.close();
	    	} catch (FileNotFoundException e) {
	    		Log.e(getClass().getName(), e.getMessage());
	    	} catch (IOException e) {
	    		Log.e(getClass().getName(), e.getMessage());
	    	} catch (Exception e) {
	    		input.delete();
	    	}
	    } else {
	    	Log.w(getClass().getName(), "No properties file found.");
	    }
	}
	
	public void startSender() {
		client = new ClientThread(serverIpAddress, serverPort);
		new Thread(client).start();
		try {
			Thread.sleep(3000);
			if (!client.getError().isEmpty()) {
				Toast.makeText(context.getApplicationContext(), client.getError(), Toast.LENGTH_SHORT).show();
			}
		} catch (InterruptedException e) {
		}
	}
	
	public void setValue(int x, int y, boolean state) {
		particles[x][y].setState(state);
	}
	
	public boolean isState() {
		return state;
	}

	public void setState(boolean state) {
		this.state = state;
	}
	
	public class ClientThread implements Runnable {
		private String ipAddress;
		private int port;
		private boolean connected = true;
		private long changed = System.currentTimeMillis();
		private String error = "";
		
		public ClientThread(String ipAddress, int port) {
			this.ipAddress = ipAddress;
			this.port = port;
		}
		
        public void run() {
            try {
                Log.d("ClientActivity", "C: Connecting...");
                Socket socket = new Socket(ipAddress, port);
                while (connected) {
                	if (changed > 0) {
                		long currentChange = changed;
                		try {
                			Log.i("ClientActivity", "C: Sending command.");
                			PrintWriter out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket
                					.getOutputStream())), true);
                			out.print("^");
                			for (int j = 0; j < TILES_Y; j++) {
                				for (int i = 0; i < TILES_X; i++) {
                					out.print(particles[i][j].isState() ? "*" : "0");
                				}
                				out.print("\n");
                			}
                			out.flush();
                			Log.i("ClientActivity", "C: Sent.");
                			if (currentChange == changed) {
                				changed = 0;
                			}
                		} catch (Exception e) {
                			Log.e("ClientActivity", "S: Error", e);
                			error = e.getMessage();
                		}
                	}
                	Thread.sleep(100);
                }
                socket.close();
                Log.d("ClientActivity", "C: Closed.");
            } catch (Exception e) {
                Log.e("ClientActivity", "C: Error", e);
    			error = e.getMessage();
                connected = false;
            }
        }
        
        public void stop() {
        	connected = false;
        }

		public long getChanged() {
			return changed;
		}

		public void setChanged(long changed) {
			this.changed = changed;
		}

		public String getError() {
			return error;
		}

    }
	
}
