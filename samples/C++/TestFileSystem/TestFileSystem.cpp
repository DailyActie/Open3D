// ----------------------------------------------------------------------------
// -                        Open3D: www.open-3d.org                            -
// ----------------------------------------------------------------------------
// Imported on Apr 11th, 2018 from the Open3D project
// Copyright (c) 2018 www.open3d.org
// released under MIT licence
//
// A fork of their project is avilable at legacy-code.open-3d.org
// Refer to http://legacy-code.open-3d.org/blob/master/LICENSE
// for their full licence
// Check http://legacy-code.open-3d.org/graphs/contributors
// to get the list of their contributors and their contributions
//
// The rights of any later updates or modifications are reserved
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018, Hamdi Sahloul - www.open-3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include <iostream>

#include <Open3D/Core/Core.h>
#include <Open3D/IO/IO.h>

void PrintHelp()
{
    using namespace open3d;
    PrintInfo("Usage :\n");
    PrintInfo("    > TestFileSystem ls [dir]\n");
    PrintInfo("    > TestFileSystem mkdir [dir]\n");
    PrintInfo("    > TestFileSystem rmdir [dir]\n");
    PrintInfo("    > TestFileSystem rmfile [file]\n");
    PrintInfo("    > TestFileSystem fileexists [file]\n");
}

int main(int argc, char **args)
{
    using namespace open3d::filesystem;

    std::string directory, function;
    if (argc <= 1) {
        PrintHelp();
        return 0;
    } else {
        function = std::string(args[1]);
        if (argc <= 2) {
            directory = ".";
        } else {
            directory = std::string(args[2]);
        }
    }

    if (function == "ls") {
        std::vector<std::string> filenames;
        ListFilesInDirectory(directory, filenames);

        for (const auto &filename : filenames) {
            std::cout << filename << std::endl;
            std::cout << "parent dir name is : " <<
                    GetFileParentDirectory(filename) << std::endl;
            std::cout << "file name only is : " <<
                    GetFileNameWithoutDirectory(filename) << std::endl;
            std::cout << "extension name is : " <<
                    GetFileExtensionInLowerCase(filename) << std::endl;
            std::cout << "file name without extension is : " <<
                    GetFileNameWithoutExtension(filename) << std::endl;
            std::cout << std::endl;
        }
    } else if (function == "mkdir") {
        bool success = MakeDirectoryHierarchy(directory);
        std::cout << "mkdir " << (success ? "succeeded" : "failed")
                << std::endl;
    } else if (function == "rmdir") {
        bool success = DeleteDirectory(directory);
        std::cout << "rmdir " << (success ? "succeeded" : "failed")
                << std::endl;
    } else if (function == "rmfile") {
        bool success = RemoveFile(directory);
        std::cout << "rmfile " << (success ? "succeeded" : "failed")
                << std::endl;
    } else if (function == "fileexists") {
        bool success = FileExists(directory);
        std::cout << "fileexists " << (success ? "yes" : "no") << std::endl;
    }
    return 0;
}
