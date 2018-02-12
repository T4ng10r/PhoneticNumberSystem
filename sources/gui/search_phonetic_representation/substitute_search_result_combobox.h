#ifndef _SUBSTITUTESEARCHRESULTCOMBOBOX_INCLUDE_
#define _SUBSTITUTESEARCHRESULTCOMBOBOX_INCLUDE_

#include <QtWidgets/QComboBox>

class SubstituteSearchResultComboBox : public QComboBox
{
public:
	int current_width;
	void paintEvent(QPaintEvent * event);
};
#endif //_SUBSTITUTESEARCHRESULTCOMBOBOX_INCLUDE_
