## 6.828 调试用脚本.

可用于 6.828 Stack 部分的练习.


```python
python
esp_initial_addr = 0xf010ffdc
cur_esp_addr = int(gdb.parse_and_eval("$esp"))
bytecount = int((esp_initial_addr - cur_esp_addr) / 4)


print("")
print("initial esp addr :" + hex(esp_initial_addr))
print("current esp addr :" + hex(cur_esp_addr))

print("bytecount\t :" + str(bytecount))
print("")

if bytecount > 0:

    for i in range(esp_initial_addr, cur_esp_addr - 1, -4):
        prestr = ""
        if i == esp_initial_addr:
            prestr = "init_esp-> " + hex(esp_initial_addr)
            add_and_value = ":\tnot-important"
        elif i == cur_esp_addr:
            prestr = "esp0-> "
            add_and_value = gdb.execute("x /x " + str(i), to_string=True)[0:-1]
        else:
            prestr = "esp"+str((i-cur_esp_addr)//4)+"-> "
            add_and_value = gdb.execute("x /x " + str(i), to_string=True)[0:-1]

        print(prestr + add_and_value)
        if i==esp_initial_addr:
            print("")
print("")
end

```