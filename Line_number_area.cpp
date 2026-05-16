#include "line_number_area.h"

#include <QPaintEvent>
#include <QPainter>



void line_number_editor::line_number_paint(QPaintEvent* event)
{
  QPainter painter(number_area);
  painter.fillRect(event->rect(), QColor(240, 240, 240));

  QTextBlock block = firstVisibleBlock();
  int block_number = block.blockNumber();
  int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom = top + qRound(blockBoundingRect(block).height());

  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      const QString number = QString::number(block_number + 1);
      painter.setPen(QColor(130, 130, 130));
      painter.drawText(0, top, number_area->width() - 3,
          fontMetrics().height(), Qt::AlignRight, number);
    }

  }
}