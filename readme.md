
# simple-gc

> A garbage collector implementation in C

Dead simple semi-automatic garbage collector.
You allocate stuff by 'binding' them to variable, pointer or a number!
Then when you want to 'free' things, just pass 'binding' or your magic number.

That is all!

## Installation

- Clone the repository:
> `git clone https://github.com/pvtmert/simple-gc.git`

- `cd` into repository.
> `cd simple-gc`

- Build & run an example (`main.c`)
> `make clean all test`

- (Optional) Build & Run in a (docker) container:
> `docker build -t simple-gc -f dockerfile`
>
> `docker run --rm -it simple-gc`

- Play with it!
> Edit `main.c` with your favorite editor!

## Usage

- Build project
- Copy `*.h` files to your project.
> Or add as an include location. (`-I` flag)
- Copy `*.a` files to your project.
> Or add as an library location. (`-L` flag or direct argument on macOS)


#### Allocation aka `malloc`:

Prototype: `void* gc_alloc(size_t size, void* binding, ref_t magic)`

Parameters:
- `size`: Length in bytes to allocate.
- `binding`: A pointer to bind this allocation.
> You can use `argc` in `main` for instance.
- `magic`: A number to specify this (kind-of) allocation. Group-ID.

Note: You can omit `binding` or `magic`.
As `NULL` or `(ref_t) { 0x00 }` respectively.

Returns: `void*` to allocated data. (Same as `malloc`-like functions.)

#### Garbage Collection aka `free`:

Prototype: `void gc_collect(void* binding, ref_t magic)`

Parameters:
- `binding`: Apply GC on those references.
- `magic`: Apply GC on those numbers. (Groups).

Note: I am not sure what to implement scope-like allocations.
See footnotes.

## Contributing
> Pull requests are welcome.
> For major changes, please open an issue first
> to discuss what you would like to change.

## License
[MIT](https://choosealicense.com/licenses/mit/)

## Footnotes:

#### Scope Bound or Temporary Allocations:

I'd like to implement scope bound allocation system.
Which doing `gc_alloc` only with size argument.

While exiting scope, doing `gc_collect` again with empty parameters
will cause only those scope's data will be erased.

First thing to consider is nested-functions or calling them recursively.
Which will cause upper scope to also be deleted. Because I'll check if an
`alloc_t` has both `ref` and `caller` empty/`NULL`. And `gc_collect` will
clean-up all `ref` and `caller` empty objects.

Haven't decided what to do. Just in case, I added `usages` member to keep count
of whats being used or not in future.
