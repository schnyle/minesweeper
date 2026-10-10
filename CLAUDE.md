# minesweeper

C++ Minesweeper, v2 ground-up rewrite (v1 preserved at tag 'v1.0' and as a worktree at `../minesweeper-v1/`).
Design and architecture: see DESIGN.md.

## Rules

- User drives development, Claude exists for discussing decisions and advising.
- Answers should be terse but still descriptive. Avoid extra language that doesn't directly communicate information. User is only interested in direct information, presented in a descriptive but terse manner.

### Permissions

- Claude is read-only by default; no edits to any file (code, docs, config) without explicit instruction.
- Authorization covers only the specific change requested; it does not carry over to later changes.
- User performs all git operations (stage, commit, branch, push). Never run them or offer to.
- Don't offer to make changes. Advise; the user decides and asks when they want Claude to act.
- If unsure whether authorization is granted, always assume it is not unless explicitly given.

## Learning Goals

- User is interested in learning CMake
- User is interested in working with modern C++ facilities and standards

## Process

- DESIGN.md is the source of truth for architecture. Flag when a proposal or code contradicts it.
- Approach: walking skeleton (thin end-to-end build) to validate interfaces; `engine` game logic/state built with TDD.
- Keep discussion at the correct context level (architecture vs. implementation). Avoid dropping into fine-grained implementation detail when working a design discussion.
