devtools::install_github('krietsch/tools4watlas@952cb1038c862075bb2df2dfc061e2df87b04340', dependencies = FALSE)
library('tools4watlas')

devtools::install_github('allertbijleveld/SIBES@24d3f06418a8aca77db4e7fbef66c45dc55873f8', dependencies = FALSE)
library('SIBES')

devtools::install_github('TempSED/TempSED@1a29a181b660ac6b2e3f038fcdc1c659e0f0962c', depend = TRUE)
require('TempSED')

devtools::install_github('leonawicz/mapmate@7c36274078c4b6ffbd9dcc278f3176b7535ba86f', dependencies = FALSE)
require('mapmate')
