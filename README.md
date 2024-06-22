# Birb

As dangerous tunnels plague the Domain and obscure the horizon, Birb has no choice but to abandon his homeland. "The end is near," says Birb downhearted. He flees from the accursed land and tears begin to trickle down his beak. **Fear not, Birb!** By the wonder of Personal Computers, the user can help you avoid the tunnel apocalypse and escape to safety.

Press **W** to make Birb jump. Play with your friends and reach the highest score!

## Installation

To install this game onto your Personal Computer, you need the following:
* GCC cross-compiler for `i686-elf`
* GNU `make`
* `nasm`
* GRUB
* _(optional)_ an x86 emulator (such as QEMU)

Clone the repository and build `birb.iso` by running:

```bash
make iso
```

Then open the image with your emulator of choice. QEMU example:

```bash
qemu-system-i386 -cdrom birb.iso
```

### Running on a physical machine

To run Birb on a physical x86-based machine, you need the following:

* a VGA-compatible graphics card
* a PS/2 keyboard.
