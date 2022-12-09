# KVM
[![Discord Chat](https://img.shields.io/discord/480318777943392266.svg)](https://discord.gg/RgFybs6) 
[![License](http://img.shields.io/:license-mit-blue.svg)](http://doge.mit-license.org)
[![Twitter](https://img.shields.io/twitter/url/http/shields.io.svg?style=social)](https://twitter.com/mikulasflorek)


Simple, language agnostic virtual machine. Used in [Lumix Engine](https://github.com/nem0/lumixengine_visualscript).

This project is in early prorotype stage!

## Embedding KVM

1. add [src/kvm.c](https://github.com/nem0/kvm/blob/master/src/kvm.c) and [src\kvm.h](https://github.com/nem0/kvm/blob/master/src/kvm.h) to your project
2. use the API (see `src\kvm.h`) to create a bytecode and run it

See [src/main.c](https://github.com/nem0/kvm/blob/master/src/main.c) for very simple example
