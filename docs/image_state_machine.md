INVALID  → VALID     (on successful update + CRC)
VALID    → ACTIVE    (on bootloader decision)
ACTIVE   → FAILED    (on repeated watchdog resets)
FAILED   → INVALID   (on explicit erase)
