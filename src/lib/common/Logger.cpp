/*
 * Logger.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "Logger.h"

#include <format.cc>

#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

namespace threescanner {

static const vector<string> LEVEL_NAMES { "FATAL", "1", "2", "ERROR", "WARNING", "5", "INFO", "DEBUG", "TRACE" };

void log(const int level, const string& text) {
	cerr << setfill(' ') << setw(8) << LEVEL_NAMES[level];
	cerr << " ";
	cerr << text;
	cerr << endl;
}

void log(const int level, const string& format, const fmt::ArgList& args) {
	cerr << setfill(' ') << setw(8) << LEVEL_NAMES[level];
	cerr << " ";
	cerr << fmt::sprintf(format, args);
	cerr << endl;
}

} /* namespace threescanner */
