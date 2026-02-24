# Gulam script to automate building and running the raster driver
echo "====================================="
echo " Cleaning old build..."
echo "====================================="

# Remove the existing executable if it is already made
rm rastdrv.tos

echo "====================================="
echo " Building Raster Driver..."
echo "====================================="

# Call the makefile
make rastdrv.tos

echo "====================================="
echo " Executing rastdrv.tos..."
echo "====================================="

# Run the executable
rastdrv.tos