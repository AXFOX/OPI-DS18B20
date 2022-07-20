# OPI-DS18B20
一个用于香橙派的DS18B20用户态简易驱动程序。
A Simple DS18B20 User Mode Driver for Orange Pi.
## Tips
你需要提前安装https://github.com/orangepi-xunlong/wiringOP/
理论适用于所有支持wiringOP的香橙派单板计算机。
# 警告
不要问我为什么驱动无法获取温度数据，在用户态根本无法保证时序正常，因此这个驱动更多的意义上是告诉你获取温度的流程而非拿去就能用！
如果你发现非时序问题造成的bug请提交issues，如果你能修复他请Pull Requests.

#Fucking sequence！
