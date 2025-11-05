#!/bin/bash

# Build script for Galaga Mega Drive on Linux using Gendev

set -e

echo "============================================"
echo "Building Galaga for Mega Drive (Linux)"
echo "============================================"
echo ""

# Check for GENDEV environment variable
if [ -z "$GENDEV" ]; then
    echo "ERROR: GENDEV environment variable not set!"
    echo ""
    echo "Please install Gendev first:"
    echo "  ./setup-linux.sh"
    echo ""
    echo "Then source your bashrc:"
    echo "  source ~/.bashrc"
    exit 1
fi

echo "GENDEV: $GENDEV"
echo ""

# Check if Java is installed
if ! command -v java &> /dev/null; then
    echo "ERROR: Java is not installed!"
    echo "Install Java Runtime Environment:"
    echo "  sudo apt-get install openjdk-11-jre-headless"
    exit 1
fi

# Create output directories
echo "[1/5] Creating output directories..."
mkdir -p out/src out/res

# Compile resources with rescomp
echo "[2/5] Compiling resources with rescomp..."
if [ -f "$GENDEV/bin/rescomp" ]; then
    # Use rescomp binary if available
    $GENDEV/bin/rescomp res/resources.res out/res/resources.s
elif [ -f "$GENDEV/bin/rescomp.jar" ]; then
    # Use jar version
    java -jar $GENDEV/bin/rescomp.jar res/resources.res out/res/resources.s
else
    echo "ERROR: rescomp not found in $GENDEV/bin"
    exit 1
fi
echo "  OK - resources.s generated"

# Assemble resources
echo "[3/5] Assembling resources..."
m68k-elf-as -m68000 --register-prefix-optional out/res/resources.s -o out/res/resources.o
echo "  OK - resources.o created"

# Compile C files
echo "[4/5] Compiling C files..."
CFLAGS="-m68000 -Wall -Wextra -O3 -fno-builtin -fomit-frame-pointer"
INCLUDES="-I. -I$GENDEV/include -Iinc -Ires"

for file in src/*.c; do
    basename=$(basename "$file" .c)
    echo "  Compiling: $basename.c"
    m68k-elf-gcc $CFLAGS $INCLUDES -c "$file" -o "out/src/$basename.o"
done
echo "  OK - All C files compiled"

# Link
echo "[5/5] Linking..."
m68k-elf-gcc -m68000 -T $GENDEV/ldscripts/sgdk.ld -nostdlib \
    out/src/*.o out/res/resources.o \
    $GENDEV/lib/libmd.a \
    -lgcc -o out/rom.out
echo "  OK - rom.out created"

# Create binary ROM
echo "[6/6] Creating ROM binary..."
m68k-elf-objcopy -O binary out/rom.out out/rom.bin

# Copy to main directory
cp out/rom.bin galaga.bin

# Generate symbol file (optional, for debugging)
m68k-elf-nm -n out/rom.out > out/symbol.txt

echo ""
echo "============================================"
echo "  BUILD SUCCESSFUL!"
echo "============================================"
echo "ROM created: out/rom.bin"
echo "Copy       : galaga.bin"
echo ""
echo "Test with a Mega Drive emulator:"
echo "  - Gens/GS"
echo "  - BlastEm"
echo "  - RetroArch (Genesis Plus GX core)"
echo "============================================"
echo ""
