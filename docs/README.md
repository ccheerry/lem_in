# lem-in

lem-in is a digital ant farm written in C. It reads a colony on standard input
(a number of ants, rooms and the tunnels between them) and prints the moves
that take every ant from `##start` to `##end` in as few turns as possible,
knowing that no room other than the start and the end may hold more than one
ant at a time. The bonus, `visu-hex`, is a visualizer written in Python with
tkinter that replays the run and checks it against the rules.

## How it works

A single shortest path is not the answer: ants queued on one route arrive one
per turn, so spreading them over several routes that share no room is faster,
but only up to a point, since a long extra route can delay the last ant.

- **Parsing.** Standard input is read in one buffer and cut into lines in
  place. The ant count must fit an `int`; a room is `name x y` with integer
  coordinates and a name that starts with neither `L` nor `#`; a tunnel is `a-b`
  between two declared rooms. Names are resolved through a djb2 hash table, and
  every accepted line is kept, because the map is printed back before the moves.
- **Node splitting.** Each room becomes an `in` and an `out` node joined by an
  edge of capacity 1, so a flow cannot cross a room twice: the one-ant rule
  becomes an edge capacity.
- **Min-cost max flow.** Dijkstra with Johnson potentials over the residual
  graph pushes one unit along the cheapest augmenting path at a time; the flow
  value is the number of room-disjoint routes, and their total length stays
  minimal. A flow of 0 means start and end are not connected.
- **Route selection.** The flow is decomposed into room sequences, sorted by
  length. For every route count `k`, a binary search finds the fewest turns the
  `k` shortest routes need; the best `k` wins and its routes are filled up to
  that turn, the surplus taken from the longest ones.
- **Output.** The map, a blank line, then one line of `Lx-room` moves per turn,
  written through a 64 KB buffer.

The generator's `--big` and `--big-superposition` colonies solve in under
50 ms, and 100 000-room colonies in about a second.

## Dependencies

| Dependency | For | Debian / Ubuntu | Arch |
|---|---|---|---|
| C compiler and GNU Make | `lem-in` | `build-essential` | `base-devel` |
| Python 3.8+ with Tk | the visualizer only | `python3 python3-tk` | `python tk` |

`lib/` holds the only helper library used, built by the same Makefile; the
visualizer needs nothing beyond Python's standard library.

## Build and run

```bash
make install   # checks the compiler, and Tk for the visualizer
make           # builds lib/libft.a, then ./lem-in
make bonus     # also links ./visu-hex to the visualizer
make test      # runs every colony of maps/ and checks each result
make maps      # writes the large stress colonies to maps/stress/generated/
make clean     # objects
make fclean    # objects, libft.a, the binary and the visualizer link
make help      # the list above
```

The program takes no arguments: `./lem-in < maps/valid/example.map` prints the
simulation on standard output, and any invalid colony prints `ERROR` on
standard error with exit status 1.

| Rejected | Example map in `maps/invalid/` |
|------|-------------|
| Ant count missing, zero, negative, not a number or beyond `INT_MAX` | `ants_*.map`, `no_ants.map`, `overflow_ants.map` |
| Empty input, or an empty line anywhere | `empty.map`, `blank_line_middle.map` |
| Room with too few or too many fields, or bad coordinates | `room_missing_field.map`, `room_extra_field.map`, `coords_not_int.map` |
| Room name starting with `L` or `#`, or declared twice | `room_name_starts_L.map`, `dup_room.map` |
| Two or no `##start` / `##end` | `dup_start.map`, `no_end.map` |
| Tunnel to an undeclared room, or to itself | `link_unknown_room.map`, `self_link.map` |
| No route between start and end | `no_path.map` |

## Bonus

`make bonus` links `./visu-hex`, used as `./lem-in < map | ./visu-hex`. It
opens a resizable window with two views, switched with `v`: the **map**, with
the rooms at their coordinates (or laid out from the graph when the
coordinates carry no shape) and every ant gliding along its tunnel, and the
**lanes**, one per route, shortest first. It re-checks every move (one per ant,
through a tunnel, into a free room, everybody home at the end) and shows the
first offending turn, if any. `space` plays, `←` / `→` step, `+` / `-` change
the speed and `q` quits; a `##visu speed=4 paused view=lanes` line in the
colony sets the defaults.

```bash
./lem-in < maps/valid/example.map | ./visu-hex                        # 3 ants, 2 routes
./lem-in < maps/valid/hundred_ants.map | ./visu-hex --speed 4         # 124 ants, 523 rooms
./lem-in < maps/stress/big_superposition.map | ./visu-hex --speed 8   # 164 ants, 2 893 rooms
```

## Layout

```
lem-in/
├── Makefile   # lem-in, bonus, test, maps
├── include/   # lem_in.h: types, constants, prototypes
├── lib/       # libft, the only helper library used
├── srcs/      # parsing/, graph/, solver/, simulation/, utils/, main.c
├── bonus/     # visu-hex and the visu_hex/ package
└── maps/      # valid/, invalid/ and stress/ colonies, generate.py for the large ones
```

## References

Algorithms:

- Maximum flow problem — <https://en.wikipedia.org/wiki/Maximum_flow_problem>
- Minimum-cost flow problem — <https://en.wikipedia.org/wiki/Minimum-cost_flow_problem>
- Johnson's algorithm (potentials) — <https://en.wikipedia.org/wiki/Johnson%27s_algorithm>
- Ford-Fulkerson method — <https://en.wikipedia.org/wiki/Ford%E2%80%93Fulkerson_algorithm>
- Ford & Fulkerson, *Constructing maximal dynamic flows from static flows*
  (1958) — <https://doi.org/10.1287/opre.6.3.419>
- Max-flow min-cut theorem — <https://en.wikipedia.org/wiki/Max-flow_min-cut_theorem>
- Menger's theorem (disjoint paths and connectivity) —
  <https://en.wikipedia.org/wiki/Menger%27s_theorem>
- Vertex capacities by node splitting —
  <https://cp-algorithms.com/graph/edmonds_karp.html>
- Breadth-first search — <https://en.wikipedia.org/wiki/Breadth-first_search>
- Suurballe's algorithm, the shortest-pair variant of the same idea —
  <https://en.wikipedia.org/wiki/Suurballe%27s_algorithm>

Bonus:

- Python `tkinter` — <https://docs.python.org/3/library/tkinter.html>
