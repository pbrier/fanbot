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

public class PreSelect {
	private static final int FACTOR = 4;
	private Rect rect;
	private boolean selected = false;
	private Paint paintNeg;
	private Paint paintPos;
	private boolean[][] matrix = new boolean[ParticleSystem.TILES_X][ParticleSystem.TILES_Y];
	private ParticleSystem system;

	public PreSelect(ParticleSystem system, int x, int y, final boolean selected) {
		this.rect = new Rect(x, y, x + ParticleSystem.TILES_X * FACTOR, y + ParticleSystem.TILES_Y * FACTOR);
		this.selected = selected;
		this.system = system;
		
		paintNeg = new Paint();
		paintNeg.setStyle(Paint.Style.FILL);
		paintNeg.setStrokeWidth(2);
		paintNeg.setColor(Color.RED);
		paintNeg.setAlpha(75);
		
		paintPos = new Paint();
		paintPos.setStyle(Paint.Style.FILL);
		paintPos.setStrokeWidth(2);
		paintPos.setColor(Color.RED);
	}
	
	public void draw(final Canvas canvas) {
		canvas.drawRect(rect, paintNeg);
		for (int j = 0; j < ParticleSystem.TILES_Y; j++) {
			for (int i = 0; i < ParticleSystem.TILES_X; i++) {
				if (matrix[i][j]) {
					int xx = i * FACTOR + rect.left;
					int yy = j * FACTOR + rect.top;
					final Rect r = new Rect(xx, yy, xx + FACTOR, yy + FACTOR);
					canvas.drawRect(r, paintPos);
				}
			}
		}
	
		if (selected) {
			final Paint paintSelected = new Paint(Paint.ANTI_ALIAS_FLAG);
			paintSelected.setStyle(Paint.Style.STROKE);
			paintSelected.setStrokeWidth(5);
			paintSelected.setColor(Color.GREEN);
			final Rect r = new Rect(rect.left - 2, rect.top - 2, rect.right + 2, rect.bottom + 2);
			canvas.drawRect(r, paintSelected);
		}
	}
		
	public void setValue(int x, int y, boolean state) {
		matrix[x][y] = state;
	}
	
	public boolean hit(int x, int y) {
		if (rect.contains(x, y)) {
			setSystemMatrix();
		} else {
			return false;
		}
		return true;
	}

	public void setSystemMatrix() {
		for (int j = 0; j < ParticleSystem.TILES_Y; j++) {
			for (int i = 0; i < ParticleSystem.TILES_X; i++) {
				system.setValue(i, j, matrix[i][j]);
			}
		}
	}
	
	public void clear() {
		for (int j = 0; j < ParticleSystem.TILES_Y; j++) {
			for (int i = 0; i < ParticleSystem.TILES_X; i++) {
				matrix[i][j] = false;
			}
		}
		setSystemMatrix();
	}
	
	public String getPropertyString() {
		final StringBuilder sb = new StringBuilder(ParticleSystem.TILES_X * ParticleSystem.TILES_Y);
		for (int j = 0; j < ParticleSystem.TILES_Y; j++) {
			for (int i = 0; i < ParticleSystem.TILES_X; i++) {
				sb.append((matrix[i][j] ? "1" : "0"));
			}
		}
		return sb.toString();
	}
	
	public void parseProperty(String property) {
		for (int j = 0; j < ParticleSystem.TILES_Y; j++) {
			for (int i = 0; i < ParticleSystem.TILES_X; i++) {
				matrix[i][j] = (property.charAt(j * ParticleSystem.TILES_X + i) == '1' ? true : false);
			}
		}
	}
	
	public boolean isSelected() {
		return selected;
	}

	public void setSelected(boolean selected) {
		this.selected = selected;
	}
	
}
