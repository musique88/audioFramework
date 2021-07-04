#include "Log.hpp"
#include <iostream>
#include <time.h>

namespace MSQ
{
	time_t rawTime;
	struct tm* timeInfo;
	Log* Log::_instance = nullptr;
	Log* Log::instance()
	{
		if (_instance == nullptr)
			_instance = new Log();
		return _instance;
	}

	Log::Log()
	{
		_flags = LOG_ALL;
	}

	void Log::Err(const std::string& s)
	{
		if(!(_flags & LOG_ERR || (_file.is_open() && _flags & LOG_FILE_ERR)))
			return;
		time(&rawTime);
		timeInfo = localtime(&rawTime);
		std::string time = std::string(asctime(timeInfo));
		time = time.substr(0,time.length()-1);
		if(_flags & LOG_ERR)
			std::cout << time << "\033[1;31m [ERROR] \033[21m" << s << "\033[0m" << std::endl;
		if(_file.is_open() && _flags & LOG_FILE_ERR)
			_file << time << " [ERROR] " << s << std::endl;
	}
	void Log::Warn(const std::string& s)
	{
		if(!(_flags & LOG_WARN || (_file.is_open() && _flags & LOG_FILE_WARN)))
			return;
		time(&rawTime);
		timeInfo = localtime(&rawTime);        
		std::string time = std::string(asctime(timeInfo));
		time = time.substr(0,time.length()-1);
		if(_flags & LOG_WARN)
			std::cout << time << "\033[1;33m [WARNING] \033[21m" << s << "\033[0m" << std::endl;
		if(_file.is_open() && _flags & LOG_FILE_WARN)
			_file << time << " [WARNING] " << s << std::endl;
	}
	void Log::Info(const std::string& s)
	{
		if(!(_flags & LOG_INFO || (_file.is_open() && _flags & LOG_FILE_INFO)))
			return;
		time(&rawTime);
		timeInfo = localtime(&rawTime);
		std::string time = std::string(asctime(timeInfo));
		time = time.substr(0,time.length()-1);
		if(_flags & LOG_INFO)
			std::cout << time << "\033[1m [INFO] \033[21m" << s << "\033[0m" << std::endl;
		if(_file.is_open() && _flags & LOG_FILE_INFO)
			_file << time << " [INFO] " << s << std::endl;
	}

	void Log::SetLogFile(const char* f)
	{
		if (_file.is_open())
			_file.close();
		_file.open(f);

		if (_file.bad())
			instance()->Err("Bad file");
		else
		{
			time(&rawTime);
			timeInfo = localtime(&rawTime);
			std::string time = std::string(asctime(timeInfo));
			time = time.substr(0,time.length()-1);
			_file << "Log file started on " << time << std::endl;  
		}
	}

	void Log::SetFlags(const int& flags)
	{
		_flags = flags;
	}

	Log::~Log()
	{
		delete _instance;
	}
}