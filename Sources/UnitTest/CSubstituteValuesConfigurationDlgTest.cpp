#include "CSubstituteValuesConfigurationDlgTest.h" 
#include <QMetaType>
#include <string>
#include <Main/GUI/Settings/SubstituteValuesConfigurationDlg.h>

const unsigned int ciSingleTimeout(1000); //in s
const unsigned int ciTimeoutsCount(5); //in s
Q_DECLARE_METATYPE(Qt::Orientation)
Q_DECLARE_METATYPE(QModelIndex)

void CSubstituteValuesConfigurationDlgTest::init()
{
	//m_ptrDialog=m_catalog[ "SubstituteValuesConfigurationDlg" ];
}
void CSubstituteValuesConfigurationDlgTest::cleanup()
{
	m_ptrDialog.reset();  
}
void CSubstituteValuesConfigurationDlgTest::test_1()
{
	//QTableView ptrTableView;
	//CShopHeaderView * shopHeaderView = new CShopHeaderView();
	//ptrTableView.setHorizontalHeader(shopHeaderView);

	////QStandardItemModel stModel;

	////QStandardItemModel * m_ptrModel = &stModel;
	//ptrTableView.setModel(m_ptrModel);
	//ptrTableView.show();
	//m_ptrModel->insertColumns(0,5);
	//m_ptrModel->insertRows(0,5);
	//QCOMPARE(m_ptrModel->rowCount(),6);
	//QCOMPARE(m_ptrModel->columnCount(),6);
	////////////////////////////////////////////////////////////////////////////
	//int iCount =0;
	//qRegisterMetaType<QModelIndex>("QModelIndex");
	//qRegisterMetaType<Qt::Orientation>("Qt::Orientation");
	//QSignalSpy spy(&ptrTableView, SIGNAL(quit()));
	//m_ptrModel->removeRow(3);
	//m_ptrModel->removeRows(0,m_ptrModel->rowCount());
	//m_ptrModel->insertRow(0);
	//m_ptrModel->insertRow(0);
	//m_ptrModel->insertRow(0);
	//m_ptrModel->removeRows(0,m_ptrModel->rowCount());
	//m_ptrModel->insertRow(m_ptrModel->rowCount());
	//while (spy.count() == 0)
	//{
	//	QTest::qWait(5000);
	//	++iCount;
	//	if (iCount>20)
	//	{
	//		QVERIFY2(false,"Timeout waiting for rowsRemoved signal");
	//		break;
	//	}
	//}
	//QCOMPARE(m_ptrModel->rowCount(),4);
	//QList<QVariant> firstSignal = spy.takeFirst();
	//QCOMPARE(firstSignal.at(1).toInt(), 0);
	//QCOMPARE(firstSignal.at(2).toInt(), 0);
	//////////////////////////////////////////////////////////////////////////
	//QTest::qWait(5000);
}
