*This project has been created as part of the 42 curriculum by ravazque and acerezo-.*

---

# lem-in

lem-in is a digital ant farm written in **C**. It reads a colony on standard
input — ants, rooms and tunnels — and prints the movements that take every ant
from `##start` to `##end` in as few turns as possible, knowing that no room
except the start and the end may hold more than one ant at a time.

## Layout

```
.
├── docs/       README.md                   # this document
├── include/    lem_in.h                    # types, constants, prototypes
├── lib/        libft                       # the only helper library used
├── maps/       valid/ · invalid/           # test colonies
└── srcs/
    ├── main.c                              # parse → build → solve → simulate
    ├── parsing/     read, validate, store the colony
    ├── graph/       adjacency lists, node splitting, room lookup
    ├── solver/      max flow, route extraction, route and ant selection
    ├── simulation/  turn-by-turn output
    └── utils/       memory cleanup
```

Build with `make`, run with `./lem-in < map`, clean with `make fclean`. The
program takes no arguments: the colony comes in on stdin, the simulation goes
out on stdout, and any error prints `ERROR` on stderr with exit status 1.

## Concepts

| Term | Meaning in this project |
|------|-------------------------|
| Colony | The whole input: ant count, rooms, tunnels, `##start` and `##end` |
| Room | A graph node; holds at most one ant, except the start and the end |
| Tunnel | An undirected edge between two rooms, usable by one ant per turn |
| Turn | One step in which every ant may move to an adjacent free room |
| Route | A room sequence from start to end taken by a block of ants |
| Node splitting | Room → `in`/`out` pair joined by a capacity-1 edge |
| Residual graph | The directed graph max flow works on: forward and reverse edges |
| Max flow | Number of routes that share no room, found by Edmonds-Karp |

## The problem

A single shortest path is not the answer. Because a room holds one ant, ants
queued behind each other on one route arrive one per turn, so a colony with
`n` ants and a shortest route of length `len` needs `len + n - 1` turns that
way. Spreading ants over several routes that share no room lets them advance in
parallel, which is almost always faster — but only up to a point, since a route
much longer than the shortest one can delay the very last ant.

So the program has to answer two questions: **which** sets of room-disjoint
routes exist, and **how many** of them are worth using.

## How it works

**Parsing.** The whole of stdin is read into one growable buffer and cut into
lines in place, so there is no syscall per line. The first line must be a
positive integer, the ant count. `##start` and `##end` mark the next room to be
declared and may appear only once each; any other `#…` line is a comment. A
room is `name x y` with exactly three fields, integer coordinates, and a name
that starts with neither `L` nor `#`. A tunnel is `a-b` between two different,
already declared rooms. Names are resolved through a djb2 hash table with
chaining, so a link never scans the room array. Every accepted line is also
duplicated into an echo buffer, because the map has to be reprinted before the
moves.

**Node splitting.** Max flow limits edges, not nodes, so each room `i` is split
into `in = 2i` and `out = 2i+1` joined by an edge of capacity 1. A tunnel `a-b`
becomes `out(a) → in(b)` and `out(b) → in(a)`, both capacity 1. Any unit of flow
crossing a room saturates that room's internal edge, which is exactly the
one-ant-per-room rule. Flow runs from `out(start)` to `in(end)`, so the internal
edges of the start and end rooms are never on the path and never constrain them.

**Max flow.** Edmonds-Karp: BFS the residual graph, push one unit along the
shortest augmenting path found, repeat until the sink is unreachable. Each edge
carries its residual reverse (capacity 0, linked through `->rev`), which is what
lets a later augmentation undo an earlier bad choice. With unit capacities the
final flow value is exactly the number of room-disjoint routes; a flow of 0
means start and end are disconnected, which is an error.

**Route extraction.** The flow is a set of saturated edges, not a list of
routes, so it has to be decomposed. Starting from `out(start)`, the walk follows
any forward edge whose capacity dropped to 0, restoring it as it goes so the
next walk finds a different route, and stops at `in(end)`. Reverse edges are
flagged so they are never mistaken for flow. Each walk yields one route, stored
as room ids with the `in`/`out` duplicates collapsed.

**Route and ant selection.** Routes are sorted by length, shortest first. A
route of length `len` delivers `t - len + 1` ants by turn `t`, so the turns
needed by the `k` shortest routes are the smallest `t` whose total capacity
reaches the ant count — found by binary search. Every `k` from 1 to the max flow
is evaluated and the one with the fewest turns is kept; the rest are freed. The
ants are then handed out one at a time, each joining the route on which it would
arrive earliest (its length plus the ants already queued on it).

**Simulation.** Each route receives a contiguous block of ant ids. Ant `j`
(0-based within its route) leaves on turn `j+1` and on turn `t` stands in
`rooms[t - j]`, so a turn is emitted by walking the routes and printing every
ant currently between the start and the end. Everything — the echoed map, the
blank line, all the turns — is appended to a growable buffer and sent out with a
single `write`.

## What gets rejected

| Case | Example map |
|------|-------------|
| Ant count missing, zero, negative or not a number | `ants_*.map`, `no_ants.map` |
| Empty input | `empty.map` |
| Room with too few or too many fields | `room_missing_field.map`, `room_extra_field.map` |
| Non-integer coordinates | `coords_not_int.map` |
| Room name starting with `L` or `#` | `room_name_starts_L.map`, `room_name_starts_hash.map` |
| Duplicate room name | `dup_room.map` |
| Two `##start` or two `##end` | `dup_start.map`, `dup_end.map` |
| No `##start` or no `##end` | `no_start.map`, `no_end.map` |
| Tunnel naming an undeclared room | `link_unknown_room.map` |
| Tunnel from a room to itself | `self_link.map` |
| Valid colony with no route between start and end | `no_path.map` |

## Complexity

With `R` rooms, `T` tunnels and `A` ants, the split graph has `V = 2R` nodes and
`E = 2R + 4T` directed edges counting the residual reverses.

| Stage | Cost |
|-------|------|
| Parsing | `O(R + T)` amortised, hash lookups in `O(1)` |
| Graph construction | `O(R + T)` |
| Max flow | `O(F * (V + E))`, `F` bounded by the degree of start and end |
| Route extraction | `O(V + E)` |
| Route selection | `O(F * F * log A)` — one binary search per candidate count |
| Ant distribution | `O(A * F)` |
| Simulation | `O(turns * A)` output tokens, one `write` |

## Summary

- **Parsing** — one read, in-place line splitting, strict validation, hash table
  for name lookup, and the input kept for the echo.
- **Modelling** — every room split into `in`/`out` with a capacity-1 edge, which
  turns "one ant per room" into a plain edge capacity.
- **Solving** — Edmonds-Karp max flow gives the room-disjoint routes; the flow
  is then decomposed into explicit room sequences.
- **Optimising** — the route count and the ant distribution are chosen to
  minimise the turn on which the last ant arrives.
- **Output** — the map, a blank line, then one line of `Lx-room` tokens per
  turn, flushed in a single write.

## References

Algorithms:

- Maximum flow problem — <https://en.wikipedia.org/wiki/Maximum_flow_problem>
- Edmonds-Karp algorithm — <https://en.wikipedia.org/wiki/Edmonds%E2%80%93Karp_algorithm>
- Ford-Fulkerson method — <https://en.wikipedia.org/wiki/Ford%E2%80%93Fulkerson_algorithm>
- Max-flow min-cut theorem — <https://en.wikipedia.org/wiki/Max-flow_min-cut_theorem>
- Menger's theorem (disjoint paths and connectivity) —
  <https://en.wikipedia.org/wiki/Menger%27s_theorem>
- Vertex capacities by node splitting —
  <https://cp-algorithms.com/graph/edmonds_karp.html>
- Breadth-first search — <https://en.wikipedia.org/wiki/Breadth-first_search>
- Suurballe's algorithm, the shortest-pair variant of the same idea —
  <https://en.wikipedia.org/wiki/Suurballe%27s_algorithm>
