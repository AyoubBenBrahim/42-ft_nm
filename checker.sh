#!/bin/bash

if [ $# -gt 1 ]; then
  flag=$1
  target_file=$2

if [ "$flag" != "-p" ] && [ "$flag" != "-r" ] && [ "$flag" != "-u" ] && [ "$flag" != "-a" ] && [ "$flag" != "-g" ]; then
    echo "Invalid flag. Valid flags are: -p -r -u -a -g"
    exit 1
  fi
else
  target_file=$1
fi

if [ -z "$target_file" ]; then
  echo "No file specified"
  exit 1
fi

./ft_nm $flag "$target_file" > my_output

nm $flag "$target_file" 2>/dev/null > system_output

diff_output=$(diff my_output system_output)

if [ -z "$diff_output" ]; then
  echo "Outputs are the same"
else
  echo "Outputs are different:"
  echo "$diff_output"
fi

# rm my_output system_output