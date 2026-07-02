# StarfieldAnimationFramework-

Starfield Animation Framework (SAF) is a framework for playing custom animations in Starfield, designed as a modern counterpart to NAF (Native Animation Framework) that works with current versions of the game.

SAF was created for two main reasons:
- the original CCF (Custom Command Framework) used by NAF is no longer maintained and no longer works correctly with the latest Starfield,
- I wanted a tool that behaves like NAF but is built on the current ecosystem (SFSE + CommonLibSF) and on a new Modern Command Framework (MCF).

The framework is an SFSE plugin. It registers its own console command `saf`, which allows you to:
- play animations on the player or on a chosen actor,
- load animations from external files (e.g. GLTF, GLB),
- control transition time, speed, playback mode, etc. (depending on the exact command and parameters).

SAF’s internal logic is modeled after NAF:
- the animation engine works directly on actor skeletons,
- data from animation files are mapped onto character bones and layered on top of the current state,
- the framework tries to minimize interference with the rest of the game – once an animation finishes, the actor returns to normal behavior,
- character skeleton same as NAF (HumanRace.json).

At the moment, due to changes between game versions and the lack of a stable way to access the “selected reference” from the console, the behavior is:
- `saf play <animation>` – by default plays the animation on the player,
- to play an animation on an NPC you must provide its FormID:
  `saf play <animation> <NPC_FormID>`
(the FormID can be read from the console after clicking on the NPC).
- 'saf stop' - Stops the animation from playing - for Player
- 'saf stop <NPC_FormID>`- Stops the animation from playing - for NPC

---

## Building (Linux → Windows cross-compile)

This fork adds an [xmake](https://xmake.io) build that cross-compiles the SFSE
plugin **from Linux** to a Windows x64 DLL, using `clang-cl` + `lld-link` +
`llvm-rc` against an [xwin](https://github.com/Jake-Shadle/xwin)/MSVC Windows SDK
tree (referenced here as `~/.vsxwin`).

```sh
# Provide the dependencies under extern/ first (see below), then:
xmake f -p windows -a x64 -m release --toolchain=clang-cl --sdk=$HOME/.vsxwin -y
xmake build
```

Output: `build/windows/x64/release/StarfieldAnimationFramework.dll`.

### Dependencies (not committed — `extern/` is git-ignored)

- `extern/commonlibsf/` — CommonLibSF (libxse fork, runtime 1.16.244.0), built from source.
- `extern/{fmt,spdlog,simdjson,fastgltf}/` — vendored and compiled into static libs by the xmake targets.
- From xrepo: `ozz-animation 0.16.0`, `zstr`, `zlib`.

`modules/detect/tools/find_rc.lua` (included) lets xmake locate an `rc` compiler on Linux.

## Credits

Original mod by **mielu91m** — <https://github.com/mielu91m/StarfieldAnimationFramework->.
This repository is a modified fork that adds the Linux cross-compile build system
above; all mod functionality and design are the original author's work.

## License

Distributed under the MIT License — see [LICENSE](LICENSE).
Note: the upstream repository declares no license; MIT is applied here for consistency
with the sibling Modern Command Framework by the same author.
