#!/bin/sh
# Drop OS page/dentry/inode cache to simulate a cold first-launch (needs root).
# Used to validate the fscache cold-cache claim (Phase 5 Step 35).
sync
echo 3 > /proc/sys/vm/drop_caches
echo "caches dropped (cold)"
