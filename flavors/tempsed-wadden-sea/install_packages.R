install.packages("ReacTran", repos="https://cran.r-project.org", dependencies = FALSE)
library("ReacTran")

install.packages("grDevices", repos="https://cran.r-project.org", dependencies = FALSE)
library("grDevices")

install.packages("heatwaveR", repos="https://cran.r-project.org", dependencies = FALSE)
library("heatwaveR")

install.packages("tidyterra", repos="https://cran.r-project.org", dependencies = FALSE)
library("tidyterra")

devtools::install_github('TempSED/TempSED', build_vignettes = TRUE, dependencies = FALSE)
require('TempSED')

pak::pkg_install("LTER-LIFE/dtR/dtRtools")
library("dtRtools")

pak::pkg_install("LTER-LIFE/dtR/dtRwad")
library("dtRwad")

pak::pkg_install("LTER-LIFE/dtR/dtRprimprod")
library("dtRprimprod")