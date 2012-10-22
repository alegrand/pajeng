/*
    This file is part of PajeNG

    PajeNG is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PajeNG is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with PajeNG. If not, see <http://www.gnu.org/licenses/>.
*/
#include "PajeTreemapView.h"

PajeTreemapView::PajeTreemapView (PajeTreemapFrame *frame, QWidget *parent)
  : QGraphicsView(parent)
{
  this->frame = frame;
  setScene (&scene);
  treemap = NULL;
}

void PajeTreemapView::recreate (void)
{
  if (treemap){
    delete treemap;
  }
  treemap = new PajeTreemapNode (NULL, this, rootInstance());
}

void PajeTreemapView::hierarchyChanged (void)
{
  this->recreate();
}

void PajeTreemapView::timeSelectionChanged (void)
{
  if (!treemap){
    this->recreate(); //to create treemap hierarchical data structure
  }

  treemap->timeSelectionChanged();

  this->repopulate ();
}

void PajeTreemapView::drawTreemap (PajeTreemap *t, PajeTreemapContainerItem *parent)
{

  // PajeTreemapContainerItem *item = new PajeTreemapContainerItem (treemap, parent, this);
  // if (!parent){
  //   scene.addItem (item);
  // }

  //aggregated children
  std::vector<PajeTreemap*> valueChildren = t->valueChildren();
  std::vector<PajeTreemap*>::iterator it;
  for (it = valueChildren.begin(); it != valueChildren.end(); it++){
    PajeTreemapValueItem *vitem = new PajeTreemapValueItem (*it, NULL, this);
    scene.addItem (vitem);
  }

  //normal children
  std::vector<PajeTreemap*> children = t->children();
  for (it = children.begin(); it != children.end(); it++){
    PajeTreemap *child = *it;
    this->drawTreemap (child, NULL);
  }
}

void PajeTreemapView::repopulate (void)
{
  if (!treemap){
    this->recreate (); //to create something to populate
  }

  QSize s = viewport()->size();
  QRectF bb = QRectF(QPointF(0,0), s);
  treemap->setRect (bb);
  treemap->calculateTreemapWithFactor ((bb.width() * bb.height())/treemap->treemapValue());

  //clear
  scene.clear();

  scene.setSceneRect (treemap->rect());
  this->drawTreemap (treemap, NULL);
}

void PajeTreemapView::wheelEvent (QWheelEvent *event)
{
  QGraphicsView::wheelEvent(event);
}

void PajeTreemapView::mouseMoveEvent (QMouseEvent *event)
{
  QGraphicsView::mouseMoveEvent(event);
}

void PajeTreemapView::showEvent (QShowEvent *event)
{
  repopulate();
}

void PajeTreemapView::resizeEvent (QResizeEvent *event)
{
  repopulate();
}