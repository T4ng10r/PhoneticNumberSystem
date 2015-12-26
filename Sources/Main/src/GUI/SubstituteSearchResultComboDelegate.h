#ifndef _SUBSTITUTESEARCHRESULTCOMBODELEGATE_INCLUDE_
#define _SUBSTITUTESEARCHRESULTCOMBODELEGATE_INCLUDE_

#include <QtWidgets/QItemDelegate>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#include <Tools/loggers.h>
#include <Data/MatchingWordsSearchTypes.h>
Q_DECLARE_METATYPE(SuccessWord);
#include <GUI/CustomComboBox_tools.h>

class SubstituteSearchResultComboDelegate : public QItemDelegate
{
public:

	inline void paint(QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index ) const
	{
		QString text = index.model()->data(index).toString();
		SuccessWord success_word = index.model()->data(index,Qt::UserRole).value<SuccessWord>();
		QStyleOptionViewItemV4 myOption = option;
		QFontMetrics font_metric(myOption.font);

		int start_pos(0), prev_pos(0), current_width(0);
		myOption.text="";
		QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
		QString text_;

		QPen standard_pen = painter->pen();
		if (myOption.state.testFlag(QStyle::State_Selected))
			standard_pen.setColor(myOption.palette.color(QPalette::HighlightedText));
		else
			standard_pen.setColor(myOption.palette.color(QPalette::WindowText));
		QPen special_pen = painter->pen();
		special_pen.setColor(QColor(Qt::red));
		special_pen.setWidth(special_pen.width()+1);

		for(char letter : success_word.matchingLetters)
		{
			start_pos = text.indexOf(letter, prev_pos, Qt::CaseInsensitive);
			if (start_pos<0)
				break;

			text_ = text.mid(prev_pos, start_pos-prev_pos);
			drawItem(painter, myOption.rect, text_, font_metric.width(text_), standard_pen, current_width);
			prev_pos = start_pos;

			text_ = text.mid(prev_pos, 1);
			drawItem(painter, myOption.rect, text_, font_metric.width(text_), special_pen, current_width);
			prev_pos++;
		}
		text_ = text.mid(prev_pos);
		drawItem(painter, myOption.rect, text_, font_metric.width(text_)+5, standard_pen, current_width);
	} 
};
#endif
