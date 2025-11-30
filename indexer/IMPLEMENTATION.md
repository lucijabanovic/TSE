# Implementation Specification for `indexer.c`

## Overview

The `indexer` program reads web pages stored in a specified directory and creates an index file that maps words to document IDs. This program is intended to work with a set of HTML documents that have been previously crawled and stored in a directory by a crawler.

## Main Components

### Main Function

- **Arguments**: The program expects two command-line arguments:
  - `pageDirectory`: The directory containing the web pages.
  - `indexFilename`: The name of the index file to be created.
  
- **Error Handling**: 
  - The program checks if the number of arguments is correct.
  - It verifies that the page directory was produced by the Crawler by checking for a `.crawler` file in the specified directory.
  - It checks if the index file can be created or opened for writing.

### Index Building

- **Function: `indexBuild(char* pageDirectory, char* indexFilename)`**
  - Initializes a new index structure.
  - Loads web pages from the specified directory using a loop.
  - For each page, it calls `indexPage()` to extract words and update the index.
  - Saves the index to the specified index file at the end.

### Indexing Pages

- **Function: `indexPage(index_t* index, webpage_t* webpage, int docID)`**
  - Extracts words from a given web page, ignoring words shorter than three characters and those that are purely HTML tags.
  - Normalizes each word and inserts it into the index associated with the document ID.

## Data Structures

- `index_t`: A data structure that stores the mapping of words to document IDs.
- `webpage_t`: A structure representing a web page that includes methods to retrieve words and their positions within the HTML content.

## Testing Plan

### Testing Objectives

The testing plan aims to verify that the `indexer` program:
1. Correctly handles valid and invalid input scenarios.
2. Generates the expected index file format.
3. Properly interacts with the underlying filesystem and handles errors gracefully.

### Test Cases

1. **Valid Input Test**
   - Create a directory containing valid web pages.
   - Run the indexer with the page directory and check if the index file is created successfully.
   - Verify the contents of the index file against expected output.

2. **Invalid Page Directory Test**
   - Run the indexer with a non-existent or invalid page directory.
   - Ensure the program prints the appropriate error message.

3. **Insufficient Arguments Test**
   - Call the indexer without the required arguments.
   - Check that the program provides usage information.

4. **Empty Directory Test**
   - Run the indexer on an empty directory.
   - Verify that the index file is created and is empty.

5. **File Creation Test**
   - Attempt to create an index file in a directory where write permissions are restricted.
   - Ensure the program handles the error properly and prints the relevant message.

### Execution

Testing will be performed using a shell script (`testing.sh`) that automates the execution of these test cases. The results will be recorded, and any discrepancies will be reported for further investigation.
