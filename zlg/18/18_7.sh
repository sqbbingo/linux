#!/bin/sh
#demo for global&local var
fun(){
	var1=global
	local var2=local
	echo "in fun(),var1=$var1,var2=$var2"
}
fun
echo "out of fun(),var1=$var1,var2=$var2"