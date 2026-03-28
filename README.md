# takenote

A terminal-based note-taking tool powered by a custom Unix-like file system implemented in C.

---

## Overview

takenote is a lightweight CLI application for managing notes through a structured, directory-based workflow. It is built on top of **NoteFS**, a custom file system engine that models notes as files within a hierarchical tree.

The project combines low-level system design with practical usability, providing both a functional developer tool and a demonstration of core file system concepts.

---

## Key Capabilities

* Organize notes using directories and subdirectories
* Create, edit, read, and delete notes
* Visualize note structure as a tree
* Navigate using familiar shell-like commands
* Interactive command-line interface

---

## Installation

### Option 1: Download Prebuilt Binary

1. Go to the Releases section of this repository
2. Download the binary (`takenote`) for your system
3. Make it executable:

```bash
chmod +x takenote
```

4. Run:

```bash
./takenote <workspace-name>
```

---

### Option 2: Build from Source

```bash
git clone https://github.com/MrHope03/takeNote.git
cd takeNote
make
./takenote <workspace-name>
```

---

## Example Usage

[Watch the demo video](./media/recording.mp4)

---

## Commands

```bash
pwd             Print current directory
ls [dir]        List contents
tree            Display full structure
cd <dir>        Change directory
mkdir <name>    Create directory
touch <name>    Create note
cat <file>      Read note
vi <file>       Edit note
rm <file>       Delete note
rmdir <dir>     Delete directory
clear           Clear terminal
exit            Exit application
```

Inside the editor:

```bash
Ctrl-S          Save note
Ctrl-Q          Exit editor
```
