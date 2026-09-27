# BST vs Linear Search — Government ID Database

**Course:** Data Structures Lab | **Language:** C

## Problem
Organise `A102, A25, A7, B100, B12, A120, B3, A45` in a BST (keys compared as strings via `strcmp`), analyse structure, and compare BST search vs linear search.

## Files
| File | Purpose |
|---|---|
| `bst_id_search.c` | Source code |
| `input.txt` | Input IDs |
| `output.txt` | Program output |

## a) BST Construction & Structure

**Insertion order:** A102, A25, A7, B100, B12, A120, B3, A45

```
                A102
                   \
                    A25
                   /    \
               A120      A7
                        /    \
                     A45      B100
                                  \
                                  B12
                                     \
                                     B3
```

**Inorder traversal:** `A102 A120 A25 A45 A7 B100 B12 B3` — matches ascending string order, confirming BST correctness.

**Insertion trace table:**
| Step | Key | Nodes compared | Comparisons | Placed as |
|---|---|---|---|---|
| 1 | A102 | — | 0 | root |
| 2 | A25 | A102 | 1 | right of A102 |
| 3 | A7 | A102, A25 | 2 | right of A25 |
| 4 | B100 | A102, A25, A7 | 3 | right of A7 |
| 5 | B12 | A102, A25, A7, B100 | 4 | right of B100 |
| 6 | A120 | A102, A25 | 2 | left of A25 |
| 7 | B3 | A102, A25, A7, B100, B12 | 5 | right of B12 |
| 8 | A45 | A102, A25, A7 | 3 | left of A7 |

**Structure analysis:** Height = 5 edges (6 levels) for 8 nodes, vs. an ideal balanced height of 3. The tree is right-skewed overall because most IDs arrive in near-increasing order; A120 and A45 are the only nodes that branch left, giving partial balance.

## b) BST Search vs Linear Search

| Key | BST comparisons | Linear comparisons | Result |
|---|---|---|---|
| A102 | 1 | 1 | found |
| B3 | 6 | 7 | found |
| A45 | 4 | 8 | found |
| C1 | 6 | 8 | not found |

**Search trace (BST, key = B3):** A102 → A25 → A7 → B100 → B12 → B3 (6 comparisons)
**Search trace (Linear, key = B3):** scans `input.txt` order, index 0–6 (7 comparisons)

BST search wins in every case here, and the gap widens for keys deeper in the array (A45, C1) since linear search must still walk sequentially from the start.

## c) Effect of Key Length & Insertion Order

- **Key length:** since keys are compared with `strcmp`, a longer prefix match (e.g. `A1` vs `A102` vs `A120`) needs more character comparisons per node, but this only affects *constant-factor* cost per comparison, not the number of node visits — tree height still depends on insertion order, not key length.
- **Insertion order:** inserting the same 8 keys in fully sorted order (`A102, A120, A25, A45, A7, B100, B12, B3`) produces a purely right-skewed tree:

  `Height = 7 edges`, and searching for `B3` now costs `8` comparisons — identical to linear search.

| Insertion order | Height | Search cost for B3 |
|---|---|---|
| Given (mixed) order | 5 | 6 |
| Fully sorted order | 7 (worst case) | 8 |
| Theoretical best (balanced) | 3 | ≤4 |

**Theoretical vs observed:**
| Case | Height | Search |
|---|---|---|
| Best case (balanced BST, n=8) | ⌊log₂n⌋ = 3 | O(log n) |
| Worst case (skewed BST) | n−1 = 7 | O(n) |
| Observed (given order) | 5 | between best/worst |
| Observed (sorted order) | 7 | matches worst case |

This confirms: an unbalanced BST degrades to O(n) — no better than linear search — whenever insertion order is close to sorted, which is a realistic risk for an ID database where new IDs are often issued in increasing sequence.

## Suggested Approach for a Growing Database
A plain BST is unsafe here since near-sorted insertions (common for allocated IDs) drive it toward O(n). Use a **self-balancing tree (AVL or Red-Black tree)**, which rebalances on every insertion and guarantees O(log n) height regardless of insertion order; for very large, disk-resident databases, a **B-Tree** (used by real DB indexes) is preferable since it also minimises disk reads.

## Complexity Summary
| Operation | BST (balanced) | BST (worst case) | Linear search |
|---|---|---|---|
| Search | O(log n) | O(n) | O(n) |
| Insert | O(log n) | O(n) | O(1) (unordered append) |
| Space | O(n) | O(n) | O(n) |

## Conclusion
The BST outperforms linear search on this dataset, but its advantage depends entirely on staying balanced. With the given insertion order it already loses over half its theoretical efficiency (height 5 vs ideal 3), and with sorted insertion it collapses to linear-search performance. For a production ID database, a self-balancing tree or B-Tree is the appropriate structure to keep search efficient as the database grows.
