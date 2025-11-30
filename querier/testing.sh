#!/bin/bash

# testing.sh - Test script for the Tiny Search Engine's Querier

# Ensure we are using bash and print each command before executing
# set -v

# Test Setup
# Define the page directory and index file paths
PAGE_DIRECTORY="../webpages"               # Adjust this to your actual page directory
INDEX_FILE="../indexer/indexFile.index"    # Adjust this to your actual index filename

# Test 1: Valid query
echo "Running Test 1: Valid Query"
echo "page and home" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"  # Expect merged set of documents

# Test 2: Query with "or" operator
echo "Running Test 2: Query with OR Operator"
echo "page home or search breadth" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"  # Expect intersected set of documents

# Test 3: Query with "and" operator
echo "Running Test 3: Query with AND Operator"
echo "for and the and algorithm" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"  # Expect merged set of documents

# Test 4: Invalid query (contains non-alphabetic characters)
echo "Running Test 4: Invalid Query (Non-Alphabetic Characters)"
echo "home 4 home page." | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"  # Expect error for invalid characters

# Test 5: Query with leading operator
echo "Running Test 5: Query with Leading Operator"
echo "and for the playground" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"  # Expect error for operator at beginning

# Test 6: Query with trailing operator
echo "Running Test 6: Query with Trailing Operator"
echo "playground breadth and" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"  # Expect error for operator at end

# Test 7: Adjacent operators
echo "Running Test 7: Adjacent Operators"
echo "home the for the and and search" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"  # Expect error for adjacent operators

# Test 8: Invalid words
echo "Running Test 8: Invalid words"
echo "Bloop Haha huhuhu jeeez" | ./querier "$PAGE_DIRECTORY" "$INDEX_FILE"  # Expect an error message
