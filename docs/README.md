*This project has been created as part of the 42 curriculum by ravazque and acerezo-.*

---

# lem-in

lem-in is a digital ant farm written in **C**. It reads a colony on standard
input — ants, rooms and tunnels — and prints the movements that take every ant
from `##start` to `##end` in as few turns as possible, knowing that no room
except the start and the end may hold more than one ant at a time.

## Layout

```
lem-in/
│
├── visualizer.html                         # bonus: the visualizer page
├── srcs/bonus/                             # bonus: ./lem-in < map | ./visu
├── include/                                # types, constants, prototypes
├── lib/               libft                # the only helper library used
├── maps/              valid/ & invalid/    # test colonies
│
└── srcs/
    ├── utils/         memory cleanup
    ├── parsing/       read, validate, store the colony
    ├── graph/         adjacency lists, node splitting, room lookup
    ├── solver/        min-cost max flow, route extraction, route and ant selection
    ├── simulation/    turn-by-turn output
    │
    └── main.c                              # parse → build → solve → simulate
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
| Max flow | Number of routes that share no room, found by min-cost max flow |

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

**Min-cost max flow.** Every forward edge costs `+1` and its residual reverse
costs `-1`, so undoing an earlier choice refunds its length. Each pass pushes
one unit along the *cheapest* augmenting path, which keeps the total length of
the routes minimal for that flow value. The search is Dijkstra over the residual
graph with Johnson potentials (`pot[v]` shifted by the previous distance) so the
negative reverse costs become non-negative, and a bucket queue settles nodes in
`O(1)`. With unit capacities the final flow value is exactly the number of
room-disjoint routes; a flow of 0 means start and end are disconnected, which is
an error.

**Route extraction.** The flow is a set of saturated edges, not a list of
routes, so it has to be decomposed. One route is walked per saturated edge
leaving `out(start)`, a cursor making sure no edge is taken twice; from there
each node has a single outgoing unit of flow, so the walk just follows the
saturated forward edges to `in(end)`. Rooms taken by an earlier route are marked
so a cycle can never trap the walk. Each walk yields one route, stored as room
ids with the `in`/`out` duplicates collapsed.

**Route and ant selection.** Routes are sorted by length, shortest first. A
route of length `len` delivers `t - len + 1` ants by turn `t`, so the turns
needed by the `k` shortest routes are the smallest `t` whose total capacity
reaches the ant count — found by binary search. Every `k` from 1 to the max flow
is evaluated and the one with the fewest turns is kept; the rest are freed. A
`k` that cannot beat the best turn count already found is never decomposed. The
ants are then spread by filling every route up to that turn limit and dropping
the surplus from the longest ones.

**Simulation.** Each route receives a contiguous block of ant ids. Ant `j`
(0-based within its route) leaves on turn `j+1` and on turn `t` stands in
`rooms[t - j]`, so a turn is emitted by walking the routes and printing every
ant currently between the start and the end. Everything — the echoed map, the
blank line, all the turns — goes through a 64 KB buffer flushed whenever it
fills, so memory stays flat however long the simulation runs.

## Visualizer

Built with `make bonus` and used as the subject describes it,
`./lem-in < map | ./visu`, which injects the simulation into `visualizer.html`
and writes a self-contained `lem-in-visu.html`.

The page draws one lane per route &mdash; start on the left, end on the right,
a dot per room &mdash; and animates every ant turn by turn, which is what makes
the flow legible: short routes saturate, long ones carry a single wave. Room
coordinates are not used, since the generator puts every room on the `x == y`
diagonal. Alongside it the page reports the colony, the solution, live
progress, and re-checks the simulation on its own: one move per ant per turn,
every move through a real tunnel, one ant per room, everyone home at the end.
A `##visu speed=4 paused` line in the colony steers it.

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
| Min-cost max flow | `O(F * (V + E))`, `F` bounded by the degree of start and end |
| Route extraction | `O(V + E)` |
| Route selection | `O(F * F * log A)` — one binary search per candidate count |
| Ant distribution | `O(F)` |
| Simulation | `O(turns * A)` output tokens, flushed every 64 KB |

## Summary

- **Parsing** — one read, in-place line splitting, strict validation, hash table
  for name lookup, and the input kept for the echo.
- **Modelling** — every room split into `in`/`out` with a capacity-1 edge, which
  turns "one ant per room" into a plain edge capacity.
- **Solving** — min-cost max flow gives the shortest room-disjoint routes; the
  flow is then decomposed into explicit room sequences.
- **Optimising** — the route count and the ant distribution are chosen to
  minimise the turn on which the last ant arrives.
- **Output** — the map, a blank line, then one line of `Lx-room` tokens per
  turn, flushed in fixed-size blocks.

## References

Algorithms:

- Maximum flow problem — <https://en.wikipedia.org/wiki/Maximum_flow_problem>
- Minimum-cost flow problem — <https://en.wikipedia.org/wiki/Minimum-cost_flow_problem>
- Johnson's algorithm (potentials) — <https://en.wikipedia.org/wiki/Johnson%27s_algorithm>
- Ford-Fulkerson method — <https://en.wikipedia.org/wiki/Ford%E2%80%93Fulkerson_algorithm>
- Max-flow min-cut theorem — <https://en.wikipedia.org/wiki/Max-flow_min-cut_theorem>
- Menger's theorem (disjoint paths and connectivity) —
  <https://en.wikipedia.org/wiki/Menger%27s_theorem>
- Vertex capacities by node splitting —
  <https://cp-algorithms.com/graph/edmonds_karp.html>
- Breadth-first search — <https://en.wikipedia.org/wiki/Breadth-first_search>
- Suurballe's algorithm, the shortest-pair variant of the same idea —
  <https://en.wikipedia.org/wiki/Suurballe%27s_algorithm>
