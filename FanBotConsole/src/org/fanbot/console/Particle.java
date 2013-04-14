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

import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;

public class Particle {
	private Rect rect;
	private boolean state = false;
	private Paint paintPos;
	private Paint paintNeg;

	public Particle(int x, int y, int sizeX, int sizeY, final Paint paintPos, final Paint paintNeg) {
		this.rect = new Rect(x, y, x + sizeX, y + sizeY);
		this.paintPos = paintPos;
		this.paintNeg = paintNeg;
	}
	
	public void draw(final Canvas canvas) {
		if (state) {
			canvas.drawRect(rect, paintPos);
		} else {
			canvas.drawRect(rect, paintNeg);
		}
	}

	public void hit(int x, int y, boolean state, PreSelect preselect, int xx, int yy) {
		if (rect.contains(x, y)) {
			this.state = state;
			preselect.setValue(xx, yy, state);
		}
	}
	
	public boolean isState() {
		return state;
	}

	public void setState(boolean state) {
		this.state = state;
	}
	
}
