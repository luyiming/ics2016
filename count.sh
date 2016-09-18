#!/bin/bash
base=114441
sum=`find . -name "*.[ch]"|xargs cat|grep -v ^$|wc -l`
echo 'total lines of code:' $sum
echo 'write:' $(($sum - $base))
