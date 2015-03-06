#include "utility.h"

#include <boost/algorithm/string.hpp>
using namespace boost::algorithm;

void split_lines(std::vector<std::string>& result, std::string& line, int cols)
{
    using namespace std;

    if((int)line.size() <= cols) {
        trim(line);
        if(line.size() > 0) {
            result.push_back(line);
        }
    } else {
        size_t pos = line.find_last_of(' ', cols);
        if(pos == string::npos) {
            pos = cols;
        }
        string sub = line.substr(0, pos);
        trim(sub);
        result.push_back(sub);

        string rest = line.substr(pos+1);
        split_lines(result, rest, cols);
    }
}
