#include "loader.h"

#include "file.h"

#include <sstream>

using namespace holodeck::Loader;


//Helper function to split string 
std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (std::getline (ss, item, delim)) {
        result.push_back (item);
    }
    return result;
}

bool is_number(const std::string& str)
{
    // for (char const &c : str) {
    //     if (std::isdigit(c) == 0) return false;
    // }
    // return true;

    try
    {
        stod(str);
        return true;
    }
    catch (const std::invalid_argument& e)
    {
        return false;
    }
}

template <typename T> 
T convert_to (const std::string &str)
{
    std::istringstream ss(str);
    T num;
    ss >> num;
    return num;
}

template <typename T>
bool valid_numeric_entry(unsigned index, std::vector<T>& vec)
{
    if (index > vec.size() - 1)
        return false;
        
    return is_number(vec[index]) && !vec[index].empty();
}

#include "log.h"
OBJFile::OBJFile(const std::filesystem::path& file_path)
{

    path = file_path;

    File::Raw file(path);
    file.open("r");

    char c[512];
        
    positions = {};
    normals = {};
    uv = {};
    
    std::vector<unsigned> indices_pos;
    std::vector<unsigned> indices_uv;
    std::vector<unsigned> indices_n;

    while(!file.eof())
    {
        fgets(c, 512, file.fp);

        if (c[0] == '#')
            continue;
        
        if(c[0] == 'v' && c[1] == 'n')
        {
            glm::vec3 normal(1.0f);
            
            auto comp = split(c, ' ');
            for (int i = 1; i < 4; i++)
            {
                if(valid_numeric_entry(i, comp))
                    normal[i - 1] = convert_to<float>(comp[i]);
            }
            normals.push_back(normal);
        }

        else if(c[0] == 'v' && c[1] == ' ')
        {   
            glm::vec4 pos(1.0f);

            auto comp = split(c, ' ');
            //Need to add possible colors at vertices when comp.size()-1 == 6
            for (int i = 1; i < 4; i++)
            {
                if(valid_numeric_entry(i, comp))
                    pos[i - 1] = convert_to<float>(comp[i]);
            }
            positions.push_back(pos);
        }
        
        else if(c[0] == 'f' && c[1] == ' ')
        {  
            auto comp = split(c, ' ');
            for (int i = 1; i < 4; i++)
            {
                auto index_fields = split(comp[i], '/');

                if(valid_numeric_entry(0, index_fields))
                    indices_pos.push_back(convert_to<unsigned>(index_fields[0]) - 1);
                
                if(valid_numeric_entry(1, index_fields))
                    indices_uv.push_back(convert_to<unsigned>(index_fields[1]) - 1);

                if(valid_numeric_entry(2, index_fields))
                    indices_n.push_back(convert_to<unsigned>(index_fields[2]) - 1);               
            }
        }

        // For debugging
        indices = indices_pos;
        
    }
}
