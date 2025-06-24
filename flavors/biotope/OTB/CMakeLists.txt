cmake_minimum_required(VERSION 2.4)
cmake_policy(SET CMP0003 NEW)
PROJECT(lifewatchIJI)
INCLUDE_REGULAR_EXPRESSION("^.*$")
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-undefined -Wl,--as-needed" CACHE INTERNAL "" FORCE)

FIND_PACKAGE(OTB)
IF(OTB_FOUND)
  INCLUDE(${OTB_USE_FILE})
ELSE(OTB_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build intensity applications without OTB.  Please set OTB_DIR.")
ENDIF(OTB_FOUND)


#Help is available for all executable if you launch it without arguments. 

##
# WARNING: all tools require that the input images occupy the same physical space (extent and pixel size).
##


#Tabulate is equivalent to the ArcGIS tabulate Area tool and works for large raster. Two input rasters (integers) are crossed and a csv file is filled with the number of pixels within each cell of the double-entry table. 
ADD_EXECUTABLE(Tabulate TabulateFilter.cxx )
TARGET_LINK_LIBRARIES(Tabulate ${OTB_LIBRARIES})

#lwLookup apply a look up (crosswalk) table in csv format to alter the values of a raster 
ADD_EXECUTABLE(lwLookUp LookUp.cxx )
TARGET_LINK_LIBRARIES(lwLookUp  ${OTB_LIBRARIES})

#lwLookupAggregate applies a look up (crosswalk) table in csv format to alter the values of a raster AND cumulates the values in case of dupliate entries in the csv keys. 
ADD_EXECUTABLE(lwLookUpAggregate LookUpAggregate.cxx )
TARGET_LINK_LIBRARIES(lwLookUpAggregate  ${OTB_LIBRARIES})


#lwZonalFilter computes the statistics per region for a given quantitative geographic layer
ADD_EXECUTABLE(lwZonalStatistics ZonalFilter.cxx )
TARGET_LINK_LIBRARIES(lwZonalStatistics  ${OTB_LIBRARIES})


INSTALL(TARGETS Tabulate lwLookUp lwLookUpAggregate lwZonalStatistics 
        RUNTIME DESTINATION bin/projects/lifewatch/iji)

#
