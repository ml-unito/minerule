#!/bin/csh

foreach n (1 2 3 4 5 6 7 8 9 10)
	mymarcocore.exe >> noindex.log
end

foreach n (1 2 3 4 5 6 7 8 9 10)
	mymarcocore.exe lt 200 >> 12.log
end

foreach n (1 2 3 4 5 6 7 8 9 10)
	mymarcocore.exe lt 300 >> 22.log
end

foreach n (1 2 3 4 5 6 7 8 9 10)
	mymarcocore.exe lt 500 >> 39.log
end

foreach n (1 2 3 4 5 6 7 8 9 10)
	mymarcocore.exe lt 800 >> 57.log
end

foreach n (1 2 3 4 5 6 7 8 9 10)
	mymarcocore.exe lt 1000 >> 67.log
end

foreach n (1 2 3 4 5 6 7 8 9 10)
	mymarcocore.exe lt 1200 >> 73.log
end

foreach n (1 2 3 4 5 6 7 8 9 10)
	mymarcocore.exe lt 1500 >> 80.log
end

foreach n (1 2 3 4 5 6 7 8 9 10)
	mymarcocore.exe lt 2000 >> 89.log
end

foreach n (1 2 3 4 5 6 7 8 9 10)
	mymarcocore.exe lt 3000 >> 96.log
end

foreach n (1 2 3 4 5 6 7 8 9 10)
	mymarcocore.exe lt 5000 >> 99.log
end

