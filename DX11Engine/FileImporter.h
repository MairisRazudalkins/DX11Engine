#pragma once

enum FileType
{
	obj,
	txt,
	dds,
	none
};

class FileImporter
{
	FileImporter() = default;
	
public:
	static void OpenExplorerDialogue(std::string& localFilePath);
	static std::string ImportFile(FileType fileType, std::string filePath);
	static FileType ParsePathToType(std::string path);
};