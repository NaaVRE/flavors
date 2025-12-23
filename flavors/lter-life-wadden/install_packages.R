install.packages("ReacTran", repos="https://cran.r-project.org", dependencies = FALSE)
library("ReacTran")

devtools::install_github('krietsch/tools4watlas@952cb1038c862075bb2df2dfc061e2df87b04340', dependencies = FALSE)
library('tools4watlas')

devtools::install_github('allertbijleveld/SIBES@24d3f06418a8aca77db4e7fbef66c45dc55873f8', dependencies = FALSE)
library('SIBES')

devtools::install_github('TempSED/TempSED@6101ced7f79b300f19999f61f50fefe6099aab76', build_vignettes = TRUE, dependencies = FALSE)
require('TempSED')

devtools::install_github('leonawicz/mapmate@7c36274078c4b6ffbd9dcc278f3176b7535ba86f', dependencies = FALSE)
require('mapmate')
