# Arcaea-Random-Trills

This is a generator which can generate a fixed length of random sky-ground trills.

这是一个用 C++ 写的随机天地拆交互生成器。

## 使用说明

建议在本地编译 `generator.cpp` 运行，运行时注意与 `pattern1.txt` 放于同一目录下。

可以修改 `generator.cpp` 中最开头 `const double` 类型的变量，具体是下方三个：

```cpp
const double bpm = 178.00;//曲目BPM
const double START_TIME = 4*beat_per_second;//第一个键按下时间
const double END_TIME = 315*beat_per_second;//最后一个键按下时间的后一个十六分音符的时间
```

`pattern1.txt` 可以修改，该文件用于传入固定交互 pattern。

该文件第一行是总 pattern 数，记为 $n$，第 $2$ 至 $n+1$ 行，每行两个正整数 $len$ 和 $tracks$，分别表示传入的 pattern 的长度和该 pattern 所需击打的**地面轨道**。

**注意：** 需保证传入 pattern 为左手起，触及最左轨为 $1$，触及最右轨不超过 $4$，无反手。

比如说，这样的 `pattern1.txt` 是合法的：

```
2
4 1212
5 23121
```

而这样的 `pattern1.txt` 是不合法的：

```
4
3 1212
4 1221
4 2334
6 233445
5 21212
```

在这个不合法例子中，第一个 pattern 长度不匹配，第二个出现反手，第三个触及最左轨为 $2$，第四个触及最右轨超过 $4$，第五个右手起。

运行后生成的是 `2.aff` 文件，可以在 Arcade 预览。

## 半成品

接下来的计划有：

1. 优化逻辑，去除三纵
2. 彻底实现可调控拆纵交互/不拆纵交互
3. 实现更长的 pattern，并支持带天键的 pattern

学业原因可能会鸽子。
