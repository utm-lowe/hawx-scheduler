#!/bin/bash
expected=4

.test/run-hawx > .test/hawx.out
cat .test/hawx.out


if grep 'All counters done.' .test/hawx.out 
then
  echo "Scheduler test ... PASSED"
  exit 0
else
  echo "Scheduler test ... FAILED"
  exit 1
fi