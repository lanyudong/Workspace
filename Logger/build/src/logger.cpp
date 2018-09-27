#include "include/logger.h"
#include <iostream>
#include <fstream>

Logger::Logger (std::string context) : context_(context) {
	msg_ =  tsu::GetDateTime () + '\t' + context + '\t';
}  // end constructor

Logger::~Logger () {
	Logger::ToFile ();
}  // end destructor

void Logger::ToFile  () {
	std::string file_name = "LOGS/test_" + context_ + "_" + tsu::GetDate () + ".log";
	std::ofstream output_file(file_name, std::ios_base::app);
	if (output_file.is_open()) {
		output_file << msg_ << '\n';
	}
	output_file.close();
}  // end To File
