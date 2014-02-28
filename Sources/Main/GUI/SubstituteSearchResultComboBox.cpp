#include <GUI/SubstituteSearchResultComboBox.h>
#include <QtGui/QPainter>
#include <QtWidgets/QStylePainter>
#include <Tools/loggers.h>
#include <QtWidgets/QApplication>
#include <Data/CSubstituteSearchTypes.h>
#include <QtWidgets/QStyle>
Q_DECLARE_METATYPE(SuccessWord);

void SubstituteSearchResultComboBox::drawItem(QPainter *painter, QRect text_rect, QString text_, int text_width, QPen pen)
{
	text_rect.setLeft(current_width);
	text_rect.setWidth(text_width+1);
	text_rect.adjust(1,0,-1,0);
	painter->setPen(pen);
	painter->drawText(text_rect, 0, text_);
	current_width += text_rect.width();
}

/*inline void paint(QPainter *painter, const QStyleOptionViewItem &option,
									const QModelIndex &index )
{
	QString text = index.model()->data(index).toString();
	SuccessWord success_word = index.model()->data(index,Qt::UserRole).value<SuccessWord>();
	QStyleOptionViewItemV4 myOption = option;
	QFontMetrics font_metric(myOption.font);

	int start_pos(0), prev_pos(0);
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
		drawItem(painter, myOption.rect, text_, font_metric.width(text_), standard_pen);
		prev_pos = start_pos;

		text_ = text.mid(prev_pos, 1);
		drawItem(painter, myOption.rect, text_, font_metric.width(text_), special_pen);
		prev_pos++;
	}
	text_ = text.mid(prev_pos);
	drawItem(painter, myOption.rect, text_, font_metric.width(text_)+5, standard_pen);
} 
*/
void SubstituteSearchResultComboBox::paintEvent(QPaintEvent * event)
{
	QComboBox::paintEvent(event);
	QStylePainter painter(this);
	int start_pos(0), prev_pos(0);
	current_width=0;
	QString text_;
	QFontMetrics font_metric(font());

	QStyleOptionComboBox cmb;
	initStyleOption(&cmb);

	QModelIndex index = model()->index(currentIndex(),0);
	QString text = model()->data(index).toString();
	SuccessWord success_word = model()->data(index,Qt::UserRole).value<SuccessWord>();

	QPen standard_pen = painter.pen();
	standard_pen.setColor(palette().color(QPalette::WindowText));
	QPen special_pen = painter.pen();
	special_pen.setColor(QColor(Qt::red));
	special_pen.setWidth(special_pen.width()+1);
	//QRect rect_ = rect();
	QRect rect_ = painter.style()->proxy()->subControlRect(QStyle::CC_ComboBox, &cmb, QStyle::SC_ComboBoxEditField, this);

	for(char letter : success_word.matchingLetters)
	{
		start_pos = text.indexOf(letter, prev_pos, Qt::CaseInsensitive);
		if (start_pos<0) break;

		text_ = text.mid(prev_pos, start_pos-prev_pos);
		drawItem(&painter, rect_ , text_, font_metric.width(text_), standard_pen);
		prev_pos = start_pos;

		text_ = text.mid(prev_pos, 1);
		drawItem(&painter, rect_ , text_, font_metric.width(text_), special_pen);
		prev_pos++;
	}
	text_ = text.mid(prev_pos);
	drawItem(&painter, rect_ , text_, font_metric.width(text_)+5, standard_pen);
}
	