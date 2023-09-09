

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
#include <unordered_set>

class GigaWeb
{
private:
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        ((std::string *)userp)->append((char *)contents, size * nmemb);
        return size * nmemb;
    }

public:
    bool fetchWebContent(const std::string &url, std::string &content)
    {
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

            res = curl_easy_perform(curl);

            if (res != CURLE_OK)
            {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
                return false;
            }

            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
            curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType);

            curl_slist_free_all(headers);

            if (httpCode == 200 && contentType && std::string(contentType).find("text/html") != std::string::npos)
            {
                curl_easy_cleanup(curl);
                curl_global_cleanup();
                return true;
            }
            else
            {
                std::cerr << "Invalid HTTP status code or content type.\n";
                std::cerr << "HTTP Status Code: " << httpCode << "\n";
                std::cerr << "Content Type: " << (contentType ? contentType : "Unknown") << "\n";
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

    bool containsOtherTags(const std::string &text, const std::string &currentTag)
    {
        static std::regex re;
        re.assign(R"(<(?!/)" + currentTag + R"(\s|>)[^>]*>)");
        return std::regex_search(text, re);
    }

    std::string cleanHTML(std::string &html)
    {
        static const std::regex tagRegex("<[^>]+>");
        return std::regex_replace(html, tagRegex, "");
    }

    std::string removeScriptAndStyleTags(const std::string &html)
    {
        if (html.empty())
        {
            return "";
        }

        static const std::string scriptStart = "<script";
        static const std::string scriptEnd = "</script>";
        static const std::string styleStart = "<style";
        static const std::string styleEnd = "</style>";

        std::string result;
        std::size_t pos = 0;
        std::size_t lastPos = 0;

        while (pos < html.size())
        {
            std::size_t scriptPos = html.find(scriptStart, pos);
            std::size_t stylePos = html.find(styleStart, pos);

            if (scriptPos != std::string::npos && (stylePos == std::string::npos || scriptPos < stylePos))
            {
                result += html.substr(lastPos, scriptPos - lastPos);
                pos = html.find(scriptEnd, scriptPos);
                if (pos != std::string::npos)
                {
                    pos += scriptEnd.length();
                }
                else
                {
                    result += html.substr(scriptPos);
                    break;
                }
            }
            else if (stylePos != std::string::npos)
            {
                result += html.substr(lastPos, stylePos - lastPos);
                pos = html.find(styleEnd, stylePos);
                if (pos != std::string::npos)
                {
                    pos += styleEnd.length();
                }
                else
                {
                    result += html.substr(stylePos);
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

    std::string removeHtmlEntities(const std::string &html)
    {
        static const std::unordered_map<std::string, std::string> knownEntities = {
            {"&quot;", ""}, {"&lt;", ""}, {"&gt;", ""}, {"&amp;", ""}, {"&apos;", ""}, {"&nbsp;", ""}, {"&excl;", ""}, {"&num;", ""}, {"&dollar;", ""}, {"&percnt;", ""}, {"&cent;", ""}, {"&copy;", ""}, {"&curren;", ""}, {"&euro;", ""}, {"&frac12;", ""}, {"&frac14;", ""}, {"&frac34;", ""}, {"&laquo;", ""}, {"&ldquo;", ""}, {"&li;", ""}, {"&mdash;", ""}, {"&middot;", ""}, {"&ndash;", ""}, {"&not;", ""}, {"&ordf;", ""}, {"&ordm;", ""}, {"&para;", ""}, {"&plusmn;", ""}, {"&pound;", ""}, {"&prime;", ""}, {"&raquo;", ""}, {"&reg;", ""}, {"&sect;", ""}, {"&shy;", ""}, {"&sup1;", ""}, {"&sup2;", ""}, {"&sup3;", ""}, {"&times;", ""}, {"&trade;", ""}, {"&uarr;", ""}, {"&uml;", ""}, {"&yen;", ""}};

        std::string cleanedHtml;
        cleanedHtml.reserve(html.size());

        std::regex htmlEntitiesPattern(R"(&\w+;)");
        std::sregex_iterator htmlEntitiesIterator(html.begin(), html.end(), htmlEntitiesPattern);
        std::sregex_iterator htmlEntitiesEnd;

        std::string::const_iterator searchStart = html.begin();

        for (std::sregex_iterator i = htmlEntitiesIterator; i != htmlEntitiesEnd; ++i)
        {
            std::smatch match = *i;
            if (knownEntities.find(match.str()) != knownEntities.end())
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

    std::string removeTag(const std::string &html, const std::string &tag)
    {
        std::string removedHtml = html;
        std::string openingTag = "<" + tag;
        std::string closingTag = "</" + tag + ">";

        size_t pos = removedHtml.find(openingTag);
        while (pos != std::string::npos)
        {
            size_t endPos = removedHtml.find(">", pos + openingTag.length());
            if (endPos == std::string::npos)
                break;

            size_t closingTagPos = removedHtml.find(closingTag, endPos);
            if (closingTagPos != std::string::npos)
            {
                removedHtml.erase(pos, closingTagPos - pos + closingTag.length());
            }

            pos = removedHtml.find(openingTag, pos);
        }

        return removedHtml;
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

        size_t pos = removedHtml.find(openingTag);
        while (pos != std::string::npos)
        {
            size_t endPos = removedHtml.find(">", pos + openingTag.length());
            if (endPos == std::string::npos)
                break;

            size_t closingTagPos = removedHtml.find(closingTag, endPos);
            if (closingTagPos != std::string::npos)
            {
                removedHtml.erase(pos, closingTagPos - pos + closingTag.length());
            }

            pos = removedHtml.find(openingTag, pos);
        }

        return removedHtml;
    }

    std::string removeAllAttributes(const std::string &html, const std::string &tag)
    {
        std::string removedHtml = html;
        std::string openingTag = "<" + tag;
        std::string closingTag = "</" + tag + ">";

        size_t pos = removedHtml.find(openingTag);
        while (pos != std::string::npos)
        {
            size_t endPos = removedHtml.find(">", pos + openingTag.length());
            if (endPos == std::string::npos)
                break;

            removedHtml.erase(pos + openingTag.length(), endPos - pos - openingTag.length());

            pos = removedHtml.find(openingTag, pos);
        }

        return removedHtml;
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

    std::vector<std::string> extractURLs(const std::string &html)
    {
        std::vector<std::string> urls;
        std::regex urlPattern("<a\\s+[^>]*href=\"([^\"]*)\"[^>]*>.*?<\\/a>");

        std::sregex_iterator urlIterator(html.begin(), html.end(), urlPattern);
        std::sregex_iterator urlEnd;

        for (std::sregex_iterator i = urlIterator; i != urlEnd; ++i)
        {
            std::smatch match = *i;
            urls.push_back(match.str(1));
        }

        return urls;
    }
    std::string encodeHtmlEntities(const std::string &input)
    {
        static const std::unordered_map<char, std::string> specialCharacters = {
            {'"', "&quot;"}, {'<', "&lt;"}, {'>', "&gt;"}, {'&', "&amp;"}, {'\'', "&apos;"}};

        std::string encodedString;
        encodedString.reserve(input.size());

        for (const char &c : input)
        {
            if (specialCharacters.find(c) != specialCharacters.end())
            {
                encodedString += specialCharacters.at(c);
            }
            else
            {
                encodedString += c;
            }
        }

        return encodedString;
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
            line = std::regex_replace(line, std::regex("^\\s+"), "");

            if (line.empty())
                continue;

            if (line.find("</") != std::string::npos)
                indentLevel--;

            indentedCode += std::string(indentLevel * spacesPerLevel, ' ') + line + "\n";

            if (line.find("<") != std::string::npos && line.find("</") == std::string::npos && line.find("/>") == std::string::npos)
                indentLevel++;
        }

        return indentedCode;
    }
    std::vector<std::string> extractInlineCss(const std::string &input)
    {
        std::vector<std::string> inlineCss;
        std::regex cssPattern("style=\"(.*?)\"");
        std::sregex_iterator cssIterator(input.begin(), input.end(), cssPattern);
        std::sregex_iterator cssEnd;

        for (std::sregex_iterator i = cssIterator; i != cssEnd; ++i)
        {
            std::smatch match = *i;
            inlineCss.push_back(match.str(1));
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
        std::regex tagPattern("<" + tag + "[^>]*>(.*?)</" + tag + ">");
        std::sregex_iterator tagIterator(html.begin(), html.end(), tagPattern);
        std::sregex_iterator tagEnd;

        for (std::sregex_iterator i = tagIterator; i != tagEnd; ++i)
        {
            std::smatch match = *i;
            tagContents.push_back(match.str(1));
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
    std::string textToHtml(const std::string &text)
    {
        std::string html = text;
        html = std::regex_replace(html, std::regex("&"), "&amp;");
        html = std::regex_replace(html, std::regex("\""), "&quot;");
        html = std::regex_replace(html, std::regex("'"), "&apos;");
        html = std::regex_replace(html, std::regex("<"), "&lt;");
        html = std::regex_replace(html, std::regex(">"), "&gt;");
        return html;
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
        std::regex tagPattern("<" + tag + "[^>]*>");
        std::regex attributePattern(attribute + "\\s*=\\s*\"([^\"]*)\"");

        std::string modifiedHTML = html;
        std::smatch match;

        while (std::regex_search(modifiedHTML, match, tagPattern))
        {
            std::string openingTag = match.str();
            if (std::regex_search(openingTag, match, attributePattern))
            {

                std::string attributeValue = match.str(1);
                std::string attributeToRemove = attribute + "=\"" + attributeValue + "\"";
                modifiedHTML = std::regex_replace(modifiedHTML, std::regex(attributeToRemove), "");
            }

            modifiedHTML = match.suffix().str();
        }

        return modifiedHTML;
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

    std::string removeAllComments(const std::string &html)
    {
        std::regex commentPattern("<!--(.*?)-->");
        return std::regex_replace(html, commentPattern, "");
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
};
extern GigaWeb *giga;