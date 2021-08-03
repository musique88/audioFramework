#include <fstream>
#include <string>

namespace MSQ
{
	class Log
	{
	public:
		static Log* Instance();
		enum Flags { 
			LOG_INFO = 0b01,
			LOG_WARN = 0b010,
			LOG_ERR = 0b0100,
			LOG_ALL = 0b0111,
			LOG_FILE_INFO = 0b01000,
			LOG_FILE_WARN = 0b010000,
			LOG_FILE_ERR = 0b0100000,
			LOG_FILE_ALL = 0b0111000
		};
		void SetLogFile(const char* file);
		void SetFlags(const int& flags);
		void Err(const std::string& s);
		void Warn(const std::string& s);
		void Info(const std::string& s);
	private:
		std::ofstream _file;
		static Log* _instance;
		int _flags;
		Log();
		~Log();
	};
} 
