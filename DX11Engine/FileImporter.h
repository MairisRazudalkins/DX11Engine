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
	static void OpenExplorerDialogue();
	static void ImportFile(FileType fileType, std::string filePath);
	static FileType ParsePathToType(std::string path);
};