#!/bin/bash
read VAR
case $VAR in
	1) echo "one"
		;;
	2) echo "two"
		;;
	*) echo "unknown"
esac
