#include <QApplication>
#include <Tools/loggers.h>
#include <GUI/MainWindow.h>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QItemDelegate>

#define USE_COLOR_TAG
#ifdef USE_COLOR_TAG
const QString start_tag("<b>");
const QString end_tag("</b>");

class delegate_ : public QItemDelegate
{
public:
	//inline delegate_(QWidget *parent) : QItemDelegate(parent) {}

	inline void paint(QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index ) const
	{
		if (index.column() != 2) {
			QItemDelegate::paint(painter, option, index);
			return;
		}
		QStyleOptionViewItemV4 myOption = option;
		QPalette standard_color = myOption.palette;
		QPalette special_color = myOption.palette;
		QFontMetrics font_metric(myOption.font);
		special_color.setColor(QPalette::WindowText, QColor(Qt::red));

		QString text = "Te<b>st</b>1";
		int start_pos(0), prev_pos(0), current_width(0);
		std::string word;
		while((start_pos=text.indexOf(start_tag, prev_pos))>=0)
		{
			myOption.text = text.mid(prev_pos, start_pos-prev_pos);
				word = myOption.text.toStdString();
			//draw in standard color
			myOption.palette = standard_color;
			myOption.rect.setWidth(current_width+font_metric.width(myOption.text));
			QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
			current_width += font_metric.width(myOption.text);
			prev_pos = start_pos+start_tag.size();

			start_pos=text.indexOf(end_tag, prev_pos);
			myOption.text = text.mid(prev_pos, start_pos-prev_pos);
				word = myOption.text.toStdString();
			myOption.palette = special_color;
			myOption.rect.setWidth(current_width+font_metric.width(myOption.text));
			QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
			current_width += font_metric.width(myOption.text);
			prev_pos = start_pos+end_tag.size();
		}
		myOption.text = text.mid(prev_pos);
		word = myOption.text.toStdString();
		//myOption.palette = special_color;
		//myOption.rect.setWidth(current_width+font_metric.width(myOption.text));
		//QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);

	} 
};
#endif

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("PhoneticNumberSystem");
	app.setOrganizationName("T4ng10r");

#ifdef USE_COLOR_TAG
	QComboBox * combo = new QComboBox();
	delegate_ delegate_obj;
	combo->addItem("");
	combo->addItem("Timmmy");
	combo->setItemDelegate(&delegate_obj);
	combo->show();
#else
	createLoggers();
	std::unique_ptr<CMainWindow> ptMainWindow(new CMainWindow);
	ptMainWindow->show();
#endif

	//////////////////////////////////////////////////////////////////////////
	int iReturn = app.exec();
	//destroyLoggers();
	return iReturn;
}
