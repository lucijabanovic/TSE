# README for Indexer Implementation

## Assumptions Made

1. **Input Format**: It is assumed that the input HTML files are well-formed and follow a standard structure. The indexer expects HTML content to be present between `<html>` and `</html>` tags.

2. **Crawler Output**: The implementation assumes that the pages in the `pageDirectory` have been produced by a crawler that follows a specific output format. This includes the presence of a `.crawler` file to indicate that the directory is valid.

3. **Word Normalization**: The implementation normalizes words by converting them to lowercase before indexing. It is assumed that words shorter than three characters should be ignored.

4. **File and Directory Permissions**: The program assumes that it has the necessary permissions to read from the specified input directory and to create/write the output index file.

5. **Memory Management**: The implementation assumes that the `index_t` and `webpage_t` structures manage their memory effectively, and appropriate deallocation functions are called.

## Differences from Specifications

1. **Error Messages**: The error messages provided in the implementation may not exactly match those specified in the requirements document.

3. **Handling of HTML Tags**: The implementation never mentioned ignoring the html tags in <> brackets but I thought it goes without saying so I also made the index_page() ignore those words along with words that have a length < 3.

