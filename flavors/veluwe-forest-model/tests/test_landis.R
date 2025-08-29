system("git clone https://github.com/LANDIS-II-Foundation/Extension-Biomass-Succession.git")
setwd("Extension-Biomass-Succession/testings/CoreV8.0-BiomassSuccession7.0-SingleCell/")
ret <- system("dotnet $LANDIS_CONSOLE scenario.txt")
if (ret != 0) quit(status=ret)
