#ifndef _SUBSTITUTESEARCHRESULTCOMBODELEGATE_INCLUDE_
#define _SUBSTITUTESEARCHRESULTCOMBODELEGATE_INCLUDE_

#include <QtWidgets/QItemDelegate>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>

const QString start_tag("<b>");
const QString end_tag("</b>");

class SubstituteSearchResultComboDelegate : public QItemDelegate
{
public:
	mutable int current_width;
	void drawItem(QPainter *painter, QRect text_rect, QString text_, int text_width, QPen pen) const
	{
		text_rect.setLeft(current_width);
		text_rect.setWidth(text_width);
		painter->setPen(pen);
		painter->drawText(text_rect, 0, text_);
		current_width += text_rect.width();
	};

	inline void paint(QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index ) const
	{
		QString text = index.model()->data(index).toString();
		QStyleOptionViewItemV4 myOption = option;
		QFontMetrics font_metric(myOption.font);

		int start_pos(0), prev_pos(0);
		current_width=0;
		myOption.text="";
		QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);

		QPen standard_pen = painter->pen();
		if (myOption.state.testFlag(QStyle::State_Selected))
			standard_pen.setColor(myOption.palette.color(QPalette::HighlightedText));
		else
			standard_pen.setColor(myOption.palette.color(QPalette::WindowText));
		QPen special_pen = standard_pen;
		special_pen.setColor(QColor(Qt::red));

		QRect text_rect = myOption.rect;
		QString text_;
		while((start_pos=text.indexOf(start_tag, prev_pos))>=0)
		{
			text_ = text.mid(prev_pos, start_pos-prev_pos);
			drawItem(painter, text_rect, text_, font_metric.width(text_), standard_pen);
			prev_pos = start_pos+start_tag.size();
			start_pos=text.indexOf(end_tag, prev_pos);
			text_ = text.mid(prev_pos, start_pos-prev_pos);
			drawItem(painter, text_rect, text_, font_metric.width(text_), special_pen);
			prev_pos = start_pos+end_tag.size();
		}
		text_ = text.mid(prev_pos);
		drawItem(painter, text_rect, text_, font_metric.width(text_), standard_pen);
	} 
};
#endif
