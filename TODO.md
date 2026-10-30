# TODO

- Currently using system SDL3 (`find_package`, dynamically linked). For releases, consider an option (e.g. STATIC_SDL) that fetches SDL3 via FetchContent and links it statically to ship a single binary.
- Improve sprite quality (e.g. smiley face has jagged curves).
- iwyu
- cmake
- minimum sizing for Layout (convert it to a class too; revisit tests)
