#include <GUI/ComposeSubstituteSentenceWidget.h>
#include <Data/CDataThread.h>
#include <tools/loggers.h>

#include <QComboBox>
#include <QBoxLayout>

class ComposeSubstituteSentenceWidgetPrivate
{
public:
     ComposeSubstituteSentenceWidgetPrivate(ComposeSubstituteSentenceWidget * ptrPublic);
     ~ComposeSubstituteSentenceWidgetPrivate();
	 void setupUI();
public:
     ComposeSubstituteSentenceWidget * m_ptrPublic;
	 QComboBox*   combo_box_container;
	 //std::vector<QComboBox*>   combo_box_container;
	 QHBoxLayout * compose_layout;
};

ComposeSubstituteSentenceWidgetPrivate::ComposeSubstituteSentenceWidgetPrivate(ComposeSubstituteSentenceWidget * ptrPublic):m_ptrPublic(ptrPublic)
{
	setupUI();
}
ComposeSubstituteSentenceWidgetPrivate::~ComposeSubstituteSentenceWidgetPrivate(){}
void ComposeSubstituteSentenceWidgetPrivate::setupUI()
{
	QHBoxLayout * main_layout = new QHBoxLayout;
	delete m_ptrPublic->layout();
	m_ptrPublic->setLayout(main_layout);

	compose_layout = new QHBoxLayout;

	combo_box_container = new QComboBox();
	compose_layout->addWidget(combo_box_container);
	main_layout->addLayout(compose_layout);

	main_layout->addStretch(5);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
ComposeSubstituteSentenceWidget::ComposeSubstituteSentenceWidget(QWidget * parent):QWidget(parent),
	m_ptrPriv(new ComposeSubstituteSentenceWidgetPrivate(this))
{}
ComposeSubstituteSentenceWidget::~ComposeSubstituteSentenceWidget(void){}
void ComposeSubstituteSentenceWidget::reset()
{
	for (int i=0;i<m_ptrPriv->combo_box_container->count();i++)
		m_ptrPriv->combo_box_container->removeItem(0);
}
void ComposeSubstituteSentenceWidget::initialize_after_success_search()
{
	reset();
	m_ptrPriv->combo_box_container->addItem("");
	WordsList result = CDataThread::getInstance()->getSearchResult(0);

  QTextCodec * codec = CDataThread::getInstance()->get_current_codepage();
	for (const SuccessWord & word : result)
	{
    QString q_word = word.getWord().c_str();
    if (codec)
		  q_word = codec->toUnicode(word.getWord().c_str());
    else
      q_word = word.getWord().c_str();
		m_ptrPriv->combo_box_container->addItem(q_word);
	}
}
