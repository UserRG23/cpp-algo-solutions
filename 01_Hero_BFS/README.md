### **Program Interface**

Task is to implement the function `find_shortest_path`, which should find a shortest route from any of the entrance rooms (`entrances`) to the treasure (`treasure`) and back to any entrance room such that the hero survives. If no such route exists, it returns an empty vector.
The parameters of `find_shortest_path` are:

- `const std::vector<Room>& rooms` – list of all rooms. The data type Room is a structure containing:

    * `std::vector<RoomId> neighbors` – list of room indices (into the `rooms` array) to which a corridor leads from this room.

    * `std::optional<Monster> monster` – contains a monster if one is present in the room.
    The data type `Monster` contains attributes `int hp, off, def`, describing the monster’s hit points, attack and defense, and also
    `int stacking_off, stacking_def`, which describe how the monster’s attack and defense change after each of its turns.
    When the hero enters a room with a monster, combat starts. Combat ends only when one of the participants dies (i.e. if neither side can hurt the other, both are trapped forever). The monster always attacks first, then the hero, then the monster again, etc. During each attack the target takes damage equal to
    `max(0, a_off - t_def)`, where `a_off` is the attacker’s attack and `t_def` the target’s defense.
    The target dies if its hit points become 0 or less.

    With a reasonably efficient implementation of the remaining parts, the provided `simulate_combat` function is efficient enough for a full score.

    Because corridors are very long, the time spent in combat and item exchange can be ignored; the hero fully heals on the way to the next room and if the hero visits a room again, the monster and all items will be there again.

    * `std::vector<Item> items` – items that are in the room (see structure `Item` below).
    If there is a monster in the room, the hero can pick up items only after defeating it.

- const std::vector<RoomId>& entrances – indices of `rooms` in the rooms array from which the maze can be entered or exited.

- RoomId treasure – index of the room containing the treasure the hero wants to pick up.

The return type is `std::vector<Action>` – a list of actions that starts and ends in an entrance room (though the entrance rooms may differ). The type `Action` is a `std::variant<Move, Pickup, Drop>`. A value of type `Move` means moving to the given room (must be adjacent to the current one). A value of type `Pickup` means picking up an item in the current room (there must be such an item there; if the hero already holds another item of the same type, it is automatically dropped). A value of type `Drop` means dropping the currently held item of the given type.

The data type `Item` contains the attributes:

- `Type` type – the item’s kind. There are 3 kinds: weapons, armor, and rubber ducks. The hero can hold at most one item of each kind at a time. An item can be discarded at any time except during combat; discarding loses the item, and it cannot be picked up again unless another copy appears in a room.

- `std::string name` – the item’s name (not necessarily unique).

- `int hp, off, def` – the values by which the hero’s hit points, attack and defense change when the hero holds the item. These bonuses can be negative. The resulting defense and attack may be negative.
-   If the maximum hit points would become negative, the hero will have 1 hit point.

- `int stacking_off, stacking_def` – the values by which the hero’s attack and defense change after each of his turns. These effects last until the end of the combat.

- `bool first_attack` – if `true`, the hero attacks first, otherwise the monster does.

- `bool stealth` – if `true`, the hero can (but does not have to) sneak through the room, thereby avoiding combat with the monster. If the hero sneaks, he cannot pick up items, including the treasure.

The number of different items in the maze is quite small, but the same item can appear in many rooms. The hero without any items has 10000 hit points, 3 attack, and 2 defense.
