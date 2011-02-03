
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
	QList<QString> errortests;
	QList<unsigned int> errors;
	unsigned int errorcount = 0;
	for (int i = 0; i < tests.size(); i++)
	{
		unsigned int testerrors = tests[i]->execute(config);
		errorcount += testerrors;
		if (testerrors > 0)
		{
			errortests.append(tests[i]->getName());
			errors.append(testerrors);
		}
	}
	// Print summary
	std::cout << "Summary:" << std::endl;
	for (int i = 0; i < errors.size(); i++)
	{
		std::cout << errortests[i].toStdString() << ": " << errors[i] << " errors." << std::endl;
	}
	std::cout << errorcount << " errors occured in all tests." << std::endl;
	return errorcount;
}

