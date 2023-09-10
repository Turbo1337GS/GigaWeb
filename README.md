# GigaWeb Class Documentation

The `GigaWeb` class provides various methods for manipulating and extracting information from HTML content. It includes functions for fetching web content, cleaning HTML and text, removing specific tags and attributes, extracting data between tags, modifying tags and attributes, and much more.

## Class Methods

### bool fetchWebContent(const std::string &url, std::string &content}

This method fetches the web content from the given URL and stores it in the `content` parameter. It uses the libcurl library to perform the HTTP request. The method returns `true` if the content was successfully fetched, and `false` otherwise.

**Example:**

```cpp
std::string content;
bool success = giga->fetchWebContent("https://example.com", content);
if (success) {
    std::cout << "Content fetched successfully: " << content << std::endl;
} else {
    std::cerr << "Failed to fetch content." << std::endl;
}
```

### std::string cleanTXT(const std::string &input}

This method cleans the given text by removing extra spaces, newlines, and trimming leading and trailing whitespace. It returns the cleaned text.

**Example:**

```cpp
std::string input = "   Hello    \n   World!   ";
std::string cleanedText = giga->cleanTXT(input);
std::cout << "Cleaned Text: " << cleanedText << std::endl;
// Output: "Hello\nWorld!"
```

### std::string cleanHTML(std::string &html}

This method cleans the given HTML by removing all HTML tags. It returns the cleaned HTML.

**Example:**

```cpp
std::string html = "<p>Hello <b>World!</b></p>";
std::string cleanedHTML = giga->cleanHTML(html);
std::cout << "Cleaned HTML: " << cleanedHTML << std::endl;
// Output: "Hello World!"
```

### std::string removeScriptAndStyleTags(const std::string &html}

This method removes `<script>` and `<style>` tags from the given HTML content. It returns the HTML content without the script and style tags.

**Example:**

```cpp
std::string html = "<p>Hello <script>alert('World!');</script></p>";
std::string cleanedHTML = giga->removeScriptAndStyleTags(html);
std::cout << "Cleaned HTML: " << cleanedHTML << std::endl;
// Output: "<p>Hello </p>"
```

### std::string removePathAndSvgTags(const std::string &html}

This method removes `<path>` and `<svg>` tags from the given HTML content. It returns the HTML content without the path and svg tags.

**Example:**

```cpp
std::string html = "<p>Hello <path d='M10 20' /></p>";
std::string cleanedHTML = giga->removePathAndSvgTags(html);
std::cout << "Cleaned HTML: " << cleanedHTML << std::endl;
// Output: "<p>Hello </p>"
```

### std::string encodeHtmlEntities(const std::string &html}

This method encodes HTML entities in the given HTML content. It replaces entities like `&lt;`, `&gt;`, `&amp;`, etc., with their corresponding characters. It returns the encoded HTML.

**Example:**

```cpp
std::string html = "<p>Hello &lt;World!&gt;</p>";
std::string encodedHTML = giga->encodeHtmlEntities(html);
std::cout << "Encoded HTML: " << encodedHTML << std::endl;
// Output: "<p>Hello <World!></p>"
```

### std::string removeHtmlEntities(const std::string &html}

This method removes HTML entities from the given HTML content. It removes entities like `&lt;`, `&gt;`, `&amp;`, etc. It returns the HTML content without the entities.

**Example:**

```cpp
std::string html = "<p>Hello &lt;World!&gt;</p>";
std::string cleanedHTML = giga->removeHtmlEntities(html);
std::cout << "Cleaned HTML: " << cleanedHTML << std::endl;
// Output: "<p>Hello World!</p>"
```

### bool hasTag(const std::string &html, const std::string &tag}

This method checks if the given HTML content contains the specified HTML tag. It returns `true` if the tag is found, and `false` otherwise.

**Example:**

```cpp
std::string html = "<p>Hello <b>World!</b></p>";
bool hasBTag = giga->hasTag(html, "b");
std::cout << "Has <b> tag: " << std::boolalpha << hasBTag << std::endl;
// Output: "Has <b> tag: true"
```

### std::string getAttributeValue(const std::string &html, const std::string &tag, const std::string &attribute}

This method retrieves the value of the specified attribute from the given HTML tag. It returns the attribute value if found, or an empty string otherwise.

**Example:**

```cpp
std::string html = "<a href='https://example.com'>Link</a>";
std::string hrefValue = giga->getAttributeValue(html, "a", "href");
std::cout << "Href Value: " << hrefValue << std::endl;
// Output: "Href Value: https://example.com"
```

### std::string replaceTag(const std::string &html, const std::string &oldTag, const std::string &newTag}

This method replaces all occurrences of the specified HTML tag with a new tag in the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<p>Hello <b>World!</b></p>";
std::string replacedHTML = giga->replaceTag(html, "b", "strong");
std::cout << "Replaced HTML: " << replacedHTML << std::endl;
// Output: "<p>Hello <strong>World!</strong></p>"
```

### std::string extractContentBetweenTags(const std::string &html, const std::string &startTag, const std::string &endTag}

This method extracts the content between the specified start and end tags from the given HTML content. It returns the extracted content.

**Example:**

```cpp
std::string html = "<p>Hello <b>World!</b></p>";
std::string extractedContent = giga->extractContentBetweenTags(html, "<b>", "</b>");
std::cout << "Extracted Content: " << extractedContent << std::endl;
// Output: "World!"
```

### int countTagOccurrences(const std::string &html, const std::string &tag}

This method counts the number of occurrences of the specified HTML tag in the given HTML content. It returns the count of occurrences.

**Example:**

```cpp
std::string html = "<p>Hello <b>World!</b></p>";
int count = giga->countTagOccurrences(html, "b");
std::cout << "Tag Count: " << count << std::endl;
// Output: "Tag Count: 1"
```

### bool hasAttribute(const std::string &html, const std::string &tag, const std::string &attribute}

This method checks if the specified HTML tag has the given attribute in the given HTML content. It returns `true` if the attribute is found, and `false` otherwise.

**Example:**

```cpp
std::string html = "<a href='https://example.com'>Link</a>";
bool hasHref = giga->hasAttribute(html, "a", "href");
std::cout << "Has href attribute: " << std::boolalpha << hasHref << std::endl;
// Output: "Has href attribute: true"
```

### std::string replaceAllTags(const std::string &html, const std::string &oldTag, const std::string &newTag}

This method replaces all occurrences of the specified HTML tag with a new tag in the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<p>Hello <b>World!</b></p>";
std::string replacedHTML = giga->replaceAllTags(html, "b", "strong");
std::cout << "Replaced HTML: " << replacedHTML << std::endl;
// Output: "<p>Hello <strong>World!</strong></p>"
```

### bool hasAttributeValue(const std::string &html, const std::string &tag, const std::string &attribute, const std::string &value}

This method checks if the specified HTML tag has the given attribute with the specified value in the given HTML content. It returns `true` if the attribute value is found, and `false` otherwise.

**Example:**

```cpp
std::string html = "<a href='https://example.com'>Link</a>";
bool hasHref = giga->hasAttributeValue(html, "a", "href", "https://example.com");
std::cout << "Has href attribute with value: " << std::boolalpha << hasHref << std::endl;
// Output: "Has href attribute with value: true"
```

### std::vector<std::string> getAttributeValues(const std::string &html, const std::string &tag, const std::string &attribute}

This method retrieves all the values of the specified attribute from the given HTML tag in the given HTML content. It returns a vector of attribute values.

**Example:**

```cpp
std::string html = "<a href='https://example.com'>Link 1</a><a href='https://example.org'>Link 2</a>";
std::vector<std::string> hrefValues = giga->getAttributeValues(html, "a", "href");
for (const auto& value : hrefValues) {
    std::cout << "Href Value: " << value << std::endl;
}
// Output: "Href Value: https://example.com"
//         "Href Value: https://example.org"
```

### void addTag(std::string &html, const std::string &tag, const std::string &content}

This method adds a new HTML tag with the specified content to the given HTML content. It modifies the `html` parameter directly.

**Example:**

```cpp
std::string html = "<p>Hello</p>";
giga->addTag(html, "b", "World!");
std::cout << "Modified HTML: " << html << std::endl;
// Output: "<p>Hello</p><b>World!</b>"
```

### std::string removeAllTags(const std::string &html, const std::string &tag}

This method removes all occurrences of the specified HTML tag from the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<p>Hello <b>World!</b></p>";
std::string cleanedHTML = giga->removeAllTags(html, "b");
std::cout << "Cleaned HTML: " << cleanedHTML << std::endl;
// Output: "<p>Hello </p>"
```

### std::string removeAllAttributes(const std::string &html, const std::string &tag}

This method removes all attributes from the specified HTML tag in the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<a href='https://example.com' target='_blank'>Link</a>";
std::string cleanedHTML = giga->removeAllAttributes(html, "a");
std::cout << "Cleaned HTML: " << cleanedHTML << std::endl;
// Output: "<a>Link</a>"
```

### std::string normalizeHTML(const std::string &html}

This method normalizes the given HTML content by removing extra spaces, newlines, and trimming leading and trailing whitespace. It returns the normalized HTML.

**Example:**

```cpp
std::string html = "   <p>   Hello   World!   </p>   ";
std::string normalizedHTML = giga->normalizeHTML(html);
std::cout << "Normalized HTML: " << normalizedHTML << std::endl;
// Output: "<p>Hello World!</p>"
```

### std::vector<std::string> extractURLs(const std::string &html, const std::string &baseURL = ""}

This method extracts all URLs from the given HTML content. It returns a vector of URLs. If a `baseURL` is provided, it is used to resolve relative URLs.

**Example:**

```cpp
std::string html = "<a href='https://example.com'>Link 1</a><a href='/page2'>Link 2</a>";
std::vector<std::string> urls = giga->extractURLs(html, "https://example.org");
for (const auto& url : urls) {
    std::cout << "URL: " << url << std::endl;
}
// Output: "URL: https://example.com"
//         "URL: https://example.org/page2"
```

### std::string extractScriptSection(const std::string &html}

This method extracts the content of `<script>` tags from the given HTML content. It returns the extracted script section.

**Example:**

```cpp
std::string html = "<script>alert('Hello World!');</script>";
std::string scriptSection = giga->extractScriptSection(html);
std::cout << "Script Section: " << scriptSection << std::endl;
// Output: "alert('Hello World!');"
```

### std::string extractStyleSection(const std::string &html}

This method extracts the content of `<style>` tags from the given HTML content. It returns the extracted style section.

**Example:**

```cpp
std::string html = "<style>body { color: red; }</style>";
std::string styleSection = giga->extractStyleSection(html);
std::cout << "Style Section: " << styleSection << std::endl;
// Output: "body { color: red; }"
```

### std::vector<std::string> extractFunctionNames(const std::string &scriptSection}

This method extracts the names of functions defined in the given script section. It returns a vector of function names.

**Example:**

```cpp
std::string scriptSection = "function hello() { console.log('Hello World!'); }";
std::vector<std::string> functionNames = giga->extractFunctionNames(scriptSection);
for (const auto& name : functionNames) {
    std::cout << "Function Name: " << name << std::endl;
}
// Output: "Function Name: hello"
```

### std::string removeHtmlComments(const std::string &input}

This method removes HTML comments from the given input. It returns the input without the comments.

**Example:**

```cpp
std::string input = "<!-- This is a comment -->";
std::string cleanedInput = giga->removeHtmlComments(input);
std::cout << "Cleaned Input: " << cleanedInput << std::endl;
// Output: ""
```

### std::string indentHtmlCode(const std::string &input, int spacesPerLevel = 4}

This method indents the HTML code in the given input. It adds spaces before each line based on the specified number of spaces per level. It returns the indented HTML code.

**Example:**

```cpp
std::string input = "<div>\n<p>Hello</p>\n</div>";
std::string indentedCode = giga->indentHtmlCode(input);
std::cout << "Indented Code:\n" << indentedCode << std::endl;
// Output:
// "<div>
//     <p>Hello</p>
// </div>"
```

### std::vector<std::string> extractInlineCss(const std::string &input}

This method extracts inline CSS from the given input. It returns a vector of inline CSS strings.

**Example:**

```cpp
std::string input = "<p style=\"color: red;\">Hello</p>";
std::vector<std::string> inlineCss = giga->extractInlineCss(input);
for (const auto& css : inlineCss) {
    std::cout << "Inline CSS: " << css << std::endl;
}
// Output: "Inline CSS: color: red;"
```

### std::string removeInlineStyles(const std::string &html}

This method removes inline styles from the given HTML content. It returns the HTML content without the inline styles.

**Example:**

```cpp
std::string html = "<p style=\"color: red;\">Hello</p>";
std::string cleanedHTML = giga->removeInlineStyles(html);
std::cout << "Cleaned HTML: " << cleanedHTML << std::endl;
// Output: "<p>Hello</p>"
```

### std::vector<std::string> getTagContents(const std::string &html, const std::string &tag}

This method retrieves the content between the specified HTML tags from the given HTML content. It returns a vector of tag contents.

**Example:**

```cpp
std::string html = "<p>Hello</p><p>World!</p>";
std::vector<std::string> tagContents = giga->getTagContents(html, "p");
for (const auto& content : tagContents) {
    std::cout << "Tag Content: " << content << std::endl;
}
// Output: "Tag Content: Hello"
//         "Tag Content: World!"
```

### bool isNormalizedHTML(const std::string &html}

This method checks if the given HTML content is already normalized. It returns `true` if the content is normalized, and `false` otherwise.

**Example:**

```cpp
std::string html = "<p>Hello World!</p>";
bool isNormalized = giga->isNormalizedHTML(html);
std::cout << "Is HTML Normalized: " << std::boolalpha << isNormalized << std::endl;
// Output: "Is HTML Normalized: true"
```

### bool hasNestedTags(const std::string &html, const std::string &tag}

This method checks if the given HTML content contains nested tags of the specified tag. It returns `true` if nested tags are found, and `false` otherwise.

**Example:**

```cpp
std::string html = "<div><p>Hello <b>World!</b></p></div>";
bool hasNestedTags = giga->hasNestedTags(html, "div");
std::cout << "Has Nested Tags: " << std::boolalpha << hasNestedTags << std::endl;
// Output: "Has Nested Tags: true"
```

### std::string addAttributeToTag(const std::string &html, const std::string &tag, const std::string &attribute, const std::string &value}

This method adds the specified attribute with the given value to the specified HTML tag in the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<a href='https://example.com'>Link</a>";
std::string modifiedHTML = giga->addAttributeToTag(html, "a", "target", "_blank");
std::cout << "Modified HTML: " << modifiedHTML << std::endl;
// Output: "<a href='https://example.com' target='_blank'>Link</a>"
```

### std::string removeAttributeFromTag(const std::string &html, const std::string &tag, const std::string &attribute}

This method removes the specified attribute from the specified HTML tag in the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<a href='https://example.com' target='_blank'>Link</a>";
std::string modifiedHTML = giga->removeAttributeFromTag(html, "a", "target");
std::cout << "Modified HTML: " << modifiedHTML << std::endl;
// Output: "<a href='https://example.com'>Link</a>"
```

### std::string replaceTagContent(const std::string &html, const std::string &tag, const std::string &newContent}

This method replaces the content of the specified HTML tag with the given new content in the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<p>Hello <b>World!</b></p>";
std::string replacedHTML = giga->replaceTagContent(html, "b", "Universe!");
std::cout << "Replaced HTML: " << replacedHTML << std::endl;
// Output: "<p>Hello <b>Universe!</b></p>"
```

### bool doesTagHaveAttribute(const std::string &html, const std::string &tag, const std::string &attribute}

This method checks if the specified HTML tag has the given attribute in the given HTML content. It returns `true` if the attribute is found, and `false` otherwise.

**Example:**

```cpp
std::string html = "<a href='https://example.com'>Link</a>";
bool hasHref = giga->doesTagHaveAttribute(html, "a", "href");
std::cout << "Has href attribute: " << std::boolalpha << hasHref << std::endl;
// Output: "Has href attribute: true"
```

### std::string removeAllOccurrencesOfTag(const std::string &html, const std::string &tag}

This method removes all occurrences of the specified HTML tag from the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<p>Hello <b>World!</b></p><p>Another <b>World!</b></p>";
std::string cleanedHTML = giga->removeAllOccurrencesOfTag(html, "b");
std::cout << "Cleaned HTML: " << cleanedHTML << std::endl;
// Output: "<p>Hello </p><p>Another </p>"
```

### std::string addCssClassToTag(const std::string &html, const std::string &tag, const std::string &cssClass}

This method adds the specified CSS class to the specified HTML tag in the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<p>Hello</p>";
std::string modifiedHTML = giga->addCssClassToTag(html, "p", "highlight");
std::cout << "Modified HTML: " << modifiedHTML << std::endl;
// Output: "<p class='highlight'>Hello</p>"
```

### std::string removeCssClassFromTag(const std::string &html, const std::string &tag, const std::string &cssClass}

This method removes the specified CSS class from the specified HTML tag in the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<p class='highlight'>Hello</p>";
std::string modifiedHTML = giga->removeCssClassFromTag(html, "p", "highlight");
std::cout << "Modified HTML: " << modifiedHTML << std::endl;
// Output: "<p>Hello</p>"
```

### std::string replaceTagAttribute(const std::string &html, const std::string &tag, const std::string &attribute, const std::string &replacement}

This method replaces the specified attribute of the specified HTML tag with the given replacement value in the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<a href='https://example.com'>Link</a>";
std::string modifiedHTML = giga->replaceTagAttribute(html, "a", "href", "https://example.org");
std::cout << "Modified HTML: " << modifiedHTML << std::endl;
// Output: "<a href='https://example.org'>Link</a>"
```

### std::string wrapContentWithTag(const std::string &html, const std::string &startTag, const std::string &endTag}

This method wraps the content in the given HTML content with the specified start and end tags. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<p>Hello World!</p>";
std::string wrappedHTML = giga->wrapContentWithTag(html, "div", "span");
std::cout << "Wrapped HTML: " << wrappedHTML << std::endl;
// Output: "<div><span><p>Hello World!</p></span></div>"
```

### bool hasCssClass(const std::string &html, const std::string &tag, const std::string &cssClass}

This method checks if the specified HTML tag has the given CSS class in the given HTML content. It returns `true` if the CSS class is found, and `false` otherwise.

**Example:**

```cpp
std::string html = "<p class='highlight'>Hello</p>";
bool hasHighlightClass = giga->hasCssClass(html, "p", "highlight");
std::cout << "Has highlight class: " << std::boolalpha << hasHighlightClass << std::endl;
// Output: "Has highlight class: true"
```

### bool hasAttributeWithValue(const std::string &html, const std::string &tag, const std::string &attribute, const std::string &value}

This method checks if the specified HTML tag has the given attribute with the specified value in the given HTML content. It returns `true` if the attribute value is found, and `false` otherwise.

**Example:**

```cpp
std::string html = "<a href='https://example.com'>Link</a>";
bool hasHref = giga->hasAttributeWithValue(html, "a", "href", "https://example.com");
std::cout << "Has href attribute with value: " << std::boolalpha << hasHref << std::endl;
// Output: "Has href attribute with value: true"
```

### std::vector<std::string> extractImgSrc(const std::string &html}

This method extracts the `src` attribute values from all `<img>` tags in the given HTML content. It returns a vector of `src` attribute values.

**Example:**

```cpp
std::string html = "<img src='image1.jpg'><img src='image2.jpg'>";
std::vector<std::string> imgSrcs = giga->extractImgSrc(html);
for (const auto& src : imgSrcs) {
    std::cout << "Image Src: " << src << std::endl;
}
// Output: "Image Src: image1.jpg"
//         "Image Src: image2.jpg"
```

### std::string removeAllCommentsFromHTML(const std::string &html}

This method removes all HTML comments from the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<!-- This is a comment --><p>Hello World!</p>";
std::string cleanedHTML = giga->removeAllCommentsFromHTML(html);
std::cout << "Cleaned HTML: " << cleanedHTML << std::endl;
// Output: "<p>Hello World!</p>"
```

### std::string removeAllCommentsFromJS(const std::string &js}

This method removes all JavaScript comments from the given JavaScript code. It returns the modified JavaScript code.

**Example:**

```cpp
std::string js = "/* This is a comment */\nconsole.log('Hello World!');";
std::string cleanedJS = giga->removeAllCommentsFromJS(js);
std::cout << "Cleaned JavaScript: " << cleanedJS << std::endl;
// Output: "\nconsole.log('Hello World!');"
```

### std::string removeAllCommentsFromCSS(const std::string &css}

This method removes all CSS comments from the given CSS code. It returns the modified CSS code.

**Example:**

```cpp
std::string css = "/* This is a comment */\nbody { color: red; }";
std::string cleanedCSS = giga->removeAllCommentsFromCSS(css);
std::cout << "Cleaned CSS: " << cleanedCSS << std::endl;
// Output: "\nbody { color: red; }"
```

### std::string removeAllMetaTags(const std::string &html}

This method removes all `<meta>` tags from the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<meta charset='UTF-8'><p>Hello World!</p>";
std::string cleanedHTML = giga->removeAllMetaTags(html);
std::cout << "Cleaned HTML: " << cleanedHTML << std::endl;
// Output: "<p>Hello World!</p>"
```

### std::string changeDoctype(const std::string &html, const std::string &doctype}

This method changes the doctype of the given HTML content to the specified doctype. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<!DOCTYPE html><p>Hello World!</p>";
std::string modifiedHTML = giga->changeDoctype(html, "html5");
std::cout << "Modified HTML: " << modifiedHTML << std::endl;
// Output: "<!DOCTYPE html5><p>Hello World!</p>"
```

### std::vector<std::string> extractAllTags(const std::string &html}

This method extracts all HTML tags from the given HTML content. It returns a vector of tags.

**Example:**

```cpp
std::string html = "<p>Hello</p><b>World!</b>";
std::vector<std::string> tags = giga->extractAllTags(html);
for (const auto& tag : tags) {
    std::cout << "Tag: " << tag << std::endl;
}
// Output: "Tag: p"
//         "Tag: b"
```

### std::string removeAllIds(const std::string &html}

This method removes all `id` attributes from the HTML tags in the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<p id='1'>Hello</p><b id='2'>World!</b>";
std::string cleanedHTML = giga->removeAllIds(html);
std::cout << "Cleaned HTML: " << cleanedHTML << std::endl;
// Output: "<p>Hello</p><b>World!</b>"
```

### std::string changeTagNames(const std::string &html, const std::string &newTagName}

This method changes the tag names of all HTML tags in the given HTML content to the specified new tag name. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<p>Hello</p><b>World!</b>";
std::string modifiedHTML = giga->changeTagNames(html, "span");
std::cout << "Modified HTML: " << modifiedHTML << std::endl;
// Output: "<span>Hello</span><span>World!</span>"
```

### std::string removeEmptyTags(const std::string &html}

This method removes all empty HTML tags from the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<p></p><b>World!</b><span></span>";
std::string cleanedHTML = giga->removeEmptyTags(html);
std::cout << "Cleaned HTML: " << cleanedHTML << std::endl;
// Output: "<b>World!</b>"
```

### std::string addAltToImg(const std::string &html, const std::string &altText}

This method adds the `alt` attribute with the specified text to all `<img>` tags in the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<img src='image.jpg'>";
std::string modifiedHTML = giga->addAltToImg(html, "Image");
std::cout << "Modified HTML: " << modifiedHTML << std::endl;
// Output: "<img src='image.jpg' alt='Image'>"
```

### std::string removeAllClasses(const std::string &html}

This method removes all `class` attributes from the HTML tags in the given HTML content. It returns the modified HTML content.

**Example:**

```cpp
std::string html = "<p class='highlight'>Hello</p><b class='bold'>World!</b>";
std::string cleanedHTML = giga->removeAllClasses(html);
std::cout << "Cleaned HTML: " << cleanedHTML << std::endl;
// Output: "<p>Hello</p><b>World!</b>"
```

## Conclusion

The `GigaWeb` class provides a comprehensive set of methods for manipulating and extracting information from HTML content. It offers flexibility and ease of use for various HTML processing tasks. By leveraging these methods, you can efficiently clean, extract, modify, and analyze HTML content according to your specific requirements.
