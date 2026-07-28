# lem-in

## 📖 About

"lem-in" is an algorithm project written in **C**: a digital ant farm. The
program reads a colony on standard input — a number of ants, a set of rooms and
the tunnels joining them — and prints, turn by turn, the movements that take
every ant from `##start` to `##end` in as few turns as possible.

The interesting part is not walking the graph, it is that a room can hold only
one ant at a time (except `##start` and `##end`), so the ants have to be spread
over **several routes that share no room**. The program therefore does not look
for one shortest path: it turns the colony into a flow network, splits every
room in two to enforce the one-ant rule, runs a **max-flow** pass to obtain the
largest family of room-disjoint routes, then decides how many of those routes
are worth using and how many ants each one carries.

Everything is built with the standard C toolchain and a local `libft`; there
are no external dependencies. Input is read from stdin in one pass, and the
whole simulation is written to stdout with a single `write` at the end.

## 🎯 Objectives

- Parsing an untrusted text format and rejecting every malformed colony
- Modelling rooms and tunnels as a graph with adjacency lists
- Enforcing a per-room capacity with **node splitting** (`in` / `out` pair)
- Implementing **Edmonds-Karp** max flow on the resulting residual graph
- Decomposing the flow into concrete room-disjoint routes
- Choosing the route count that minimises the number of turns
- Distributing the ants so the last one arrives as early as possible
- Emitting the simulation efficiently, without thousands of small writes
- Freeing every allocation on both the success and the error path

## 📋 Function Overview

<details>
<summary><strong>lem-in — modules breakdown</strong></summary>

<br>

| Module | Feature | Description |
|--------|---------|-------------|
| **parsing** | Single-pass read | `read_stdin` slurps stdin into one growable buffer, no syscall per line |
| **parsing** | Ant count | First line must be a positive decimal integer, nothing else |
| **parsing** | Rooms | `name x y`, three fields exactly, integer coordinates, no leading `L` or `#` |
| **parsing** | Commands | `##start` / `##end` apply to the next room; any other `#…` line is a comment |
| **parsing** | Tunnels | `a-b`, both rooms must already exist, no self-link |
| **parsing** | Echo buffer | Every accepted line is kept so the input can be reprinted before the moves |
| **graph** | Hash table | djb2 with chaining, 1024 buckets: room lookup by name in O(1) |
| **graph** | Adjacency lists | Each tunnel becomes two directed edges plus their residual reverses |
| **graph** | Node splitting | Room `i` becomes `in = 2i` and `out = 2i+1`, joined by an edge of capacity 1 |
| **solver** | Max flow | Edmonds-Karp from `out(start)` to `in(end)`; the flow is the number of routes |
| **solver** | Decomposition | Saturated forward edges are walked back into explicit room sequences |
| **solver** | Route selection | Tries every count from 1 to the max flow, keeps the one with fewest turns |
| **solver** | Turn count | Binary search on the smallest turn `t` whose capacity covers every ant |
| **solver** | Ant distribution | Each ant joins the route on which it would arrive earliest |
| **simulation** | Turn emission | One line per turn, `Lx-room` tokens for every ant that moves |
| **simulation** | Output buffer | Everything is appended to a growable buffer and flushed in one `write` |
| **utils** | Cleanup | A single `free_all` releases input, graph, edges, hash table and routes |

<br>

</details>

<details>
<summary><strong>Usage Example & Testing</strong></summary>

### Build and run

```bash
make                                    # builds libft, then the lem-in binary
./lem-in < maps/valid/example.map
make fclean                             # removes objects, libft.a and the binary
```

### A solved colony

```bash
$ ./lem-in < maps/valid/example.map
3
##start
1 23 3
2 16 7
…
7-4
6-5

L1-2 L3-3
L1-4 L2-2 L3-5
L1-0 L2-4 L3-6
L2-0 L3-0
```

The map is echoed back first, then a blank line, then one line per turn. Three
ants, four turns: they travel on two room-disjoint routes at once instead of
queueing on the shortest one.

### More routes are not always better

```bash
$ ./lem-in < maps/valid/diff_len_paths.map | tail -6
L1-a L5-c
L1-b L2-a L5-d L6-c
L1-end L2-b L3-a L5-e L6-d
L2-end L3-b L4-a L5-end L6-e
L3-end L4-b L6-end
L4-end
```

Six ants, one route of length 3 and one of length 4. Using both takes 6 turns,
using only the short one would take 8. The solver evaluates every route count
and keeps the best.

### Error handling

```bash
$ ./lem-in < maps/invalid/no_path.map
ERROR
$ ./lem-in < maps/invalid/dup_start.map
ERROR
$ ./lem-in extra_argument
Error!
Compile the executable without any arguments.
```

Any malformed colony — bad ant count, duplicate room or command, unknown room
in a tunnel, missing `##start` / `##end`, or no route between them — prints
`ERROR` and exits with status 1.

### Full sweep

```bash
for f in maps/valid/*.map;   do ./lem-in < "$f" > /dev/null || echo "FAIL $f"; done
for f in maps/invalid/*.map; do ./lem-in < "$f" 2>&1 | head -1; done   # 18x ERROR
valgrind --leak-check=full ./lem-in < maps/valid/example.map
```

<br>

</details>

## 🚀 Installation & Structure

<details>
<summary><strong>📥 Setup & Usage</strong></summary>

<br>

### Host requirements

| Tool | Needed for | Notes |
|------|------------|-------|
| `cc` (gcc or clang) | Compiling | Built with `-Wall -Wextra -Werror` |
| `make` | Driving the build | Also builds `lib/libft.a` |
| A POSIX system | Running | Only `read`, `write`, `malloc`, `free` and `exit` are used |

Nothing else has to be installed: `lib/` ships the only helper library used.

### Make targets

```bash
make            # build lib/libft.a and the lem-in binary
make clean      # remove the objects/ directory
make fclean     # clean + remove libft.a and the binary
make re         # fclean + build
```

### Running

```bash
./lem-in < maps/valid/two_paths.map     # from a file
cat colony.map | ./lem-in               # from a pipe
./lem-in                                # typed by hand, end with Ctrl-D
```

The program takes **no arguments**: it reads the colony on standard input,
writes the simulation on standard output and `ERROR` on standard error.

### Map format

```
4                 # number of ants, positive integer, first line
##start           # the next room is the entry
0 0 3             # name x y
2 2 5
3 4 0
##end             # the next room is the exit
1 8 3
0-2               # tunnel between two declared rooms
2-3
3-1
```

Room names may not start with `L` or `#`, coordinates must be integers, and
every tunnel must name two different rooms that already exist. Lines starting
with `#` are comments; `##start` and `##end` are the only commands with a
meaning, any other `##…` line is ignored.

<br>

</details>

<details>
<summary><strong>📁 Project Structure</strong></summary>

<br>

```
lem-in/
│
├── README.md
├── Makefile
├── .gitignore
│
├── docs/
│   └── README.md                   # Condensed project documentation
│
├── include/
│   └── lem_in.h                    # Types, constants and every prototype
│
├── lib/                            # libft — the only helper library used
│   ├── Makefile
│   ├── libft.h
│   └── ft_*.c                      # strings, memory, lists, conversions
│
├── maps/                           # Test colonies
│   ├── valid/                      # Must produce a simulation
│   │   ├── example.map             # 3 ants, two routes of different length
│   │   ├── linear.map              # one corridor, the ants queue up
│   │   ├── two_paths.map           # two routes of equal length, 4 ants
│   │   ├── diff_len_paths.map      # the longer route is still worth using
│   │   ├── single_ant.map          # smallest possible colony
│   │   └── comments_commands.map   # comments and unknown commands ignored
│   └── invalid/                    # Must print ERROR (18 cases)
│       ├── empty.map               # no input at all
│       ├── no_ants.map             # ants_zero / ants_negative / ants_not_number
│       ├── room_missing_field.map  # room_extra_field, coords_not_int
│       ├── room_name_starts_L.map  # room_name_starts_hash
│       ├── dup_room.map            # dup_start, dup_end
│       ├── no_start.map            # no_end
│       ├── link_unknown_room.map   # self_link
│       └── no_path.map             # valid colony, start and end disconnected
│
└── srcs/
    ├── main.c                      # parse → build → solve → simulate → free
    ├── parsing/
    │   ├── read_stdin.c            # slurps stdin into one growable buffer
    │   ├── parse_input.c           # line splitting, ant count, dispatch
    │   ├── parse_rooms.c           # room validation and storage
    │   ├── parse_links.c           # tunnel validation and storage
    │   └── store_line.c            # keeps the input for the echo
    ├── graph/
    │   ├── graph_init.c            # adjacency lists + residual reverse edges
    │   ├── node_split.c            # room → in/out pair with capacity 1
    │   └── hash_table.c            # room lookup by name (djb2, chaining)
    ├── solver/
    │   ├── algorithm.c             # Edmonds-Karp max flow
    │   ├── paths.c                 # flow decomposition into room sequences
    │   ├── select.c                # how many routes to keep
    │   └── turns.c                 # turn count and ant distribution
    ├── simulation/
    │   ├── simulate.c              # one line per turn
    │   └── output.c                # growable output buffer
    └── utils/
        └── memory.c                # free helpers
```

Every module talks through `t_lem_in`, declared in `include/lem_in.h`: it owns
the ant count, the graph, the hash table, the extracted routes and the echoed
input, so a single `free_all` can clean up from anywhere in the program.

<br>

</details>

<details>
<summary><strong>🧱 Algorithm Overview</strong></summary>

<br>

### Pipeline

```
stdin ─> parse ─> graph ─> node split ─> max flow ─> decompose ─> select ─> assign ─> simulate ─> stdout
```

### Node splitting — one ant per room

```
   ── A ── B ── C ──         in(A) → out(A) → in(B) → out(B) → in(C) → out(C)
                     ==>                        └ cap 1 ┘
   room B, capacity 1        crossing B consumes its single internal edge
```

Every room `i` becomes two nodes, `in = 2i` and `out = 2i+1`, joined by an edge
of capacity 1. A tunnel `a-b` becomes `out(a) → in(b)` and `out(b) → in(a)`,
both capacity 1. Sending flow from `out(start)` to `in(end)` therefore cannot
reuse a room: its internal edge is already saturated.

### Max flow — how many routes exist

```
[ out(start) ] ──BFS──> … ──BFS──> [ in(end) ]
      each augmenting path = one more room-disjoint route
      flow value == number of routes    (0 => ERROR, start and end disconnected)
```

Edmonds-Karp repeats a BFS over the residual graph and pushes one unit of flow
along each shortest augmenting path it finds. Because every capacity is 1, the
final flow value is exactly the number of room-disjoint routes, and augmenting
along shortest paths keeps those routes short.

### Route selection — fewest turns, not most routes

```
routes sorted by length:  [3] [4] [7]        6 ants
   k = 1 -> 8 turns
   k = 2 -> 6 turns   <-- kept
   k = 3 -> 6 turns
```

A route of length `len` delivers `t - len + 1` ants by turn `t`, so the turns
needed by the `k` shortest routes are found by binary search on `t`. Every `k`
from 1 to the max flow is evaluated and the cheapest one wins: a long extra
route can delay the last ant instead of helping.

### Simulation

```
turn t, route of length len, ant j (0-based on that route):
     ant j leaves on turn j+1  ->  it stands in rooms[t - j]
     printed while 0 <= t - j <= len
```

Ants are handed out one by one to the route on which they would arrive first
(`len` plus the ants already queued there), each route gets a contiguous block
of ids, and every turn is appended to one buffer flushed with a single `write`.

<br>

</details>

## 💡 Key Learning Outcomes

- **Graph modelling**: turning a text description into adjacency lists, and
  turning a vertex constraint into an edge constraint through node splitting
- **Network flow**: residual graphs, reverse edges, Edmonds-Karp, and the fact
  that unit capacities make the max flow equal a disjoint-route count
- **Flow decomposition**: reading concrete routes back out of a flow, by
  following the forward edges that ended up saturated
- **Optimisation over a discrete choice**: the best answer is not "use every
  route", it is the route count that minimises the last arrival
- **Buffered I/O**: one read to take the input and one write to give the output,
  instead of a syscall per line
- **Defensive parsing**: rejecting malformed input early and cleanly, through a
  single exit path that frees everything

## ⚙️ Technical Specifications

- **Language**: C, compiled with `cc -Wall -Wextra -Werror`
- **Dependencies**: none beyond libc; `lib/` holds the local libft
- **Input**: standard input only, the program takes no arguments
- **Output**: the map echoed back, a blank line, then one line per turn
- **Errors**: `ERROR` on standard error, exit status 1
- **Graph**: adjacency lists, `2 * rooms` nodes and `2 * (rooms + 2 * tunnels)`
  directed edges counting the residual reverses
- **Lookup**: djb2 hash table, 1024 buckets, separate chaining
- **Max flow**: Edmonds-Karp, `O(F * (V + E))` here, since every capacity is 1
  and `F` is bounded by the degree of `##start` and `##end`
- **Turn count**: binary search between the shortest route length and that
  length plus the ant count
- **Buffers**: 4096-byte initial size, doubling growth, for both input and output

## 🔧 Requirements

- A C compiler (`gcc` or `clang`) and `make`
- A POSIX system — only `read`, `write`, `malloc`, `free` and `exit` are used
- Nothing to install: the helper library lives in `lib/` and is built by `make`

---

> [!NOTE]
> lem-in is a small program with a large algorithm behind it: the
> one-ant-per-room rule turns a plain shortest-path exercise into a max-flow
> problem, and the answer is never a single route but the right *set* of routes
> with the right number of ants on each of them.
