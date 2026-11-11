# Contributing to the Hospital Management System

## Branch and pull-request workflow

1. Update your local `main`: `git checkout main` then `git pull origin main`.
2. Create a focused branch, for example `feature/patient-search-validation` or `fix/appointment-overlap`.
3. Keep each commit limited to one coherent change. Commit messages should describe the change, not the number of commits.
4. Add or update automated tests for behavior changes.
5. Configure, build, and run CTest locally before pushing.
6. Push your branch and open a pull request targeting `main`.
7. Describe the behavior changed, tests run, and known limitations. Link the related issue when one exists.
8. Wait for the required review and green Ubuntu/Windows CI checks before merging.

## Scope and compatibility

- Keep public interfaces in `include/hms/` consistent with the existing module contracts. Discuss interface changes in a PR before changing call sites across modules.
- Keep module implementation in its corresponding `src/<module>/` directory and register new source files with that module's CMake configuration.
- Avoid global mutable state; use the existing application context and manager interfaces.
- Preserve existing data-file compatibility when changing persistence formats. Include a backward-compatibility test when possible.
- Do not add real patient information, passwords, API keys, or generated build output to Git.
- Keep changes small enough that a teammate can review the actual diff.

## Local validation

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

On Visual Studio's multi-configuration generator, use `cmake --build build --config Debug` and `ctest --test-dir build -C Debug --output-on-failure`.

## Commit attribution

Use your own GitHub account and configured Git author identity. Do not create commits under another person's identity or split one change into artificial commits to inflate contribution counts. If multiple people contributed, describe each person's actual work in the PR and use separate commits for genuinely separate changes.
