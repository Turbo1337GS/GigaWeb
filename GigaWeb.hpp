#include <cstring>
#include <unordered_set>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <curl/curl.h>
#include <regex>
#include <algorithm>
#include <stack>
#include <unordered_map>
#include <map>
#include <string_view>
#include "knownEntities.hpp"
#include <utility>
#include <gumbo.h>
class GigaWeb
{
private:
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        ((std::string *)userp)->append((char *)contents, size * nmemb);
        return size * nmemb;
    }

    void extract_main_content(GumboNode *node, size_t &max_words, std::string &content)
    {
        if (node->type == GUMBO_NODE_TEXT)
        {
            std::string text(node->v.text.text);
            size_t word_count = std::count(text.begin(), text.end(), ' ') + 1;

            if (word_count > max_words)
            {
                max_words = word_count;
                content = text;
            }
        }
        else if (node->type == GUMBO_NODE_ELEMENT && node->v.element.tag != GUMBO_TAG_SCRIPT &&
                 node->v.element.tag != GUMBO_TAG_STYLE)
        {
            GumboVector *children = &node->v.element.children;

            for (unsigned int i = 0; i < children->length; ++i)
            {
                extract_main_content(static_cast<GumboNode *>(children->data[i]), max_words, content);
            }
        }
    }

    void extract_multiple_contents(GumboNode *node, std::vector<std::string> &contents)
    {
        if (node->type == GUMBO_NODE_TEXT)
        {
            std::string text(node->v.text.text);
            size_t word_count = std::count(text.begin(), text.end(), ' ') + 1;

            if (word_count > 20)
            {
                contents.push_back(text);
            }
        }
        else if (node->type == GUMBO_NODE_ELEMENT &&
                 node->v.element.tag != GUMBO_TAG_SCRIPT &&
                 node->v.element.tag != GUMBO_TAG_STYLE &&
                 node->v.element.tag != GUMBO_TAG_HEADER &&
                 node->v.element.tag != GUMBO_TAG_FOOTER &&
                 node->v.element.tag != GUMBO_TAG_NAV &&
                 node->v.element.tag != GUMBO_TAG_ASIDE)
        {
            GumboVector *children = &node->v.element.children;

            for (unsigned int i = 0; i < children->length; ++i)
            {
                extract_multiple_contents(static_cast<GumboNode *>(children->data[i]), contents);
            }
        }
    }

public:
    bool isValidURL(const std::string &url)
    {
        std::regex urlPattern(R"(^(https?|ftp)://[^\s/$.?#].[^\s]*$)");
        return std::regex_match(url, urlPattern);
    }

    bool fetchWebContent(const std::string &url, std::string &content)
    {
        if (!isValidURL(url))
        {
            printf("\nError Bad URL: %s\n", url.c_str());
            return false;
        }
        printf("\n%s\n",url.c_str());
        CURL *curl;
        CURLcode res;
        long httpCode = 0;
        char *contentType = nullptr;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (curl)
        {
            struct curl_slist *headers = nullptr;
            headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537");

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

            res = curl_easy_perform(curl);

            if (res != CURLE_OK)
            {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
                return false;
            }

            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
            curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType);

            curl_slist_free_all(headers);

            if (httpCode == 200 && contentType &&
                (std::string(contentType).find("text/html") != std::string::npos ||
                 std::string(contentType).find("application/json") != std::string::npos))
            {
                curl_easy_cleanup(curl);
                curl_global_cleanup();
                return true;
            }
            else
            {
                // std::cerr << "Invalid HTTP status code or content type.\n";
                // std::cerr << "HTTP Status Code: " << httpCode << "\n";
                // std::cerr << "Content Type: " << (contentType ? contentType : "Unknown") << "\n";
                return false;
            }
        }

        curl_global_cleanup();
        return false;
    }

    std::string cleanTXT(const std::string &input)
    {
        static const std::regex spacePattern("[ \t]+");
        static const std::regex newlinePattern("[ \t]*\n[ \t]*");
        static const std::regex trimPattern("^\\s+|\\s+$");
        static const std::regex multiNewlinePattern("\n+");

        std::string output = std::regex_replace(input, spacePattern, " ");
        output = std::regex_replace(output, newlinePattern, "\n");
        output = std::regex_replace(output, trimPattern, "");
        output = std::regex_replace(output, multiNewlinePattern, "\n");

        return output;
    }

    std::string cleanHTML(std::string &html)
    {
        static const std::regex tagRegex("<[^>]+>");
        return std::regex_replace(html, tagRegex, "");
    }

    std::string removeScriptAndStyleTags(const std::string &html)
    {
        if (html.empty())
            return "";

        static const std::string scriptStart = "<script";
        static const std::string scriptEnd = "</script>";
        static const std::string styleStart = "<style";
        static const std::string styleEnd = "</style>";

        std::string result;
        result.reserve(html.size());

        const char *p = html.data();
        const char *end = p + html.size();
        const char *lastValid = p;

        while (p < end)
        {
            const char *nextScript = std::strstr(p, scriptStart.c_str());
            const char *nextStyle = std::strstr(p, styleStart.c_str());

            if (nextScript && (!nextStyle || nextScript < nextStyle))
            {
                result.append(lastValid, nextScript);
                p = std::strstr(nextScript, scriptEnd.c_str());
                if (p)
                {
                    p += scriptEnd.size();
                }
                else
                {
                    break;
                }
            }
            else if (nextStyle)
            {
                result.append(lastValid, nextStyle);
                p = std::strstr(nextStyle, styleEnd.c_str());
                if (p)
                {
                    p += styleEnd.size();
                }
                else
                {
                    break;
                }
            }
            else
            {
                result.append(lastValid, end);
                break;
            }

            lastValid = p;
        }

        return result;
    }

    std::string removePathAndSvgTags(const std::string &html)
    {
        if (html.empty())
        {
            return "";
        }

        static const std::string pathStart = "<path";
        static const std::string pathEnd = "</path>";
        static const std::string svgStart = "<svg";
        static const std::string svgEnd = "</svg>";

        std::string result;
        std::size_t pos = 0;
        std::size_t lastPos = 0;

        while (pos < html.size())
        {
            std::size_t pathPos = html.find(pathStart, pos);
            std::size_t svgPos = html.find(svgStart, pos);

            if (pathPos != std::string::npos && (svgPos == std::string::npos || pathPos < svgPos))
            {
                result += html.substr(lastPos, pathPos - lastPos);
                pos = html.find(pathEnd, pathPos);
                if (pos != std::string::npos)
                {
                    pos += pathEnd.length();
                }
                else
                {
                    result += html.substr(pathPos);
                    break;
                }
            }
            else if (svgPos != std::string::npos)
            {
                result += html.substr(lastPos, svgPos - lastPos);
                pos = html.find(svgEnd, svgPos);
                if (pos != std::string::npos)
                {
                    pos += svgEnd.length();
                }
                else
                {
                    result += html.substr(svgPos);
                    break;
                }
            }
            else
            {
                result += html.substr(lastPos);
                break;
            }

            lastPos = pos;
        }

        return result;
    }

    std::string encodeHtmlEntities(const std::string &html)
    {
        std::string cleanedHtml;
        cleanedHtml.reserve(html.size());

        std::regex htmlEntitiesPattern(R"(&\w+;)");
        std::sregex_iterator htmlEntitiesIterator(html.begin(), html.end(), htmlEntitiesPattern);
        std::sregex_iterator htmlEntitiesEnd;

        std::string::const_iterator searchStart = html.begin();

        for (std::sregex_iterator i = htmlEntitiesIterator; i != htmlEntitiesEnd; ++i)
        {
            std::smatch match = *i;
            auto it = knownEntities.find(match.str());
            if (it != knownEntities.end())
            {
                cleanedHtml.append(searchStart, html.begin() + match.position());
                cleanedHtml.append(it->second);
                searchStart = html.begin() + match.position() + match.length();
            }
        }

        cleanedHtml.append(searchStart, html.end());

        return cleanedHtml;
    }
    std::string removeHtmlEntities(const std::string &html)
    {
        std::string cleanedHtml;
        cleanedHtml.reserve(html.size());

        std::regex htmlEntitiesPattern(R"(&\w+;)");
        std::sregex_iterator htmlEntitiesIterator(html.begin(), html.end(), htmlEntitiesPattern);
        std::sregex_iterator htmlEntitiesEnd;

        std::string::const_iterator searchStart = html.begin();

        for (std::sregex_iterator i = htmlEntitiesIterator; i != htmlEntitiesEnd; ++i)
        {
            std::smatch match = *i;
            auto it = knownEntities.find(match.str());
            if (it != knownEntities.end())
            {
                cleanedHtml.append(searchStart, html.begin() + match.position());
                searchStart = html.begin() + match.position() + match.length();
            }
        }

        cleanedHtml.append(searchStart, html.end());

        return cleanedHtml;
    }

    bool hasTag(const std::string &html, const std::string &tag)
    {
        std::string openingTag = "<" + tag;
        std::string closingTag = "</" + tag + ">";

        return (html.find(openingTag) != std::string::npos) || (html.find(closingTag) != std::string::npos);
    }

    std::string getAttributeValue(const std::string &html, const std::string &tag, const std::string &attribute)
    {
        std::string openingTag = "<" + tag;
        std::string closingTag = "</" + tag + ">";

        size_t openingTagPos = html.find(openingTag);
        if (openingTagPos == std::string::npos)
            return "";

        size_t attributePos = html.find(attribute, openingTagPos);
        if (attributePos == std::string::npos)
            return "";

        size_t valueStartPos = html.find_first_of("\"'", attributePos);
        if (valueStartPos == std::string::npos)
            return "";

        size_t valueEndPos = html.find_first_of("\"'", valueStartPos + 1);
        if (valueEndPos == std::string::npos)
            return "";

        return html.substr(valueStartPos + 1, valueEndPos - valueStartPos - 1);
    }

    std::string replaceTag(const std::string &html, const std::string &oldTag, const std::string &newTag)
    {
        std::string replacedHtml = html;
        std::string openingOldTag = "<" + oldTag;
        std::string closingOldTag = "</" + oldTag + ">";
        std::string openingNewTag = "<" + newTag;
        std::string closingNewTag = "</" + newTag + ">";

        size_t pos = replacedHtml.find(openingOldTag);
        while (pos != std::string::npos)
        {
            replacedHtml.replace(pos, openingOldTag.length(), openingNewTag);
            pos = replacedHtml.find(openingOldTag, pos + openingNewTag.length());
        }

        pos = replacedHtml.find(closingOldTag);
        while (pos != std::string::npos)
        {
            replacedHtml.replace(pos, closingOldTag.length(), closingNewTag);
            pos = replacedHtml.find(closingOldTag, pos + closingNewTag.length());
        }

        return replacedHtml;
    }

    std::string extractContentBetweenTags(const std::string &html, const std::string &startTag, const std::string &endTag)
    {
        size_t startPos = html.find(startTag);
        if (startPos == std::string::npos)
            return "";

        startPos += startTag.length();

        size_t endPos = html.find(endTag, startPos);
        if (endPos == std::string::npos)
            return "";

        return html.substr(startPos, endPos - startPos);
    }

    int countTagOccurrences(const std::string &html, const std::string &tag)
    {
        int count = 0;
        std::string openingTag = "<" + tag;
        std::string closingTag = "</" + tag + ">";

        size_t pos = html.find(openingTag);
        while (pos != std::string::npos)
        {
            count++;
            pos = html.find(openingTag, pos + openingTag.length());
        }

        pos = html.find(closingTag);
        while (pos != std::string::npos)
        {
            count++;
            pos = html.find(closingTag, pos + closingTag.length());
        }

        return count;
    }

    bool hasAttribute(const std::string &html, const std::string &tag, const std::string &attribute)
    {
        std::string openingTag = "<" + tag;
        std::string closingTag = "</" + tag + ">";

        size_t openingTagPos = html.find(openingTag);
        if (openingTagPos == std::string::npos)
            return false;

        size_t attributePos = html.find(attribute, openingTagPos);
        if (attributePos == std::string::npos)
            return false;

        return true;
    }

    std::string replaceAllTags(const std::string &html, const std::string &oldTag, const std::string &newTag)
    {
        std::string replacedHtml = html;
        std::string openingOldTag = "<" + oldTag;
        std::string openingNewTag = "<" + newTag;

        size_t pos = replacedHtml.find(openingOldTag);
        while (pos != std::string::npos)
        {
            replacedHtml.replace(pos, openingOldTag.length(), openingNewTag);
            pos = replacedHtml.find(openingOldTag, pos + openingNewTag.length());
        }

        return replacedHtml;
    }

    bool hasAttributeValue(const std::string &html, const std::string &tag, const std::string &attribute, const std::string &value)
    {
        std::string openingTag = "<" + tag;
        std::string closingTag = "</" + tag + ">";

        size_t openingTagPos = html.find(openingTag);
        if (openingTagPos == std::string::npos)
            return false;
        size_t attributePos = html.find(attribute, openingTagPos);
        if (attributePos == std::string::npos)
            return false;

        size_t valueStartPos = html.find(value, attributePos);
        if (valueStartPos == std::string::npos)
            return false;

        return true;
    }

    std::vector<std::string> getAttributeValues(const std::string &html, const std::string &tag, const std::string &attribute)
    {
        std::vector<std::string> attributeValues;
        std::string openingTag = "<" + tag;
        std::string closingTag = "</" + tag + ">";

        size_t pos = html.find(openingTag);
        while (pos != std::string::npos)
        {
            size_t attributePos = html.find(attribute, pos);
            if (attributePos != std::string::npos)
            {
                size_t valueStartPos = html.find_first_of("\"'", attributePos);
                if (valueStartPos != std::string::npos)
                {
                    size_t valueEndPos = html.find_first_of("\"'", valueStartPos + 1);
                    if (valueEndPos != std::string::npos)
                    {
                        std::string attributeValue = html.substr(valueStartPos + 1, valueEndPos - valueStartPos - 1);
                        attributeValues.push_back(attributeValue);
                    }
                }
            }

            pos = html.find(openingTag, pos + openingTag.length());
        }

        return attributeValues;
    }

    void addTag(std::string &html, const std::string &tag, const std::string &content)
    {
        std::string newTag = "<" + tag + ">" + content + "</" + tag + ">";
        html += newTag;
    }
    std::string removeAllTags(const std::string &html, const std::string &tag)
    {
        std::string removedHtml = html;
        std::string openingTag = "<" + tag;
        std::string closingTag = "</" + tag + ">";

        size_t pos = 0;
        while ((pos = removedHtml.find(openingTag, pos)) != std::string::npos)
        {
            size_t endPos = removedHtml.find(">", pos + openingTag.length());
            if (endPos == std::string::npos)
            {
                break;
            }

            size_t closingTagPos = removedHtml.find(closingTag, endPos);
            if (closingTagPos == std::string::npos)
            {
                break;
            }

            removedHtml.erase(pos, closingTagPos - pos + closingTag.length());
        }

        return removedHtml;
    }

    std::string removeAllAttributes(const std::string &html, const std::string &tag)
    {
        std::string result;
        result.reserve(html.length());

        std::string openingTag = "<" + tag;
        size_t pos = 0;
        size_t lastPos = 0;

        while ((pos = html.find(openingTag, pos)) != std::string::npos)
        {
            result.append(html, lastPos, pos - lastPos);
            size_t endPos = html.find(">", pos + openingTag.length());
            if (endPos == std::string::npos)
            {
                break;
            }

            result.append(openingTag + ">");
            pos = endPos + 1;
            lastPos = pos;
        }

        result.append(html, lastPos, html.length() - lastPos);
        return result;
    }

    std::string normalizeHTML(const std::string &html)
    {
        std::regex spacePattern("[ \t]+");
        std::regex newlinePattern("[ \t]*\n[ \t]*");
        std::regex trimPattern("^\\s+|\\s+$");
        std::regex multiSpacePattern(" {2,}");

        std::string normalizedHTML = std::regex_replace(html, spacePattern, " ");
        normalizedHTML = std::regex_replace(normalizedHTML, newlinePattern, "");
        normalizedHTML = std::regex_replace(normalizedHTML, trimPattern, "");
        normalizedHTML = std::regex_replace(normalizedHTML, multiSpacePattern, " ");

        return normalizedHTML;
    }


std::vector<std::string> extractURLs(const std::string &html, const std::string &baseURL = "") {
    std::vector<std::string> urls;
    std::unordered_set<std::string> uniqueUrls;

    std::string::size_type pos = 0;
    std::string::size_type end = 0;

    while ((pos = html.find("<a ", pos)) != std::string::npos) {
        end = html.find('>', pos);
        if (end == std::string::npos) break;

        std::string::size_type startQuote = html.find("href=\"", pos);
        if (startQuote == std::string::npos || startQuote > end) {
            pos = end + 1;
            continue;
        }
        startQuote += 6;  // Length of "href=\""

        std::string::size_type endQuote = html.find('\"', startQuote);
        if (endQuote == std::string::npos || endQuote > end) {
            pos = end + 1;
            continue;
        }

        std::string url = html.substr(startQuote, endQuote - startQuote);

        if (!url.empty() && isalpha(url[0])) {
            if (url[0] == '/')
                url = baseURL + url;
            else if (url.find("://") == std::string::npos)
                url = "http://" + url;

            if (uniqueUrls.find(url) == uniqueUrls.end()) {
                uniqueUrls.insert(url);
                urls.push_back(url);
            }
        }
        pos = end + 1;
    }

    return urls;
}


    std::string extractScriptSection(const std::string &html)
    {
        std::regex script_regex("<script[^>]*>([\\s\\S]*?)</script>", std::regex_constants::icase);
        std::smatch match;
        std::string result;

        for (std::sregex_iterator it = std::sregex_iterator(html.begin(), html.end(), script_regex);
             it != std::sregex_iterator(); ++it)
        {
            std::smatch match = *it;
            result += match[1].str() + "\n";
        }

        return result;
    }

    std::string extractStyleSection(const std::string &html)
    {
        std::regex style_regex("<style[^>]*>([\\s\\S]*?)</style>", std::regex_constants::icase);
        std::smatch match;
        std::string result;

        for (std::sregex_iterator it = std::sregex_iterator(html.begin(), html.end(), style_regex);
             it != std::sregex_iterator(); ++it)
        {
            std::smatch match = *it;
            result += match[1].str() + "\n";
        }

        return result;
    }

    std::vector<std::string> extractFunctionNames(const std::string &scriptSection)
    {
        std::regex func_regex(R"(\bfunction\s+(\w+)\s*\()", std::regex_constants::icase);
        std::smatch match;
        std::vector<std::string> functionNames;

        for (std::sregex_iterator it = std::sregex_iterator(scriptSection.begin(), scriptSection.end(), func_regex);
             it != std::sregex_iterator(); ++it)
        {
            std::smatch match = *it;
            functionNames.push_back(match[1].str());
        }

        return functionNames;
    }

    std::string removeHtmlComments(const std::string &input)
    {
        static const std::regex commentPattern("<!--(.*?)-->");
        return std::regex_replace(input, commentPattern, "");
    }
    std::string indentHtmlCode(const std::string &input, int spacesPerLevel = 4)
    {
        std::string indentedCode;
        std::istringstream iss(input);
        std::string line;
        int indentLevel = 0;

        while (std::getline(iss, line))
        {
            std::string trimmedLine = std::regex_replace(line, std::regex("^\\s+"), "");

            if (trimmedLine.empty())
                continue;

            if (trimmedLine.find("</") != std::string::npos)
                indentLevel--;

            if (indentLevel < 0)
                indentLevel = 0;

            indentedCode += std::string(indentLevel * spacesPerLevel, ' ') + trimmedLine + "\n";

            if (trimmedLine.find("<") != std::string::npos &&
                trimmedLine.find("</") == std::string::npos &&
                trimmedLine.find("/>") == std::string::npos)
                indentLevel++;
        }

        return indentedCode;
    }

    std::vector<std::string> extractInlineCss(const std::string &input)
    {
        std::vector<std::string> inlineCss;
        std::regex cssPattern(R"(style=\"(.*?)\")");
        std::sregex_iterator cssIterator(input.begin(), input.end(), cssPattern);
        std::sregex_iterator cssEnd;

        for (; cssIterator != cssEnd; ++cssIterator)
        {
            std::smatch match = *cssIterator;
            inlineCss.push_back(match[1].str());
        }

        return inlineCss;
    }

    std::string removeInlineStyles(const std::string &html)
    {
        std::regex stylePattern("style=\"(.*?)\"");
        return std::regex_replace(html, stylePattern, "");
    }
    std::vector<std::string> getTagContents(const std::string &html, const std::string &tag)
    {
        std::vector<std::string> tagContents;
        std::regex tagPattern("<" + tag + "[^>]*>(.*?)</" + tag + ">", std::regex_constants::ECMAScript);
        std::sregex_iterator tagIterator(html.begin(), html.end(), tagPattern);
        std::sregex_iterator tagEnd;

        for (std::sregex_iterator i = tagIterator; i != tagEnd; ++i)
        {
            std::smatch match = *i;
            tagContents.push_back(match[1]);
        }

        return tagContents;
    }
    bool isNormalizedHTML(const std::string &html)
    {
        std::regex spacePattern("[ \t]+");
        std::regex newlinePattern("[ \t]*\n[ \t]*");
        std::regex trimPattern("^\\s+|\\s+$");
        std::regex multiSpacePattern(" {2,}");

        std::string normalizedHTML = std::regex_replace(html, spacePattern, " ");
        normalizedHTML = std::regex_replace(normalizedHTML, newlinePattern, "");
        normalizedHTML = std::regex_replace(normalizedHTML, trimPattern, "");
        normalizedHTML = std::regex_replace(normalizedHTML, multiSpacePattern, " ");

        return normalizedHTML == html;
    }

    bool hasNestedTags(const std::string &html, const std::string &tag)
    {
        std::regex tagPattern("<" + tag + "[^>]*>.*</" + tag + ">");
        return std::regex_search(html, tagPattern);
    }
    std::string addAttributeToTag(const std::string &html, const std::string &tag, const std::string &attribute, const std::string &value)
    {
        std::regex tagPattern("<" + tag + "[^>]*>");
        std::regex attributePattern(attribute + "\\s*=\\s*\"([^\"]*)\"");
        std::smatch match;

        std::string modifiedHTML = html;
        if (std::regex_search(html, match, tagPattern))
        {
            std::string openingTag = match.str();
            if (std::regex_search(openingTag, match, attributePattern))
            {

                std::string attributeValue = match.str(1);
                std::string newAttributeValue = attribute + "=\"" + value + "\"";
                modifiedHTML = std::regex_replace(modifiedHTML, std::regex(attributeValue), newAttributeValue);
            }
            else
            {

                std::string newAttribute = " " + attribute + "=\"" + value + "\"";
                modifiedHTML = std::regex_replace(modifiedHTML, tagPattern, openingTag + newAttribute);
            }
        }

        return modifiedHTML;
    }

    std::string removeAttributeFromTag(const std::string &html, const std::string &tag, const std::string &attribute)
    {
        std::string modifiedHTML = html;
        std::regex tagPattern("<" + tag + "\\b[^>]*>", std::regex_constants::ECMAScript);
        std::regex attributePattern(attribute + "\\s*=\\s*\"([^\"]*)\"", std::regex_constants::ECMAScript);
        std::sregex_iterator tagIterator(modifiedHTML.begin(), modifiedHTML.end(), tagPattern);
        std::sregex_iterator tagEnd;
        std::smatch match;

        std::string result;

        size_t lastPos = 0;
        for (std::sregex_iterator i = tagIterator; i != tagEnd; ++i)
        {
            match = *i;
            std::string openingTag = match.str();
            result.append(modifiedHTML, lastPos, match.position() - lastPos);
            std::smatch attrMatch;
            if (std::regex_search(openingTag, attrMatch, attributePattern))
            {
                std::string attributeValue = attrMatch[1].str();
                std::string attributeToRemove = attribute + "=\"" + attributeValue + "\"";
                openingTag = std::regex_replace(openingTag, std::regex(attributeToRemove), "");
            }
            result += openingTag;
            lastPos = match.position() + match.length();
        }

        result += modifiedHTML.substr(lastPos);

        return result;
    }

    std::string replaceTagContent(const std::string &html, const std::string &tag, const std::string &newContent)
    {
        std::regex tagPattern("<" + tag + "[^>]*>(.*?)</" + tag + ">");
        std::smatch match;

        std::string modifiedHTML = html;
        while (std::regex_search(modifiedHTML, match, tagPattern))
        {
            std::string fullTag = match.str();
            std::string openingTag = fullTag.substr(0, fullTag.find(">") + 1);
            std::string closingTag = "</" + tag + ">";
            modifiedHTML = std::regex_replace(modifiedHTML, std::regex(fullTag), openingTag + newContent + closingTag);

            modifiedHTML = match.suffix().str();
        }

        return modifiedHTML;
    }
    bool doesTagHaveAttribute(const std::string &html, const std::string &tag, const std::string &attribute)
    {
        std::regex tagPattern("<" + tag + "[^>]*>");

        std::string modifiedHTML = html;
        std::smatch match;

        if (std::regex_search(modifiedHTML, match, tagPattern))
        {
            std::string openingTag = match.str();
            if (openingTag.find(attribute) != std::string::npos)
            {
                return true;
            }
        }

        return false;
    }
    std::string removeAllOccurrencesOfTag(const std::string &html, const std::string &tag)
    {
        std::string openingTag = "<" + tag;
        std::string closingTag = "</" + tag + ">";

        std::string modifiedHTML = html;
        size_t start = modifiedHTML.find(openingTag);
        while (start != std::string::npos)
        {
            size_t end = modifiedHTML.find(closingTag, start);
            if (end != std::string::npos)
            {

                modifiedHTML.erase(start, end - start + closingTag.length());
            }

            start = modifiedHTML.find(openingTag, start);
        }

        return modifiedHTML;
    }
    std::string addCssClassToTag(const std::string &html, const std::string &tag, const std::string &cssClass)
    {
        std::regex tagPattern("<" + tag + "[^>]*>");

        std::string modifiedHTML = html;
        std::smatch match;

        while (std::regex_search(modifiedHTML, match, tagPattern))
        {
            std::string openingTag = match.str();

            if (openingTag.find("class=\"") != std::string::npos)
            {

                modifiedHTML = std::regex_replace(modifiedHTML, std::regex("class=\"(.*?)\""), "class=\"$1 " + cssClass + "\"");
            }
            else
            {

                modifiedHTML = std::regex_replace(modifiedHTML, std::regex(">$"), " class=\"" + cssClass + "\">");
            }

            modifiedHTML = match.suffix().str();
        }

        return modifiedHTML;
    }
    std::string removeCssClassFromTag(const std::string &html, const std::string &tag, const std::string &cssClass)
    {
        std::regex tagPattern("<" + tag + "[^>]*>");

        std::string modifiedHTML = html;
        std::smatch match;

        while (std::regex_search(modifiedHTML, match, tagPattern))
        {
            std::string openingTag = match.str();

            if (openingTag.find("class=\"") != std::string::npos)
            {

                modifiedHTML = std::regex_replace(modifiedHTML, std::regex("class=\"(.*?)\\b" + cssClass + "\\b(.*?)\""), "class=\"$1$2\"");
            }

            modifiedHTML = match.suffix().str();
        }

        return modifiedHTML;
    }
    std::string replaceTagAttribute(const std::string &html, const std::string &tag, const std::string &attribute, const std::string &replacement)
    {
        std::regex tagPattern("<" + tag + "[^>]*>");

        std::string modifiedHTML = html;
        std::smatch match;

        while (std::regex_search(modifiedHTML, match, tagPattern))
        {
            std::string openingTag = match.str();

            if (openingTag.find(attribute + "=\"") != std::string::npos)
            {

                modifiedHTML = std::regex_replace(modifiedHTML, std::regex(attribute + "\\s*=\\s*\"(.*?)\""), attribute + "=\"" + replacement + "\"");
            }

            modifiedHTML = match.suffix().str();
        }

        return modifiedHTML;
    }
    std::string wrapContentWithTag(const std::string &html, const std::string &startTag, const std::string &endTag)
    {
        std::string modifiedHTML = html;
        std::string openingTag = "<" + startTag + ">";
        std::string closingTag = "</" + endTag + ">";

        size_t pos = modifiedHTML.find(openingTag);
        while (pos != std::string::npos)
        {
            size_t endPos = modifiedHTML.find(">", pos + openingTag.length());
            if (endPos == std::string::npos)
                break;

            modifiedHTML.insert(endPos + 1, openingTag);

            size_t closingTagPos = modifiedHTML.find(closingTag, endPos);
            if (closingTagPos != std::string::npos)
            {
                modifiedHTML.insert(closingTagPos, closingTag);
            }

            pos = modifiedHTML.find(openingTag, pos + openingTag.length() + startTag.length() + endTag.length());
        }

        return modifiedHTML;
    }
    bool hasCssClass(const std::string &html, const std::string &tag, const std::string &cssClass)
    {
        std::string openingTag = "<" + tag;
        std::string closingTag = "</" + tag + ">";
        std::string classAttribute = "class=\"" + cssClass + "\"";

        size_t openingTagPos = html.find(openingTag);
        if (openingTagPos == std::string::npos)
            return false;

        size_t classAttrPos = html.find(classAttribute, openingTagPos);
        if (classAttrPos == std::string::npos)
            return false;

        size_t closingTagPos = html.find(closingTag, classAttrPos);
        if (closingTagPos == std::string::npos)
            return false;

        return true;
    }
    bool hasAttributeWithValue(const std::string &html, const std::string &tag, const std::string &attribute, const std::string &value)
    {
        std::string openingTag = "<" + tag;
        std::string closingTag = "</" + tag + ">";

        size_t openingTagPos = html.find(openingTag);
        if (openingTagPos == std::string::npos)
            return false;

        size_t attributePos = html.find(attribute, openingTagPos);
        if (attributePos == std::string::npos)
            return false;

        size_t valueStartPos = html.find(value, attributePos);
        if (valueStartPos == std::string::npos)
            return false;

        return true;
    }

    std::vector<std::string> extractImgSrc(const std::string &html)
    {
        std::vector<std::string> imgSrcs;
        std::regex imgPattern("<img[^>]*src=\"([^\"]*)\"[^>]*>");
        std::sregex_iterator imgIterator(html.begin(), html.end(), imgPattern);
        std::sregex_iterator imgEnd;

        for (std::sregex_iterator i = imgIterator; i != imgEnd; ++i)
        {
            std::smatch match = *i;
            imgSrcs.push_back(match.str(1));
        }

        return imgSrcs;
    }

    std::string removeAllCommentsFromHTML(const std::string &html)
    {
        std::regex multiLineCommentPattern("<!--([\\s\\S]*?)-->");
        return std::regex_replace(html, multiLineCommentPattern, "");
    }

    std::string removeAllCommentsFromJS(const std::string &js)
    {
        std::regex multiAndSingleLineCommentPattern("/\\*([\\s\\S]*?)\\*/|//.*");
        return std::regex_replace(js, multiAndSingleLineCommentPattern, "");
    }

    std::string removeAllCommentsFromCSS(const std::string &css)
    {
        std::regex multiAndSingleLineCommentPattern("/\\*([\\s\\S]*?)\\*/");
        return std::regex_replace(css, multiAndSingleLineCommentPattern, "");
    }

    std::string removeAllMetaTags(const std::string &html)
    {
        std::regex metaTagPattern("<meta[^>]*>");
        return std::regex_replace(html, metaTagPattern, "");
    }
    std::string changeDoctype(const std::string &html, const std::string &doctype)
    {
        std::regex doctypePattern("<!DOCTYPE[^>]*>");
        return std::regex_replace(html, doctypePattern, "<!DOCTYPE " + doctype + ">");
    }
    std::vector<std::string> extractAllTags(const std::string &html)
    {
        std::vector<std::string> tags;
        std::regex tagPattern("<(\\w+)[^>]*>");
        std::sregex_iterator tagIterator(html.begin(), html.end(), tagPattern);
        std::sregex_iterator tagEnd;

        for (std::sregex_iterator i = tagIterator; i != tagEnd; ++i)
        {
            std::smatch match = *i;
            tags.push_back(match.str(1));
        }

        return tags;
    }

    std::string removeAllIds(const std::string &html)
    {
        std::regex idPattern(" id=\"[^\"]*\"");
        return std::regex_replace(html, idPattern, "");
    }
    std::string changeTagNames(const std::string &html, const std::string &newTagName)
    {
        std::regex tagPattern("<(/?)(\\w+)([^>]*)>");
        return std::regex_replace(html, tagPattern, "<$1" + newTagName + "$3>");
    }

    std::string removeEmptyTags(const std::string &html)
    {
        std::regex emptyTagPattern("<(\\w+)[^>]*/>");
        return std::regex_replace(html, emptyTagPattern, "");
    }
    std::string addAltToImg(const std::string &html, const std::string &altText)
    {
        std::regex imgPattern("<img((?!alt=)[^>])*>");
        return std::regex_replace(html, imgPattern, "<img alt=\"" + altText + "\"$1>");
    }
    std::string removeAllClasses(const std::string &html)
    {
        std::regex classPattern(" class=\"[^\"]*\"");
        return std::regex_replace(html, classPattern, "");
    }

    std::string getMainContent(const std::string &html)
    {
        GumboOutput *output = gumbo_parse(html.c_str());

        size_t max_words = 0;
        std::string content;
        extract_main_content(output->root, max_words, content);

        gumbo_destroy_output(&kGumboDefaultOptions, output);

        return content;
    }
    std::vector<std::string> getMultipleContents(const std::string &html)
    {
        GumboOutput *output = gumbo_parse(html.c_str());

        std::vector<std::string> contents;
        std::unordered_set<std::string> unique_contents;

        extract_multiple_contents(output->root, contents);

        gumbo_destroy_output(&kGumboDefaultOptions, output);

        std::vector<std::string> unique_vector;
        for (const auto &str : contents)
        {
            if (unique_contents.find(str) == unique_contents.end())
            {
                unique_vector.push_back(str);
                unique_contents.insert(str);
            }
        }

        return unique_vector;
    }
    std::string extractDomainFromURL(const std::string &url)
    {
        std::string domain;

        auto start = url.find("://");
        if (start == std::string::npos)
        {
            return domain;
        }
        start += 3;

        auto end = url.find('/', start);
        if (end == std::string::npos)
        {
            end = url.length();
        }

        domain = url.substr(start, end - start);
        return domain;
    }
};
extern GigaWeb *giga;
