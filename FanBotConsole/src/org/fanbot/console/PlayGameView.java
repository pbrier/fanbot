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

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnTouchListener;

public class PlayGameView extends SurfaceView implements SurfaceHolder.Callback, OnTouchListener {

	private CanvasThread canvasthread = null;
	private ParticleSystem system = null;

	public PlayGameView(Context context, AttributeSet  attrs) {
		super(context, attrs);
		SurfaceHolder holder = getHolder();
		holder.addCallback(this);
		canvasthread = new CanvasThread(getHolder());
		setFocusable(true);
		
		final Paint paintPos = new Paint(Paint.ANTI_ALIAS_FLAG);
		paintPos.setStyle(Paint.Style.FILL);
		paintPos.setStrokeWidth(2);
		paintPos.setColor(Color.RED);

		final Paint paintNeg = new Paint(Paint.ANTI_ALIAS_FLAG);
		paintNeg.setStyle(Paint.Style.FILL);
		paintNeg.setStrokeWidth(2);
		paintNeg.setColor(Color.RED);
		paintNeg.setAlpha(75);
		
		setFocusable(true);
		setFocusableInTouchMode(true);

		this.setOnTouchListener(this);

		system = new ParticleSystem(context, paintPos, paintNeg, 30, 20, 2, 22, 22);
		system.readFromDisk();
		system.startSender();
	}

	@Override
	public void onDraw(Canvas  canvas) {     
		if (canvas != null) {
			system.draw(canvas);
		}
	}

	@Override
	public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) {
		Log.d("View", "Surface changed.");
		system.writeToDisk();
	}

	@Override
	public void surfaceCreated(SurfaceHolder arg0) {
		Log.d("View", "Surface created.");
		canvasthread.setRunning(true);
		canvasthread.start();
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder arg0) {
		Log.d("View", "Surface destroyed.");
		boolean retry = true;
		canvasthread.setRunning(false);
		while (retry) {
			try {
				canvasthread.join();
				retry = false;
			} catch (InterruptedException  e) {
				// we will try it again and again...
			}
		}
		system.writeToDisk();
	}

	public boolean onTouch(View view, MotionEvent event) {
		system.hit((int) event.getX(), (int) event.getY());
		invalidate();
		return true;
	}

	private class CanvasThread extends Thread  {
		private SurfaceHolder _holder;
		private boolean _run = false;

		public CanvasThread(SurfaceHolder surfaceHolder) {
			_holder = surfaceHolder;
		}

		public void setRunning(boolean run) {
			_run = run;
		}

		@Override
		public void run() {
			Canvas  c;
			while (_run) {
				c = null;
				try {
					c = _holder.lockCanvas(null);
					synchronized (_holder) {
						if (_run && c != null) {
							draw(c);
						}
					}
				} finally {
					if (c != null) {
						_holder.unlockCanvasAndPost(c);
					}
				}
			}
		}
	}
}
