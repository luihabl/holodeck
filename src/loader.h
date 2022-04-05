#pragma once 

#include <glm/glm.hpp>

#include <vector>
#include <filesystem>

namespace holodeck
{
    namespace Loader
    {
        struct FileLoader
        {
            FileLoader() = default;
            
            std::filesystem::path path;
            void clear() {};
        };
        

        struct OBJFile : public FileLoader
        {
            OBJFile(const std::filesystem::path& file_path, bool invert_uv = false);

            std::vector<glm::vec4> positions;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> uv;
            std::vector<unsigned> indices;

        };

        struct DDSFile : public FileLoader
        {
            DDSFile(const std::filesystem::path& file_path);
            // ~DDSFile();
            void clear();
            
            std::vector<std::vector<unsigned char>> data;

            enum class Compression
            {
                DXT1 = 0x31545844,
                DXT2 = 0x32545844,
                DXT3 = 0x33545844,
                DXT4 = 0x34545844,
                DXT5 = 0x35545844
            } format;

            typedef uint32_t DWORD_local;
            struct DDS_PIXELFORMAT {
                DWORD_local dwSize;
                DWORD_local dwFlags;
                DWORD_local dwFourCC;
                DWORD_local dwRGBBitCount;
                DWORD_local dwRBitMask;
                DWORD_local dwGBitMask;
                DWORD_local dwBBitMask;
                DWORD_local dwABitMask;
            };

            struct {
                DWORD_local     dwSize;
                DWORD_local     dwFlags;
                DWORD_local     dwHeight;
                DWORD_local     dwWidth;
                DWORD_local     dwPitchOrLinearSize;
                DWORD_local     dwDepth;
                DWORD_local     dwMipMapCount;
                DWORD_local     dwReserved1[11];
                DDS_PIXELFORMAT ddspf;
                DWORD_local     dwCaps;
                DWORD_local     dwCaps2;
                DWORD_local     dwCaps3;
                DWORD_local     dwCaps4;
                DWORD_local     dwReserved2;
            } header;

            DWORD_local block_size;

        };



    }
}