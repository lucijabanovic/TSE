# CS50 TSE Querier
## Implementation Spec

In this document, we reference the requirements specifications and design specifications while focusing on implementation-specific decisions relevant to the Querier.

## Data Structures

The Querier utilizes the following data structures:
- **Twosets_t**: A struct containing two counters for easier intersections between sets.
- **Counter**: To represent collections of documents satisfying queries and their scores.
- **Index_t**: A hashtable structure containing words mapping to counters.

## Control Flow

The Querier is implemented in one file `querier.c`, with the following key functions:

### main
- Validates command-line arguments.
- Checks the validity of the page directory.
- Loads the index from the specified file.
- Enters a loop to read queries from standard input, processes them, and prints the results.

### parseQuery
- Validates the input query:
  - Ensures it contains only letters and spaces.
  - Tokenizes the input.
  - Performs a literal check for query syntax (ensuring operators are used correctly).

### tokenize
- Breaks the input line into words without using `strtok`.
- Dynamically allocates memory for each token and stores them in an array.

### literalCheck
- Ensures the syntax of the query is valid:
  - Checks for operators at the beginning or end of the query.
  - Checks for adjacent operators.

### getDocSet
- Processes the parsed words and builds a counters set of documents that satisfy the query.
- Handles both "AND" and "OR" logic between terms and precedence of "AND" over "OR"

### intersect and merge
- These functions handle set operations:
  - **`intersect`**: Computes the intersection of two counters.
  - **`merge`**: Computes the union of two counters.

## Document Printing
- The `docPrint` function:
  - Iterates through the document set to print:
    - Scores
    - IDs
    - URLs of the matching documents.

This function formats and outputs the final results of a query, listing the scores, document IDs, and corresponding URLs.

## Error Handling and Recovery

Error handling is a crucial aspect of the Querier's implementation. The following strategies are employed:
- Command-line arguments are checked rigorously. Errors result in messages printed to `stderr`, and the program exits with a non-zero status.
- Memory allocation errors are managed through checks. If memory allocation fails, an error message is printed, and the program exits.
- The program handles I/O errors, including issues with reading files or the index, returning appropriate error messages and exiting.

## Testing Plan

Run in terminal:

```
make test
```

