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
	void fill_combo_box(const WordsList & list, StartingIndex combo_id);
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
	combo_box_container->setSizeAdjustPolicy(QComboBox::AdjustToContents);

	compose_layout->addWidget(combo_box_container);
	main_layout->addLayout(compose_layout);

	main_layout->addStretch(5);
}
void ComposeSubstituteSentenceWidgetPrivate::fill_combo_box(const WordsList & list,
	StartingIndex combo_id)
{
	//combo_id
	combo_box_container->addItem("");

	QTextCodec * codec = CDataThread::getInstance()->get_current_codepage();
	for (const SuccessWord & word : list)
	{
		QString q_word = word.getWord().c_str();
		if (codec)
			q_word = codec->toUnicode(word.getWord().c_str());
		else
			q_word = word.getWord().c_str();
		combo_box_container->addItem(q_word,word.coveragePairs.front().second);
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
ComposeSubstituteSentenceWidget::ComposeSubstituteSentenceWidget(QWidget * parent):QWidget(parent),
	priv_part(new ComposeSubstituteSentenceWidgetPrivate(this))
{}
ComposeSubstituteSentenceWidget::~ComposeSubstituteSentenceWidget(void){}
void ComposeSubstituteSentenceWidget::reset()
{
	for(int i=0;i<priv_part->combo_box_container->count();i++)
		priv_part->combo_box_container->removeItem(0);
}

void ComposeSubstituteSentenceWidget::initialize_after_success_search()
{
	reset();
	WordsList result = CDataThread::getInstance()->getSearchResult(0);
	priv_part->fill_combo_box(result, 0);
}
