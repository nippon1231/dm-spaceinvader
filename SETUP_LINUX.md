# Linux Setup Guide - Galaga Mega Drive

This guide explains how to set up the Sega Mega Drive/Genesis development environment on Linux for building the Galaga project.

## Quick Start

```bash
# 1. Run the setup script (installs Gendev)
chmod +x setup-linux.sh
./setup-linux.sh

# 2. Reload environment variables
source ~/.bashrc

# 3. Make build script executable
chmod +x build-linux.sh

# 4. Build the ROM
./build-linux.sh

# 5. Test with an emulator
# The ROM file is: galaga.bin
```

## What is Gendev?

[Gendev](https://github.com/kubilus1/gendev) is a complete development environment for Sega Genesis/Mega Drive on Linux. It includes:
- **SGDK** (Sega Genesis Development Kit) - The main development framework
- **m68k-elf-gcc** - Cross-compiler for Motorola 68000 (Genesis CPU)
- **rescomp** - Resource compiler for sprites, tiles, music, etc.
- All necessary build tools and libraries

## Prerequisites

### Required Packages

The setup script will install these automatically, but you can also install them manually:

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    git \
    wget \
    openjdk-11-jre-headless \
    texinfo
```

## Manual Installation

If the automatic setup script fails, you can install Gendev manually:

### Option 1: Install from Release Package (.deb)

```bash
# Download the latest release
wget https://github.com/kubilus1/gendev/releases/download/1.0.0/gendev_1.0.0_amd64.deb

# Install
sudo dpkg -i gendev_1.0.0_amd64.deb
sudo apt-get install -f  # Fix any dependency issues
```

### Option 2: Build from Source

```bash
# Clone the repository
git clone https://github.com/kubilus1/gendev.git
cd gendev

# Build and install
make
sudo make install
```

### Configure Environment

Add to your `~/.bashrc`:

```bash
export GENDEV=/opt/gendev
export PATH=$PATH:$GENDEV/bin
```

Then reload:

```bash
source ~/.bashrc
```

## Building the Project

### Using the Build Script (Recommended)

```bash
./build-linux.sh
```

### Using Make (Alternative)

The project includes a Makefile that works with SGDK/Gendev:

```bash
make
```

The ROM will be generated as `out.bin`.

### Build Process Explained

The build process consists of these steps:

1. **Resource Compilation**: `rescomp` converts PNG sprites and other resources to Genesis-compatible format
2. **Resource Assembly**: Assembled into object code for the 68000
3. **C Compilation**: Source files compiled with `m68k-elf-gcc`
4. **Linking**: All object files linked with SGDK libraries
5. **Binary Creation**: Final ROM binary created from ELF output

## Project Structure

```
dm-spaceinvader/
├── src/                    # C source files
│   ├── main.c             # Entry point
│   ├── game.c             # Game logic
│   ├── player.c           # Player management
│   ├── enemies.c          # Enemy management
│   └── bullets.c          # Bullet/projectile management
├── inc/                   # Header files
│   └── game.h            # Main declarations
├── res/                   # Resources
│   ├── resources.res     # Resource definitions for SGDK
│   ├── sprite/           # PNG sprites
│   │   ├── player.png
│   │   ├── enemy.png
│   │   └── bullet.png
│   └── tileset/          # Background tiles
│       └── stars.png
├── out/                   # Build output directory
├── build-linux.sh        # Linux build script
├── setup-linux.sh        # Linux setup script
└── Makefile              # Make build configuration
```

## Testing the ROM

### Recommended Emulators for Linux

1. **BlastEm** (Most accurate)
   ```bash
   sudo apt-get install blastem
   blastem galaga.bin
   ```

2. **Gens/GS** (Development-friendly)
   ```bash
   sudo apt-get install gens-gs
   gens galaga.bin
   ```

3. **RetroArch** (Multi-system)
   ```bash
   sudo apt-get install retroarch libretro-genesis-plus-gx
   retroarch -L /usr/lib/libretro/genesis_plus_gx_libretro.so galaga.bin
   ```

## Cleaning Build Artifacts

```bash
# Using make
make clean

# Or manually
rm -rf out/*.o out/*.elf out/*.bin res/resources.s res/resources.h
```

## Troubleshooting

### "GENDEV environment variable not set"

**Solution**: Ensure you've run the setup script and reloaded your environment:
```bash
source ~/.bashrc
echo $GENDEV  # Should show /opt/gendev
```

### "Java is not installed"

**Solution**: Install Java Runtime Environment:
```bash
sudo apt-get install openjdk-11-jre-headless
```

### "m68k-elf-gcc: command not found"

**Solution**: Gendev is not properly installed or not in PATH. Reinstall Gendev:
```bash
./setup-linux.sh
```

### "rescomp failed"

**Solution**:
- Check that PNG sprites are valid and properly formatted
- Ensure Java is installed and working: `java -version`
- Verify `res/resources.res` file is correct

### Build succeeds but ROM doesn't work

**Solution**:
- Try different emulators (BlastEm is most accurate)
- Check ROM file size (should be > 0 bytes)
- Verify the ROM header is correct

## Alternative: Using Docker

If you prefer not to install Gendev directly, you can use Docker:

```bash
# Pull a Genesis development image
docker pull kubilus1/gendev

# Build the project
docker run --rm -v $(pwd):/src kubilus1/gendev make
```

## Resources

- **Gendev GitHub**: https://github.com/kubilus1/gendev
- **SGDK Documentation**: https://github.com/Stephane-D/SGDK/wiki
- **SGDK Tutorials**: https://www.ohsat.com/tutorial/
- **Genesis/Mega Drive Specs**: http://md.squee.co/

## Controls

- **D-Pad Left/Right**: Move ship
- **A Button**: Fire
- **Start**: Restart after Game Over

## Development Tips

1. **Fast Iteration**: Use `make` for quick rebuilds (only recompiles changed files)
2. **Debugging**: Use the `symbol.txt` file generated in `out/` for debugging
3. **Resource Changes**: If you modify sprites, run `make clean` before rebuilding
4. **Emulator**: BlastEm has good debugging features for development

## License

Educational example project. Code and sprites are free to use.
