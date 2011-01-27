
#include "TestSuite.hpp"
#include "Test.hpp"

#include <iostream>
#include <QFile>
#include <QTextStream>

TestSuite::TestSuite(QObject *parent)
	: QObject(parent)
{
}
TestSuite::~TestSuite()
{
}

bool TestSuite::load(QString path)
{
	QString directory = "";
	if (path.lastIndexOf("/") != -1)
		directory = path.left(path.lastIndexOf("/") + 1);
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}
	QTextStream stream(&file);
	while (!stream.atEnd())
	{
		QString line = stream.readLine();
		if (line != "")
		{
			Test *test = new Test(line, this);
			if (!test->load(directory + line))
			{
				std::cerr << "Warning: Could not load test \"" << (directory + line).toStdString() << "\"." << std::endl;
				delete test;
			}
			else
			{
				tests.push_back(test);
			}
		}
	}
	return true;
}

unsigned int TestSuite::execute(TestConfig &config)
{
	unsigned int errorcount = 0;
	for (int i = 0; i < tests.size(); i++)
	{
		errorcount += tests[i]->execute(config);
	}
	return errorcount;
}

