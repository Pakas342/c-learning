# Retro Game Inventory Manager

You’re designing an 8-bit-style Role Playing Game where memory is scarce.
Your character can carry 8 unique items(sword, shield, potion, key, pick axe,
wand, sandwich, juice) and has 4 status effects (poisoned, burning, frozen,
cursed). Use bit encodings to represent these in just TWO byte (2 \* 8 bits) to
save memory!

Requirements:

- Bit Encodings: Define bit encodings to pack:
  - 4 status flags (1 bit each: e.g., is_poisoned, is_burning).
  - 0-15 health points (represented with 4 consecutive bits).
  - 8 inventory slots (1 bit each: 1 = item is present, 0 = absent).

- Functions:
  - add_item(): Set an inventory bit (e.g., sword = bit 0).
  - apply_status(): Set a status flag (e.g., poisoned = bit 4).
  - use_potion(): Decrement the 4-bit health counter (handle underflow!).
  - print_status(): Show inventory, status, and potions in a human-readable format.

- Constraints:
  - Total character size must be 2 bytes (use sizeof() to verify).
  - Handle invalid inputs (e.g., trying to add item 9).
