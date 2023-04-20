#include <iostream>
#include <vector>
#include <algorithm>

struct Count
{
    char c;
    int count;
};

std::vector<Count> count_chars(const std::string &str)
{
    std::vector<Count> counts;
    for (char c : str)
    {
        auto it = std::find_if(counts.begin(), counts.end(), [=](const Count &count)
                               { return count.c == c; });
        if (it != counts.end())
        {
            it->count++;
        }
        else
        {
            counts.push_back({c, 1});
        }
    }
    return counts;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input string>\n";
        return 1;
    }
    std::string input = argv[1];
    std::vector<Count> counts = count_chars(input);
    std::cout << "[";
    for (size_t i = 0; i < counts.size(); i++)
    {
        std::cout << "{\"c\": \"" << counts[i].c << "\", \"count\": " << counts[i].count << "}";
        if (i != counts.size() - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";
    return 0;
}
