
#include "Test.hpp"
#include "TestConfig.hpp"

#include <iostream>
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QProcess>

Test::Test(QString name, QObject *parent)
	: QObject(parent), name(name)
{
}
Test::~Test()
{
}

bool Test::load(QString path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}
	QTextStream stream(&file);
	while (!stream.atEnd())
	{
		data.push_back(stream.readLine());
	}
	return true;
}

unsigned int Test::execute(TestConfig &config)
{
	std::cerr << "-- Test \"" << name.toStdString() << "\" --------" << std::endl;
	unsigned int errorcount = 0;
	// Start the process
	QProcess process;
	process.setProcessChannelMode(QProcess::MergedChannels);
	process.setWorkingDirectory(config.getWorkingDirectory());
	process.start("java", QStringList(config.getMainClass()));
	// Work through all rules and check the program output
	for (int i = 0; i < data.size(); i++)
	{
		QString rule = data[i];
		if (rule.left(1) == ">")
		{
			std::cout << rule.toStdString() << std::endl;
			QByteArray input = (rule.mid(1) + "\n").toUtf8();
			if (process.write(input) != input.size())
			{
				std::cerr << "Error: Unexpected EOF (" << i << ")" << std::endl;
				errorcount++;
				break;
			}
		}
		else if (rule.mid(0, 1) == "<")
		{
			while (!process.canReadLine())
			{
				if (!process.waitForReadyRead(3000))
				{
					std::cerr << "Error: Timeout (" << i << ")" << std::endl;
					errorcount++;
					break;
				}
			}
			QString line = process.readLine();
			if (line.size() != 0)
				line = line.left(line.size() - 1);
			std::cout << "<" << line.toStdString() << std::endl;
			QRegExp regex(rule.mid(1));
			if (regex.exactMatch(line))
			{
				std::cerr << "PASS (" << i << ")" << std::endl;
			}
			else
			{
				std::cerr << "FAIL (" << i << ")" << std::endl;
				errorcount++;
			}
		}
		else
		{
			std::cerr << "Error: Invalid test rule (" << i << ")." << std::endl;
			errorcount++;
		}
	}
	// Check for EOF
	char dummy;
	if (process.read(&dummy, 1) == 1)
	{
		std::cerr << "Error: EOF expected at the end of the test." << std::endl;
	}
	process.kill();
	std::cerr << "-- " << errorcount << " errors. ----------" << std::endl;
	return errorcount;
}
