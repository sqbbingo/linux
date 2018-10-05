#!/bin/bash
a=1234

let "a+=1"
echo $a

b=${a/23/cd}
echo $b