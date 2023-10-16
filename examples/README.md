# NSUKit使用用例

## 使用方法

```shell
cd NSUKit/examples/
mkdir build
cd build

cmake ..

make

cp ../../icd.json ./
```

## 示例使用

### 指令控制
```shell
sudo ./SocConfig 127.0.0.1         # 发送对Soc的全部配置

sudo ./SocConfig 127.0.0.1 系统停止  # 发送单条配置指令
```

### 固件更新(发送文件)
```shell
sudo ./FirmwareUpdate 127.0.0.1 ~/BOOT.bin
```

### 单包数据上行示例
```shell
sudo ./DSOpUseCase 127.0.0.1 10485760 ./ds_data.dat
```

### 数据流多线程上行示例
```shell
sudo ./DataUpload 127.0.0.1 1048576000 ./ds_data.dat
```
