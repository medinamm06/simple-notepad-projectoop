#include "line_number_area.h"

#include <QPaintEvent>
#include <QPainter>


// функция для рисования номеров строк сбоку
void line_number_editor::line_number_paint(QPaintEvent* event)
{
  // создаем объект для рисования и красим фон
  QPainter painter(number_area);
  painter.fillRect(event->rect(), QColor(300, 240, 240));

  // берем первую видимую строку
  QTextBlock block = firstVisibleBlock();

  // получаем номер строки и ее координаты
  int block_number = block.blockNumber();
  int top = qRound(blockBoundingGeometry(block)
      .translated(contentOffset()).top());

  int bottom = top +
      qRound(blockBoundingRect(block).height());

  // идем по всем строкам пока они не закончились
  // и пока еще есть что рисовать
  while (block.isValid() &&
      top <= event->rect().bottom())
  {
    // если строка реально видна на экране
    if (block.isVisible() &&
        bottom >= event->rect().top())
    {
      // +1 потому что счет идет с 0
      const QString number =
          QString::number(block_number + 1);

      // цифры сделала серыми
      painter.setPen(QColor(130,130,130));

      // рисуем номер справа
      painter.drawText(
          0,
          top,
          number_area->width() - 3,
          fontMetrics().height(),
          Qt::AlignRight,
          number
      );
    }

    // переходим к следующей строке и обновляем координаты
    block = block.next();

    top = bottom;
    bottom = top +
        qRound(blockBoundingRect(block).height());

    ++block_number;
  }
}