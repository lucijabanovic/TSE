# CS50 TSE Querier
## Design Spec

In this document, we reference the requirements specifications and focus on the implementation-independent design decisions.

- User interface
- Inputs and outputs
- Functional decomposition into modules
- Major data structures
- Testing plan

## User interface

The Querier's interface with the user is through the command-line; it requires exactly two arguments.

```bash
$ ./querier <pageDirectory> <indexFilename>
```

For example, to run the querier on a set of web pages stored in `data` folder and using an index file `index.txt`, the command would be:

```bash
$ ./querier ./data index.txt
```

## Inputs and outputs

*Input:* The Querier reads from standard input for queries.

*Output:* For each valid query, the Querier outputs the following for each document satisfying the query:

```
Score: <document score>
ID: <document id>
URL: <document url>
```

## Functional decomposition into modules

We have used the following functions:

1. *main*, which parses arguments and initializes other modules.
2. *parseQuery*, which validates and processes the input query string.
3. *tokenize*, which splits the input line into individual words.
4. *literalCheck*, which checks for valid operators in the query.
5. *getDocSet*, which retrieves a set of documents that match the query.
6. *intersect*, which computes the intersection of two document sets.
7. *merge*, which combines two document sets.
8. *docPrint*, which prints the results for the documents satisfying the query.

## Major data structures

The main data structures utilized in this module are:

- *counters_t*: A structure to maintain document IDs and their corresponding scores.
- *twosets_t*: A structure that holds two counters sets for intersection operations.

## Testing plan

A sampling of tests that should be run:

1. Test the program with various forms of invalid command-line arguments to ensure correct error handling.
2. Validate that input queries are parsed correctly, including edge cases like leading or trailing operators.
3. Test with various query combinations (e.g., using "and" and "or") to ensure correct document retrieval.
4. Check the output format to ensure it matches the specified format for scores, IDs, and URLs.
5. Use a known dataset and compare the results against expected document outputs to validate correctness.