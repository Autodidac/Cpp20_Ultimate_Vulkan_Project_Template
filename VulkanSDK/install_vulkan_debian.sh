#!/bin/bash
set -e

# Define the specific Vulkan SDK version and platform.
SDK_VERSION="1.3.275.0"
PLATFORM="linux"
FILE_NAME="vulkansdk-linux-x86_64-${SDK_VERSION}.tar.xz"
DOWNLOAD_URL="https://sdk.lunarg.com/sdk/download/${SDK_VERSION}/${PLATFORM}/${FILE_NAME}?Human=true"

# Get absolute path of current directory (container folder)
CONTAINER_DIR="$(pwd)"

echo "Container directory: ${CONTAINER_DIR}"
echo "Installing Vulkan SDK version ${SDK_VERSION}..."

# Check if the tarball already exists; if not, download it.
if [ -f "${FILE_NAME}" ]; then
  echo "Tarball ${FILE_NAME} already exists. Reusing it."
else
  echo "Downloading from: ${DOWNLOAD_URL}"
  if ! curl -fLo "${FILE_NAME}" "${DOWNLOAD_URL}"; then
      echo "Download failed: Vulkan SDK version ${SDK_VERSION} may not be available." >&2
      exit 1
  fi
fi

# Retrieve the expected SHA hash.
SHA_URL="https://sdk.lunarg.com/sdk/sha/${SDK_VERSION}/${PLATFORM}/${FILE_NAME}.txt"
EXPECTED_SHA=$(curl -fsSL "${SHA_URL}" | awk '{print $1}') || { echo "Failed to fetch SHA."; exit 1; }
echo "Expected SHA: ${EXPECTED_SHA}"

# Compute the SHA of the downloaded file.
DOWNLOADED_SHA=$(sha256sum "${FILE_NAME}" | awk '{print $1}')
echo "Downloaded file SHA: ${DOWNLOADED_SHA}"

if [[ "${EXPECTED_SHA}" != "${DOWNLOADED_SHA}" ]]; then
    echo "SHA verification failed! Expected ${EXPECTED_SHA} but got ${DOWNLOADED_SHA}" >&2
    exit 1
fi

echo "SHA verification succeeded."

# Check if the version folder is already extracted.
if [ -d "${SDK_VERSION}" ]; then
  echo "Vulkan SDK version ${SDK_VERSION} is already extracted."
else
  echo "Extracting Vulkan SDK..."
  if ! tar -xf "${FILE_NAME}"; then
      echo "Extraction failed." >&2
      exit 1
  fi
fi

# Prepare the environment block to be appended to ~/.bashrc.
# Use the absolute path to the SDK folder.
VULKAN_SDK_PATH="${CONTAINER_DIR}/${SDK_VERSION}"
ENV_BLOCK=$(cat <<EOF

# BEGIN VULKAN_SDK_ENVIRONMENT
export VULKAN_SDK="${VULKAN_SDK_PATH}"
export PATH="\$VULKAN_SDK/bin:\$PATH"
export LD_LIBRARY_PATH="\$VULKAN_SDK/lib:\$LD_LIBRARY_PATH"
export VK_ICD_FILENAMES="\$VULKAN_SDK/etc/vulkan/icd.d"
export VK_LAYER_PATH="\$VULKAN_SDK/etc/vulkan/explicit_layer.d"
# END VULKAN_SDK_ENVIRONMENT
EOF
)

BASHRC="$HOME/.bashrc"

# Append the environment block to ~/.bashrc if it's not already present.
if grep -q "BEGIN VULKAN_SDK_ENVIRONMENT" "$BASHRC"; then
  echo "Vulkan SDK environment block already present in $BASHRC."
else
  echo "Creating backup of $BASHRC as ${BASHRC}.bak before appending environment block..."
  cp "$BASHRC" "${BASHRC}.bak"
  echo "Appending Vulkan SDK environment block to $BASHRC..."
  echo "${ENV_BLOCK}" >> "$BASHRC"
  echo "Environment variables added. To activate them in your current session, run: source $BASHRC"
fi

echo "Vulkan SDK version ${SDK_VERSION} installation complete."
echo "SDK installed at: ${VULKAN_SDK_PATH}"
