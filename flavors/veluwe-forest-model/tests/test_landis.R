file.create("empty_file.txt")
ret <- system("dotnet $LANDIS_CONSOLE empty_file.txt", intern = TRUE)
# Check that the return code indicates that the scenario file is invalid
if (!grepl("Expected the name", ret[5])) {
  quit(status = ret)
}