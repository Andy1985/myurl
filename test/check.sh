#!/bin/bash

[ $# -ne 2 ] && { echo "Usage: $0 <num> value"; exit; }

MemHost=192.168.119.128
DnsFailMem=$MemHost:13000
RedirectMem=$MemHost:13001
DnsARecordMem=$MemHost:13002
UrlInfoMem=$MemHost:13003
BlackListMem=$MemHost:13004

MemTool=memOption

DnsFailCheck()
{
	$MemTool $DnsFailMem get $1
}

RedirectCheck()
{
	$MemTool $RedirectMem get $1
}

DnsARecordCheck()
{
	$MemTool $DnsARecordMem get $1
}

UrlInfoCheck()
{
	$MemTool $UrlInfoMem get $1
}

BlackListCheck()
{
	$MemTool $BlackListMem get $1
}


case $1 in
	1|df)
		DnsFailCheck $2
		;;
	2|rd)
		RedirectCheck $2
		;;
	3|da)
		DnsARecordCheck $2
		;;
	4|ui)
		UrlInfoCheck $2
		;;
	5|bl)
		BlackListCheck $2
		;;
esac
