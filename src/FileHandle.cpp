#include "FileHandle.h"





std::string FileHandle::getProjectDir()
{
	std::filesystem::path project_dir = std::filesystem::current_path().parent_path().parent_path().parent_path();
	m_p_logger->writeLog(LogLevel::INFO, "FILEHANDLE", "Found project dir in: " + project_dir.string());
	return project_dir.string();

}

void FileHandle::setLogger(Log* logger)
{
	m_p_logger = logger;
}

