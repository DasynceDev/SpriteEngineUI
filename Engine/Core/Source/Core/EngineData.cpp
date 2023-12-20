#include "EngineData.h"

std::string spe::EngineData::s_PathUserProject = "";
std::string spe::EngineData::s_PathOfEngine = "";
std::string spe::EngineData::s_NameOfUser = "";
std::string spe::EngineData::s_Scene = "";
bool spe::EngineData::s_BuildDebug = true;
bool spe::EngineData::s_BuildRelease = false;

void spe::EngineData::BuildProjectFiles()
{
	const std::string current = spe::Utility::GetCurrentDir();

	spe::Utility::SetCurrentDir("Build");

	spe::Log::LogString("Building project files..");
	spe::Log::LogString(spe::Utility::RunCommand("cmake .."));
	spe::Log::LogString("========FINISHED==========");

	spe::Utility::SetCurrentDir(current);
}

void spe::EngineData::BuildProject()
{
    const std::string current = spe::Utility::GetCurrentDir();

    spe::Utility::SetCurrentDir("Build");

    std::thread projectFiles = std::thread([]() {
        spe::Log::LogString("Building project files..");
        spe::Log::LogString(spe::Utility::RunCommand("cmake .."));
        spe::Log::LogString("========FINISHED==========");
        });

    projectFiles.detach();


    if (spe::EngineData::s_BuildRelease) {
        auto releaseThread = std::thread([]() {
            spe::Log::LogString("Compiling in Release..");
            spe::Log::LogString(spe::Utility::RunCommand("cmake --build . --config Release"));
            spe::Log::LogString("========FINISHED======");
            });
        releaseThread.detach();
    }

    if (spe::EngineData::s_BuildDebug) {
        auto debugThread = std::thread([]() {
            spe::Log::LogString("Compiling in Debug..");
            spe::Log::LogString(spe::Utility::RunCommand("cmake --build . --config Debug"));
            spe::Log::LogString("========FINISHED======");
            });
        debugThread.detach();
    }

	spe::Utility::SetCurrentDir(current);
}
