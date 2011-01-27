
#include <QObject>
#include <QList>

class TestConfig;
class Test;

class TestSuite : public QObject
{
	Q_OBJECT
	public:
		TestSuite(QObject *parent = 0);
		~TestSuite();

		bool load(QString path);

		unsigned int execute(TestConfig &config);
	private:
		QList<Test*> tests;
};

