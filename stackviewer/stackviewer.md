## 6.828 调试用脚本.

可用于 6.828 Stack 部分的练习.




```python
stacktop_addr = 0xf010ffd8
print("ebp addr:"+hex(stacktop_addr))
esp_addr = int(gdb.parse_and_eval("$esp"))
print("esp addr:"+hex(esp_addr))
bytecount = int((stacktop_addr - esp_addr) / 4)
print("bytecount:"+str(bytecount))
if bytecount>0:
    for i in range(stacktop_addr,esp_addr,-4):
        prestr=""
        if i==stacktop_addr:
            prestr=" ebp-> "
        else:
            prestr="       "
        add_and_value = gdb.execute("x /x "+str(i),to_string=True)[0:-1]
        print(prestr+add_and_value)
    print(" esp-> "+hex(esp_addr)+":\tnot-impotant")
end
```

```shell
┌──Register group: general───────────────────────────────────────────────────────────────────────────────┐
│eax            0x0      0                                                                               │
│ecx            0x3d4    980                                                                             │
│edx            0x3d5    981                                                                             │
│ebx            0x5      5                                                                               │
│esp            0xf010ffc0       0xf010ffc0                                                              │
│ebp            0xf010ffd8       0xf010ffd8                                                              │
│esi            0x10094  65684                                                                           │
│edi            0x0      0                                                                               │
│eip            0xf0100050       0xf0100050 <test_backtrace+16>                                          │
│eflags         0x92     [ AF SF ]                                                                       │
│cs             0x8      8                                                                               │
│ss             0x10     16                                                                              │
│ds             0x10     16                                                                              │
│es             0x10     16                                                                              │
│fs             0x10     16                                                                              │
│gs             0x10     16                                                                              │
│                                                                                                        │
└────────────────────────────────────────────────────────────────────────────────────────────────────────┘
   │0xf0100028 <entry+28>           mov    $0xf010002f,%eax                                              │
   │0xf010002d <entry+33>           jmp    *%eax                                                         │
   │0xf010002f <relocated>          mov    $0x0,%ebp                                                     │
   │0xf0100034 <relocated+5>        mov    $0xf0110000,%esp                                              │
   │0xf0100039 <relocated+10>       call   0xf0100094 <i386_init>                                        │
   │0xf010003e <spin>               jmp    0xf010003e <spin>                                             │
B+ │0xf0100040 <test_backtrace>     push   %ebp                                                          │
   │0xf0100041 <test_backtrace+1>   mov    %esp,%ebp                                                     │
   │0xf0100043 <test_backtrace+3>   push   %ebx                                                          │
   │0xf0100044 <test_backtrace+4>   sub    $0xc,%esp                                                     │
   │0xf0100047 <test_backtrace+7>   mov    0x8(%ebp),%ebx                                                │
   │0xf010004a <test_backtrace+10>  push   %ebx                                                          │
   │0xf010004b <test_backtrace+11>  push   $0xf01018a0                                                   │
  >│0xf0100050 <test_backtrace+16>  call   0xf0100908 <cprintf>                                          │
   │0xf0100055 <test_backtrace+21>  add    $0x10,%esp                                                    │
   │0xf0100058 <test_backtrace+24>  test   %ebx,%ebx                                                     │
   │0xf010005a <test_backtrace+26>  jle    0xf010006d <test_backtrace+45>                                │
   └─────────────────────────────────────────────────────────────────────────────────────────────────────┘
remote Thread 1 In: test_backtrace                                                   L14   PC: 0xf0100050
(gdb) where
#0  0xf0100050 in test_backtrace (x=5) at kern/init.c:14
#1  0xf01000d4 in i386_init () at kern/init.c:39
#2  0xf010003e in relocated () at kern/entry.S:80
(gdb) python
 >
 >stacktop_addr = 0xf010ffd8
 >print("ebp addr:"+hex(stacktop_addr))
 >esp_addr = int(gdb.parse_and_eval("$esp"))
 >print("esp addr:"+hex(esp_addr))
 >bytecount = int((stacktop_addr - esp_addr) / 4)
 >print("bytecount:"+str(bytecount))
 >if bytecount>0:
 >    for i in range(stacktop_addr,esp_addr,-4):
 >        prestr=""
 >        if i==stacktop_addr:
 >            prestr=" ebp-> "
 >        else:
 >            prestr="       "
 >        add_and_value = gdb.execute("x /x "+str(i),to_string=True)[0:-1]
 >        print(prestr+add_and_value)
 >    print(" esp-> "+hex(esp_addr)+":\tnot-impotant")
 >end
ebp addr:0xf010ffd8
esp addr:0xf010ffc0
bytecount:6
 ebp-> 0xf010ffd8:      0xf010fff8
       0xf010ffd4:      0x00010094
       0xf010ffd0:      0x00010094
       0xf010ffcc:      0x00010094
       0xf010ffc8:      0x00000000
       0xf010ffc4:      0x00000005
 esp-> 0xf010ffc0:      not-impotant