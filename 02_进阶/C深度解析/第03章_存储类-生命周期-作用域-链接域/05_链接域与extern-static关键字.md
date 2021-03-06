# 5. 链接域与extern、static关键字

## 5.1 回顾链接

我们在第1章详细的讲过链接，这里因为课程的需求，我们需要再回顾下。

一个真正的C工程一定是多文件的（多.c、多.h），这些文件被编译为.o后，需要被链接为一个完整的可执行文件，链接的工作由链接器来完成。

链接时主要做两件事：
+ （1）符号解析
   + 1）对全局符号进行符号统一
   + 2）将符号的引用 与 符号的定义关联起来

+ （2）地址重定位

本小节的要讲“链接域”其实就与链接时的“全局符号统一”有关。  

## 5.2 回顾 代码块作用域
形参和局部变量的作用域就是代码块作用域，对于形参和局部变量来说，不允许出现同名符号，所以不存在需要统一同名符号的情况。  
而且代码块作用域只局限在代码块内，与其它文件没有任何关系，所以与链接无关。  

## 5.3 回顾 本文件作用域
在单个.c中，全局变量和函数的作用域就是本文件作用域，由于允许对全局变量和函数进行声明，所以在单个.c中存在同名符号的问题，编译时需要进行同名符号的统一，统一规则就是强弱符号的统一规则。  
由于本文件作用域只与当前文件有关，与其它文件无关，因此也与链接无关。  

## 5.4 跨文件作用域 与 extern关键字

### （1）为什么需要跨文件作用域
对于全局变量和函数来说，有时不仅仅只希望在本文件可以被使用，还希望在其它的文件中也能被使用，此时作用域就必须跨越到其它文件中，这就所谓的涉及跨文件作用域。跨文件作用域说白了就是将作用域延伸到到其它文件中。  
跨文件作用域涉及到多个文件，由于多文件最后要被链接到一起，与链接有关，**所以我们也将跨文件作用域称为链接域**。  

### （2）如何实现跨文件的作用域
只要满足两个条件即可。

+ 1）将定义标记为extern  
	extern表示定义的符号是一个全局符号，由于是全局符号，因此对于其它文件来说这个符号是可见的。  

+ 2）在其它文件中进行声明，声明也需要标记为extern  
	extern表示声明的符号也是一个全局符号，对于其它文件也是可见的。  

正式因为extern将符号标记为了全局可见，在链接阶段才能对全局符号进行“符号统一”。  


### （3）例子
```c
// a.c                          // b.c

extern int a;                   int a = 100; //全局符号，extern可以省略
extern int fun();								
int main(void)                  int fun()
{                               {
																	printf("helloworld\n");

}                               }
```


extern可以省略，省略后默认就是extern的，与auto有点像。  

对于几乎所有的编译器来说，都认可在定义时将extern省略，但是对于声明来说，有些编译其允许省略extern，但是有些就不允许，我们目前使用的gcc就允许声明时省略extern。  

不过为了保证不出错，经常的做法是，**定义时省略extern，但是声明时必须保留extern**。	 

由于全局符号的定义和声明是同名的，所以在链接阶段需要按照强弱符号的统一规则，对全局符号进行统一，声明作为弱符号最后会消失，虽然消失了，但是它却将“作用域跨”拓展到了其它文件中。  

从这里可以看出，想要实现跨文件作用域的话，必须使用声明这个弱符号来拓展作用域。   

不过有一点需要注意，我们说全局变量和全局符号时，这两个全局的意思不相同。  
+  `全局变量`的“全局”：指的是**文件**  
+  `全局符号`的“全局”：指的是**整个C工程项目**  



## 5.5 全局符号的重名问题 与 static关键字			

### （1）全局符号的重名问题	

extern所修饰的符号是所有文件都可见的全局符号。  
如果在不同文件中存在同名强符号的话，全局符号符号统一时就会报错，但是大家要知道一旦C工程变得复杂之后，在不同的文件中，误定义同名的函数和全局变量的情况是无法避免的。  
为了避免同名全局强符号的错误，我们应该尽量使用static关键字来避免这个问题。  

### （2）static修饰函数和全局变量时的作用
将符号标记为本地符号。


#### 1）什么是本地符号？
我们在第1章中详细介绍过，这里再简单回顾下。

所谓本地符号，就是符号只在本文件内可见，其它文件不可见，链接阶段进行全局符号统一时，所有static修饰的本地符号在全局是不可见的，所以不参与链接阶段的符号统一，因此就算同名了也不会报错。  

#### 2）本地符号的作用域

static将符号变为本地符号，说白了就是关闭符号的链接域，或者说关闭符号的跨文件作用域，符号此时只剩下“本文件作用域”。  

为了最大化的防止重名问题，**建议**凡是`只在本文件起作用，而其它文件根本用不到的函数和全局变量，统统使用static修饰`，让符号在全局不可见，防止全局强符号的同名冲突。  

C中使用static来解决全局强符号的命名冲突，其实是非黑即白的解决方式，为了能够更加精细化的解决命名冲突问题，从c扩展得到c++时，C++引入了命名空间这一概念，当然这个就是属于C++的内容。  


## 5.6 总结一下extern 和 static关键字					
本章我们介绍了不少的关键字，其中extern和static的用法稍微凌乱些，所以总结下。

### （1）static
+ 1）修饰局部变量  
	与存储类有关，表示局部变量的存储类为静态数据段。   

+ 2）修饰全局变量  
	与存储类无关，因为全局变量的存储类本来就是固定的静态数据段。  
	static修饰全局变量，表示符号为本地符号，关闭链接域（跨文件作用域），让其在全局不可见。   
 
+ 3）修饰函数  
	与修饰全局变量是一样的，将符号变为本地符号，关闭链接域，让其全局不可见。  




### （2）extern
+ 1）修饰函数、全局变量的定义和声明时  
	表示符号是全局符号，将链接域（跨文件作用域）被打开，让其全局可见。  

+ 2）将函数体外的全局变量和函数，声明到函数内部  
```c
a.c 
int main(void)
{
    extern int a;
    extern int fun();
    a = a+1;
    fun();
}

int a;
int fun()
{

}
```

此时fun函数也可以在其它的.c中，此时涉及到的就跨文件作用域。

