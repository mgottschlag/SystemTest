
#include "TestConfig.hpp"


TestConfig::TestConfig(QObject *parent) : QObject(parent)
{
}
TestConfig::~TestConfig()
{
}

bool TestConfig::load(QString path)
{
	QFile file(path);
	if (!file.open(IO_ReadOnly))
	{
		return false;
	}

	return true;
}

