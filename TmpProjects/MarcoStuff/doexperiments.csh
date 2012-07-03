#!/bin/csh -f

#foreach s (0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9)
foreach s (16 17 18 19 20)
	echo "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" >> log
#	echo "select tid,item from connect4 as BODY where item > 129*$s order by tid, item" > queryConnect1.txt
	echo extractItemset.exe 0.7 $s >> log
	extractItemset.exe 0.7 $s >> log
end
