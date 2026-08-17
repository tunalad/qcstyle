# qcstyle

Quaketastic Style is a free, fast, and small automatic formatter for C and QuakeC. Fork of [Artistic Style](https://astyle.sourceforge.net/) (astyle) 1.23, with a lot of code cut out. Works as a regular C formatter, with an additional QuakeC formatting style.

## Features

- `$frame`, `$cd`, `$origin` and other model directives
- `local` variable declarations
- Entity frame definitions (`void() name = [$frame, next] { ... };`)
- `.` as a field access operator (not pointer dereference)

## Install

Requires a C++14 compiler and `make`. Build settings live in `config.mk`.

```
git clone https://github.com/tunalad/qcstyle.git
cd qcstyle
make && make install
```

## Usage

Preview changes without writing:

```
qcstyle < myfile.qc
```

Format in place (creates a `.orig` backup):

```
qcstyle myfile.qc
```

Format in place without backup:

```
qcstyle -n myfile.qc
```

Run `qcstyle --help` for all options.

Available styles:

- `-A1` / `allman` — Broken braces
- `-A2` / `k&r` — Linux braces
- `-A3` / `stroustrup` — Linux braces, broken closing headers
- `-A4` / `whitesmith` — Broken, indented braces
- `-A5` / `banner` — Attached, indented braces
- `-A6` / `gnu` — Broken braces, indented blocks (2 spaces)
- `-A7` / `linux` — Linux braces (8 spaces)
- `-A8` / `quakec` — QuakeC style (Allman, 4 spaces)

## Acknowledgments

Based on [Artistic Style](https://astyle.sourceforge.net/) by Jim Pattee. See the [source repository](https://gitlab.com/saalen/astyle) for the original project.

## License

GNU Lesser General Public License v3.0
