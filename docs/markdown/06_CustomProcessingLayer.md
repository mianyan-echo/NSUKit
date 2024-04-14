# 自定义处理层

@todo 自定义处理层

<span id="icd_paresr"></span>

## icd_paresr     {#md_icd_paresr}
_**icd_paresr：指令处理中间件，用来使用约定式的icd指令**_

<center>![](professional_icd_parser.png)</center>
<center>icd_parser结构</center>

```python


```

### icd.json

<center>![](professional_icd_parser_icd.json.png)</center>
<center>icd.json文件格式</center>

```python
# icd.json是指令处理中间件的必要文件
# 其文件格式可以大概描述为
```

```json
{
    "param": {
        "参数1": ["参数1", "数据类型", "数据值"],
        "参数2": ["参数2", "数据类型", "数据值"],
        "参数3": ["参数3", "数据类型", "数据值"]
    },
    "command":{
        "指令1": {
            "send": [
              "参数1", 
              "参数2", 
              ["参数4", "数据类型", "数据值"]
            ],
            "recv": [
              "参数3"
            ]
        },
        "指令2": {
            "send": [
              "参数1",
              "参数2",
              ["参数4", "数据类型", "数据值"]
            ],
            "recv": [
              "参数3"
            ]
        }
    },
    "sequence": {
        "指令3": [
            "参数",
            ["参数", "数据类型", "数据值"]
        ]
    }
}
```

```python
# 暨以上三大类，由param、command、sequence组成
# param中有多个参数
# command中有两种指令类型 send、recv， 每种指令由多个参数拼接而成
# sequence 指令的变长部分，每一项为一list，结构与command中的项相同，其可在command中被调用{{sequence1}}，程序自动根据excel文件中的内容重复一项
# ！！注意！！ 参数中支持以下数据类型uint8、int8、uint16、int16、uint32、int32、float、double、file_data、file_length
```

### config(self, **kwargs): 
```python

```

---

<center>![](professional_icd_parser_getset_param.png)</center>
<center>get_param/set_param调用流程</center>

### get_param(self, param_name: str, default=0, fmt_type=int):
```python

```

### set_param(self, param_name: str, value, fmt_type=int):
```python

```

---

<span id="virtual_chnl"></span>

## virtual_chnl
_**virtual_chnl：虚拟通道中间件**_
```text
虚拟通道主要用于：当设备只有1个数传通道，但想分开传输多路数据时所使用
```

### virtual_chnl未开启时

<center>![](professional_virtual_chnl_stop.png)</center>
<center>虚拟通道未开启时数据流的流程图</center>

```text
举例说明：
花园需要浇水，每种花需要浇不同的水，且每种水不相容。现在只有一个水管（1个数传通道）
当我用水管进行浇水时，我需要一个挡板把水管中的不同的水分开进行使用
```

### virtual_chnl开启时

<center>![](professional_virtual_chnl_start.png)</center>
<center>虚拟通道开启时数据流的流程图</center>

```text
举例说明：(还是上边的例子)
花园需要浇水，每种花需要浇不同的水，且每种水不相容。现在只有一个水管（1个数传通道）
现在我买了一台机器，这台机器上有很多出水口，并且机器可以自动把每种水自动分开
这时我只需要使用机器上对应出水口的水去浇特定的花就可以了
```

---

