#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

//不允许复制或者赋值
//把拷贝构造和赋值运算  delete掉
class NonCopyable
{
protected:
    NonCopyable()
    {
    }

    ~NonCopyable()
    {

    }
    //NOnCopyable(const NonCopyable &) =delete;
    //NonCopyable &operator=(const NonCopyable &)=delete;
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;
};

#endif
