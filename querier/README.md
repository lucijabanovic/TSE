# Tiny Search Engine - Querier

## Overview
`querier.c` is part of a Tiny Search Engine project that processes user queries and retrieves relevant documents based on a previously built index. The program parses, validates, and executes logical queries involving words, using the AND and OR operators to rank matching documents.

## Features
- Parses user input to validate and structure queries.
- Supports logical operations (AND, OR) to refine search results.
- Ranks and outputs documents based on relevance and scores.
- Retrieves URLs of documents from the specified directory for display.

## Usage
The `querier.c` program takes two command-line arguments:
1. **pageDirectory**: The directory where crawler-generated pages are stored.
2. **indexFilename**: The index file containing mappings of words to document occurrences.

### Running the Program
```bash
./querier <pageDirectory> <indexFilename>
```

After starting, type a query in the terminal to search the indexed documents.

### Example Query
```bash
hospital and portrait originally fresh
```

This example query uses an AND operation, which requires that the listed words appear together in any document results.

## Query Syntax Rules
1. **Alphanumeric characters only**: Queries can only contain alphabetic characters and spaces.
2. **Logical operators**: Supports `and` and `or` operators. 
3. **Operator positions**:
   - Operators cannot be at the beginning or end of the query.
   - Operators cannot be adjacent to each other.

## Files
- **`index.h`**: Defines the data structure for indexing and functions for loading the index file.
- **`counters.h`, `file.h`**: Part of `libcs50`, these files provide data structures and helper functions used in `querier.c`.

## Functions

### Key Functions
- **`parseQuery`**: Validates query syntax.
- **`tokenize`**: Tokenizes query input into words.
- **`literalCheck`**: Checks syntax rules for logical operators.
- **`getDocSet`**: Retrieves a set of documents matching the query.
- **`intersect` & `merge`**: Implements AND and OR logic for sets of documents.
- **`docPrint`**: Outputs matching documents with their scores and URLs.

### Helper Functions
- **`intersectHelper` & `unionHelper`**: Apply set operations on document sets.
- **`maxScore` & `getKey`**: Retrieve the highest scoring document in a set.

## Testing
Refer to `testing.sh` for test cases and expected behavior, including:
1. Valid queries.
2. Queries with AND/OR operators.
3. Invalid queries (e.g., with non-alphabetic characters, adjacent operators).

## Example Output
```
Score: <document score>
ID: <document ID>
URL: <document URL>
```

## Error Handling
The program outputs relevant error messages for invalid queries or issues accessing the page directory or index file.

---

### Author
Lucija Banovic, FA24