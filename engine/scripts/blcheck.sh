#!/bin/bash
arr=() 

for code in `./memcached-tool 192.168.119.128:13004 dump 2>/dev/null|grep -v set|awk -F '_' '{print $1}'`
do 
((arr[$code]++)) 
done

for((i=0;i<1000;i++))
do 
	[ "${arr[$i]}" != "" ] && echo $i ${arr[$i]} `cat ../keyrule.l|grep "return $i"|awk -F '{' '{print $1}'`
done 
