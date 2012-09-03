#!/bin/bash

for ((i=1;i<10000;++i))
do
	./test http://www.lixm__.com &

	usleep 10
done
