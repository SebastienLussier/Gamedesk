

#include "Core.h"
#include "FileManager/FileManager.h"
#include "FileManager/Checksum.h"

using namespace Gamedesk;

#include "Cpp Parser/HeaderReader.h"
#include "VCProj Parser/ProjReader.h"


void ParseAllFiles( const VC::Section* pSection, Cpp::HeaderReader& pReader );
void OutputFileHeader( TextOutputStream& out, const String& pFilename, const String& pDesc );
void GeneratePropertyFile( const String& pModuleName, const String& pPrecompiledHeader, Cpp::HeaderReader& reader );
void GeneratEnumFiles( const String& pModuleName, const String& pPrecompiledHeader, Cpp::HeaderReader& reader );
void ReplaceIfChanged( const String& pSourceFile, const String& pDestinationFile );
void PrintUsage();


int main( int argc, char* argv[] )
{
    //try
    {
        if( argc < 2 )
        {
            Core::SystemOut("Missing parameters\n");
            PrintUsage();
            return EXIT_FAILURE;
        }
        
        if( !FileManager::FileExist(argv[1]) )
        {
            Core::SystemOut("Project file (\"%s\") not found.\n", argv[1]);
            PrintUsage();
            return EXIT_FAILURE;
        }

        // Read and parse project file
        VC::ProjReader projReader;
        VC::Proj*      project;
        project = projReader.Read( argv[1] );

        // Get the project name.
        String projectName;
        if( !project->GetPropertyValue("VisualStudioProject.Name", projectName) )
        {
            GD_ASSERT_M(false, "Missing property VisualStudioProject.Name in project file!");
        }

        // Get the precompiled header filename.
        String precompiledHeaderOption;
        String precompiledHeaderFilename;
        if( project->GetPropertyValue("VisualStudioProject.Configurations.Configuration.Tool.UsePrecompiledHeader", precompiledHeaderOption) )
        {
            if( precompiledHeaderOption == String("2") )
                project->GetPropertyValue("VisualStudioProject.Configurations.Configuration.Tool.PrecompiledHeaderThrough", precompiledHeaderFilename);
        }

        // Change current directory...
        String path = argv[1];
        size_t pos = path.rfind('/');
        size_t posTmp = path.rfind('\\');
        if( pos == String::npos || (posTmp != String::npos && posTmp > pos) )
            pos = posTmp;
        path = path.substr(0, pos);
        SetCurrentDirectory(path.c_str());

        // Parse all header files found in the solution.
        Cpp::HeaderReader reader;
        for( MultiMap<String,VC::Section*>::const_iterator itProj(project->mSubSections.begin()); itProj != project->mSubSections.end(); ++itProj )
        {
            if( itProj->first == String("VisualStudioProject") )
            {
                for( MultiMap<String,VC::Section*>::const_iterator itFiles(itProj->second->mSubSections.begin()); itFiles != itProj->second->mSubSections.end(); itFiles++ )
                {
                    if( itFiles->first == String("Files") )
                    {
                        ParseAllFiles(itFiles->second, reader);
                    }                
                }
            }
        }
        
        // Generate the files with temp names...
        GeneratePropertyFile( projectName, precompiledHeaderFilename, reader );
        GeneratEnumFiles( projectName, precompiledHeaderFilename, reader );


        // Finaly, copy the files only if they changed
        const String PROP_SOURCE_FILE(projectName + String("Properties.cpp"));
        const String PROP_SOURCE_FILE_TMP(projectName + String("Properties.cpp.tmp"));
        ReplaceIfChanged(PROP_SOURCE_FILE_TMP, PROP_SOURCE_FILE);

        const String ENUM_HEADER_FILE(projectName + String("Enums.h"));
        const String ENUM_HEADER_FILE_TMP(projectName + String("Enums.h.tmp"));
        ReplaceIfChanged(ENUM_HEADER_FILE_TMP, ENUM_HEADER_FILE);

        const String ENUM_SOURCE_FILE(projectName + String("Enums.cpp"));
        const String ENUM_SOURCE_FILE_TMP(projectName + String("Enums.cpp.tmp"));
        ReplaceIfChanged(ENUM_SOURCE_FILE_TMP, ENUM_SOURCE_FILE);

        Core::SystemOut("Done!\n\n");
    }
    /*
    catch( const char* pError )
    {
        Core::SystemOut( "Property generation failed!\n" );
        Core::SystemOut( "Error encoutered: %s\n", pError );
        return EXIT_FAILURE;
    }
    catch( ... )
    {
        Core::SystemOut( "Property generation failed!\n" );
        return EXIT_FAILURE;
    }
    */

    return EXIT_SUCCESS;
}


void PrintUsage()
{
}


void ParseAllFiles( const VC::Section* pSection, Cpp::HeaderReader& pReader )
{
    for( MultiMap<String,VC::Section*>::const_iterator it(pSection->mSubSections.begin()); it != pSection->mSubSections.end(); it++ )
    {
        if( it->first == String("File") )
        {
            Map<String,String>::const_iterator itPath = it->second->mProperties.find("RelativePath");
            if(itPath != it->second->mProperties.end() )
            {
                static const String HEADER_FILE("h");

                String extension = FileManager::GetFileExtension( itPath->second );

                if( extension == HEADER_FILE )
                {
                    if( FileManager::FileExist( itPath->second ) )
                        pReader.Read(itPath->second);
                    else
                        Core::SystemOut("File not found on disk: %s\n", itPath->second.c_str() );
                }
                else
                {

                }
            }
        }
        else if( it->first == String("Filter") )
        {
            ParseAllFiles(it->second, pReader);
        }
    }
}


void OutputFileHeader( TextOutputStream& out, const String& pFilename, const String& pDesc )
{
    out << "/**\n"
        " *  @file       ";
    out << pFilename.c_str();
    out << "\n"
        " *  @brief      ";
    out << pDesc.c_str();
    out << "\n"
        " *  @author     gdprop tool.\n"
        " *\n"
        " *  WARNING!\n" 
        " *  This file has been autogenerated, all change made will be lost!\n"
        " */\n";
}


void GeneratePropertyFile( const String& pModuleName, const String& pPrecompiledHeader, Cpp::HeaderReader& reader )
{
    String fileName = pModuleName;
    fileName += String("Properties.cpp");

    String fileDesc("Properties for the ");
    fileDesc += pModuleName;
    fileDesc += " module.";

    OutputStream* binaryStream = reinterpret_cast<OutputStream*>(FileManager::CreateOutputStream(fileName + String(".tmp")));
    if( !binaryStream )
        throw "Unable to open property file for writing!";

    TextOutputStream out(*binaryStream);


    OutputFileHeader( out, fileName, fileDesc );
    if( !pPrecompiledHeader.empty() )
    {
        out << "#include \"";
        out << pPrecompiledHeader.c_str();
        out << "\"\n";
    }

    out << "#include \"";
    out << pModuleName.c_str();
    out << "Enums.h\"\n\n";

    out << "#if GD_CFG_USE_PROPERTIES == GD_ENABLED\n\n";

    for( List<Cpp::ClassDesc*>::iterator it(reader.mClassDesc.begin()); it != reader.mClassDesc.end(); ++it )
    {
        (*it)->OutputProperties(out);
    }

    out << "\n"
        "#endif\n";

    GD_DELETE(binaryStream);
}


void GeneratEnumFiles( const String& pModuleName, const String& pPrecompiledHeader, Cpp::HeaderReader& reader )
{
    const UInt32 HEADER = 0;
    const UInt32 SOURCE = 1;
    const UInt32 NB_FILES = 2;

    const String HEADER_FILE(pModuleName + String("Enums.h"));
    const String SOURCE_FILE(pModuleName + String("Enums.cpp"));

    // Output Header + Source
    for( UInt32 iFile = 0; iFile < NB_FILES; iFile++ )
    {
        String fileName = iFile == HEADER ? HEADER_FILE : SOURCE_FILE;

        String fileDesc("Enum strings for the ");
        fileDesc += pModuleName;
        fileDesc += " module.";

        OutputStream* binaryStream = reinterpret_cast<OutputStream*>(FileManager::CreateOutputStream(fileName + String(".tmp")));
        if( !binaryStream )
            throw "Unable to open enum file for writing!";

        TextOutputStream out(*binaryStream);

        OutputFileHeader( out, fileName, fileDesc );
        if( !pPrecompiledHeader.empty() )
        {
            out << "#include \"";
            out << pPrecompiledHeader.c_str();
            out << "\"\n";
        }

        if( iFile == SOURCE )
        {
            out << "#include \"";
            out << HEADER_FILE.c_str();
            out << "\"\n";
        }

        out << "\n";
        out << "#if GD_CFG_USE_PROPERTIES == GD_ENABLED\n\n";

        // Dependencies
        if( iFile == SOURCE )
        {
            reader.mEnumIncludes.unique(); 
            for( List<String>::iterator it(reader.mEnumIncludes.begin()); it != reader.mEnumIncludes.end(); ++it )
            {
                out << "#include \"";
                out << (*it);
                out << "\"\n";
            }
            out << "\n";
        }

        out << "namespace EnumInfo\n"
            "{\n";

        if( iFile == HEADER )
        {
            for( List<Cpp::EnumDesc*>::iterator it(reader.mEnumDesc.begin()); it != reader.mEnumDesc.end(); ++it )
                (*it)->OutputEnumHeaderInfo(out);
        }
        else
        {
            for( List<Cpp::EnumDesc*>::iterator it(reader.mEnumDesc.begin()); it != reader.mEnumDesc.end(); ++it )
                (*it)->OutputEnumCppInfo(out);
        }

        out << "}\n"
            "\n"
            "#endif\n";

        GD_DELETE(binaryStream);
    }
}


void ReplaceIfChanged( const String& pSourceFile, const String& pDestinationFile )
{
    Bool bReplace = false;

    if( !FileManager::FileExist(pDestinationFile) )
    {
        bReplace = true;
    }
    else
    {
        FILE* srcFile = 0;
        FILE* dstFile = 0;

        fopen_s(&srcFile, pSourceFile.c_str(), "rb");
        fopen_s(&dstFile, pDestinationFile.c_str(), "rb");

        GD_ASSERT(srcFile);
        GD_ASSERT(dstFile);

        MD5Checksum checksumSrc(srcFile);
        MD5Checksum checksumDst(dstFile);

        fclose(srcFile);
        fclose(dstFile);

        bReplace = checksumSrc.Digest() != checksumDst.Digest();
    }

    if( bReplace )
    {
        FileManager::MoveFile(pSourceFile, pDestinationFile, true);
        Core::SystemOut( "  %s updated...\n", pDestinationFile.c_str() );
    }
    else
    {
        FileManager::DeleteFile(pSourceFile);
    }
}
