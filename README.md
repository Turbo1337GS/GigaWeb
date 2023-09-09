# GigaWeb Class Documentation

## Introduction
GigaWeb is a C++ class that provides various functionalities for processing and manipulating web content. It includes methods for fetching web content using CURL, cleaning HTML code, extracting information from HTML tags, modifying HTML attributes, and much more. This documentation will provide a detailed description of the class, its methods, and usage examples along with the expected results.

## Class Structure
The GigaWeb class is implemented in a single header file and does not require any external dependencies other than the CURL library.

```cpp
class GigaWeb {
  // Class members and methods
};
```

## Class Methods

### 1. `bool fetchWebContent(const std::string &url, std::string &content)`
This method fetches the web content from the specified URL using CURL. It takes the URL as input and stores the fetched content in the `content` parameter. Returns `true` if the content is successfully fetched, and `false` otherwise.

Example usage:
```cpp
std::string url = "https://example.com";
std::string content;

bool success = giga->fetchWebContent(url, content);
if (success) {
    std::cout << "Content fetched successfully!" << std::endl;
    std::cout << "Content: " << content << std::endl;
} else {
    std::cout << "Failed to fetch content." << std::endl;
}
```

### 2. `std::string cleanTXT(const std::string &input)`
This method cleans the input text by removing extra spaces, newlines, and trimming leading/trailing whitespaces. Returns the cleaned text.

Example usage:
```cpp
std::string dirtyText = "   This      is   a   dirty\n\n\ttext.   ";
std::string cleanedText = giga->cleanTXT(dirtyText);
std::cout << "Cleaned text: " << cleanedText << std::endl;
```
Expected output:
```
Cleaned text: This is a dirty text.
```

### 3. `bool containsOtherTags(const std::string &text, const std::string &currentTag)`
This method checks if the provided text contains any other tags other than the specified `currentTag`. Returns `true` if other tags are found, and `false` otherwise.

Example usage:
```cpp
std::string text = "<div><p>Hello World!</p></div>";
std::string currentTag = "div";

bool hasOtherTags = giga->containsOtherTags(text, currentTag);
if (hasOtherTags) {
    std::cout << "Text contains other tags besides the current tag." << std::endl;
} else {
    std::cout << "Text does not contain any other tags besides the current tag." << std::endl;
}
```
Expected output:
```
Text contains other tags besides the current tag.
```

### 4. `std::string cleanHTML(std::string &html)`
This method removes all HTML tags from the provided HTML code and returns the cleaned text.

Example usage:
```cpp
std::string dirtyHTML = "<h1>Hello <em>World</em>!</h1>";
std::string cleanedText = giga->cleanHTML(dirtyHTML);
std::cout << "Cleaned text: " << cleanedText << std::endl;
```
Expected output:
```
Cleaned text: Hello World!
```

### 5. `std::string removeScriptAndStyleTags(const std::string &html)`
This method removes the `<script>` and `<style>` tags from the provided HTML code and returns the modified HTML.

Example usage:
```cpp
std::string html = "<h1>Title</h1><script>alert('Hello');</script>";
std::string result = giga->removeScriptAndStyleTags(html);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <h1>Title</h1>
```

### 6. `std::string removePathAndSvgTags(const std::string &html)`
This method removes the `<path>` and `<svg>` tags from the provided HTML code and returns the modified HTML.

Example usage:
```cpp
std::string html = "<h1>Title</h1><svg><path d='M10 10'></path></svg>";
std::string result = giga->removePathAndSvgTags(html);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <h1>Title</h1>
```

### 7. `std::string removeHtmlEntities(const std::string &html)`
This method removes HTML entities from the provided HTML code and returns the modified HTML.

Example usage:
```cpp
std::string html = "<p>&lt;Hello&gt; &amp; &quot;World&quot;</p>";
std::string result = giga->removeHtmlEntities(html);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <p><Hello> & "World"</p>
```

### 8. `bool hasTag(const std::string &html, const std::string &tag)`
This method checks if the provided HTML code contains the specified tag. Returns `true` if the tag is found, and `false` otherwise.

Example usage:
```cpp
std::string html = "<div><p>Hello World!</p></div>";
std::string tag = "div";

bool hasTag = giga->hasTag(html, tag);
if (hasTag) {
    std::cout << "HTML contains the specified tag: " << tag << std::endl;
} else {
    std::cout << "HTML does not contain the specified tag: " << tag << std::endl;
}
```
Expected output:
```
HTML contains the specified tag: div
```

### 9. `std::string getAttributeValue(const std::string &html, const std::string &tag, const std::string &attribute)`
This method retrieves the value of the specified attribute for the given tag in the provided HTML code. Returns the attribute value if found, otherwise an empty string.

Example usage:
```cpp
std::string html = "<a href='https://example.com'>Link</a>";
std::string tag = "a";
std::string attribute = "href";

std::string value = giga->getAttributeValue(html, tag, attribute);
std::cout << "Attribute value: " << value << std::endl;
```
Expected output:
```
Attribute value: https://example.com
```

### 10. `std::string replaceTag(const std::string &html, const std::string &oldTag, const std::string &newTag)`
This method replaces all occurrences of the specified `oldTag` with the `newTag` in the provided HTML code. Returns the modified HTML.

Example usage:
```cpp
std::string html = "<h1>Title</h1><h2>Subtitle</h2>";
std::string oldTag = "h1";
std::string newTag = "h3";

std::string result = giga->replaceTag(html, oldTag, newTag);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <h3>Title</h3><h2>Subtitle</h2>
```

### 11. `std::string extractContentBetweenTags(const std::string &html, const std::string &startTag, const std::string &endTag)`
This method extracts the content between the specified `startTag` and `endTag` from the provided HTML code and returns the extracted content.

Example usage:
```cpp
std::string html = "<div><p>Hello World!</p></div>";
std::string startTag = "<p>";
std::string endTag = "</p>";

std::string extractedContent = giga->extractContentBetweenTags(html, startTag, endTag);
std::cout << "Extracted content: " << extractedContent << std::endl;
```
Expected output:
```
Extracted content: Hello World!
```

### 12. `int countTagOccurrences(const std::string &html, const std::string &tag)`
This method counts the number of occurrences of the specified tag in the provided HTML code and returns the count.

Example usage:
```cpp
std::string html = "<p>First</p><p>Second</p><p>Third</p>";
std::string tag = "p";

int count = giga->countTagOccurrences(html, tag);
std::cout << "Tag count: " << count << std::endl;
```
Expected output:
```
Tag count: 3
```

### 13. `bool hasAttribute(const std::string &html, const std::string &tag, const std::string &attribute)`
This method checks if the specified attribute exists for the given tag in the provided HTML code. Returns `true` if the attribute is found, and `false` otherwise.

Example usage:
```cpp
std::string html = "<a href='https://example.com'>Link</a>";
std::string tag = "a";
std::string attribute = "href";

bool hasAttribute = giga->hasAttribute(html, tag, attribute);
if (hasAttribute) {
    std::cout << "HTML contains the specified attribute: " << attribute << std::endl;
} else {
    std::cout << "HTML does not contain the specified attribute: " << attribute << std::endl;
}
```
Expected output:
```
HTML contains the specified attribute: href
```

### 14. `std::string removeTag(const std::string &html, const std::string &tag)`
This method removes all occurrences of the specified tag from the provided HTML code and returns the modified HTML.

Example usage:
```cpp
std::string html = "<h1>Title</h1><p>Paragraph</p><h2>Subtitle</h2>";
std::string tag = "p";

std::string result = giga->removeTag(html, tag);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <h1>Title</h1><h2>Subtitle</h2>
```

### 15. `std::string replaceAllTags(const std::string &html, const std::string &oldTag, const std::string &newTag)`
This method replaces all occurrences of the specified `oldTag` with the `newTag` in the provided HTML code. Returns the modified HTML.

Example usage:
```cpp
std::string html = "<h1>Title</h1><h2>Subtitle</h2>";
std::string oldTag = "h";
std::string newTag = "h3";

std::string result = giga->replaceAllTags(html, oldTag, newTag);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <h3>Title</h3><h3>Subtitle</h3>
```

### 16. `bool hasAttributeValue(const std::string &html, const std::string &tag, const std::string &attribute, const std::string &value)`
This method checks if the specified attribute with the given value exists for the provided tag in the HTML code. Returns `true` if the attribute with the value is found, and `false` otherwise.

Example usage:
```cpp
std::string html = "<a href='https://example.com'>Link</a>";
std::string tag = "a";
std::string attribute = "href";
std::string value = "https://example.com";

bool hasAttributeValue = giga->hasAttributeValue(html, tag, attribute, value);
if (hasAttributeValue) {
    std::cout << "HTML contains the specified attribute value pair: " << attribute << "=" << value << std::endl;
} else {
    std::cout << "HTML does not contain the specified attribute value pair: " << attribute << "=" << value << std::endl;
}
```
Expected output:
```
HTML contains the specified attribute value pair: href=https://example.com
```

### 17. `std::vector<std::string> getAttributeValues(const std::string &html, const std::string &tag, const std::string &attribute)`
This method retrieves all attribute values for the specified attribute and tag in the provided HTML code. Returns a vector of attribute values.

Example usage:
```cpp
std::string html = "<a href='https://example.com'>Link 1</a><a href='https://example2.com'>Link 2</a>";
std::string tag = "a";
std::string attribute = "href";

std::vector<std::string> attributeValues = giga->getAttributeValues(html, tag, attribute);
std::cout << "Attribute values: ";
for (const auto& value : attributeValues) {
    std::cout << value << ", ";
}
std::cout << std::endl;
```
Expected output:
```
Attribute values: https://example.com, https://example2.com,
```

### 18. `void addTag(std::string &html, const std::string &tag, const std::string &content)`
This method adds the specified tag with the provided content to the given HTML code.

Example usage:
```cpp
std::string html = "<h1>Title</h1>";
std::string tag = "p";
std::string content = "Paragraph";

giga->addTag(html, tag, content);
std::cout << "Modified HTML: " << html << std::endl;
```
Expected output:
```
Modified HTML: <h1>Title</h1><p>Paragraph</p>
```

### 19. `std::string removeAllTags(const std::string &html, const std::string &tag)`
This method removes all occurrences of the specified tag from the provided HTML code and returns the modified HTML.

Example usage:
```cpp
std::string html = "<p>Paragraph</p><div><p>Inner Paragraph</p></div>";
std::string tag = "p";

std::string result = giga->removeAllTags(html, tag);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <div></div>
```

### 20. `std::string removeAllAttributes(const std::string &html, const std::string &tag)`
This method removes all attributes from the specified tag in the provided HTML code and returns the modified HTML.

Example usage:
```cpp
std::string html = "<a href='https://example.com' target='_blank'>Link</a>";
std::string tag = "a";

std::string result = giga->removeAllAttributes(html, tag);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <a>Link</a>
```

### 21. `std::string normalizeHTML(const std::string &html)`
This method normalizes the provided HTML code by removing extra spaces, newlines, and trimming leading/trailing whitespaces. Returns the normalized HTML.

Example usage:
```cpp
std::string html = "   <h1>   Title   </h1>   ";
std::string normalizedHTML = giga->normalizeHTML(html);
std::cout << "Normalized HTML: " << normalizedHTML << std::endl;
```
Expected output:
```
Normalized HTML: <h1>Title</h1>
```

### 22. `std::vector<std::string> extractURLs(const std::string &html)`
This method extracts all URLs from the provided HTML code and returns them as a vector of strings.

Example usage:
```cpp
std::string html = "<a href='https://example.com'>Link 1</a><a href='https://example2.com'>Link 2</a>";
std::vector<std::string> urls = giga->extractURLs(html);
std::cout << "Extracted URLs: ";
for (const auto& url : urls) {
    std::cout << url << ", ";
}
std::cout << std::endl;
```
Expected output:
```
Extracted URLs: https://example.com, https://example2.com,
```

### 23. `std::string encodeHtmlEntities(const std::string &input)`
This method encodes special characters in the provided text as HTML entities. Returns the encoded string.

Example usage:
```cpp
std::string text = "<Hello World!>";
std::string encodedText = giga->encodeHtmlEntities(text);
std::cout << "Encoded text: " << encodedText << std::endl;
```
Expected output:
```
Encoded text: &lt;Hello World!&gt;
```

### 24. `std::string removeHtmlComments(const std::string &input)`
This method removes HTML comments from the provided input and returns the modified text.

Example usage:
```cpp
std::string html = "<p>Hello<!-- This is a comment -->World!</p>";
std::string result = giga->removeHtmlComments(html);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <p>HelloWorld!</p>
```

### 25. `std::string indentHtmlCode(const std::string &input, int spacesPerLevel = 4)`
This method indents the lines of the provided HTML code to improve readability. Returns the indented HTML code.

Example usage:
```cpp
std::string html = "<div><p>Paragraph</p><div><p>Inner Paragraph</p></div></div>";
std::string indentedCode = giga->indentHtmlCode(html);
std::cout << "Indented HTML: " << indentedCode << std::endl;
```
Expected output:
```
Indented HTML:
<div>
    <p>Paragraph</p>
    <div>
        <p>Inner Paragraph</p>
    </div>
</div>
```

### 26. `std::vector<std::string> extractInlineCss(const std::string &input)`
This method extracts the inline CSS styles from the provided HTML code and returns them as a vector of strings.

Example usage:
```cpp
std::string html = "<p style='color: red;'>Hello World!</p>";
std::vector<std::string> inlineCss = giga->extractInlineCss(html);
std::cout << "Inline CSS: ";
for (const auto& css : inlineCss) {
    std::cout << css << ", ";
}
std::cout << std::endl;
```
Expected output:
```
Inline CSS: color: red;
```

### 27. `std::string removeInlineStyles(const std::string &html)`
This method removes all inline CSS styles from the provided HTML code and returns the modified HTML.

Example usage:
```cpp
std::string html = "<p style='color: red;'>Hello World!</p>";
std::string result = giga->removeInlineStyles(html);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <p>Hello World!</p>
```

### 28. `std::vector<std::string> getTagContents(const std::string &html, const std::string &tag)`
This method retrieves the contents of all occurrences of the specified tag from the provided HTML code and returns them as a vector of strings.

Example usage:
```cpp
std::string html = "<div><p>Paragraph 1</p></div><div><p>Paragraph 2</p></div>";
std::string tag = "div";

std::vector<std::string> tagContents = giga->getTagContents(html, tag);
std::cout << "Tag contents: ";
for (const auto& content : tagContents) {
    std::cout << content << ", ";
}
std::cout << std::endl;
```
Expected output:
```
Tag contents: <p>Paragraph 1</p>, <p>Paragraph 2</p>,
```

### 29. `bool isNormalizedHTML(const std::string &html)`
This method checks if the provided HTML code is already normalized by comparing it with the normalized version. Returns `true` if the HTML is normalized, and `false` otherwise.

Example usage:
```cpp
std::string html = "<h1>Title</h1><p>Paragraph</p>";
bool isNormalized = giga->isNormalizedHTML(html);
if (isNormalized) {
    std::cout << "HTML is already normalized." << std::endl;
} else {
    std::cout << "HTML is not normalized." << std::endl;
}
```
Expected output:
```
HTML is already normalized.
```

### 30. `std::string textToHtml(const std::string &text)`
This method converts the provided plain text to HTML format by replacing special characters with their respective HTML entities. Returns the HTML formatted text.

Example usage:
```cpp
std::string text = "<Hello World>";
std::string html = giga->textToHtml(text);
std::cout << "HTML formatted text: " << html << std::endl;
```
Expected output:
```
HTML formatted text: &lt;Hello World&gt;
```

### 31. `bool hasNestedTags(const std::string &html, const std::string &tag)`
This method checks if the provided HTML code contains any occurrences of the specified tag with nested tags. Returns `true` if nested tags are found, and `false` otherwise.

Example usage:
```cpp
std::string html = "<div><p>Paragraph <strong>with</strong> nested tags</p></div>";
std::string tag = "p";

bool hasNestedTags = giga->hasNestedTags(html, tag);
if (hasNestedTags) {
    std::cout << "HTML contains nested tags for the specified tag." << std::endl;
} else {
    std::cout << "HTML does not contain nested tags for the specified tag." << std::endl;
}
```
Expected output:
```
HTML contains nested tags for the specified tag.
```

### 32. `std::string addAttributeToTag(const std::string &html, const std::string &tag, const std::string &attribute, const std::string &value)`
This method adds the specified attribute with the provided value to all occurrences of the specified tag in the provided HTML code. Returns the modified HTML.

Example usage:
```cpp
std::string html = "<a href='https://example.com'>Link 1</a><a href='https://example2.com'>Link 2</a>";
std::string tag = "a";
std::string attribute = "target";
std::string value = "_blank";

std::string result = giga->addAttributeToTag(html, tag, attribute, value);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <a href='https://example.com' target='_blank'>Link 1</a><a href='https://example2.com' target='_blank'>Link 2</a>
```

### 33. `std::string removeAttributeFromTag(const std::string &html, const std::string &tag, const std::string &attribute)`
This method removes the specified attribute from all occurrences of the specified tag in the provided HTML code. Returns the modified HTML.

Example usage:
```cpp
std::string html = "<a href='https://example.com' target='_blank'>Link 1</a><a href='https://example2.com' target='_blank'>Link 2</a>";
std::string tag = "a";
std::string attribute = "target";

std::string result = giga->removeAttributeFromTag(html, tag, attribute);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <a href='https://example.com'>Link 1</a><a href='https://example2.com'>Link 2</a>
```

### 34. `std::string replaceTagContent(const std::string &html, const std::string &tag, const std::string &newContent)`
This method replaces the content of all occurrences of the specified tag in the provided HTML code with the provided `newContent`. Returns the modified HTML.

Example usage:
```cpp
std::string html = "<p>Paragraph 1</p><p>Paragraph 2</p>";
std::string tag = "p";
std::string newContent = "New Paragraph";

std::string result = giga->replaceTagContent(html, tag, newContent);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <p>New Paragraph</p><p>New Paragraph</p>
```

### 35. `bool doesTagHaveAttribute(const std::string &html, const std::string &tag, const std::string &attribute)`
This method checks if the specified tag in the provided HTML code has the specified attribute. Returns `true` if the tag has the attribute, and `false` otherwise.

Example usage:
```cpp
std::string html = "<a href='https://example.com'>Link</a>";
std::string tag = "a";
std::string attribute = "href";

bool hasAttribute = giga->doesTagHaveAttribute(html, tag, attribute);
if (hasAttribute) {
    std::cout << "Tag has the specified attribute: " << attribute << std::endl;
} else {
    std::cout << "Tag does not have the specified attribute: " << attribute << std::endl;
}
```
Expected output:
```
Tag has the specified attribute: href
```

### 36. `std::string removeAllOccurrencesOfTag(const std::string &html, const std::string &tag)`
This method removes all occurrences of the specified tag from the provided HTML code and returns the modified HTML.

Example usage:
```cpp
std::string html = "<p>First Paragraph</p><p>Second Paragraph</p>";
std::string tag = "p";

std::string result = giga->removeAllOccurrencesOfTag(html, tag);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: 
```

### 37. `std::string addCssClassToTag(const std::string &html, const std::string &tag, const std::string &cssClass)`
This method adds the specified CSS class to all occurrences of the specified tag in the provided HTML code. Returns the modified HTML.

Example usage:
```cpp
std::string html = "<p>Paragraph 1</p><p>Paragraph 2</p>";
std::string tag = "p";
std::string cssClass = "highlight";

std::string result = giga->addCssClassToTag(html, tag, cssClass);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <p class="highlight">Paragraph 1</p><p class="highlight">Paragraph 2</p>
```

### 38. `std::string removeCssClassFromTag(const std::string &html, const std::string &tag, const std::string &cssClass)`
This method removes the specified CSS class from all occurrences of the specified tag in the provided HTML code. Returns the modified HTML.

Example usage:
```cpp
std::string html = "<p class='highlight'>Paragraph 1</p><p class='highlight'>Paragraph 2</p>";
std::string tag = "p";
std::string cssClass = "highlight";

std::string result = giga->removeCssClassFromTag(html, tag, cssClass);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <p>Paragraph 1</p><p>Paragraph 2</p>
```

### 39. `std::string replaceTagAttribute(const std::string &html, const std::string &tag, const std::string &attribute, const std::string &replacement)`
This method replaces the specified attribute with the provided replacement in all occurrences of the specified tag in the provided HTML code. Returns the modified HTML.

Example usage:
```cpp
std::string html = "<a href='https://example.com'>Link</a>";
std::string tag = "a";
std::string attribute = "href";
std::string replacement = "https://newexample.com";

std::string result = giga->replaceTagAttribute(html, tag, attribute, replacement);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <a href='https://newexample.com'>Link</a>
```

### 40. `std::string wrapContentWithTag(const std::string &html, const std::string &startTag, const std::string &endTag)`
This method wraps the content of the provided HTML code with the specified start and end tags. Returns the modified HTML.

Example usage:
```cpp
std::string html = "<p>This is some text.</p>";
std::string startTag = "<div>";
std::string endTag = "</div>";

std::string result = giga->wrapContentWithTag(html, startTag, endTag);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <div><p>This is some text.</p></div>
```

### 41. `bool hasCssClass(const std::string &html, const std::string &tag, const std::string &cssClass)`
This method checks if the specified tag in the provided HTML code has the specified CSS class. Returns `true` if the tag has the CSS class, and `false` otherwise.

Example usage:
```cpp
std::string html = "<p class='highlight'>Paragraph</p>";
std::string tag = "p";
std::string cssClass = "highlight";

bool hasCssClass = giga->hasCssClass(html, tag, cssClass);
if (hasCssClass) {
    std::cout << "Tag has the specified CSS class: " << cssClass << std::endl;
} else {
    std::cout << "Tag does not have the specified CSS class: " << cssClass << std::endl;
}
```
Expected output:
```
Tag has the specified CSS class: highlight
```

### 42. `bool hasAttributeWithValue(const std::string &html, const std::string &tag, const std::string &attribute, const std::string &value)`
This method checks if the provided HTML code contains the specified attribute with the given value for the specified tag. Returns `true` if the attribute with the value is found, and `false` otherwise.

Example usage:
```cpp
std::string html = "<a href='https://example.com'>Link</a>";
std::string tag = "a";
std::string attribute = "href";
std::string value = "https://example.com";

bool hasAttributeWithValue = giga->hasAttributeWithValue(html, tag, attribute, value);
if (hasAttributeWithValue) {
    std::cout << "HTML contains the specified attribute value pair: " << attribute << "=" << value << std::endl;
} else {
    std::cout << "HTML does not contain the specified attribute value pair: " << attribute << "=" << value << std::endl;
}
```
Expected output:
```
HTML contains the specified attribute value pair: href=https://example.com
```

### 43. `std::vector<std::string> extractImgSrc(const std::string &html)`
This method extracts the `src` attribute values from all `<img>` tags in the provided HTML code and returns them as a vector of strings.

Example usage:
```cpp
std::string html = "<img src='image1.jpg'><img src='image2.jpg'>";
std::vector<std::string> imgSrcs = giga->extractImgSrc(html);
std::cout << "Image srcs: ";
for (const auto& src : imgSrcs) {
    std::cout << src << ", ";
}
std::cout << std::endl;
```
Expected output:
```
Image srcs: image1.jpg, image2.jpg,
```

### 44. `std::string removeAllComments(const std::string &html)`
This method removes all HTML comments from the provided HTML code and returns the modified HTML.

Example usage:
```cpp
std::string html = "<p>Hello<!-- This is a comment -->World!</p>";
std::string result = giga->removeAllComments(html);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <p>HelloWorld!</p>
```

### 45. `std::string removeAllMetaTags(const std::string &html)`
This method removes all `<meta>` tags from the provided HTML code and returns the modified HTML.

Example usage:
```cpp
std::string html = "<p>Hello</p><meta charset='UTF-8'><meta name='description' content='Website'>";
std::string result = giga->removeAllMetaTags(html);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <p>Hello</p>
```

### 46. `std::string changeDoctype(const std::string &html, const std::string &doctype)`
This method changes the doctype of the provided HTML code to the specified doctype. Returns the modified HTML.

Example usage:
```cpp
std::string html = "<!DOCTYPE html><html><head><title>Title</title></head><body>Content</body></html>";
std::string doctype = "html5";

std::string result = giga->changeDoctype(html, doctype);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <!DOCTYPE html5><html><head><title>Title</title></head><body>Content</body></html>
```

### 47. `std::vector<std::string> extractAllTags(const std::string &html)`
This method extracts all unique tags present in the provided HTML code and returns them as a vector of strings.

Example usage:
```cpp
std::string html = "<h1>Title</h1><p>Paragraph</p><div><p>Inner Paragraph</p></div>";
std::vector<std::string> tags = giga->extractAllTags(html);
std::cout << "Extracted tags: ";
for (const auto& tag : tags) {
    std::cout << tag << ", ";
}
std::cout << std::endl;
```
Expected output:
```
Extracted tags: h1, p, div,
```

### 48. `std::string removeAllIds(const std::string &html)`
This method removes all `id` attributes from the provided HTML code and returns the modified HTML.

Example usage:
```cpp
std::string html = "<div id='container'><p id='paragraph'>Hello World!</p></div>";
std::string result = giga->removeAllIds(html);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <div><p>Hello World!</p></div>
```

### 49. `std::string changeTagNames(const std::string &html, const std::string &newTagName)`
This method changes all occurrences of the specified tag name to the provided new tag name in the provided HTML code. Returns the modified HTML.

Example usage:
```cpp
std::string html = "<h2>Title</h2><h2>Subtitle</h2>";
std::string oldTagName = "h2";
std::string newTagName = "h3";

std::string result = giga->changeTagNames(html, newTagName);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <h3>Title</h3><h3>Subtitle</h3>
```

### 50. `std::string removeEmptyTags(const std::string &html)`
This method removes all empty tags from the provided HTML code and returns the modified HTML.

Example usage:
```cpp
std::string html = "<div id='container'></div><p></p>";
std::string result = giga->removeEmptyTags(html);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML:
```

### 51. `std::string addAltToImg(const std::string &html, const std::string &altText)`
This method adds the `alt` attribute with the provided `altText` to all occurrences of the `<img>` tag in the provided HTML code. Returns the modified HTML.

Example usage:
```cpp
std::string html = "<img src='image1.jpg'><img src='image2.jpg'>";
std::string altText = "Image";

std::string result = giga->addAltToImg(html, altText);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <img src='image1.jpg' alt='Image'><img src='image2.jpg' alt='Image'>
```

### 52. `std::string removeAllClasses(const std::string &html)`
This method removes all `class` attributes from the provided HTML code and returns the modified HTML.

Example usage:
```cpp
std::string html = "<p class='highlight'>Paragraph 1</p><p class='highlight'>Paragraph 2</p>";
std::string result = giga->removeAllClasses(html);
std::cout << "Modified HTML: " << result << std::endl;
```
Expected output:
```
Modified HTML: <p>Paragraph 1</p><p>Paragraph 2</p>
```

## Conclusion
The GigaWeb class provides a wide range of functionalities for processing and manipulating web content. By using these methods, developers can fetch web content, clean HTML code, extract information from HTML tags, modify HTML attributes, and perform various other operations. The examples provided demonstrate the usage and expected results for each method. Feel free to explore and make use of this class to enhance your web content processing capabilities.
