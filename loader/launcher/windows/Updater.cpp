#include <Windows.h>
#include <iostream>
#include <array>
#include <filesystem>

std::filesystem::path workingDir;
std::filesystem::path sapphireDir;
std::filesystem::path updatesDir;
std::filesystem::path resourcesDir;

void showError(std::string const& error) {
    MessageBoxA(nullptr, error.c_str(), "Error Loading Sapphire", MB_ICONERROR);
}

bool waitForFile(std::filesystem::path const& path) {
    if (!path.has_filename())
        return false;

    int delay = 10;
    HANDLE hFile;
    while ((hFile = CreateFileA(path.string().c_str(), FILE_GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_SHARING_VIOLATION) {
            Sleep(delay);
            // the delay would raise and go up to about 5 seconds, after which it will fail
            if (delay < 5120)
                delay *= 2;
        }
        else
            break;
    }
    if (hFile)
        CloseHandle(hFile);
    else {
        showError("Unable to update Sapphire: " + path.filename().string() + " is open by another process.");
        return false;
    }
    return true;
}

void updateFile(std::string const& name) {
    std::error_code error;
    if (!std::filesystem::exists(updatesDir / name, error) || error)
        return;
    if (!waitForFile(workingDir / name))
        return;

    std::filesystem::rename(updatesDir / name, workingDir / name, error);
    if (error) {
        showError("Unable to update Sapphire: Unable to move " + name + " - " + error.message());
        return;
    }
}

void removePath(std::filesystem::path const& path) {
    std::error_code error;
    if (!std::filesystem::exists(path, error) || error)
        return;
    if (path.has_filename() && !waitForFile(path))
        return;

    if (std::filesystem::is_directory(path) && !std::filesystem::is_empty(path))
        std::filesystem::remove_all(path, error);
    std::filesystem::remove(path, error);
    if (error) {
        if (path.has_filename())
            showError("Unable to update Sapphire: Unable to remove " + path.filename().string() + " - " + error.message());
        else
            showError("Unable to update Sapphire: Unable to remove " + path.string() + " - " + error.message());
        return;
    }
}

void updateResources() {
    std::error_code error;
    if (!std::filesystem::exists(updatesDir / "resources", error) || error)
        return;

    std::filesystem::remove_all(resourcesDir / "sapphire.loader", error);
    if (error) {
        showError("Unable to update Sapphire resources:" + error.message());
        return;
    }

    std::filesystem::rename(updatesDir / "resources", resourcesDir / "sapphire.loader", error);
    if (error) {
        showError("Unable to update Sapphire resources: " + error.message());
        return;
    }
}

int main(int argc, char* argv[]) {
    workingDir = std::filesystem::current_path();
    sapphireDir = workingDir / "sapphire";
    updatesDir = sapphireDir / "update";
    resourcesDir = sapphireDir / "resources";

    if (std::filesystem::exists(workingDir / "SapphireBootstrapper.dll"))
        removePath(workingDir / "SapphireBootstrapper.dll");

    if (std::filesystem::exists(sapphireDir) && std::filesystem::exists(updatesDir)) {
        updateFile("XInput9_1_0.dll");
        updateFile("Sapphire.dll");
        updateResources();
        removePath(updatesDir);
    }

    if (argc < 2)
        return 0;

    if (!waitForFile(workingDir / argv[1])) {
        showError("There was an error restarting GD. Please, restart the game manually.");
        return 0;
    }

    // restart gd using the provided path
    ShellExecuteA(NULL, "open", (workingDir / argv[1]).string().c_str(), "", workingDir.string().c_str(), TRUE);
    return 0;
}
