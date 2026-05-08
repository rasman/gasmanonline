# Gas Man Online — Linux Build Guide (Qt 6 / WebAssembly)

This guide covers building and running Gas Man Online on Linux using Qt 6.8 and
Emscripten 3.1.56. The application compiles to WebAssembly and runs in any
modern browser.

---

## Prerequisites

Install the system packages Qt and Emscripten need:

```bash
sudo apt update
sudo apt install -y \
    build-essential cmake ninja-build python3 python3-pip \
    git curl wget perl \
    libgl1-mesa-dev libglu1-mesa-dev \
    libfontconfig1-dev libfreetype-dev \
    libx11-dev libxext-dev libxfixes-dev libxi-dev \
    libxrender-dev libxcb1-dev libxkbcommon-dev \
    libxkbcommon-x11-dev
```

> On Fedora/RHEL replace `apt` with `dnf` and adjust package names accordingly.

---

## Step 1 — Install Emscripten 3.1.56

```bash
git clone https://github.com/emscripten-core/emsdk.git ~/emsdk
cd ~/emsdk
./emsdk install 3.1.56
./emsdk activate 3.1.56
source ./emsdk_env.sh

# Verify
emcc --version   # should print:  emcc (Emscripten ...) 3.1.56
```

---

## Step 2 — Install Qt 6.8 for WebAssembly

### Option A — Qt Online Installer (recommended)

```bash
wget https://d13lb3tujbc8s0.cloudfront.net/onlineinstallers/qt-online-installer-linux-x64-4.8.1.run
chmod +x qt-online-installer-linux-x64-4.8.1.run
./qt-online-installer-linux-x64-4.8.1.run
```

In the installer select:
- **Qt 6.8.x → WebAssembly (multi-thread)** — best performance; requires
  COOP/COEP headers on the server (see Deployment section below)
- **Qt 6.8.x → WebAssembly (single-thread)** — easier to deploy; no special
  headers needed
- **Qt 6.8.x → Desktop gcc 64-bit** — required for the host-side build tools
  (`qmake`, `moc`, `rcc`, etc.)

Default install location: `~/Qt/6.8.x/`

### Option B — Build Qt 6.8 from source

```bash
# Activate Emscripten first (Step 1 above), then:
git clone https://code.qt.io/qt/qt5.git ~/qt6-src
cd ~/qt6-src
git checkout 6.8
perl init-repository --module-subset=qtbase,qtsvg,qtmultimedia,qtimageformats

mkdir ~/qt6-wasm-build && cd ~/qt6-wasm-build
../qt6-src/configure \
    -xplatform wasm-emscripten \
    -feature-thread \
    -no-dbus \
    -nomake examples \
    -nomake tests \
    -prefix ~/qt6-wasm
cmake --build . --parallel
cmake --install .
```

---

## Step 3 — Set environment variables

Add these to `~/.bashrc` (adjust paths to match your installation):

```bash
# Emscripten SDK
export EMSDK_REPO=~/emsdk

# Qt 6 WebAssembly installation
# Use the path that matches what you installed in Step 2:
export QT6_WASM_REPO=~/Qt/6.8.3/wasm_multithread   # Qt Online Installer (multi-thread)
# export QT6_WASM_REPO=~/Qt/6.8.3/wasm_singlethread # Qt Online Installer (single-thread)
# export QT6_WASM_REPO=~/qt6-wasm                   # built from source

# Gas Man Online repository
export GASMAN_REPO=~/gasmanonline   # adjust to wherever you cloned this repo
```

Reload your shell:

```bash
source ~/.bashrc
```

### Verify everything is wired up

```bash
emcc --version                        # 3.1.56
$QT6_WASM_REPO/bin/qmake --version    # Qt 6.8.x
echo $GASMAN_REPO                     # path to this repo
```

---

## Step 4 — Build

```bash
cd $GASMAN_REPO

# Activate Emscripten for this session (if not already active)
source $EMSDK_REPO/emsdk_env.sh

# Full clean build + launch dev server
bash bin/build-and-run.sh

# Or: clean build only (no server)
bash bin/build-again.sh
```

Build output goes to `$GASMAN_REPO/build/gasman_main/`.

---

## Step 5 — Run / serve

### Option A — emrun (included with emsdk)

```bash
bash $GASMAN_REPO/bin/gasmanonline-run.sh
```

Then open **http://localhost:8081** in your browser.

### Option B — Python simple server (single-thread WASM only)

```bash
cd $GASMAN_REPO/build/gasman_main
python3 -m http.server 8081
```

Open **http://localhost:8081/index.html**.

### Option C — Python server with COOP/COEP headers (required for multi-thread WASM)

```bash
cd $GASMAN_REPO/build/gasman_main
python3 ../../qtwasmserver.py --all
```

Or use the included helper which sets the correct
`Cross-Origin-Opener-Policy` and `Cross-Origin-Embedder-Policy` headers
that SharedArrayBuffer (threading) requires.

---

## Multi-thread vs Single-thread WASM

| | `wasm_multithread` | `wasm_singlethread` |
|---|---|---|
| Performance | Better | Adequate |
| Browser requirement | `SharedArrayBuffer` enabled | None |
| Server headers required | `COOP: same-origin` + `COEP: require-corp` | None |
| Best for | Self-hosted / controlled server | Simple static hosting (GitHub Pages, etc.) |

Set `QT6_WASM_REPO` to the matching Qt installation path and rebuild.

---

## Features restored in this version

The following features were previously disabled for WASM and have been
re-enabled using Qt 6's native async browser file APIs:

| Feature | Qt 6 API used |
|---|---|
| **Open simulation** (File → Open) | `QFileDialog::getOpenFileContent()` — triggers the browser's native file picker; file content is read into memory and deserialized |
| **Save simulation** (File → Save As) | `QFileDialog::saveFileContent()` — serializes to memory and triggers a browser download |
| **Open Recent** menu | Re-enabled alongside open/save |

## Features still disabled

| Feature | Reason |
|---|---|
| Print / Print Preview | Qt print support requires native OS integration not available in WASM |
| Export to PDF/XML/HTML | Same reason as above |
| Send (email attachment) | Requires OS mail client |
| Drag-and-drop between tabs | Uses deprecated Qt APIs; separate effort to fix |

---

## Troubleshooting

**`emcc: command not found` during build**
```bash
source $EMSDK_REPO/emsdk_env.sh
```
Emscripten's environment must be activated in the same shell session that runs
the build.

**`qmake: No such file or directory`**
Check that `QT6_WASM_REPO` points to the correct directory and that
`$QT6_WASM_REPO/bin/qmake` exists.

**Browser shows blank page / SharedArrayBuffer error**
Switch to the `wasm_singlethread` Qt build, or configure your web server to
send these response headers:
```
Cross-Origin-Opener-Policy: same-origin
Cross-Origin-Embedder-Policy: require-corp
```

**File open/save dialog does nothing**
Ensure you are using the `wasm_multithread` build, or that your browser
supports `<input type="file">` (all modern browsers do). The
`getOpenFileContent` / `saveFileContent` APIs require Qt 6.x — they do not
exist in Qt 5.

---

## Quick-reference cheat sheet

```bash
# One-time setup
git clone https://github.com/emscripten-core/emsdk.git ~/emsdk
cd ~/emsdk && ./emsdk install 3.1.56 && ./emsdk activate 3.1.56

# Add to ~/.bashrc
export EMSDK_REPO=~/emsdk
export QT6_WASM_REPO=~/Qt/6.8.3/wasm_multithread
export GASMAN_REPO=~/gasmanonline

# Every build session
source $EMSDK_REPO/emsdk_env.sh
cd $GASMAN_REPO && bash bin/build-and-run.sh

# Open in browser
# http://localhost:8081
```
