#!/bin/bash


[ "$1" == "" ] && { echo "Usage: $0 <url>"; exit; }

key_rule_file=./engine/keyrule.l
CHECK="./check.sh"

url=$1
echo -e "\033[32;49;1murl: \033[32;49;0m$url"

domain=`echo $url|awk -F 'http://' '{print $2}'|awk -F '/' '{print $1}'`
echo -e "\033[32;49;1mdomain:\033[32;49;0m $domain" 

dns_fail_check=`$CHECK 1 $domain`
echo -e "\033[32;49;1mdns fail check:\033[32;49;0m $dns_fail_check"

redirect_check=`$CHECK 2 $url`
echo -e "\033[32;49;1mredirect check:\033[32;49;0m $redirect_check"

a_record_check=`$CHECK 3 $domain`
echo -e "\033[32;49;1ma record check:\033[32;49;0m $a_record_check"

url_count_check=`$CHECK 4 $domain`
echo -e "\033[32;49;1murl count check:\033[32;49;0m $url_count_check"

ip=(`echo $a_record_check|sed "s/|/ /g"`)

echo -e "\033[32;49;1mblacklist check:\033[32;49;0m"
for ((i=0;i<${#ip[@]};i++))
do
	blacklist_check=`$CHECK 5 ${ip[$i]}`
	[ "$blacklist_check" != "(null)" ] && key_check_code=`echo $blacklist_check|awk -F '_' '{print $1}'`
	[ "$key_check_code" != "" ] && key_word=`cat $key_rule_file |grep "return $key_check_code"|awk -F '{' '{print $1}'`
	echo -e "\033[31;49;1m${ip[$i]} $blacklist_check $key_word\033[31;49;0m"
done
