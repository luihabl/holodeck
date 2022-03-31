#pragma once 

#include <string>
#include <filesystem>
#include <cstdio>

namespace holodeck {
    namespace File {
        
        std::string load_txt(std::string file_path);
        unsigned char * load_image(std::string file_path, int *w, int *h, int *comp);
        void free_image(unsigned char * data);

        struct Raw
        {
            FILE* fp;
            std::filesystem::path path;

            Raw() = default;
            Raw(const std::filesystem::path& file_path) : path(file_path) {};
            
            // Since the destructor will close the file, the file 
            // will close when copying.
            ~Raw();

            void open(const std::string& mode = "rb");
            void close();

            bool eof(){return feof(fp);}

        };
    }
}