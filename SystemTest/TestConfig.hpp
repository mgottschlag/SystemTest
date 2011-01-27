
#include <QObject>

class TestConfig
{
	public:
		TestConfig(QString workingdir, QString mainclass)
			: workingdir(workingdir), mainclass(mainclass)
		{
		}

		QString getWorkingDirectory()
		{
			return workingdir;
		}
		QString getMainClass()
		{
			return mainclass;
		}
	private:
		QString workingdir;
		QString mainclass;
};

