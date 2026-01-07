#!/bin/bash

# Define the list of files to be formatted using the find command
FILES=$(find . -type f -iname "*.cpp" -o -iname "*.hpp")

# Get the total number of files being analyzed
TOTAL_FILES=$(echo "$FILES" | wc -l | tr -d ' ')

LOG_INFO="\033[0;36m[INFO]\033[m"
GREEN='\033[0;32m'
NC='\033[0m'
echo -e "$LOG_INFO Formatting $TOTAL_FILES files..."

# Initialize a counter for the number of changed files
CHANGED_FILES=0

# Initialize an array to store the names of the changed files
CHANGED_FILENAMES=()

# Loop through each file and run clang-format
for file in ${FILES[@]}
do
  # Save a copy of the original file
  cp $file $file.orig

  # Run clang-format on the file
  clang-format -i $file

  # Compare the original file to the formatted file
  if ! cmp -s $file $file.orig
  then
    ((CHANGED_FILES++))
    CHANGED_FILENAMES+=("$file")
  fi

  # Delete the temporary file
  rm $file.orig
done

# Print the total number of changed files

# Print the names of the changed files
if [ $CHANGED_FILES -gt 0 ]
then
  echo "Formatted $CHANGED_FILES files:"
  for changed in ${CHANGED_FILENAMES[@]}
  do
    echo -e "\t${GREEN}${changed}${NC}"
  done
else
  echo "All files already formatted!"
fi
