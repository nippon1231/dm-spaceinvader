#!/bin/bash

# Setup script for Galaga Mega Drive development on Linux
# This script installs Gendev (Genesis Development Kit for Linux)

set -e

echo "============================================"
echo "Galaga Mega Drive - Linux Setup"
echo "============================================"
echo ""

# Check if running as root for apt operations
if [ "$EUID" -ne 0 ]; then
    echo "Note: This script may require sudo for installing dependencies"
    echo ""
fi

# Install required dependencies
echo "[1/4] Installing build dependencies..."
sudo apt-get update
sudo apt-get install -y build-essential git wget openjdk-11-jre-headless texinfo

echo "[2/4] Downloading Gendev..."
GENDEV_VERSION="1.0.0"
INSTALL_DIR="/opt/gendev"

# Create temporary directory
TMP_DIR=$(mktemp -d)
cd "$TMP_DIR"

# Download Gendev
echo "Downloading Gendev from GitHub..."
wget https://github.com/kubilus1/gendev/releases/download/${GENDEV_VERSION}/gendev_${GENDEV_VERSION}_amd64.deb -O gendev.deb || {
    echo "Failed to download .deb package, trying to build from source..."

    # Clone and build from source
    git clone https://github.com/kubilus1/gendev.git
    cd gendev

    echo "[3/4] Building Gendev from source..."
    make

    echo "[4/4] Installing Gendev..."
    sudo make install

    INSTALL_DIR="/opt/gendev"
}

# Install from .deb if download succeeded
if [ -f gendev.deb ]; then
    echo "[3/4] Installing Gendev package..."
    sudo dpkg -i gendev.deb || {
        echo "Resolving dependencies..."
        sudo apt-get install -f -y
    }
fi

# Cleanup
cd /
rm -rf "$TMP_DIR"

echo ""
echo "============================================"
echo "Setting up environment variables..."
echo "============================================"

# Add GENDEV to .bashrc if not already there
if ! grep -q "export GENDEV=" ~/.bashrc; then
    echo "" >> ~/.bashrc
    echo "# Gendev - Sega Genesis Development Kit" >> ~/.bashrc
    echo "export GENDEV=${INSTALL_DIR}" >> ~/.bashrc
    echo "export PATH=\$PATH:\$GENDEV/bin" >> ~/.bashrc
    echo "Added GENDEV environment variable to ~/.bashrc"
else
    echo "GENDEV already configured in ~/.bashrc"
fi

# Also set for current session
export GENDEV=${INSTALL_DIR}
export PATH=$PATH:$GENDEV/bin

echo ""
echo "============================================"
echo "Installation complete!"
echo "============================================"
echo ""
echo "GENDEV installed at: ${INSTALL_DIR}"
echo ""
echo "IMPORTANT: Run the following command to update your current session:"
echo "  source ~/.bashrc"
echo ""
echo "Or close and reopen your terminal."
echo ""
echo "To build the project, run:"
echo "  ./build-linux.sh"
echo ""
echo "============================================"
