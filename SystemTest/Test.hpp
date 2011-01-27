
#include <QObject>
#include <QString>
#include <QList>

class TestConfig;

class Test : public QObject
{
	Q_OBJECT
	public:
		Test(QString name, QObject *parent);
		~Test();

		bool load(QString path);

		unsigned int execute(TestConfig &config);
	private:
		QString name;
		QList<QString> data;
};
