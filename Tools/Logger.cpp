#include "Logger.h"


/*
this function is taken from:
https://stackoverflow.com/questions/17223096/outputting-date-and-time-in-c-using-stdchrono
*/
std::string return_current_time_and_date()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

Logger& ConLog = Logger::getInstance();

Logger& Logger::getInstance() {
	static Logger instance;
	return instance;
}

void Logger::print(const LogLevel lvl, const std::string &str) {
    switch(lvl) {
        case LogLevel::INFO:
            printLog(str);
            break;
        case LogLevel::WARNING:
            printWarning(str);
            break;
        case LogLevel::ERROR:
            printError(str);
            break;
    }
}

Logger::Logger() {
    outfile.open("prev_game_log.log");
    if (!outfile.is_open()) {
        std::cerr << "Failed to open log file!" << std::endl;
    } else {
	    printLog("Logger initialised");
    }
}

Logger::~Logger() {
	saveLog();
    outfile.close();
}

void Logger::saveLog() {
	std::string now = return_current_time_and_date();
	std::string name = "[" + now + "].log";
	std::replace(name.begin(), name.end(), ' ', '_');
    std::replace(name.begin(), name.end(), '-', '_');
    std::replace(name.begin(), name.end(), ':', '_');
	if (outfile.is_open()) {
		outfile << "[" << now << "] log saved as " << name << std::endl;
		outfile.flush();
        LogEntry new_entry;
        new_entry.level = INFO;
        new_entry.message = "[" + now + "]", "log saved as " + name;
        new_entry.Timestamp = now;

		entries.push_back(new_entry);
	}

	try {
		std::filesystem::copy("prev_game_log.log", name, std::filesystem::copy_options::overwrite_existing);
	} catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Error during file copy: " << e.what() << std::endl;
	}
}

void Logger::printLog(const std::string & message_) {
    std::string time = return_current_time_and_date();

    LogEntry newlog;
    newlog.level = INFO;
    newlog.message = message_;
    newlog.Timestamp = time;

    outfile << time << "  " << message_<< std::endl;
    entries.push_back(newlog);
}

void Logger::printWarning(const std::string & message_) {
    std::string time = return_current_time_and_date();

    LogEntry newlog;
    newlog.level = WARNING;
    newlog.message = message_;
    newlog.Timestamp = time;

    outfile << time << "  " << message_ << std::endl;
    entries.push_back(newlog);

}

void Logger::printError(const std::string & message_) {
    std::string time = return_current_time_and_date();

    LogEntry newlog;
    newlog.level = ERROR;
    newlog.message = message_;
    newlog.Timestamp = time;

    outfile << time << "  " << message_ << std::endl;
    entries.push_back(newlog);
}

std::string Logger::loglvltostr(const LogLevel lvl) {
    switch(lvl) {
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        default: 
            return "UNKNOWN";
    }
}

void Logger::draw() {
    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(500, 300), ImVec2(FLT_MAX, FLT_MAX));  // Minimum size 500x300, no max size
	float window_width = ImGui::GetWindowWidth();
    
    ImGui::Begin("Logger");
    
    if (ImGui::Button("Test Log: INFO")) {
        print(INFO, "this is an INFO message");
    //     ImGui::LogFinish();
    }

    ImGui::SameLine(0.0f, 11.0f);
    if (ImGui::Button("Test Log: WARNING")) {
        print(WARNING, "this is a WARNING message");
    //     ImGui::LogFinish();
    }

    ImGui::SameLine(0.0f, 11.0f);
    if (ImGui::Button("Test Log: ERROR")) {
        print(ERROR, "this is an ERROR message");
    //     ImGui::LogFinish();
    }

    if (ImGui::Button("Show All Logs")) {
        showInfo = true;
        showWarnings = true;
        showErrors = true;
    //     ImGui::LogFinish();
    }

    ImGui::SameLine(0.0f, 11.0f);
    if (ImGui::Button("Only Show INFO")) {
        showInfo = true;
        showWarnings = false;
        showErrors = false;
    //     ImGui::LogFinish();
    }

    ImGui::SameLine(0.0f, 11.0f);
    if (ImGui::Button("Only Show WARNINGs")) {
        showInfo = false;
        showWarnings = true;
        showErrors = false;
    }

    ImGui::SameLine(0.0f, 11.0f);
    if (ImGui::Button("Only Show ERRORs")) {
        showInfo = false;
        showWarnings = false;
        showErrors = true;
    }

    
    ImGui::BeginChild("Log Entries", ImVec2(0, 0), true); 
	if (ImGui::BeginTable("LoggerTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Level", ImGuiTableColumnFlags_WidthFixed, 80.0f);
		ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, 150.0f);
		ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();

        for(LogEntry& data : entries) {
            const std::string& lev_ = loglvltostr(data.level);

            ImVec4 color;
            switch (data.level) {
                case LogLevel::INFO: 
                    color = ImVec4(0.6f, 0.8f, 1.0f, 1.0f); 
                    break;
                case LogLevel::WARNING: 
                    color = ImVec4(1.0f, 0.8f, 0.2f, 1.0f); 
                    break;
                case LogLevel::ERROR: 
                    color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); 
                    break;
                default: 
                    color = ImVec4(1, 1, 1, 1); break;
            }

            if ((lev_ == "INFO" && showInfo) ||
                (lev_ == "WARNING" && showWarnings) ||
                (lev_ == "ERROR" && showErrors)) {

                std::string mes_ = data.message;
                std::string& timestamp_ = data.Timestamp;
        
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextColored(color, "%s", lev_.c_str());
                //ImGui::Image((ImTextureID)(intptr_t)icons[i], ImVec2(32, 32));

                ImGui::TableSetColumnIndex(1);
                ImGui::TextWrapped("%s", timestamp_.c_str());
                
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%s", mes_.c_str());
                }
        }

	    ImGui::EndTable();
    }
    ImGui::EndChild();

    ImGui::End();
}