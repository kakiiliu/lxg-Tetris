# 19-03-17

## debug

- 解决了之前方块未消去的问题（问题在于没有将上一层的状态复制下去）
- UpdateScore通过DisplayClear的方法成功通过测试。
- 在处理BlockExistence的时候搞错了y与x的先后顺序，导致出现bug，费了一定时间。不过最后debug成功。

## rewrite

- 改了一下变量名，score改成CurrentScore，cnt改成BlockCnt，看上去更标准些。
- 增加了与Neighbor相关的元素：NeighborSize，NeighborXY，CurrentType，InitNeighbor用于初始化（能否以json文件取而代之？），并重写了MovementDetermination的函数，发现存在问题。
- CurrentType目前只有Single与Quad两类，但后者并未经过测试。
- 添加了原本没有的CountCnt，在MovePosition中添加了MoveThis的函数。

## to-do

- 判定失败的方法还没有写。
- Quad还需经过测试，在此之前，需要再添加一个和BlockExistence相似而不相同的标记，还没想好叫什么名字。
- 其他CurrentType待补充。
- 旋转的方法还没想好。
- InitNeighbor有待用数据集取而代之。

# 19-03-18

## done

- 为每天的文件附上日期，以备不时之需。
- 把函数的顺序理了一理，还有一些尚未理出头绪。
- 添加了NeighborExistence并重写了MoveWholeDetection，添加了MoveSingleDetection，原来是MovementDetermination。
- 除去了一些冗余的代码，并将newx的计算改得更简洁。
- 将CollisionDetection留作判定失败的用途，但并未经过测试。
- TimerEventProcess中添加ClearNeighbors以防bug。

# 19-03-19

## done

- 添加了CurrentType的类型，总计四种，游戏目前具有一定的可玩性。
- 一些小的调整——如增大了游戏框的大小，添加VK_UP的方法等。
- 重写了CollisionDetection与GenerateNewBlock，搞出了判定失败的方法。

## debug

- 测试了TYPE_QUAD，发现有时有消去不全的情况？完全没有头绪是哪里有问题。将GenerateNewBlock中的X，Y更改后暂时没有问题了。尚不知其中原因，总之在边界上还需要特判。

## to-do

- 方块的颜色需要加上，不然容易引起混淆。
- 未知的bug：在方块未显示的时候仍可能得分（尤其在上层），游戏框内显得非常混乱。
- 边界的数值还望统一。
- 其他"经典的"CurrentType待补充。
- 旋转的方法还没想好。
- InitNeighbor有待用数据集取而代之。

## conclusion

今天写了比较久，但没有搞出很多的功能；碰到了很多bug，主要集中在边界这一方面。写代码的状态也不佳，原因尚不明。

# 19-03-26

## done

- 添加了CurrentType的类型——Vert和Hori，TriAngel，但是游戏的可玩性还是比较低。
- 在每个方格中填入红圈以作标识，能看出来放了哪些了，但真的好丑啊……
- 添加了RotateBlock的方法——通过TypeMap得到NextType，再通过RotateDetection后ClearNeighbor以及替换实现旋转。
- 更改了一下ROWS和COLS的数值，努力往市面上的标准看齐。
- 还有些小的改动，如OutOfBorder等等。

## to-do

- 还有一些CurrentType需要补充（也许可以添加诡异一些的）
- 模块化的设计还未实现
- 之前的BUG还未解决
- 其他需要实现的部分还未进行分析
- 涂色和“next”

## conclusion

虽然今天总体状态比较差，但写代码状态还是可以的，出了bug能马上发现并改掉。

