#include "TestCases/CSubstituteValuesConfigurationDlgTest.h" 
#include <GUI\Settings\SubstituteValuesConfigurationDlg.h> 
#include <GUI\Settings\SubstituteValuesConfigurationDlg.cpp> 
#include <Data/CAppSettings_XMLKeywords.h>
//#include <QMetaType>
#include <string>

const unsigned int ciSingleTimeout(1000); //in s
const unsigned int ciTimeoutsCount(5); //in s
//Q_DECLARE_METATYPE(Qt::Orientation)
//Q_DECLARE_METATYPE(QModelIndex)

class CSubstituteValuesConfigurationDlgTemp : public CSubstituteValuesConfigurationDlg
{
	friend class CSubstituteValuesConfigurationDlgTest;
public:
	CSubstituteValuesConfigurationDlgTemp(CSubstituteValuesConfigurationDlgInit init_data)
		: CSubstituteValuesConfigurationDlg(init_data){}
};


void CSubstituteValuesConfigurationDlgTest::createProperiestConfiguration()
{
  char tabConsonants[] = {'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 
    'S', 'T', 'V', 'X', 'Z', 'W'};
  int iCount(sizeof(tabConsonants)/sizeof(char));
  consonants_values_set.put(CONSONANTS_COUNT_KEYWORD,iCount);
  for(int i=0;i<iCount;i++)
  {
    consonants_values_set.add(CONSONANTS_KEYWORD,tabConsonants[i]);
  }
}
void CSubstituteValuesConfigurationDlgTest::createDigitsConfiguration()
{
  systemDigitsConfiguration.reset();
  configurations.clear();
  systemDigitsConfiguration.mSystem[0] = std::make_pair('Z','S');
  systemDigitsConfiguration.mSystem[1] = std::make_pair('T','D');
  systemDigitsConfiguration.mSystem[2] = std::make_pair('N',' ');
  systemDigitsConfiguration.mSystem[3] = std::make_pair('M',' ');
  systemDigitsConfiguration.mSystem[4] = std::make_pair('R',' ');
  systemDigitsConfiguration.mSystem[5] = std::make_pair('L',' ');
  systemDigitsConfiguration.mSystem[6] = std::make_pair('J',' ');
  systemDigitsConfiguration.mSystem[7] = std::make_pair('K','G');
  systemDigitsConfiguration.mSystem[8] = std::make_pair('F','W');
  systemDigitsConfiguration.mSystem[9] = std::make_pair('P','B');

  for(SystemMap::const_iterator iter=systemDigitsConfiguration.mSystem.begin();
    iter!=systemDigitsConfiguration.mSystem.end();iter++)
  {
    systemDigitsConfiguration.allConsonants.push_back(iter->second.first);
    systemDigitsConfiguration.allConsonants.push_back(iter->second.second);
  }
  systemDigitsConfiguration.createConsonantsDigitsMap();
  systemDigitsConfiguration.strName="basic";
  configurations.push_back(systemDigitsConfiguration);
}

CSubstituteValuesConfigurationDlgTest::CSubstituteValuesConfigurationDlgTest()
{
  createProperiestConfiguration();
  createDigitsConfiguration();
}
void CSubstituteValuesConfigurationDlgTest::init()
{
  CSubstituteValuesConfigurationDlgInit init_data = 
  { consonants_values_set, configurations, "basic" };

  dialog.reset(new CSubstituteValuesConfigurationDlgTemp(init_data));
}
void CSubstituteValuesConfigurationDlgTest::cleanup()
{
	dialog.reset();  
}
void CSubstituteValuesConfigurationDlgTest::test_NoneInFirstMenusColumnSelected()
{
  CSubstituteValuesConfigurationDlgInit init_data = 
  { consonants_values_set, configurations, "" };

  dialog.reset(new CSubstituteValuesConfigurationDlgTemp(init_data));
	//check if in all FIRST Menus None is selected
	for(unsigned int index=0;index<dialog->priv_part->m_ptrDigitsEntries.size();index++)
	{
		EntryLine & entry = dialog->priv_part->m_ptrDigitsEntries[index];
		QVERIFY2(entry.m_ptrConsonantsActions1[0]->isChecked()==true,QString("Entry line for %1 row").arg(index).toLatin1());
		//QCOMPARE(entry.m_ptrConsonantsActions1[0]->isChecked(),true);
		QVERIFY2(entry.m_ptrConsonantButton1->isEnabled()==true,QString("Entry line for %1 row").arg(index).toLatin1());
		QVERIFY2(entry.m_ptrConsonantButton2->isEnabled()==false,QString("Entry line for %1 row").arg(index).toLatin1());
	}

}
void CSubstituteValuesConfigurationDlgTest::test_OtherMenusEntriesDisabled()
{
	unsigned int iDigitEntry(1);
	unsigned int iConsonantEntry(2);
	//choose one of consonants for digit '1' 
	EntryLine & entry = dialog->priv_part->m_ptrDigitsEntries[iDigitEntry];
	//choose C (3rd after None and B
	entry.m_ptrConsonantsActions1[iConsonantEntry]->trigger();
	//wait till all signal/Q_SLOTS are triggered

	//check if in others digits this Action is deactivated
	for(unsigned int index=0;index<dialog->priv_part->m_ptrDigitsEntries.size();index++)
	{
		if (index==iDigitEntry)
			continue;
		EntryLine & entry = dialog->priv_part->m_ptrDigitsEntries[index];
		QCOMPARE(entry.m_ptrConsonantsActions1[iConsonantEntry]->isEnabled(),false);
		QCOMPARE(entry.m_ptrConsonantsActions2[iConsonantEntry]->isEnabled(),false);
	}
}
void CSubstituteValuesConfigurationDlgTest::test_SecondMenusDisabled()
{
	unsigned int iDigitEntry(1);
	unsigned int iConsonantEntry(2);
	//choose one of consonants for digit '1'
	EntryLine & entry = dialog->priv_part->m_ptrDigitsEntries[iDigitEntry];
	//choose C (3rd after None and B
	entry.m_ptrConsonantsActions1[iConsonantEntry]->trigger();
	QCOMPARE(entry.m_ptrConsonantButton2->isEnabled(),true);
	entry.m_ptrConsonantsActions1[0]->trigger();
	QCOMPARE(entry.m_ptrConsonantButton2->isEnabled(),false);
}
void CSubstituteValuesConfigurationDlgTest::test_SelectNoneConfiguration()
{
	int iCount =0;
	QSignalSpy spy(dialog.get(), SIGNAL(set_selected_consonant_system(const QString &)));
	dialog->priv_part->m_ptrSystemsCombo->setCurrentIndex(0);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for columnsInserted signal");
			return;
		}
	}
	QList<QVariant> firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(0).toString(), QString(""));
	for(unsigned int index=0;index<dialog->priv_part->m_ptrDigitsEntries.size();index++)
	{
		EntryLine & entry = dialog->priv_part->m_ptrDigitsEntries[index];
		QVERIFY2(entry.m_ptrConsonantsActions1[0]->isChecked()==true,QString("Entry line for %1 row").arg(index).toAscii());
		QVERIFY2(entry.m_ptrConsonantButton1->isEnabled()==true,QString("Entry line for %1 row").arg(index).toAscii());
		QVERIFY2(entry.m_ptrConsonantButton2->isEnabled()==false,QString("Entry line for %1 row").arg(index).toAscii());
	}
}
