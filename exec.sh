#!/bin/bash

set -e

# Create external directory
mkdir -p external
cd external

echo "🔽 Downloading stb_image.h..."
wget -O stb_image.h https://raw.githubusercontent.com/nothings/stb/master/stb_image.h

echo "🔽 Downloading Vulkan SDK (glslc)..."
VULKAN_VERSION="1.3.275.0"
VULKAN_FILE="vulkan-sdk-linux-x86_64-${VULKAN_VERSION}.tar.gz"
VULKAN_URL="https://sdk.lunarg.com/sdk/download/${VULKAN_VERSION}/linux/${VULKAN_FILE}?Human=true"

wget -O ${VULKAN_FILE} "${VULKAN_URL}"

echo "📦 Extracting Vulkan SDK..."
tar -xf ${VULKAN_FILE}
rm ${VULKAN_FILE}

echo "✅ Everything downloaded into ./external"
