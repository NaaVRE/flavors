install.packages("ReacTran", repos="https://cran.r-project.org", dependencies = FALSE)
library("ReacTran")

install.packages("tidyterra", repos="https://cran.r-project.org", dependencies = FALSE)
library("tidyterra")

devtools::install_github('TempSED/TempSED@6101ced7f79b300f19999f61f50fefe6099aab76', build_vignettes = TRUE, dependencies = FALSE)
require('TempSED')

install.packages("grDevices", repos="https://cran.r-project.org", dependencies = FALSE)
library("grDevices")

install.packages("heatwaveR", repos="https://cran.r-project.org", dependencies = FALSE)
library("heatwaveR")

devtools::install_github("LTER-LIFE/dtR/dtRtools")
library("dtRtools")

devtools::install_github("LTER-LIFE/dtR/dtRwad")
library("dtRwad")

devtools::install_github("LTER-LIFE/dtR/dtRprimprod")
library("dtRprimprod")