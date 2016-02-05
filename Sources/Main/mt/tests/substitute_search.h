#include <QtTest/QtTest>
#include <TestRunner.h>
#include <boost/shared_ptr.hpp>
#include <data/CSystemDigitsConfiguration.h>

class MatchingWordsSearch;
class DictionaryData;

class mt_substitute_search : public QObject
{
  Q_OBJECT
  CSingleSubstituteDigitsConfiguration   single_substitute_digits_configuration;
  boost::shared_ptr<MatchingWordsSearch> substitute_search;
  boost::shared_ptr<DictionaryData>      dictionary_data;

private:
  void create_system_digits_configuration();
  void create_dictionary_data();
private Q_SLOTS:
  void initTestCase();
  void init();
  void test_SubstituteSearch_WholeCorrectWord_01();
};
DECLARE_TEST(mt_substitute_search)
