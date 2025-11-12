## 语言结构

### 设计哲学

该语言只能进行逐点操作(pointwise operations)。这种简化意味着，在线程级别，操作仅发生在每个张量的相同元素上，逐点操作很容易在GPU上实现。

因此，不需要切片(slicing)、分支(branching)和循环(looping)。

### Data数据

两种数据结构：；标量(scalar)和张量(ternsor)。

标量是单个数字，张量可以有任意维度，

但在一个核函数(kernel)中他们需要具有相同的大小。

数据类型：float32 (f32)

* 定义一个标量：`f32 scalar_name`;
* 定义一个张量：`f32[] tensor_name`;

### 操作

操作数可以是(对于二元操作)：

* scalar, scalar (标量，标量)
* tensor, scalar (张量， 标量)
* scalar, tensor (标量， 张量)
* tensor, tensor (要求相同的形状和类型的张量)

操作数(对于一元操作):

* scalar (标量)
* tensor (张量)

运算符:

* elementwise add (逐元素加法)
* elementwise sub (逐元素减法)
* elementwise mul (逐元素乘法)
* elementwise div  (逐元素除法)
* sqrt （平方根）
* exp2 (2的指数）
* log2  (以2为底)
* abs   (绝对值)

### 代码示例

文件应以 **.tgl** 结尾。不支持导入其他文件。

```
func device f32 calc_square_diff(f32 a, f32 b)
{
    var e = a - b;     # result is stored in a temporary variable (defined with var)
    var e2 = e * e;
    return e2;
}

func global void calc_mse(f32[] a, f32[] b, f32[] c, f32[] d)
{
    var e2 = calc_square_diff(a, b);  # calling device function
    var me2 = e2 * c;                 # some normalization factor
    var me2h = me2 * 0.5;             # 0.5 constant scalar, immediate value
    d = sqrt(me2h);                   # calling built-inf cuntion, then copies the result into d
    return;                           # return is compulsory

    # other examples
    # d = c;  // copy c to d
    # d = d + a;
}
```

 下一步

[抽象语法树](s3_ast抽象语法树.md)
