#include <GUI/ComposeSubstituteSentenceWidget.h>
#include <GUI/SubstituteSearchResultComboDelegate.h>
#include <Data/CDataThread.h>
#include <tools/loggers.h>
#include <Tools/qtTools.h>

#include <QComboBox>
#include <QBoxLayout>

class ComposeSubstituteSentenceWidgetPrivate
{
public:
	ComposeSubstituteSentenceWidgetPrivate(ComposeSubstituteSentenceWidget * ptrPublic);
	~ComposeSubstituteSentenceWidgetPrivate();
	void setupUI();
	void add_combo_box();
	void fill_combo_box(const WordsList & list, StartingIndex combo_id);
	void reset(unsigned int starting_index = 1);
	void set_connections();
	QString process_word(SuccessWord word, QTextCodec * codec);

public:
	ComposeSubstituteSentenceWidget * public_part;
	std::vector<QComboBox*>   combo_box_container;
	QHBoxLayout * compose_layout;
	SubstituteSearchResultComboDelegate delegate_;
};

ComposeSubstituteSentenceWidgetPrivate::ComposeSubstituteSentenceWidgetPrivate(ComposeSubstituteSentenceWidget * ptrPublic):public_part(ptrPublic)
{
	setupUI();
	set_connections();
}
ComposeSubstituteSentenceWidgetPrivate::~ComposeSubstituteSentenceWidgetPrivate()
{
	reset();
}
void ComposeSubstituteSentenceWidgetPrivate::add_combo_box()
{
	QComboBox * combo_box = new QComboBox();
	combo_box->setItemDelegate(&delegate_);
	combo_box->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	combo_box_container.push_back(combo_box);
	compose_layout->addWidget(combo_box);

	bool bResult = false;
	bResult = QObject::connect(combo_box, SIGNAL(activated(int)), 
		public_part, SLOT(on_word_selected(int)));
	logConnection("ComposeSubstituteSentenceWidgetPrivate::add_combo_box",
		"'combo_box::activated' with 'public_part::on_word_selected'", 
		bResult);

}
void ComposeSubstituteSentenceWidgetPrivate::setupUI()
{
	QHBoxLayout * main_layout = new QHBoxLayout;
	delete public_part->layout();
	public_part->setLayout(main_layout);

	compose_layout = new QHBoxLayout;
	main_layout->addLayout(compose_layout);
	main_layout->addStretch(5);
	add_combo_box();
}
void ComposeSubstituteSentenceWidgetPrivate::fill_combo_box(const WordsList & list,
	StartingIndex combo_id)
{
	QComboBox * combo_box = combo_box_container[combo_id];
	//combo_id
	combo_box->addItem("");

	QTextCodec * codec = CDataThread::getInstance()->get_current_codepage();
	for (const SuccessWord & word : list)
	{
		QString q_word = process_word(word, codec);
		combo_box->addItem(q_word,word.coveragePairs.front().second);
	}
}
void ComposeSubstituteSentenceWidgetPrivate::reset(unsigned int starting_index)
{
	if (starting_index<0) starting_index=0;
	if (starting_index>combo_box_container.size()) 
		starting_index=combo_box_container.size()-1;
	for(int i=starting_index;i<combo_box_container.size();i++)
	{
		QComboBox * combo_box = combo_box_container.back();
		compose_layout->removeWidget(combo_box);
		delete combo_box;
		combo_box_container.pop_back();
	}
}
void ComposeSubstituteSentenceWidgetPrivate::set_connections()
{
	bool bResult = false;
	//bResult = QObject::connect(m_actionConfiguration, SIGNAL(activated(int)), 
	//	m_ptrPublic, SLOT(on_word_selected(int)));
	//logConnection("CMainWindowPrivate::setConnections",
	//	"'CMainWindowPrivate::triggered' with 'CMainWindow::onActionTrigger'", 
	//	bResult);
}
QString ComposeSubstituteSentenceWidgetPrivate::process_word(SuccessWord word, QTextCodec * codec)
{
	QString style_word;
	if (codec)
		style_word = codec->toUnicode(word.getWord().c_str());
	else
		style_word = word.getWord().c_str();
	int start_pos=0;
	for(char letter : word.matchingLetters)
	{
		start_pos = style_word.indexOf(letter, start_pos, Qt::CaseInsensitive);
		if (start_pos<0)
			break;
		style_word = style_word.left(start_pos)+start_tag+letter+end_tag+style_word.right(style_word.size()-start_pos-1);
		std::string word_2 = style_word.toStdString();
		start_pos++;
	}
	return style_word;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
ComposeSubstituteSentenceWidget::ComposeSubstituteSentenceWidget(QWidget * parent):QWidget(parent),
	priv_part(new ComposeSubstituteSentenceWidgetPrivate(this))
{}
ComposeSubstituteSentenceWidget::~ComposeSubstituteSentenceWidget(void){}
void ComposeSubstituteSentenceWidget::initialize_after_success_search()
{
	priv_part->reset();
	WordsList result = CDataThread::getInstance()->getSearchResult(0);
	priv_part->fill_combo_box(result, 0);
}
void ComposeSubstituteSentenceWidget::on_word_selected(int selected_index)
{
	QComboBox * sender_ = dynamic_cast<QComboBox*>(sender());
	int starting_index=1;
	for(QComboBox * combo_box : priv_part->combo_box_container)
	{
		if (combo_box != sender_)
			starting_index++;
		else
			break;
	}
	priv_part->reset(starting_index);
	int node_id = sender_->itemData(selected_index).toInt()+1;
	WordsList result = CDataThread::getInstance()->getSearchResult(node_id);
	if (result.size())
	{
		priv_part->add_combo_box();
		priv_part->fill_combo_box(result, starting_index);
	}
}
