library(odbc)
drivers <- odbc::odbcListDrivers()
any(grepl("MDBTools", drivers$name, ignore.case = TRUE))