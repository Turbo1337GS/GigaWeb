#include <fstream>
#include <filesystem>
#include <set>
#include "WebAnalize.hpp"
#include <boost/crc.hpp>
#include <queue>
GigaWeb *giga = new GigaWeb();
#define gvn(s) (#s)
/*
U can use this command for compile code with g++
// g++ -std=c++20 ./main.cpp ./WebAnalize.hpp -lgumbo -lcurl -lboost_system -o GigaSoft && ./GigaSoft
*/

// An example of how to use GigaWeb as a recursive scraper
std::set<std::string> queueUrls;
std::set<std::string> visitedUrls;

std::string FileName;
std::set<std::string> savedChecksums;
long CountOfUrl;
int indexFile = 0;
int SavedText = 0;
void clearScreen()
{
#ifdef _WIN32
    std::system("CLS");
#elif defined(__linux__)
    std::system("reset");
#elif defined(__APPLE__)
    std::system("clear");
#else
    std::cout << "Not know system" << std::endl;
#endif
}


std::string generateChecksum(const std::string &text)
{
    boost::crc_32_type result;
    result.process_bytes(text.data(), text.length());
    return std::to_string(result.checksum());
}

void appendFile(const std::string &FileName, const std::string &content)
{
    std::ofstream outFile;
    outFile.open(FileName, std::ios::app);

    if (outFile.is_open())
    {
        outFile << content << "\n";
        outFile.close();
    }
    else
    {
        std::cerr << "Error when open file" << FileName << std::endl;
    }
}

void createDir(const std::filesystem::path &path)
{
    if (std::filesystem::exists(path))
    {
        std::cout << "Directory " << path << " already exists.\n";
    }
    else
    {
        if (std::filesystem::create_directories(path))
        {
            std::cout << "Created directory: " << path << "\n";
        }
        else
        {
            std::cout << "Failed to create directory: " << path << "\n";
        }
    }
}
void createFile(const std::string &fileName)
{
    std::ifstream file(fileName);
    if (!file)
    {
        std::ofstream newFile(fileName.c_str());
    }
}

float getFileSizeInMB(const std::string &filePath)
{
    std::filesystem::path path(filePath);
    try
    {
        auto fileSize = std::filesystem::file_size(path);
        return static_cast<float>(fileSize) / (1024.0f * 1024.0f);
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << e.what() << '\n';
        return -1.0f;
    }
}

float getFolderSizeInMB(const std::string &folderPath)
{
    std::filesystem::path path(folderPath);
    long int folderSize = 0;
    try
    {
        for (const auto &entry : std::filesystem::recursive_directory_iterator(path))
        {
            if (!std::filesystem::is_directory(entry))
            {
                folderSize += std::filesystem::file_size(entry);
            }
        }
        return static_cast<float>(folderSize) / (1024.0f * 1024.0f);
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << e.what() << '\n';
        return -1.0f;
    }
}

void CollectUrl(std::string &html)
{
    std::vector<std::string> newUrls = giga->extractURLs(html);
    CountOfUrl = 0;
    if (queueUrls.size() > 100)
        return;

    for (const auto &url : newUrls)
    {
        if (visitedUrls.find(url) == visitedUrls.end())
        {
            auto result = queueUrls.insert(url);
            if (result.second)
            {
                ++CountOfUrl;
            }
        }
    }
}

int countWords(const std::string &text)
{
    std::istringstream iss(text);
    std::string word;
    int count = 0;

    while (iss >> word)
        ++count;

    return count;
}

std::string getLastElement()
{
    if (queueUrls.empty())
    {
        return "queueUrls.empty";
    }
    return *(--queueUrls.end());
}

void removeLastElement()
{
    if (queueUrls.empty())
    {
        std::cout << "queueUrls.empty" << std::endl;
        return;
    }
    queueUrls.erase(--queueUrls.end());
}

void Stats()
{
    printf("queue %ld \t"
           "Folder %.4f \t"
           "Saved %d \t"
           "File %s | Size %.4f \t"
           "File name %s\n",
           queueUrls.size(),
           getFolderSizeInMB("TXT"),
           SavedText,
           FileName.c_str(),
           getFileSizeInMB(FileName),
           FileName.c_str());
}
std::string saveToFile(std::string text)
{
    text = giga->cleanHTML(text);
    text = giga->cleanTXT(text);

    std::string checksum = generateChecksum(text);

    if (text.empty() || savedChecksums.find(checksum) != savedChecksums.end())
        return "";

    FileName = "TXT/" + std::to_string(indexFile) + ".txt";
    appendFile(FileName, text);

    int MB_FILE = getFileSizeInMB(FileName);
    int MAX_MB_FILE = 1;

    if (MB_FILE >= MAX_MB_FILE)
        indexFile++;

    savedChecksums.insert(checksum);
    SavedText++;
    Stats();

    return "";
}

void Recursive()
{
    while (!queueUrls.empty())
    {
        auto it_last = --queueUrls.end();
        std::string Last = *it_last;
        queueUrls.erase(it_last);

        if (visitedUrls.find(Last) != visitedUrls.end())
        {
            printf("\nSkipping because URL is in visited_list\n");
            continue;
        }

        std::string content = "";
        if (giga && giga->fetchWebContent(Last, content))
        {
            CollectUrl(content);

            std::vector<std::string> contents = giga->getMultipleContents(content);
            std::string TextToSave;

            for (const auto &ContentToSave : contents)
            {
                if (!ContentToSave.empty() && countWords(ContentToSave) > 10)
                {
                    TextToSave.append(ContentToSave);
                }
            }

            if (contents.empty())
            {
                TextToSave.append(giga->getMainContent(content));
            }

            visitedUrls.insert(Last);

            saveToFile(TextToSave.c_str());
        }
        else
        {
            visitedUrls.insert(Last);
        }
    }
}

int main()
{
    createDir("TXT");
    clearScreen();
    std::string initHTML;
    if (giga->fetchWebContent("https://docs.python.org/pl/3/whatsnew/3.11.html", initHTML))
    {
        CollectUrl(initHTML);
        Recursive();
    }

    return 0;
}
