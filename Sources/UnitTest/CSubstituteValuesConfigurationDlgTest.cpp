#include "CSubstituteValuesConfigurationDlgTest.h" 
#include <GUI\Settings\SubstituteValuesConfigurationDlg.h> 
#include <GUI\Settings\SubstituteValuesConfigurationDlg.cpp> 
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
	CSubstituteValuesConfigurationDlgTemp(const boost::property_tree::ptree &stProperties)
		: CSubstituteValuesConfigurationDlg(stProperties){}
};


CSubstituteValuesConfigurationDlgTest::CSubstituteValuesConfigurationDlgTest()
{
	char tabConsonants[] = {'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 
		'S', 'T', 'V', 'X', 'Z', 'W'};
	int iCount(sizeof(tabConsonants)/sizeof(char));
	m_stConfiguration.put("count",iCount);
	for(int i=0;i<iCount;i++)
	{
		m_stConfiguration.add("consonant",tabConsonants[i]);
	}
}
void CSubstituteValuesConfigurationDlgTest::init()
{
	m_ptrDialog.reset(new CSubstituteValuesConfigurationDlgTemp(m_stConfiguration));
}
void CSubstituteValuesConfigurationDlgTest::cleanup()
{
	m_ptrDialog.reset();  
}
void CSubstituteValuesConfigurationDlgTest::test_OtherMenusEntriesDisabled()
{
	unsigned int iDigitEntry(1);
	unsigned int iConsonantEntry(2);
	//choose one of consonants for digit '1' 
	EntryLine & entry = m_ptrDialog->m_ptrPriv->m_ptrDigitsEntries[iDigitEntry];
	//choose C (3rd after None and B
	entry.m_ptrConsonantsActions1[iConsonantEntry]->trigger();
	//wait till all signal/slots are triggered

	//check if in others digits this Action is deactivated
	for(unsigned int index=0;index<m_ptrDialog->m_ptrPriv->m_ptrDigitsEntries.size();index++)
	{
		if (index==iDigitEntry)
			continue;
		EntryLine & entry = m_ptrDialog->m_ptrPriv->m_ptrDigitsEntries[index];
		QCOMPARE(entry.m_ptrConsonantsActions1[iConsonantEntry]->isEnabled(),false);
		QCOMPARE(entry.m_ptrConsonantsActions2[iConsonantEntry]->isEnabled(),false);
	}
}
void CSubstituteValuesConfigurationDlgTest::test_SecondMenusDisabled()
{
	//check if in all FIRST Menus None is selected
	for(unsigned int index=0;index<m_ptrDialog->m_ptrPriv->m_ptrDigitsEntries.size();index++)
	{
		EntryLine & entry = m_ptrDialog->m_ptrPriv->m_ptrDigitsEntries[index];
		QCOMPARE(entry.m_ptrConsonantsActions1[0]->isChecked(),true);
		QCOMPARE(entry.m_ptrConsonantButton1->isEnabled(),true);
		QCOMPARE(entry.m_ptrConsonantButton2->isEnabled(),false);
	}
	unsigned int iDigitEntry(1);
	unsigned int iConsonantEntry(2);
	//choose one of consonants for digit '1' 
	EntryLine & entry = m_ptrDialog->m_ptrPriv->m_ptrDigitsEntries[iDigitEntry];
	//choose C (3rd after None and B
	entry.m_ptrConsonantsActions1[iConsonantEntry]->trigger();
	QCOMPARE(entry.m_ptrConsonantButton2->isEnabled(),true);
	entry.m_ptrConsonantsActions1[0]->trigger();
	QCOMPARE(entry.m_ptrConsonantButton2->isEnabled(),false);
}
