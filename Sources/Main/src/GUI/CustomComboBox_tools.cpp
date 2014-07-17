#include <GUI/CustomComboBox_tools.h>
#include <QtGui/QPainter>

const unsigned int correction_sep(1);

void drawItem(QPainter *painter, QRect text_rect, QString text_, int text_width, QPen pen, int & current_width)
{
	text_rect.setLeft(current_width);
	text_rect.setWidth(text_width);
	painter->setPen(pen);
	painter->drawText(text_rect, 0, text_);
	current_width += text_rect.width()+correction_sep;
}
