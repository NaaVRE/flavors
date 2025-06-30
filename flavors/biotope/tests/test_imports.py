import subprocess

try:
    from osgeo import gdal
except:
    import gdal

subprocess.call(["ogr2ogr"])
subprocess.call(["gdal_rasterize"])
subprocess.call(["/usr/local/lw_apps/lwZonalStatistics"])
