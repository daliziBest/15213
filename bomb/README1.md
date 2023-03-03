# bomblab（6个bomb）
1. 判断两个字符串是否相等，`x/s 0x402400`即可知道答案
2. 从输入字符串提取6个整数，第一个必须是1，后面依次是前一个的两倍
   1. 1 2 4 8 16 32
3. 从输入字符串提取2个整数，第一个整数范围在0~7，然后程序根据提取的第一个整数索引到数组中的数值，用以判断是否等于提取的第二个整数
   1. 0  207、1  311、2  707、3  256、4  389、5  206、6  682、7  327均可
4. 从输入的字符串提取2个整数，第二个必须为0，第一个需要在[0,14]中，接着程序会在[0,14]中用二分搜索来查找你输入的第一个数，当递归调用没有向右转时，二分查找return 0，bomb不会爆炸
   1. ![image.png](https://cdn.nlark.com/yuque/0/2023/png/1357696/1677766599536-db48e8fa-ef5c-47ee-af00-8a403f52d440.png#averageHue=%23b1a89e&clientId=u80a5d2e9-865b-4&from=paste&height=425&id=NItQ4&name=image.png&originHeight=425&originWidth=561&originalType=binary&ratio=1&rotation=0&showTitle=false&size=193935&status=done&style=none&taskId=ufa605bed-c3a6-4c2f-8e27-e4a981e3717&title=&width=561)
   2. 7  0、3  0、1  0、0  0均可
5. 输入6个字符，根据这6个字符ASCII码的低四位（范围0~f）去0x4024b0处的数组索引到新的字符，将其放在栈中，然后与'flyers'比较，相等即可拆除炸弹
   1. 'flyers'对应0x4024b0处数组中的位置分别为9，f，e，5，6，7
   2. 输入字符只要满足ascii码低四位的值依次为9，f，e，5，6，7即可，例如'yonuvw'
6. 输入6个整数，下图是phase_6流程图

![c438f709d7b437617bded7330fec298.png](https://cdn.nlark.com/yuque/0/2023/png/1357696/1677814614263-1c5c81f7-b092-4fbb-87da-2908478da623.png#averageHue=%23fdfdfc&clientId=u21eb4715-b344-4&from=paste&height=2772&id=fOVGq&name=c438f709d7b437617bded7330fec298.png&originHeight=2772&originWidth=4250&originalType=binary&ratio=1&rotation=0&showTitle=false&size=652328&status=done&style=none&taskId=ub0e20def-1e00-4c2f-ab5b-b15d8290add&title=&width=4250)

   1. 假设输入的6个整数分别为Xi：A，B，C，D，E，F
   2. 上图第1部分要求6个整数在[0,6]中，且互不相等
   3. 上图第2部分会把输入的6个整数改为7-Xi：7-A，7-B，7-C，7-D，7-E，7-F
   4. 上图第3部分根据7-Xi的值去0x6032d0处找到相应的地址，然后依次存储在栈中（$rsp + 0x20处），且因为后面需要引用地址，所以输入整数Xi里不能有0
| 7-Xi | 找到的地址 | Xi |
| --- | --- | --- |
| 1 | 0x6032d0 | 6 |
| 2 | 0x6032e0 | 5 |
| 3 | 0x6032f0 | 4 |
| 4 | 0x603300 | 3 |
| 5 | 0x603310 | 2 |
| 6 | 0x603320 | 1 |
| 7 | 0 | 0 |

   5. 上图第4部分根据栈中（$rsp + 0x20处）存储的地址依次对0x6032d0处存储的地址重新整理
      1. 假设Ai是上文根据Xi找到的地址，最后0x6032d0处可能被整理成如下形式
| 0x6032d0（A1） | 8byte | A2（8byte） |
| --- | --- | --- |
| 0x6032e0（A5） | 8byte | 0（8byte） |
| 0x6032f0（A0） | 8byte | A1（8byte） |
| 0x603300（A3） | 8byte | A4（8byte） |
| 0x603310（A4） | 8byte | A5（8byte） |
| 0x603320（A2） | 8byte | A3（8byte） |

   6. 上图第5部分会依次拿出Ai地址处低4byte存储的int与后一个进行比较，要求int0>=int1>=int2>=int3>=int4>=int5

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1357696/1677818783083-d2a8bb39-a465-4c7f-a1f2-43c4c9d57a74.png#averageHue=%23120e08&clientId=ub1ce67a9-cf41-4&from=paste&height=144&id=u60974021&name=image.png&originHeight=144&originWidth=704&originalType=binary&ratio=1&rotation=0&showTitle=false&size=16017&status=done&style=none&taskId=u7418c411-d30f-425e-a9a7-6c5172bcb76&title=&width=704)

   7. 由f.得到栈中（$rsp + 0x20处）存储的地址A0-A5依次为0x6032f0，0x603300，0x603310，0x603320，0x6032d0，0x6032e0，结合d.从而得到最终输入整数序列应该为4  3  2  1  6  5





