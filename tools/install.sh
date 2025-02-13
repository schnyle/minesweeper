#!/bin/bash
cd "$(dirname "$0")/../build"
if ! make; then
  echo "build failed"
  exit 1
fi
cd ..
rm /opt/minesweeper/minesweeper 2>/dev/null || true
sudo mkdir -p /opt/minesweeper && sudo mv build/minesweeper /opt/minesweeper/
ln -sf /opt/minesweeper/minesweeper /usr/local/bin/minesweeper