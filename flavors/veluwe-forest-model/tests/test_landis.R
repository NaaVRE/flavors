setwd("TestPnET_AllExtension")
ret <- system("call landis-ii-8 scenario.txt")
if (ret != 0) quit(status=ret)
