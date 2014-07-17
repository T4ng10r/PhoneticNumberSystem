#include <GUI/SearchResultWordWidget.h>
#include <Tools/loggers.h>


const QColor colorHighlightedLetter(Qt::red);
const QString startTextColoringTag( QString("<span style=\"color:#%1%2%3;\">")
								   .arg(colorHighlightedLetter.red(),2,16, QChar('0')) 
								   .arg(colorHighlightedLetter.green(),2,16, QChar('0'))
								   .arg(colorHighlightedLetter.blue(),2,16, QChar('0')) );
const QString endTextColoringTag("</span>");

class CSearchResultWordWidgetPrivate
{
public:
     CSearchResultWordWidgetPrivate(CSearchResultWordWidget * ptrPublic);
     ~CSearchResultWordWidgetPrivate();
	 void prepareFormatedText();
public:
     CSearchResultWordWidget *                              m_ptrPublic;
	 std::string found_word;
	 QString formated_text;
	 std::string highlight_letters;
};

CSearchResultWordWidgetPrivate::CSearchResultWordWidgetPrivate(CSearchResultWordWidget * ptrPublic):m_ptrPublic(ptrPublic)
{
	m_ptrPublic->setTextFormat(Qt::RichText);
}
CSearchResultWordWidgetPrivate::~CSearchResultWordWidgetPrivate(){}
void CSearchResultWordWidgetPrivate::prepareFormatedText()
{
	//find letters to highlight
	formated_text.clear();
	std::size_t pos(0);
	while((pos= found_word.find_first_of(highlight_letters))!=std::string::npos)
	{
		formated_text += found_word.substr(0,pos).c_str();
		formated_text += QString("%1%2%3").arg(startTextColoringTag).arg(found_word.substr(pos,1).c_str()).arg(endTextColoringTag);
		found_word = found_word.substr(pos+1);
	};
	m_ptrPublic->setText(formated_text);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CSearchResultWordWidget::CSearchResultWordWidget(QWidget * parent):QLabel(parent), 
	m_ptrPriv(new CSearchResultWordWidgetPrivate(this))
{}
CSearchResultWordWidget::~CSearchResultWordWidget(void){}
void CSearchResultWordWidget::setFoundWord(const std::string & text)
{
	m_ptrPriv->found_word=text;
	m_ptrPriv->prepareFormatedText();
}
void CSearchResultWordWidget::setHighlightedLetters(const std::string & highlight_letters)
{
	m_ptrPriv->highlight_letters=highlight_letters;		 
	m_ptrPriv->prepareFormatedText();
}
