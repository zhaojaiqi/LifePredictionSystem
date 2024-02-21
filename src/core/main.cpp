#include "main.h"

#include "Application.h"

#include "../python/Py.h"
#include "../core/FileSystem.h"

Status Prologue()
{
    LOG("On Prologue");

    FileSystem::Init();

    if (Py::Initialize() == Status::FAILURE) return Status::FAILURE;

    Application::Initialize();


    return Status::SUCCESS;
}

int Main()
{
    auto file = fopen("./script/CNN/CNN.py", "rb");

    if (file) {
        PyRun_SimpleFile(file, "./script/CNN/CNN.py");

        delete file;
    }

    LOG("On Main");

    return Application::Execute();
}

void Epilogue()
{
    LOG("On Epilogue");

    Py::Finalize();

    Application::Shutdown();
}

int main(int argc, char* argv[]){
    int result;
    args = {argc, argv};

    result = static_cast<int>(Prologue());
    if (result == static_cast<int>(Status::SUCCESS)) result = Main();
    Epilogue();

    return result;
}
