#ifndef _CUSTOMCOMBOBOX_TOOLS_INCLUDE_
#define _CUSTOMCOMBOBOX_TOOLS_INCLUDE_
#include <QtCore/QRect>
#include <QtCore/QString>
#include <QtGui/QPen>

class QPainter;

void drawItem(QPainter *painter, QRect text_rect, QString text_, int text_width, QPen pen, int & current_width);
#endif //_CUSTOMCOMBOBOX_TOOLS_INCLUDE_