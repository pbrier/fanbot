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
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.RectF;

public class ManualPaint {
	private Rect rect;
	private boolean selected = false;
	private Paint paint;

	public ManualPaint(int x, int y, int sizeX, int sizeY, final Paint paint, final boolean selected) {
		this.rect = new Rect(x, y, x + sizeX, y + sizeY);
		this.paint = paint;
		this.selected = selected;
	}
	
	public void draw(final Canvas canvas) {
		RectF r = new RectF(rect);
		canvas.drawRoundRect(r, 10, 10, paint);
		
		if (selected) {
			final Paint paintSelected = new Paint(Paint.ANTI_ALIAS_FLAG);
			paintSelected.setStyle(Paint.Style.STROKE);
			paintSelected.setStrokeWidth(5);
			paintSelected.setColor(Color.GREEN);
			canvas.drawRoundRect(r, 10, 10, paintSelected);
		}
	}

	public boolean hit(int x, int y) {
		return rect.contains(x, y);
	}

	public boolean isSelected() {
		return selected;
	}

	public void setSelected(boolean selected) {
		this.selected = selected;
	}
	
}
