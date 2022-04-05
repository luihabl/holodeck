#include "loader.h"

#include "file.h"
#include "log.h"

#include <sstream>
#include <array>
#include <algorithm>

using namespace holodeck::Loader;

// Helper function to split string
std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delim))
    {
        result.push_back(item);
    }
    return result;
}

bool is_number(const std::string &str)
{
    try
    {
        stod(str);
        return true;
    }
    catch (const std::invalid_argument &e)
    {
        return false;
    }
}

template <typename T>
T convert_to(const std::string &str)
{
    std::istringstream ss(str);
    T num;
    ss >> num;
    return num;
}

template <typename T>
bool valid_numeric_entry(unsigned index, std::vector<T> &vec)
{
    if (index > vec.size() - 1)
        return false;

    return is_number(vec[index]) && !vec[index].empty();
}

template <size_t N_INDEX>
void generate_global_indices(
    const std::array<std::vector<unsigned>, N_INDEX> indices_list,
    std::vector<unsigned> &out_global_indices,
    std::vector<std::array<int, N_INDEX>> &out_grouped_indices)
{
    out_global_indices = {};
    out_grouped_indices = {};

    unsigned size = indices_list[0].size();
    for (int i = 0; i < size; i++)
    {
        std::array<int, N_INDEX> group;
        for (int j = 0; j < N_INDEX; j++)
        {
            if (i < indices_list[j].size())
                group[j] = indices_list[j][i];
            else
                group[j] = -1;
        }

        auto it = std::find(out_grouped_indices.begin(), out_grouped_indices.end(), group);
        if (it != out_grouped_indices.end())
        {
            size_t id = std::distance(out_grouped_indices.begin(), it);
            out_global_indices.push_back(id);
        }
        else
        {
            out_grouped_indices.push_back(group);
            out_global_indices.push_back(out_grouped_indices.size() - 1);
        }
    }
}

OBJFile::OBJFile(const std::filesystem::path &file_path, bool invert_uv)
{

    path = file_path;

    File::Raw file(path);
    file.open("r");

    char c[512];

    std::vector<glm::vec4> packed_positions = {};
    std::vector<glm::vec3> packed_normals = {};
    std::vector<glm::vec2> packed_uv = {};

    std::vector<unsigned> indices_pos;
    std::vector<unsigned> indices_n;
    std::vector<unsigned> indices_uv;

    while (!file.eof())
    {
        fgets(c, 512, file.fp);

        if (c[0] == '#')
            continue;

        if (c[0] == 'v' && c[1] == 'n')
        {
            glm::vec3 normal(1.0f);

            auto comp = split(c, ' ');
            for (int i = 1; i < 4; i++)
            {
                if (valid_numeric_entry(i, comp))
                    normal[i - 1] = convert_to<float>(comp[i]);
            }
            packed_normals.push_back(normal);
        }

        else if (c[0] == 'v' && c[1] == 't')
        {
            glm::vec2 u(1.0f);

            auto comp = split(c, ' ');
            for (int i = 1; i < 3; i++)
            {
                if (valid_numeric_entry(i, comp))
                    u[i - 1] = convert_to<float>(comp[i]);
            }
            packed_uv.push_back(u);
        }

        else if (c[0] == 'v' && c[1] == ' ')
        {
            glm::vec4 pos(1.0f);

            auto comp = split(c, ' ');
            // Need to add possible colors at vertices when comp.size()-1 == 6
            for (int i = 1; i < 4; i++)
            {
                if (valid_numeric_entry(i, comp))
                    pos[i - 1] = convert_to<float>(comp[i]);
            }
            packed_positions.push_back(pos);
        }

        else if (c[0] == 'f' && c[1] == ' ')
        {
            auto comp = split(c, ' ');
            for (int i = 1; i < 4; i++)
            {
                auto index_fields = split(comp[i], '/');

                if (valid_numeric_entry(0, index_fields))
                    indices_pos.push_back(convert_to<unsigned>(index_fields[0]) - 1);

                if (valid_numeric_entry(1, index_fields))
                    indices_uv.push_back(convert_to<unsigned>(index_fields[1]) - 1);

                if (valid_numeric_entry(2, index_fields))
                    indices_n.push_back(convert_to<unsigned>(index_fields[2]) - 1);
            }
        }
    }

    std::vector<std::array<int, 3>> packed_indices;
    generate_global_indices({indices_pos, indices_n, indices_uv}, indices, packed_indices);

    positions = {};
    normals = {};
    uv = {};

    for (const auto &group : packed_indices)
    {
        if (group[0] >= 0)
            positions.push_back(packed_positions[group[0]]);

        if (group[1] >= 0)
            normals.push_back(packed_normals[group[1]]);

        if (group[2] >= 0)
        {
            glm::vec2 tmp = packed_uv[group[2]];
            if (invert_uv)
                tmp.y = 1.0f - tmp.y;

            uv.push_back(tmp);
        }
            
    }
}

DDSFile::DDSFile(const std::filesystem::path &file_path)
{
    path = file_path;

    File::Raw file(path);
    file.open("rb");

    DWORD_local filecode;
    fread(&filecode, sizeof(DWORD_local), 1, file.fp);
    if (strncmp((char *)&filecode, "DDS ", 4) != 0)
    {
        Log::warn("Not DDS file: %s", file_path.c_str());
        return;
    }

    fread(&header, sizeof(header), 1, file.fp);

    format = (Compression)header.ddspf.dwFourCC;

    DWORD_local bufsize = header.dwPitchOrLinearSize;
    bufsize = header.dwMipMapCount > 1 ? 2 * bufsize : bufsize;

    std::vector<unsigned char> temp_data(bufsize);
    fread(temp_data.data(), sizeof(unsigned char), bufsize, file.fp);

    data.resize(header.dwMipMapCount);

    block_size = format == Compression::DXT1 ? 8 : 16;
    unsigned int width = header.dwWidth;
    unsigned int height = header.dwHeight;
    unsigned int i0 = 0, i1 = 0;

    for (int i = 0; i < header.dwMipMapCount; i++)
    {       
        unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * block_size;
        
        i1 = size;
        data[i] = std::vector<unsigned char>(temp_data.begin() + i0, temp_data.end() + i1 + 1);

        i0 += size;
        width /= 2;
        height /= 2;

        // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
        if (width < 1)
            width = 1;
        if (height < 1)
            height = 1;
    }
}
