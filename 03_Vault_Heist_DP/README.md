### **Formal Assignment**

Your task is to implement the function `plan_heist`, which determines which of the given vaults to rob in order to maximize profit while not exceeding the total time spent in the vault room. The return value is a list of vaults to be looted identified by their indices in the input, and for each of them information on how to unlock it (a vector containing `true` for each pebble that must be moved). The arguments are:

- An array `vaults` describing individual vaults. Each vault is represented by an instance of the data type `Vault`, which contains:
    * A non-negative integer `value` – the value of the items in the vault.
    * A non-negative integer `pebbles` – the number of pebbles (stones) in the vault’s lock.
    * An array of pairs `missing_connections` – the list of missing connections between pebbles in the lock (because the missing connections are significantly fewer than the present ones).

    Initially all pebbles of the lock are on the left side of the vault door. To unlock the vault it is necessary to move exactly half of the pebbles to the right side so that every pebble on the right is connected to every pebble on the left. The time to unlock a vault is the number of moved pebbles. The time to actually loot it is negligible. Some vaults might be broken and their locks cannot be unlocked.

- A non-negative integer `transition_time` – the time to move from one vault to another (this time is the same for every pair of vaults).

- A non-negative integer `max_time` – the maximum time you can remain in the room with the vaults. This time is not too big.

All computations should fit in the range of type `int`.
