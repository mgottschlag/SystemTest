
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
	std::cerr << "\033[34m-- Test \"" << name.toStdString() << "\" --------\033[m" << std::endl;
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
				std::cerr << "\033[31mError: Unexpected EOF (" << i << ")\033[m" << std::endl;
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
					std::cerr << "\033[31mError: Timeout (" << i << ")\033[m" << std::endl;
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
				std::cerr << "\t\033[33mPASS (" << i << ")\n\033[m" << std::endl;
			}
			else
			{
				std::cerr << "\t\033[1m\033[31mFAIL (" << i << ")\033[m\n" << std::endl;
				errorcount++;
			}
		}
		else
		{
			std::cerr << "\033[31mError: Invalid test rule (" << i << ").\033[m" << std::endl;
			errorcount++;
		}
	}
	// Check for EOF
	char dummy;
	if (process.read(&dummy, 1) == 1)
	{
		std::cerr << "\033[31mError: EOF expected at the end of the test.\033[m" << std::endl;
	}
	process.kill();
	std::cerr << "\033[1m\033[36m-- " << errorcount << " errors. ----------\033[m" << std::endl;
	return errorcount;
}