system("git clone https://github.com/LANDIS-II-Foundation/Extension-Biomass-Succession.git")
setwd("Extension-Biomass-Succession/testings/CoreV8.0-BiomassSuccession7.0-SingleCell/")
system("dotnet $LANDIS_CONSOLE scenario.txt")
