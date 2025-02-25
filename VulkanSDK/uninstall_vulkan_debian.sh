#!/bin/bash
set -e

# Define the specific Vulkan SDK version.
SDK_VERSION="1.3.275.0"
BASHRC="$HOME/.bashrc"

# Check if the version folder exists in the current directory.
if [ ! -d "${SDK_VERSION}" ]; then
  echo "Vulkan SDK version ${SDK_VERSION} is not present in $(pwd). Nothing to uninstall."
  exit 0
fi

echo "Uninstalling Vulkan SDK version ${SDK_VERSION}..."
rm -rf "${SDK_VERSION}"
echo "Vulkan SDK version ${SDK_VERSION} has been successfully uninstalled."

# Unset VULKAN_SDK in the current shell if it points to the removed folder.
if [ -n "$VULKAN_SDK" ]; then
  if [ "$(realpath "${SDK_VERSION}")" = "$(realpath "$VULKAN_SDK")" ]; then
      unset VULKAN_SDK
      echo "Environment variable VULKAN_SDK has been unset in the current session."
  fi
fi

# Remove the Vulkan SDK environment block from ~/.bashrc.
if grep -q "BEGIN VULKAN_SDK_ENVIRONMENT" "$BASHRC"; then
    echo "Removing Vulkan SDK environment block from $BASHRC..."
    cp "$BASHRC" "${BASHRC}.bak"
    sed -i '/BEGIN VULKAN_SDK_ENVIRONMENT/,/END VULKAN_SDK_ENVIRONMENT/d' "$BASHRC"
    echo "Environment block removed. Backup saved as ${BASHRC}.bak."
else
    echo "No Vulkan SDK environment block found in $BASHRC."
fi

echo "Uninstallation complete."
