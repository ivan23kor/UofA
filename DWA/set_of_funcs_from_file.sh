#!/bin/bash

grep define $1 | sed -E 's/^.+@"?//g' | sed -E 's/\(.+//g' > /tmp/run_on_file.txt
echo "{`paste -sd ',' /tmp/run_on_file.txt`}"
