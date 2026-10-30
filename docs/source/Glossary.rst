Glossary
========
- **bounding volume**: a closed volume of a specific kind that contains a given
  object (*e.g.*, box, sphere, k-DOP).

- **BVH**: Bounding Volume Hierarchy, a tree structure on a set of geometric
  objects to accelerate a spatial search. All geometric objects are wrapped in
  bounding volumes that form the leaf nodes of the tree. An alternative to
  *k*-d tree.

- **callback**: a user-defined function executed upon finding a primitive that satisfies a predicate.
- **predicate**: an object carrying information about the search task.
  Typically combines a geometric object (that is used to identify close objects)
  with additional data (such as a radius or a number of nearest neighbors).
  A *spatial predicate* returns true or false when acted upon a bounding
  volume (*e.g., predicate based on checking intersection of a sphere with a bounding
  volume, used for radius-based search).
  A *nearest predicate* searches for a given number of nearest neighbors.

- **primitive**: a geometric object that is boundable, *i.e.* that can be
  contained within a bounding volume (*e.g.*, triangle, sphere, point, box).
