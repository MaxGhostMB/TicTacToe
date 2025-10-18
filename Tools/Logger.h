/*
* Max Brockmann
* CMPM 123: Advanced Programming
* Assignment:
* make a basic logger class
* what do I need:
*      log to consol (or an imGUI window)
*      logs to a file
*      Contains levels of logging
*      Can set level of log to console
*/

#include "../imgui/imgui.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <cmath>

class Logger { 
    public:
        enum LogLevel {
            INFO     = 1 << 0,
            WARNING  = 1 << 1,
            ERROR    = 1 << 2
        };

        struct LogEntry {
            std::string message;
            LogLevel level;
            std::string Timestamp;
        };

    	static Logger& getInstance();

        void print(const LogLevel, const std::string&);

        void saveLog();

        void printLog(const std::string&);

        void printWarning(const std::string&);
        
        void printError(const std::string&);

        std::string loglvltostr(const LogLevel);
        
        void draw();

    private:
        Logger();
        ~Logger();

        std::ofstream outfile;
        std::vector<LogEntry> entries;
        bool scrollBottom = true;
        bool showInfo = true;
        bool showWarnings = true;
        bool showErrors = true;
     
};

extern Logger& ConLog;