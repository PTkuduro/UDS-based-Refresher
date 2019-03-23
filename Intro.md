## 前言与使用方法

* #### 文档目的

  本文档用于指导用户如何进一步开发该下载工具的功能，并告知用户应如何使用该下载工具。

* #### 读者对象

  本文档预期读者对象为：测试工程师、软件工程师

* #### 参考标准

  下列文件对本文档的应用是必不可少的。

| 标题 |
| :--- |
| ISO 14229-1 Road vehicles - Unified diagnostic services\(UDS\) - Part 1 :Specification and requirements |
| ISO 15765-2 Road vehicles - Diagnostic communication over Controller Area Network\(Do CAN\) - Part 2:Transport Protocol and network layer services |
| ISO 15765-3 Road vehicles - Diagnostic on Controller Area Networks \(CAN\) - Part 3: Implementation of unified services\(UDS on CAN\) |

* #### 使用方法

  通过DB9接口连接BeagleBone Black与待重编程控制器的CAN接口，通过FTP或SD卡挂载方式将规定安全访问算法的`libSA.so`文件拷贝入BBB `usr/lib`,待刷写的`.s19`文件与规定下载流程的`.xml`文件(待实现)拷贝入`/usr/Diagbox_src` 目录中，运行`diagbox.exe`并同时输入文件名称，按照提示选择刷写模式，等待下载完成，并查看软件完整性与依赖性检查结果。



