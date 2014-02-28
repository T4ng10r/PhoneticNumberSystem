#ifndef _SUBSTITUTESEARCHRESULTCOMBOBOX_INCLUDE_
#define _SUBSTITUTESEARCHRESULTCOMBOBOX_INCLUDE_

#include <QtWidgets/QComboBox>

class SubstituteSearchResultComboBox : public QComboBox
{
public:
	int current_width;
	void paintEvent(QPaintEvent * event);
	void drawItem(QPainter *painter, QRect text_rect, QString text_, int text_width, QPen pen);
};
#endif //_SUBSTITUTESEARCHRESULTCOMBOBOX_INCLUDE_
