#include <GUI/ComposeSubstituteSentenceWidget.h>
#include <GUI/SubstituteSearchResultComboDelegate.h>
#include <GUI/SubstituteSearchResultComboBox.h>
#include <Data/DataThread.h>
#include <Tools/loggers.h>
#include <Tools/qtTools.h>
#include <boost/format.hpp>

#include <QComboBox>
#include <QBoxLayout>
#include <algorithm>
//Q_DECLARE_METATYPE(MatchingWord);

bool cmp_success_words(const MatchingWord & first, const MatchingWord & second)
{
	if (first.matchingLetters.size()>second.matchingLetters.size())
		return true;
	if (first.matchingLetters.size()==second.matchingLetters.size() &&
		first.words.front()<second.words.front())
		return true;
	return false;
}

class ComposeSubstituteSentenceWidgetPrivate
{
public:
	ComposeSubstituteSentenceWidgetPrivate(ComposeSubstituteSentenceWidget * ptrPublic);
	~ComposeSubstituteSentenceWidgetPrivate();
	void setupUI();
	void add_combo_box();
	void fill_combo_box(const WordsList & list, StartingIndex combo_id);
	void reset(std::size_t starting_index = 1);
	void set_connections();
	QString process_word(MatchingWord word, QTextCodec * codec);

public:
	ComposeSubstituteSentenceWidget * public_part;
	std::vector<SubstituteSearchResultComboBox*>   combo_box_container;
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
	SubstituteSearchResultComboBox * combo_box = new SubstituteSearchResultComboBox();
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

	QTextCodec * codec = gDataThread->get_current_codepage();
	for (const MatchingWord & word : list)
	{
		QString q_word = process_word(word, codec);
		combo_box->addItem(q_word,QVariant::fromValue(word));
	}
}
void ComposeSubstituteSentenceWidgetPrivate::reset(std::size_t starting_index)
{
	//if (starting_index<0) starting_index=0;
	if (starting_index>combo_box_container.size()) 
		starting_index=combo_box_container.size()-1;
	for(std::size_t i=starting_index;i<combo_box_container.size();i++)
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
QString ComposeSubstituteSentenceWidgetPrivate::process_word(MatchingWord word, QTextCodec * codec)
{
	QString style_word;
	if (codec)
		style_word = codec->toUnicode(word.getWord().c_str());
	else
		style_word = word.getWord().c_str();
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
	WordsList result = gDataThread->getSearchResult(0);
	result.sort(cmp_success_words);
	priv_part->fill_combo_box(result, 0);
}
void ComposeSubstituteSentenceWidget::on_word_selected(int selected_index)
{
	QComboBox * sender_ = dynamic_cast<QComboBox*>(sender());
	std::size_t combo_box_starting_index=1;
	for(QComboBox * combo_box : priv_part->combo_box_container)
	{
		if (combo_box != sender_)
			combo_box_starting_index++;
		else
			break;
	}
	priv_part->reset(combo_box_starting_index);
	MatchingWord success_word = sender_->itemData(selected_index).value<MatchingWord>();
	std::size_t node_id = success_word.coveragePairs.front().second+1;

	printLog(eDebug, eInfoLogLevel, str(boost::format("node id %1%") % node_id));
	WordsList result = gDataThread->getSearchResult(node_id);
	if (result.size())
	{
		result.sort(cmp_success_words);
		priv_part->add_combo_box();
		WordsList::iterator it = std::unique(result.begin(),result.end());
		priv_part->fill_combo_box(WordsList(result.begin(),it), combo_box_starting_index);
	}
}
