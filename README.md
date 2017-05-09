# xed_for_windbg
XEDParse extension for windbg

Since the a command does not support 64-bit instruction mnemonics on windbg:

```
0:000> a .
00000000`77ae7980 xor rax,rax
xor rax,rax
        ^ Couldn't resolve 'xor rax,rax'
```

I developed a new plugin with XEDParse to support 64-bit instruction mnemonics:

```
0:000> !xed_for_windbg.a64 .
0000000077ae7980  xor rax,rax
xor rax,rax = 48 31 c0 
```
