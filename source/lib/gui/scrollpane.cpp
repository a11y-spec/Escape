/**
 * $Id$
 * Copyright (C) 2008 - 2011 Nils Asmussen
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <esc/common.h>
#include <gui/scrollpane.h>
#include <gui/panel.h>

using namespace std;

namespace gui {
	void ScrollPane::resizeTo(const Size &size) {
		// a scrollpane does never use more space than available. if the content is larger, we
		// show scrollbars for it.
		Size rsize = minsize(size,getParent()->getContentSize());
		Control::resizeTo(rsize);
		Size visible = rsize - Size(BAR_SIZE,BAR_SIZE);

		// determine what size the control will actually use, if it gets visiblex X visibley.
		_ctrl->resizeTo(_ctrl->getUsedSize(visible));

		// ensure that the position of the control is in the allowed range; of course, this changes
		// as soon as the size of the scrollpane or the control changes
		gpos_t minX = visible.width - _ctrl->getSize().width;
		gpos_t minY = visible.height - _ctrl->getSize().height;
		gpos_t newX = min((gpos_t)0,max(minX,_ctrl->getX()));
		gpos_t newY = min((gpos_t)0,max(minY,_ctrl->getY()));
		if(_ctrl->getX() != newX || _ctrl->getY() != newY)
			_ctrl->moveTo(newX,newY);
	}

	void ScrollPane::scrollBy(short mx,short my) {
		Graphics *g = getGraphics();
		gsize_t visibley = max(0,getSize().height - BAR_SIZE);
		gsize_t visiblex = max(0,getSize().width - BAR_SIZE);
		if((_focus & FOCUS_HORSB) && mx != 0 && visiblex != 0) {
			short x = (short)(_ctrl->getSize().width / ((double)visiblex / mx));
			int minX = visiblex - _ctrl->getSize().width;
			// scroll left, i.e. move content right
			if(x < 0 && _ctrl->getX() < 0) {
				x = max(_ctrl->getX(),x);
				_ctrl->moveTo(_ctrl->getX() - x,_ctrl->getY());
				if(-x >= visiblex)
					repaint();
				else {
					// move cols right and repaint the first few cols
					g->moveCols(0,0,visiblex + x,visibley,x);
					repaintRect(0,0,Size(-x,visibley));
				}
			}
			// scroll right, i.e. move content left
			else if(x > 0 && _ctrl->getX() > minX) {
				x = min((short)(_ctrl->getX() - minX),x);
				_ctrl->moveTo(_ctrl->getX() - x,_ctrl->getY());
				if(x >= visiblex)
					repaint();
				else {
					// move cols left and repaint the last few cols
					g->moveCols(x,0,(gsize_t)(visiblex - x),visibley,x);
					repaintRect(visiblex - x,0,Size(x,visibley));
				}
			}
		}
		else if((_focus & FOCUS_VERTSB) && my != 0 && visibley != 0) {
			short y = (short)(_ctrl->getSize().height / ((double)visibley / my));
			int minY = visibley - _ctrl->getSize().height;
			// scroll up, i.e. move content down
			if(y < 0 && _ctrl->getY() < 0) {
				y = max(_ctrl->getY(),y);
				_ctrl->moveTo(_ctrl->getX(),_ctrl->getY() - y);
				if(-y >= visibley)
					repaint();
				else {
					// move rows down and repaint the first few rows
					g->moveRows(0,0,visiblex,visibley + y,y);
					repaintRect(0,0,Size(visiblex,-y));
				}
			}
			// scroll down, i.e. move content up
			else if(y > 0 && _ctrl->getY() > minY) {
				y = min((short)(_ctrl->getY() - minY),y);
				_ctrl->moveTo(_ctrl->getX(),_ctrl->getY() - y);
				if(y >= visibley)
					repaint();
				else {
					// move rows up and repaint the last few rows
					g->moveRows(0,y,visiblex,visibley - y,y);
					repaintRect(0,visibley - y,Size(visiblex,y));
				}
			}
		}
	}

	void ScrollPane::onMouseMoved(const MouseEvent &e) {
		scrollBy(e.getXMovement(),e.getYMovement());
	}
	void ScrollPane::onMouseReleased(A_UNUSED const MouseEvent &e) {
		_focus &= ~(FOCUS_HORSB | FOCUS_VERTSB);
	}
	void ScrollPane::onMousePressed(const MouseEvent &e) {
		Size size = getSize();
		if(e.getY() >= size.height - BAR_SIZE) {
			gsize_t visiblex = size.width - BAR_SIZE;
			if(e.getX() < visiblex) {
				gsize_t ctrlw = _ctrl->getSize().width;
				gpos_t barPos = getBarPos(ctrlw,visiblex,_ctrl->getX());
				gsize_t barSize = getBarSize(ctrlw,visiblex);
				_focus = FOCUS_HORSB;
				if(e.getX() < barPos) {
					scrollBy(-barSize,0);
					_focus = 0;
				}
				else if(e.getX() >= barPos + barSize) {
					scrollBy(barSize,0);
					_focus = 0;
				}
			}
		}
		else if(e.getX() >= size.width - BAR_SIZE) {
			gsize_t ctrlh = _ctrl->getSize().height;
			gsize_t visibley = size.height - BAR_SIZE;
			gpos_t barPos = getBarPos(ctrlh,visibley,_ctrl->getY());
			gsize_t barSize = getBarSize(ctrlh,visibley);
			_focus = FOCUS_VERTSB;
			if(e.getY() < barPos) {
				scrollBy(0,-barSize);
				_focus = 0;
			}
			else if(e.getY() >= barPos + barSize) {
				scrollBy(0,barSize);
				_focus = 0;
			}
		}
		else {
			MouseEvent ce(e.getType(),e.getXMovement(),e.getYMovement(),e.getWheelMovement(),
					e.getX() - _ctrl->getX(),e.getY() - _ctrl->getY(),e.getButtonMask());
			_ctrl->onMousePressed(ce);
			_focus = FOCUS_CTRL;
		}
	}
	void ScrollPane::onMouseWheel(const MouseEvent &e) {
		gsize_t visibley = getSize().height - BAR_SIZE;
		if(getBarSize(_ctrl->getSize().height,visibley) < visibley) {
			_focus = FOCUS_VERTSB;
			scrollBy(0,e.getWheelMovement() * SCROLL_FACTOR);
		}
		else {
			_focus = FOCUS_HORSB;
			scrollBy(e.getWheelMovement() * SCROLL_FACTOR,0);
		}
		_focus = 0;
	}

	void ScrollPane::paint(Graphics &g) {
		_ctrl->repaint(false);
		paintBars(g);
	}

	void ScrollPane::paintBars(Graphics &g) {
		Size ctrlSize = _ctrl->getSize();
		gsize_t visiblex = getSize().width - BAR_SIZE;
		gsize_t visibley = getSize().height - BAR_SIZE;

		// horizontal scrollbar
		gpos_t ybarPos = visibley;
		g.setColor(getTheme().getColor(Theme::CTRL_DARKBORDER));
		g.drawRect(0,ybarPos,visiblex,BAR_SIZE);
		g.setColor(getTheme().getColor(Theme::CTRL_LIGHTBACK));
		g.fillRect(1,ybarPos + 1,visiblex - 2,BAR_SIZE - 2);
		if(ctrlSize.width > visiblex) {
			g.setColor(getTheme().getColor(Theme::CTRL_DARKBACK));
			g.fillRect(getBarPos(ctrlSize.width,visiblex,_ctrl->getX()) + 2,
					ybarPos + 2,getBarSize(ctrlSize.width,visiblex) - 3,BAR_SIZE - 4);
		}

		// vertical scrollbar
		gpos_t xbarPos = visiblex;
		g.setColor(getTheme().getColor(Theme::CTRL_DARKBORDER));
		g.drawRect(xbarPos,0,BAR_SIZE,visibley);
		g.setColor(getTheme().getColor(Theme::CTRL_LIGHTBACK));
		g.fillRect(xbarPos + 1,1,BAR_SIZE - 2,visibley - 2);
		if(ctrlSize.height > visibley) {
			g.setColor(getTheme().getColor(Theme::CTRL_DARKBACK));
			g.fillRect(xbarPos + 2,getBarPos(ctrlSize.height,visibley,_ctrl->getY()) + 2,
					BAR_SIZE - 4,getBarSize(ctrlSize.height,visibley) - 3);
		}

		// corner
		g.setColor(getTheme().getColor(Theme::CTRL_BACKGROUND));
		g.fillRect(xbarPos,ybarPos,BAR_SIZE,BAR_SIZE);
	}

	void ScrollPane::paintRect(Graphics &g,gpos_t x,gpos_t y,const Size &size) {
		Size rsize = size;
		rsize.width = min(rsize.width,(gsize_t)(getSize().width - BAR_SIZE));
		rsize.height = min(rsize.height,(gsize_t)(getSize().height - BAR_SIZE));
		_ctrl->paintRect(*_ctrl->getGraphics(),-_ctrl->getX() + x,-_ctrl->getY() + y,rsize);
		paintBars(g);
	}

	gpos_t ScrollPane::getBarPos(gsize_t ctrlSize,gsize_t viewable,gpos_t ctrlPos) {
		if(ctrlPos == 0 || ctrlSize == 0)
			return 0;
		return (gpos_t)(viewable / ((double)ctrlSize / -ctrlPos));
	}

	gsize_t ScrollPane::getBarSize(gsize_t ctrlSize,gsize_t viewable) {
		if(viewable == 0 || ctrlSize == 0)
			return 0;
		return (gsize_t)(viewable / ((double)ctrlSize / viewable));
	}
}
