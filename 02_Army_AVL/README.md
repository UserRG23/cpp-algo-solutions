### **Program Interface**

Your task is to implement a class `HobbitArmy` which must contain:

- A method `add` that adds a hobbit to the army. If the army already contains a hobbit with that name or the added hobbit does not have a positive number of life points, the method returns `false` and the army remains unchanged.

- A method `erase` that takes the name of a hobbit and removes him from the army. The return value is the removed hobbit, or an empty `optional` if a hobbit with that name was not found in the army.

- A method `stats` that returns the current stats of the hobbit with the given name, or an empty `optional` if a hobbit with that name was not found in the army.

- A method `for_each` that takes a functor as an argument and calls it on each hobbit in the army in alphabetical order. This method is for reading and it does not modify the army, hence you can pass hobbits by a const refrence.

- A method `enchant` that takes the names of two hobbits `first` and `last` and the values by which to change their properties, and modifies the properties of all hobbits in the army whose names are in the alphabetical range from `first` to `last` (inclusive of both boundaries). If `first` is greater than `last`, the method does nothing and returns `true`.

- If, as a result of the modification, the life points of any hobbit fall to 0 or below, the modification cannot be performed, the method must not modify any hobbit and must return `false`. An efficient implementation of this functionality is difficult, therefore the constant `CHECK_NEGATIVE_HP` is set to `false`.

- A static constant `CHECK_NEGATIVE_HP` of type `bool`. If `true`, tests are also provided where the `enchant` method can return `false`. If `false`, it is guaranteed that the `enchant` method will not have a reason to fail at the cost of a point penalty.
