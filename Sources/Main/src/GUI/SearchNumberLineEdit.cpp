#include <GUI/SearchNumberLineEdit.h>
#include <QtGui/QValidator>
#include <Tools/loggers.h>

class SearchNumberValidator : public QValidator
{
public:
	State validate(QString & input, int & pos) const
	{
		QString result_text;
		int i=0;
		while(i<input.length())
		{
			if (input[i].isDigit())
				result_text+=input[i];
			i++;
		}
		input=result_text;
		return QValidator::Acceptable;
	}
};

//////////////////////////////////////////////////////////////////////////

class SearchNumberLineEditPrivate
{
public:
     SearchNumberLineEditPrivate(SearchNumberLineEdit * ptrPublic);
     ~SearchNumberLineEditPrivate();
public:
     SearchNumberLineEdit *                              m_ptrPublic;
	 SearchNumberValidator validator;
};

SearchNumberLineEditPrivate::SearchNumberLineEditPrivate(SearchNumberLineEdit * ptrPublic):m_ptrPublic(ptrPublic)
{
	m_ptrPublic->setMaxLength(20);
	m_ptrPublic->setValidator(&validator);
}
SearchNumberLineEditPrivate::~SearchNumberLineEditPrivate(){}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
SearchNumberLineEdit::SearchNumberLineEdit(QWidget* parent):QLineEdit(parent), m_ptrPriv(new SearchNumberLineEditPrivate(this))
{}
SearchNumberLineEdit::~SearchNumberLineEdit(void){}
