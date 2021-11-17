#include "CoreMinimal.h"
#include "FileImporter.h"
#include "OBJLoader.h"

#include "Graphics.h"

void FileImporter::OpenExplorerDialogue() // directly from windows - https://docs.microsoft.com/en-us/windows/win32/learnwin32/example--the-open-dialog-box
{
    if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
    {
        COMDLG_FILTERSPEC fileTypes[] = { { L".obj", L"*.obj" }, { L".txt", L"*.txt" } };

        IFileOpenDialog* dialogue;

        if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&dialogue))))
        {
            dialogue->SetFileTypes(2, fileTypes);
            dialogue->Show(NULL);

            IShellItem* item;

            if (SUCCEEDED(dialogue->GetResult(&item)))
            {
                PWSTR filePath;

                if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &filePath)))
                {
                    std::wstring ws(filePath);
                    std::string filePathStr(ws.begin(), ws.end()); // convert PWSTR to string

                    ImportFile(ParsePathToType(filePathStr), filePathStr);
                }

                item->Release();
            }

            dialogue->Release();
        }

        CoUninitialize();
    }
}

FileType FileImporter::ParsePathToType(std::string path)
{
    FileType type = FileType::none;
    std::string fileType = path.substr(path.find_last_of(".") + 1);

    if (fileType == "obj")
        type = FileType::obj;
    else if (fileType == "txt")
        type = FileType::txt;
    else if (fileType == "dds")
        type = FileType::dds;

    return type;
}

void FileImporter::ImportFile(FileType fileType, std::string filePath) // TODO: Implement actual loading of objects into memory for use inside application.
{
    TCHAR path[MAX_PATH] = L"";
    GetCurrentDirectory(MAX_PATH, path);

    std::wstring ws(path);
    std::string projPathStr(ws.begin(), ws.end()); // convert TCHAR to string

    //TODO: Allow for adding to different project directories
    if (!CopyFileA(filePath.c_str(), (projPathStr + "\\Assets\\Models\\" + filePath.substr(filePath.find_last_of("\\") + 1)).c_str(), FALSE)) // copy file to project directory.
    {
        Logger::ENGINE_LOG(Logger::Error, "Failed to copy file to project path. ERROR:", GetLastError());
        return;
    }

    //MeshData data = OBJLoader::Load(filePath.c_str(), Graphics::GetDevice(), false);

    //switch (fileType)
    //{
    //case obj:
    //    MeshData data = OBJLoader::Load(filePath.c_str(), Graphics::GetDevice(), false);
    //    break;
    //case txt:
    //
    //    break;
    //case dds:
    //
    //    break;
    //default:
    //    break;
    //}

    Logger::ENGINE_LOG(Logger::Info, "Imported " + filePath.substr(filePath.find_last_of("\\") + 1));
}